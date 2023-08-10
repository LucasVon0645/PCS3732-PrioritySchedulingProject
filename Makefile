FONTES = os/stubs.c os/kernel.s os/boot/boot.c os/interrupts/timer.c os/os.c os/interrupts/interrupts.c os/scheduler/scheduler.c main.c os/iomanager.c os/queue/multiqueue.c os/queue/queue.c

#
# Arquivos de saída 
#
EXEC = kernel.elf
MAP = kernel.map

PREFIXO = arm-none-eabi-
LDSCRIPT = kernel.ld
AS = ${PREFIXO}as
LD = ${PREFIXO}ld
GCC = ${PREFIXO}gcc
OPTS = -march=armv4 -g 
OBJ = $(FONTES:.s=.o)
OBJETOS = $(OBJ:.c=.o)

#
# OpÃ§Ãµes do linker
#
LDOPTS = -L/usr/lib/gcc/arm-none-eabi/10.3.1/ -L/usr/lib/arm-none-eabi/lib
LDOPTS += -lc -lgcc

#
# Gerar executável
#
${EXEC}: ${OBJETOS}
		${LD} -T ${LDSCRIPT} -M=${MAP} ${OBJETOS} ${LDOPTS} -o $@

#
# Compilar arquivos em C
#
.c.o:
	${GCC} ${OPTS} -c -o $@ $<

#
# Montar arquivos em assembler
#
.s.o:
	${AS} -g -o $@ $<

#
# Limpar tudo
#
clean:
	rm -f *.o ${EXEC} ${MAP}

#
# Iniciar openocd
#
ocd: 
	@if pgrep openocd >/dev/null ; then\
		echo "openocd já está executando"; \
	else openocd -f evlt7t/evaluator7t.cfg & \
	fi

#
# Iniciar gdb
#
gdb: ${EXEC}
	gdb-multiarch -ex "set architecture arm" \
		           -ex "target extended-remote :1234" \
					  -ex "load" \
					  -ex "b reset"\
					  -ex "b start"\
					  -ex "j *0"\
					  ${EXEC}

#
# Iniciar qemu
#
qemu: ${EXEC}
	@if lsof -Pi :1234 >/dev/null ; then\
		echo "qemu já está executando"; \
	else qemu-system-arm -s -M virt & \
	fi

kill:
	pkill ocd
	pkill qemu
	pkill gdb