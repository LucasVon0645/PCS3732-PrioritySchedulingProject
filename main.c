#include "os/iomanager.h"
#include "os/os.h"

int os_thread(void) {
   for(;;) showOsThreadDisplayAndLEDs();
}

int user_thread(void) {
   int pid = getpid();
   int time_to_halt = 8;
   int cpu_time = 0;

   for(;;) {
      int priority = get_priority();
      updateLed(priority);
      updateDisplay(pid);

      cpu_time = get_cpu_time();

      if (cpu_time > time_to_halt) {
         halt();
      }
   }
}