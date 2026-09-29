all: write printf

write: write.c mywrite.s
	gcc -o write -g -Wall write.c mywrite.s

printf: printf.c
	gcc -o printf -g -Wall printf.c
	
clean:
	rm write printf
