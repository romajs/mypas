CFLAGS=-I. -g

relloc=keyword.o lexer.o parser.o args.o debug.o tests.o mypas.o

mypas: $(relloc)
	$(CC) -o $@ $^

clean:
	$(RM) $(relloc)
mostlyclean: clean
	$(RM) mypas *~
