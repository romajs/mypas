CFLAGS=-I. -g

relloc=keyword.o symtab.o lexer.o parser.o args.o debug.o mypas.o

mypas: $(relloc)
	$(CC) -o $@ $^

clean:
	$(RM) $(relloc)
mostlyclean: clean
	$(RM) mypas *~
