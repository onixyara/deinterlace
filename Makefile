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

deinterlace: deinterlace.cpp
	$(CC) $(CFLAGS) -o deinterlace deinterlace.cpp $(LDFLAGS)

clean:
	rm -f deinterlace
