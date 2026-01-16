#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

// Βοηθητική συνάρτηση για εκτύπωση κενών
void
print_padding(int len, int width)
{
  int gap = width - len;
  for(int i = 0; i < gap; i++){
    printf(" ");
  }
  printf("| "); // Εκτύπωση διαχωριστικού
}

// Υπολογισμός ψηφίων ενός αριθμού
int
int_len(int n)
{
  if(n == 0) return 1;
  int count = 0;
  if(n < 0) {
    n = -n;
    count++; // για το πρόσημο
  }
  while(n > 0){
    n /= 10;
    count++;
  }
  return count;
}

int
main(int argc, char *argv[])
{
  struct pstat st;

  if (getpinfo(&st) < 0) {
    printf("Error: getpinfo failed\n");
    exit(1);
  }

  // Ορισμός πλάτους στηλών (μπορείτε να τα αλλάξετε)
  int w_pid = 5;
  int w_ppid = 5;
  int w_name = 10;
  int w_prio = 9;
  int w_state = 10;
  int w_size = 8;
  // Το τελευταίο πεδίο (TICKS) δεν χρειάζεται padding

  // --- ΕΚΤΥΠΩΣΗ ΕΠΙΚΕΦΑΛΙΔΑΣ ---
  printf("PID  | PPID | NAME      | PRIORITY | STATE     | SIZE    | TICKS\n");
  printf("-----|------|-----------|----------|-----------|---------|------\n");

  // --- ΕΚΤΥΠΩΣΗ ΔΕΔΟΜΕΝΩΝ ---
  for(int i = 0; i < st.num_proc; i++) {
    
    // 1. PID
    printf("%d", st.pid[i]);
    print_padding(int_len(st.pid[i]), w_pid);

    // 2. PPID
    printf("%d", st.ppid[i]);
    print_padding(int_len(st.ppid[i]), w_ppid);

    // 3. NAME
    printf("%s", st.p_name[i]);
    print_padding(strlen(st.p_name[i]), w_name);

    // 4. PRIORITY
    printf("%d", st.priority[i]);
    print_padding(int_len(st.priority[i]), w_prio);

    // 5. STATE
    printf("%s", st.st_name[i]);
    print_padding(strlen(st.st_name[i]), w_state);

    // 6. SIZE
    printf("%d", (int)st.size[i]); // Cast σε int για απλότητα στην εκτύπωση
    print_padding(int_len((int)st.size[i]), w_size);

    // 7. TICKS (Τελευταίο πεδίο, χωρίς padding και διαχωριστικό)
    printf("%d\n", st.ticks[i]);
  }

  exit(0);
}