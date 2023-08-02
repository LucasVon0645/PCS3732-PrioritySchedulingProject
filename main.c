#include "os/iomanager.h"

int main(void) {
   int i;
   for(;;) {
      updateLed(green1);
      updateDisplay(0);
   }
}

int main2(void) {
   int i;
   for(;;) {
      updateLed(orange);
      updateDisplay(1);
   }
}

int main3(void) {
   int i;
   for(;;) {
      updateLed(yellow);
      updateDisplay(2);
   }
}