#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAXLEN 1000

// command: "ls a.c b.c c.c d.c"
// args      0: "ls", 1: "a.c", 2: "b.c", 3: "c.c", 4: "d.c", 5: NULL
void runCommand(char *command) {
	char *args[MAXLEN];
	int index = 0;
	//tokenize command string: strtok()
	args[index] = strtok(command, " \n");
	while (args[index]) 
		args[++index] = strtok(NULL, " \n");

	//invoke execvp
	execvp(args[0], args);
	fprintf(stderr, "This is bad!\n");
	exit(1);
}

// command1 = command2
void runLoop(char *line) {
	char *command1 = strtok(line, "=");
	char *command2 = strtok(NULL, "\n");

	//create 2 pipes
	int toparent[2], tochild[2];
	pipe(toparent);
	pipe(tochild);
	//fork()
	//reconfigure pipes for 0 and 1
	//runCommand()
	if (fork()) {
		dup2(toparent[0], 0); //read from child: toparent[0]
		dup2(tochild[1],  1); //write to child: tochild[1]
		close(toparent[0]); close(toparent[1]);
		close(tochild[0]); close(tochild[1]);
		runCommand(command1);
	} else {
		dup2(tochild[0], 0); //read from parent: tochild[0]
		dup2(toparent[1], 1); //write to parent: toparent[1]
		close(toparent[0]); close(toparent[1]);
		close(tochild[0]); close(tochild[1]);
		runCommand(command2);
	}
}

//behave like shell!
//keep asking the user for the next command & execute it using child process
// loop functionality: command1 = command2
int main() {
	char buffer[MAXLEN];
	printf("Your wish is my command: ");
	fgets(buffer, MAXLEN, stdin);
	runLoop(buffer);
}
