#include "os/iomanager/iomanager.h"
#include "os/syscalls/syscalls.h"

int os_thread(void) {
   for(;;) showOsThreadOnDisplay();
}

int user_thread(void) {
   int pid = get_pid();
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