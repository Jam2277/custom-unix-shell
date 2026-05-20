#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAXLEN 1000

//this code is almost same as ../process/execvp3.c

// command: "ls a.c b.c c.c d.c"
// args      0: "ls", 1: "a.c", 2: "b.c", 3: "c.c", 4: "d.c", 5: NULL
// tokenize the parameter string and invoke the command
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

//get a line input from the user, parse it and run that as a command!
int main() {
	char buffer[MAXLEN];
	printf("Your wish is my command: ");
	fgets(buffer, MAXLEN, stdin);
	runCommand(buffer);
	// execvp(argv[0], ++argv);
	// execvp(argv[1], argv+1);
	// execvp(argv[1], &argv[1]);  // arrayp[i] is same as *(arrayp + i)
}
