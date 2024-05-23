# Compiler and flags
CC = g++
CFLAGS = -Wall -O2

# Check for the operating system
UNAME_S := $(shell uname -s)

# Include and library paths for libjpeg
ifeq ($(UNAME_S), Darwin) # macOS
    CFLAGS += -I/opt/homebrew/opt/jpeg/include -arch arm64
    LDFLAGS = -L/opt/homebrew/opt/jpeg/lib -ljpeg -arch arm64
else ifeq ($(UNAME_S), Linux) # Linux
    CFLAGS += -I/usr/include
    LDFLAGS = -L/usr/lib -ljpeg
endif

# Targets
all: deinterlace

deinterlace: main.o deinterlace.o
	$(CC) $(CFLAGS) -o deinterlace main.o deinterlace.o $(LDFLAGS)

main.o: main.cpp deinterlace.h
	$(CC) $(CFLAGS) -c main.cpp

deinterlace.o: deinterlace.cpp deinterlace.h
	$(CC) $(CFLAGS) -c deinterlace.cpp

clean:
	rm -f deinterlace main.o deinterlace.o
