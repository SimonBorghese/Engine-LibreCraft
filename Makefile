CC=g++

RELEASE_FLAGS=-Ofast -Wall -Wextra
DEBUG_FLAGS=-Og -g -Wall -Wextra

LIBS=-lSDL2 -lGL -lXi -lpthread -lX11 -lXrandr -ldl
FILES=src/*.cpp src/*.c
OTHER=
INCLUDE=-I. -I./include/.

PROGRAM_NAME=Engine

.PHONY: build release execute debug clean leak

debug_build:
	$(CC) $(DEBUG_FLAGS) $(INCLUDE) $(FILES) $(OTHER) $(LIBS) -o $(PROGRAM_NAME).elf

release_build:
	$(CC) $(RELEASE_FLAGS) $(INCLUDE) $(FILES) $(OTHER) $(LIBS) -o $(PROGRAM_NAME).elf

Release: release_build
	./$(PROGRAM_NAME).elf

Debug: debug_build
	gdb $(PROGRAM_NAME).elf

leak: build
	valgrind --leak-check=full --xml=yes --xml-file=valgrind.xml ./$(PROGRAM_NAME).elf

clean:
	rm vgcore*
	rm *.xml
	rm *.elf
