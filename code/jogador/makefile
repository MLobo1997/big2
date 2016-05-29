CFLAGS= -Wall -Wextra -pedantic -Wno-long-long -O2
CFLAGSDEBUG= 
OBJS=$(patsubst %.c, %.o, $(wildcard *.c))
EXEC=jogador_inteligente

$(EXEC): $(OBJS)
		 $(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

debug: 
		gcc -Wall -Wextra -pedantic -Wno-long-long -g *.c -o jogador_inteligente

limpar:
		rm $(EXEC) *.o