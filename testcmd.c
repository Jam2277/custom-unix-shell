#include <stdio.h>
#include <string.h>

char line[100] = "abc = def";

int main() {
	strtok(line, "=");
	char *cmd2 = strtok(NULL, "=");
	puts(line);
	puts(cmd2);
}
