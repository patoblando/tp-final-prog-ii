CFLAGS=-I./include -g -Wextra -pedantic
SRCDIR=src
BINDIR=bin
OBJDIR=obj
TESTDIR=test

OBJS=$(OBJDIR)/safe_functions.o $(OBJDIR)/directorio.o $(OBJDIR)/normalizado_texto.o $(OBJDIR)/main.o
TESTOBJS=$(OBJDIR)/safe_functions.o $(OBJDIR)/directorio.o $(OBJDIR)/normalizado_texto.o

start: $(OBJS)
	cc $(CFLAGS) $(OBJS) -o $(BINDIR)/start

tests: $(OBJS)
	cc $(CFLAGS) $(TESTOBJS) -o $(TESTDIR)/CTests $(SRCDIR)/test_normalizado_texto.c 

$(OBJDIR)/safe_functions.o: $(SRCDIR)/safe_functions.c
	cc $(CFLAGS) -c $(SRCDIR)/safe_functions.c -o $(OBJDIR)/safe_functions.o

$(OBJDIR)/directorio.o: $(SRCDIR)/directorio.c $(OBJDIR)/safe_functions.o
	cc $(CFLAGS) -c $(SRCDIR)/directorio.c -o $(OBJDIR)/directorio.o

$(OBJDIR)/normalizado_texto.o: $(SRCDIR)/normalizado_texto.c $(OBJDIR)/directorio.o
	cc $(CFLAGS) -c $(SRCDIR)/normalizado_texto.c -o $(OBJDIR)/normalizado_texto.o

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(OBJDIR)/normalizado_texto.o
	cc $(CFLAGS) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o

clean:
	rm -f $(OBJDIR)/*.o