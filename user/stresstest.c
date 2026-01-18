#include "kernel/types.h"
#include "kernel/pstat.h" // Απαραίτητο για το struct pstat
#include "user/user.h"

#define LARGE_TIME 100000000 // Μειώθηκε λίγο για να μην αργεί υπερβολικά στο demo
#define SMALL_TIME 10000000

// Συνάρτηση βοηθητική για στοίχιση (όπως στο ps)
void
print_padding_stress(int len, int width)
{
  int gap = width - len;
  for(int i = 0; i < gap; i++){
    printf(" ");
  }
  printf("| ");
}

int
int_len_stress(int n)
{
  if(n == 0) return 1;
  int count = 0;
  if(n < 0) { n = -n; count++; }
  while(n > 0){ n /= 10; count++; }
  return count;
}

// Η συνάρτηση που θα καλείται περιοδικά για να δείχνει την κατάσταση
void
print_scheduler_status(int step)
{
  struct pstat st;
  if (getpinfo(&st) < 0) {
    printf("Monitor: Error getting pinfo\n");
    return;
  }

  printf("\n=== MONITOR SNAPSHOT #%d ===\n", step);
  printf("PID  | NAME      | PRIO | TICKS | STATE\n");
  printf("-----|-----------|------|-------|----------\n");

  for(int i = 0; i < st.num_proc; i++) {
    // Φιλτράρουμε τις πολύ βασικές διεργασίες για να μειώσουμε τον θόρυβο,
    // ή τις δείχνουμε όλες. Εδώ τις δείχνουμε όλες που είναι ενεργές.
    if (st.pid[i] > 2) { 
        printf("%d", st.pid[i]);
        print_padding_stress(int_len_stress(st.pid[i]), 5);
        
        printf("%s", st.p_name[i]);
        print_padding_stress(strlen(st.p_name[i]), 10);
        
        printf("%d", st.priority[i]);
        print_padding_stress(int_len_stress(st.priority[i]), 5);
        
        printf("%d", st.ticks[i]);
        print_padding_stress(int_len_stress(st.ticks[i]), 6);
        
        printf("%s\n", st.st_name[i]);
    }
  }
  printf("============================\n");
}

uint64 useless_calc(uint64 z, uint64 time)
{
  volatile uint64 t = z;
  volatile uint64 sum = 0;
  for (;;) {
    t += 2;
    sum += t;
    t -= 1;
    if (t >= time) // Άλλαξα το == σε >= για ασφάλεια
      break;
  }
  return sum;
}

void
useless(uint64 time)
{
  uint64 sum = useless_calc(0, time);
  // Εκτύπωση αποτελέσματος (προαιρετική για να μην γεμίζει η οθόνη)
  printf("Child %d finished. Sum: %ld\n", getpid(), sum);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int pid;

  // 1. Δημιουργία της διεργασίας MONITOR
  // Αυτή θα τρέχει παράλληλα και θα τυπώνει στατιστικά
  if ((pid = fork()) == 0) {
      // Monitor process
      for (int i = 1; i <= 5; i++) { // Θα πάρει 5 μετρήσεις
          pause(0.001);
          print_scheduler_status(i);
      }
      printf("Monitor finished.\n");
      exit(0);
  }

  printf("Main: Starting workload...\n");

  // 2. Δημιουργία 4 Long Running Processes (CPU Bound)
  // Αυτές αναμένεται να πέσουν γρήγορα σε χαμηλή προτεραιότητα (lower queues)
  for (int i = 0; i < 4; i++)
  {
    pid = fork();
    if(pid == 0) {
      useless(LARGE_TIME);
    }
  }

  // 3. Δημιουργία 40 Small Processes (Short burst)
  // Αυτές θα τρέξουν γρήγορα και μάλλον θα μείνουν σε ψηλή προτεραιότητα
  int n = 40;
  for(int i = 0; i < n; i++) {
      pid = fork();
      if (pid == 0) {
          useless(SMALL_TIME);
      }
  }

  // Η main περιμένει όλα τα παιδιά να τελειώσουν
  while(wait(0) != -1);

  printf("Main: All processes finished.\n");
  exit(0);
}
