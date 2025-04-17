CC = gcc
CFLAGS = -Wall -O2 -Iinclude -I"C:/msys64/mingw64/include"
LDFLAGS = -L"C:/msys64/mingw64/lib" -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -mconsole

TARGET = opengl.exe
OBJS = main.o glad.o boids.o camera.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q *.o *.exe