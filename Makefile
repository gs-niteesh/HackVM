CC = g++
CFLAGS = -g -Iinclude/

VM: src/VM.cpp src/Parser.cpp
	$(CC) $(CFLAGS) -o VM src/VM.cpp src/Parser.cpp