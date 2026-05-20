#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAXLEN 1000

void runCommand(char *command) {
	char *args[MAXLEN];
	int index = 0;
	//tokenize command string: strtok()
	args[index] = strtok(command, " \n");
	while (args[index]) 
		args[++index] = strtok(NULL, " \n");

	//invoke execvp
	execvp(args[0], args);
	fprintf(stderr, "Bad tool name %s?\n", args[0]);
	exit(1);
}

//Usage: pipecmd "command1 ..." "command2 ..."
//Implements command1 | command2
//Example: pipecmd "cat quotes.txt" "wc -l"
//         cat quotes.txt | wc -l
int main(int argc, char *argv[]) {
	// argv[1] is the first command
	// argv[2] is the second command
	int fds[2];
	pipe(fds);
	//first command should write to pipe & second command should read from the pipe.
	//fork() & reconfigure pipe, then invoke runCommand()
	if(fork()) { // same as if (fork() != 0)
		//parent should run command2 - input needs to be redirected!
		dup2(fds[0], 0);
		close(fds[0]); close(fds[1]);
		runCommand(argv[2]);
	} else {
		//child should run command1 - output needs to be redirected!
		dup2(fds[1], 1);
		close(fds[0]); close(fds[1]);
		runCommand(argv[1]);
	}
}
