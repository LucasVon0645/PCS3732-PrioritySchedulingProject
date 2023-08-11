# Makefile para compilar arquivos .o e gerar um executável

# Nome do executável a ser gerado
TARGET = test

# Lista de arquivos .c (source) que compõem seu programa
SOURCES = ../os/queue/multiqueue.c ../os/queue/queue.c ../os/scheduler/scheduler.c test.c

# Gere a lista de arquivos .o a partir dos arquivos .c
OBJECTS = $(SOURCES:.c=.o)

# Comando do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -g

# Comando para linkar os objetos e gerar o executável
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Regras para compilar cada arquivo .c em um arquivo .o
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza - remove os arquivos .o e o executável
clean:
	rm -rf $(OBJECTS) $(TARGET)
