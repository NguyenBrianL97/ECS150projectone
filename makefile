sshell: sshell.c
	gcc -o sshell sshell.c -I.
clean:
	rm sshell