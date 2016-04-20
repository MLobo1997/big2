CFLAGS= -Wall -Wextra -pedantic -Wno-long-long -O2
CFLAGSDEBUG= -Wall -Wextra -pedantic -Wno-long-long -g
OBJS=$(patsubst %.c, %.o, $(wildcard *.c))
EXEC=cartas

instalar: 	$(EXEC)
			sudo cp $(EXEC) /usr/lib/cgi-bin/
			sudo cp -r ../cards /var/www/html
			touch instalar

$(EXEC): $(OBJS)
		 $(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

limpar:
		rm $(EXEC) *.o

debug:
		gcc -Wall -Wextra -pedantic -Wno-long-long -g *.c -o cartas




