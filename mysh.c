#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

//read all the quotes from quotes.txt
//when client asks for a motivational quote, select one randomly and send it out.

#define MAXQUOTES 10000
#define MAXLEN 1000

//runs a simple command
//cmdname arg1 arg2 arg3 ...
void runCommand(char *command) {
	//split and assemble the arguments and invoke execvp()
	//use strtok(..)
}

//cmd0 | cmd1 | cmd2 | cmd3 | cmd4 ....
void child(int i) {
	//rewire pipes to 0 and 1 
	//do NOT rewire 0 for the first command - leave it as std input
	//do NOT rewrite 1 for the last command - leave it as std output
	//close unnecessary pipes - for loop?
	//run ith command
	runCommand(commands[i]);
}

void processLine(char *line) {
	char *pipePtr = strchr(line, '|'); //does this command have | chars?
	char *equalPtr = strchr(line, '='); //does this command have =?
	if (pipePtr) { //not NULL - cmd1 | cmd2 | cmd3 ....
		command has several sub-commands connected with pipes
		setup commands array
		setup pipes array
		fork & create children --> invoke child(i) in a loop
		//cmd0 | cmd1 | cmd2 | cmd3 | cmd4 
		// invoke child(i) for the last command directly?
	} else if (equalPtr) { 
		command has = operator, so 2 commands --> 2 processes
    } else 
		//it is a simple command, no pipe or = character
		runCommand(line);

	exit(0);
}

int main() {
	// load up all the quotes from quotes.txt - look at quoteserver.c for the code
	
	// infinite loop to serve the customer
	srand(time(NULL));
	while (1) {
		//output a random quote to stderr
		fputs(quotes[rand()%numQuotes], stderr);
		fprintf(stderr, "# "); //show prompt to the user
		//get the user input -- command line
		fgets(line, 1000, stdin);

		//spawn a child for taking care of it
		if (fork() == 0) 
			processLine(line);

		//wait the child to finish the job!
		wait(NULL);
	}
}
