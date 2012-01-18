SHELL = /bin/bash
CC = g++
CXXFLAGS = -g 

OBJECTS =  main.o mutation_controller.o screen.o chromosomal.o allele.o gene.o manage_time.o

plan-shifts : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) -lyaml

main.o : chromosomal.h screen.h ag.h 
mutation_controller.o : manage_time.o allele.h gene.h mutation_controller_private.h
screen.o : manage_time.h ag.h gene.h chromosomal.h allele.h
chromosomal.o : manage_time.h allele.h gene.h ag.h 
manage_time.o : manage_time.h 

.PHONY : clean indent copy release

clean : 
	rm plan-shifts $(OBJECTS) *\~

indent :
	indent *.cpp *.h

copy : 
	cp plan-shifts ../bin/

release : copy clean
