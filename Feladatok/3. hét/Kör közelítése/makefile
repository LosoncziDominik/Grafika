CC = gcc
CFLAGS = -Wall -Wextra -g3 -I/mingw64/include/SDL2
LDFLAGS = -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -mwindows
SRCS = 4runnable.c circle.c
OBJS = $(SRCS:.c=.o)
EXEC = kor.exe

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
