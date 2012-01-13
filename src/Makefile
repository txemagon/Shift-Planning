SHELL = /bin/bash
CC = g++
CFLAGS = -g 

OBJECTS =  main.o screen.o chromosomal.o allele.o gene.o manage_time.o

plan-shifts : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

main.o : manage_time.o chromosomal.h screen.h ag.h allele.h gene.h
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
