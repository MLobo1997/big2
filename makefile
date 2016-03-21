CFLAGS= -Wall -Wextra -pedantic -Wno-long-long -O2
OBJS=$(patsubst %.c, %.o, $(wildcard *.c))
EXEC=cartas

$(EXEC): $(OBJS)
		$(ghc) $(CFLAGS) -o $(EXEC) $(OBJS)

limpar:
		rm $(EXEC) *.o

		cp EXEC /usr/lib/cgi-bin/