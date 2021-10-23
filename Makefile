CC=g++

RELEASE_FLAGS=-Ofast -Wall -Wextra
DEBUG_FLAGS=-Og -g -Wall -Wextra

LIBS=-lSDL2 -lGL -lXi -lpthread -lX11 -lXrandr -ldl
FILES=src/*.cpp src/*.c
OTHER=
INCLUDE=-I. -I./include/.

PROGRAM_NAME=Engine

.PHONY: build release execute debug clean leak

build:
	$(CC) $(DEBUG_FLAGS) $(INCLUDE) $(FILES) $(OTHER) $(LIBS) -o $(PROGRAM_NAME).elf

release:
	$(CC) $(RELEASE_FLAGS) $(INCLUDE) $(FILES) $(OTHER) $(LIBS) -o $(PROGRAM_NAME).elf

execute:
	./$(PROGRAM_NAME).elf

debug:
	gdb $(PROGRAM_NAME).elf

leak: build
	valgrind --leak-check=full --xml=yes --xml-file=valgrind.xml ./$(PROGRAM_NAME).elf

clean:
	rm vgcore*
	rm *.xml
	rm *.elf