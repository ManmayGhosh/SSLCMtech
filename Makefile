CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = bus_booking
OBJS = main.o bookings.o operations.o

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile main.c
main.o: main.c bookings.h operations.h
	$(CC) $(CFLAGS) -c main.c

# Compile bookings.c
bookings.o: bookings.c bookings.h
	$(CC) $(CFLAGS) -c bookings.c

# Compile operations.c
operations.o: operations.c operations.h bookings.h
	$(CC) $(CFLAGS) -c operations.c

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Force rebuild
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild