SERVER=server
CLIENT=client

#BINS=server client
#EXES=$(BINS:.c=.exe)
# $@ <- is replaced with the specific target
# $< <- is replaced with the name of the first requisite
# all: $(BINS)
all: clean comp 
#.c.exe:
#	g++ -o $@ $<.exe
comp:
	gcc -Wall -Wextra -o ${SERVER}.exe ${SERVER}.c
	gcc -Wall -Wextra -o ${CLIENT}.exe ${CLIENT}.c

clean: 
	rm -f ${SERVER}.exe ${CLIENT}.exe