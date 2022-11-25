# Made by CYB3RSP1D3R

ARCH=arm
CC_PATH=/opt/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/
CROSS_COMPILE=$(CC_PATH)arm-linux-gnueabihf-

CC=$(CROSS_COMPILE)gcc
CFLAGS=-c -std=c11 -O2
SRC=memtest.c
OBJ=memtest.o
EXEC=memtest

.PHONY: clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm -f $(EXEC) $(OBJ) *.s *.i