
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = -pthread -lrt

TARGET = campuscompute

SOURCES = src/main.c \
          src/jobs.c \
          src/scheduling.c \
          src/logger.c \
          src/ipc.c \
          src/banker.c \
          src/worker.c \
          src/resource.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
