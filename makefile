# Makefile
# Liam Duncan

make: 
	gcc -g main.c list.o controller.c read.c write.c send.c receive.c -lpthread -o s-talk

clean:
	rm -f s-talk
