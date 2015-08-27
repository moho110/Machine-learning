CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = 
INCLUDES = 
LIBS = -lm -lpthread \
	   -lglut32 -lglu32 -lopengl32

TARGET = 26
OBJS = 26.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

clean:
	-rm -f $(OBJS) $(TARGET)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

dist:
	tar czvf $(TARGET)-1.0.tar.gz *

install:
	cp $(TARGET) /usr/local/bin

#end of Makefile
