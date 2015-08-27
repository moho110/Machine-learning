CC = g++
CFLAGS = -Wall -O2
LDFLAGS =
INCLUDES =
LIBS = -lglut32 -lglu32 -lopengl32

TARGET = curve_disp.exe
OBJS = curve_disp.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	-rm -f $(OBJS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

#end of Makefile
