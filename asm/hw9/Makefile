CC = nasm
CC_FLAGS = -felf

EXEC = run
SOURCES = $(wildcard *.asm)
OBJECTS := $(SOURCES:.asm=.o) io.o

$(EXEC): $(OBJECTS) 
	ld -m elf_i386 $(OBJECTS) -o $(EXEC)

%.o: %.asm
	$(CC) $(CC_FLAGS) $< -o $@
