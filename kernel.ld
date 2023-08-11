
SECTIONS {                  /* arquivo kernel.ld para QEMU arm virt */
   /*
    * Vetor de reset
    */
   . = 0;
   .reset : { *(.reset) }

   /*
    * Segmentos text e data
    */
   /* . = 0x40000000; */
  . = 0x8000;
  .text : { *(.text) }
  .data : { *(.data) }

  /*
   * Segmento bss
   */
  inicio_bss = .;
  .bss : { *(.bss) }
  . = ALIGN(4); 
  fim_bss = .;

  /* 
  * Segmento heap
  */
  heap_start = .;
  . = . + 4096;
  . = ALIGN(8);

  /*
   * Reserva espaço para as pilhas
   * do sistema.
   */

  . = . + 4096;
  . = ALIGN(8);
  stack_irq = .;

  . = . + 4096;
  . = ALIGN(8);
  stack_svr = .;
}
