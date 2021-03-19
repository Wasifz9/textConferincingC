CC = gcc
CFLAGS = -Wall -pthread
OBJFILES = server.o serverCommands.o helpers.o
TARGET = TCserver
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~