CFLAGS=-I. -g

relloc=keyword.o lexer.o parser.o symtab.o codegen.o typecheck.o mypas.o error.o

mypas: $(relloc)
	$(CC) -o $@ $^

clean:
	$(RM) $(relloc)
mostlyclean: clean
	$(RM) mypas *~
