CC = gcc
CFLAGS = -Wall -std=gnu99
LDFLAGS =
OBJFILES = client.o clientCommands.o helpers.o
TARGET = textConf
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~