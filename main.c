
#include <stdint.h>
#include "kernel.h"
#include "iomanager.h"

int main(void) {
   int i;
   for(;;) {
      lightUpLed(green1);
      setDisplay(1);
   }
}

int main2(void) {
   int i;
   for(;;) {
      lightUpLed(orange);
      setDisplay(2);
   }
}