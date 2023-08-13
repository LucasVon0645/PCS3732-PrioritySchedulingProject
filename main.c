#include "os/iomanager.h"
#include "os/os.h"

int os_thread(void) {
   for(;;) {
      showOsThreadDisplay();
   }
}

int user_thread(void) {
   int pid = getpid();
   int time_to_halt = 10;
   int cpu_time = 0;
   int stop = 0;

   while(!stop) {
      int priority = get_priority();
      updateLed(priority);
      updateDisplay(pid);

      cpu_time = get_cpu_time();

      if (cpu_time > time_to_halt) {
         stop = 1;
      }
   }

   return 0;
}