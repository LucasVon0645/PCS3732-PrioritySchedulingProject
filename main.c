#include "os/iomanager.h"
#include "os/os.h"

int main(void) {
   int pid = getpid();
   for(;;) {
      int priority = get_priority();
      updateLed(priority);
      updateDisplay(pid);
   }
}

int main2(void) {
   int pid = getpid();
   for(;;) {
      int priority = get_priority();
      updateLed(priority);
      updateDisplay(pid);
   }
}

int main3(void) {
   int pid = getpid();
   for(;;) {
      int priority = get_priority();
      updateLed(priority);
      updateDisplay(pid);
   }
}