#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h> 

//Usage: bah [-s] [-n count]
//
int main(char argc, char *argv[]) {
	char ch;
	int repeatCount=1000;
	bool startFlag = false;

	//arguments processing
	while ((ch = getopt(argc, argv, "sn:")) != -1) {
		//option character
		switch(ch) {
			case 's': startFlag = true;
						break;
			case 'n': repeatCount = atoi(optarg);
						break;
			case '?':
				fprintf(stderr, "Unrecognized option: -%c\n", optopt);
				exit(1);

			default:
				fprintf(stderr, "bah [-s] [-n count]\n");
				exit(2);
		}
	}

	//CODE HERE
	//if startFlag is true, output ba
	//in a loop read string from std input add b in front and a at the end, output
	if (startFlag)
	{
		printf("ba\n");
		fflush(stdout);
		fprintf(stderr , "bah!\n");
	}
	//for loop
	// scanf the string
	//
	char buffer[1000];
	for(int i=0; i<repeatCount; i++) {
		if (!scanf("%s", buffer))
			break;
	printf("b%sa\n", buffer);
	fflush(stdout);
	fprintf(stderr , "b%sah!\n", buffer);
	
	}

}
