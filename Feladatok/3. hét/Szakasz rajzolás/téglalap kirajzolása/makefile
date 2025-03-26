# Fordító és beállítások
CC = gcc
CFLAGS = -Wall -Wextra -g -I/mingw64/include/SDL2
LDFLAGS = -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -mwindows

# Fájlnevek
SRC = main.c
OBJ = $(SRC:.c=.o)
TARGET = teglalap.exe

# Alap cél
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
