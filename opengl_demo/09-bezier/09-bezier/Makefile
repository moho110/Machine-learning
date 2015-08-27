CC = gcc
CFLAGS = -Wall -O2
LDFLAGS =
INCLUDES =
LIBS = -lglut32 -lglu32 -lopengl32

TARGET = bezier.exe
OBJS = bezier.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	-rm -f $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

