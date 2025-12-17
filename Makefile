CFLAGS=-Wall -Wextra -g
shell: shell.c
	gcc shell.c $(CFLAGS) -o shell
