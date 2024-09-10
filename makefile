# Makefile
# Assignment 2 - CMPT 300
# Liam Duncan - 301476562
# March 1, 2024

make: 
	gcc -g main.c list.o controller.c read.c write.c send.c receive.c -lpthread -o s-talk

clean:
	rm -f s-talk
