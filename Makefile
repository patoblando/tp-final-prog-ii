CFLAGS=-I./include -g -Wextra -pedantic
SRCDIR=src
BINDIR=bin
OBJDIR=obj
TESTDIR=test
ENTRADAS=Entradas

OBJS=$(OBJDIR)/safe_functions.o $(OBJDIR)/directorio.o $(OBJDIR)/normalizado_texto.o $(OBJDIR)/main.o
TESTOBJS=$(OBJDIR)/safe_functions.o $(OBJDIR)/directorio.o $(OBJDIR)/normalizado_texto.o

all: start tests

start: $(OBJS) | $(OBJDIR)  $(BINDIR)
	cc $(CFLAGS) $(OBJS) -o $(BINDIR)/start

tests: $(OBJS)  | $(ENTRADAS) $(TESTDIR)
	cc $(CFLAGS) $(TESTOBJS) -o $(TESTDIR)/CTests $(SRCDIR)/test_normalizado_texto.c 

$(OBJDIR)/safe_functions.o: $(SRCDIR)/safe_functions.c | $(OBJDIR)
	cc $(CFLAGS) -c $(SRCDIR)/safe_functions.c -o $(OBJDIR)/safe_functions.o

$(OBJDIR)/directorio.o: $(SRCDIR)/directorio.c $(OBJDIR)/safe_functions.o | $(OBJDIR)
	cc $(CFLAGS) -c $(SRCDIR)/directorio.c -o $(OBJDIR)/directorio.o

$(OBJDIR)/normalizado_texto.o: $(SRCDIR)/normalizado_texto.c $(OBJDIR)/directorio.o | $(OBJDIR)
	cc $(CFLAGS) -c $(SRCDIR)/normalizado_texto.c -o $(OBJDIR)/normalizado_texto.o

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(OBJDIR)/normalizado_texto.o | $(OBJDIR)
	cc $(CFLAGS) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(ENTRADAS):
	mkdir -p $(ENTRADAS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(TESTDIR):
	mkdir -p $(TESTDIR)
clean:
	rm -f $(OBJDIR)/*.o