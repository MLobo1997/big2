CFLAGS= -Wall -Wextra -pedantic -Wno-long-long -O2
OBJS=$(patsubst %.c, %.o, $(wildcard *.c))
EXEC=/usr/lib/cgi-bin/cartas/cartas

$(EXEC): $(OBJS)
		$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

limpar:
		rm $(EXEC) *.o