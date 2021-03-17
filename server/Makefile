CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = server.o serverCommands.o helpers.o
TARGET = TCserver
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~