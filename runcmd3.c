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

//behave like shell!
//keep asking the user for the next command & execute it using child process
int main() {
	char buffer[MAXLEN];
	while (1) {
		printf("Your wish is my command: ");
		fgets(buffer, MAXLEN, stdin);
		if (!fork())
			runCommand(buffer); //child process invokes this!
		wait(NULL); //waits for the child to finish!
	}
}
