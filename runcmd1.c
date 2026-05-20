#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

// runcmd1 ls -l
// command name: ls
// arg list: ls -l
int main(int argc, char *argv[]) {

	//following 4 mechanisms are equivalent:
	//execvp(argv[0], ++argv);

	//argv++;
	//execvp(argv[0], argv);
	
	execvp(argv[1], argv+1);

	//execvp(argv[1], &argv[1]);  // arrayp[i] is same as *(arrayp + i)
	
	fprintf(stderr, "This is bad!\n");
	exit(1);
}
