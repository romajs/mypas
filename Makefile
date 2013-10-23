CFLAGS=-I. -g

relloc=lexer.o parser.o keyword.o mypas.o symtab.o args.o debug.o

mypas: $(relloc)
	$(CC) -o $@ $^

clean:
	$(RM) $(relloc)
mostlyclean: clean
	$(RM) mypas *~
