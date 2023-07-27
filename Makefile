FONTES = kernel.s sched.c main.c iomanager.c queue.c

EXEC = kernel.elf
MAP = kernel.map

PREFIXO = arm-none-eabi-
LDSCRIPT = kernel.ld
AS = ${PREFIXO}as
LD = ${PREFIXO}ld
GCC = ${PREFIXO}gcc
OBJ = $(FONTES:.s=.o)
OBJETOS = $(OBJ:.c=.o)
OPTS = -g

# Alvo: gerar executável
${EXEC}: ${OBJETOS}
	${LD} -T ${LDSCRIPT} -M=${MAP} -o $@ ${OBJETOS}

# Alvo: montar arquivos em assembler
.s.o:
	${AS} -g -o $@ $<

# Alvo: Compilar arquivos em C
.c.o:
	${GCC} ${OPTS} -c -o $@ $<

# Alvo: limpar tudo
clean:
	rm -f *.o ${EXEC} ${MAP}

# Alvo: iniciar qemu
qemu: ${EXEC}
	@if lsof -Pi :1234 >/dev/null ; then\
		echo "qemu já está executando"; \
	else qemu-system-arm -s -M virt & \
	fi

# Alvo: iniciar gdb
gdb: ${EXEC}
	gdb-multiarch -ex "set architecture arm" \
						-ex "target extended-remote :3333" \
						-ex "load" \
						-ex "b start" \
						${EXEC}

# Iniciar openocd
ocd: 
	@if pgrep openocd >/dev/null ; then\
		echo "openocd já está executando"; \
	else openocd -f evaluator7t.cfg & \
	fi