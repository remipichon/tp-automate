TOOL	=auto
YACC	=bison
LEX	=flex
CC	=gcc
TRACE	=

CFLAGS	+=-g -Wall

all: $(TOOL)
	
trace: debug_compile all

debug_compile: 
	 $(eval TRACE += -DTRACE )

#utilisation de lex
lex.yy.c: $(TOOL).l
	@echo "#########Generation analyseur lexical d'expressions regulieres"
	$(LEX) -o $@  $(TOOL).l 

#utilisation de bison
$(TOOL).tab.c: $(TOOL).y  lex.yy.c
	@echo "#########Generation analyseur syntaxique d'expressions regulieres"
	$(YACC) -o $@ -d $(TOOL).y

#renomage de auto.tab.c en auto.c
$(TOOL).c: $(TOOL).tab.c 
	@echo "#########Renomage " $< " en " $@
	mv $(TOOL).tab.c $(TOOL).c

#compilation de auto
#Pour le tester: auto <ex1.reg
$(TOOL): $(TOOL).c  tp_agp_auto.c
	@echo "#########Compilation de l'analyseur d'expressions reguliere"
	$(CC)  $(CFLAGS)  $(TOOL).c -c -o $(TOOL).o 
	@echo "#########Compilation des action faites sur l'AST (votre travail)"
	$(CC) $(CFLAGS) tp_agp_auto.c -c $(TRACE)
	@echo "#########Compilation des action faites sur l'AST (votre travail)"
	$(CC) $(CFLAGS) ensemble.c -c
	@echo "#########Edition de lien: production du generateur d'automate"
	$(CC) $(CFLAGS)  ensemble.o $(TOOL).o  tp_agp_auto.o -o $@ 
	@echo "#########Exemple d'utilisation: "$(TOOL)" < ex1.reg"

#automate.c : fichier d'exemple de programme C implementant automate
automate: automate.c
	gcc  automate.c -DTRACE -o automate

tar:  clean
	cd ..; tar cvf TP6_AUTO.tar TP6_AUTO/; gzip -f TP6_AUTO.tar 

clean: 
	\rm -rf *~ *.o $(TOOL).tab.c $(TOOL).tab.h lex.yy.c $(TOOL).c $(TOOL)
	
testEnsemble:  testEnsemble.o ensemble.o
	$(CC) $(CFLAGS)  ensemble.o testEnsemble.o -o testEnsemble
	@echo "#########Exemple d'utilisation: testEnsemble"
	
ensemble.o: ensemble.c ensemble.h
	$(CC) $(CFLAGS) ensemble.c -c
	
testEnsemble.o: testEnsemble.c ensemble.h
	$(CC) $(CFAGS) testEnsemble.c -c