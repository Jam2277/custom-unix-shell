#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAXQUOTES 10000
#define MAXLEN 1000
#define MAXARGS 100
#define MAXCOMMANDS 100

char *quotes[MAXQUOTES];
int numQuotes = 0;

char *commands[MAXCOMMANDS];
int numCommands = 0;
int pipes[MAXCOMMANDS - 1][2];

void trim(char *str) {
    while (*str == ' ' || *str == '\t' || *str == '\n')
        memmove(str, str + 1, strlen(str));

    int len = strlen(str);
    while (len > 0 &&
           (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n')) {
        str[len - 1] = '\0';
        len--;
    }
}

void loadQuotes() {
    FILE *fp = fopen("quotes.txt", "r");

    if (fp == NULL) {
        quotes[numQuotes++] = strdup("Welcome to my shell.\n");
        return;
    }

    char line[MAXLEN];

    while (fgets(line, MAXLEN, fp) != NULL && numQuotes < MAXQUOTES) {
        quotes[numQuotes] = strdup(line);
        numQuotes++;
    }

    fclose(fp);

    if (numQuotes == 0)
        quotes[numQuotes++] = strdup("Welcome to my shell.\n");
}

void runCommand(char *command) {
    char *args[MAXARGS];
    int argc = 0;

    trim(command);

    char *token = strtok(command, " \t\n");

    while (token != NULL && argc < MAXARGS - 1) {
        args[argc++] = token;
        token = strtok(NULL, " \t\n");
    }

    args[argc] = NULL;

    if (argc == 0)
        exit(0);

    execvp(args[0], args);

    perror("execvp failed");
    exit(1);
}

void child(int i) {
    if (i > 0) {
        dup2(pipes[i - 1][0], STDIN_FILENO);
    }

    if (i < numCommands - 1) {
        dup2(pipes[i][1], STDOUT_FILENO);
    }

    for (int j = 0; j < numCommands - 1; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

    runCommand(commands[i]);
}

void runPipeline(char *line) {
    numCommands = 0;

    char *token = strtok(line, "|");

    while (token != NULL && numCommands < MAXCOMMANDS) {
        trim(token);
        commands[numCommands++] = token;
        token = strtok(NULL, "|");
    }

    for (int i = 0; i < numCommands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe failed");
            exit(1);
        }
    }

    for (int i = 0; i < numCommands; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            child(i);
        } else if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
    }

    for (int i = 0; i < numCommands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < numCommands; i++) {
        wait(NULL);
    }
}

void runEqual(char *line) {
    char *left = strtok(line, "=");
    char *right = strtok(NULL, "=");

    if (left == NULL || right == NULL) {
        fprintf(stderr, "Invalid = command\n");
        exit(1);
    }

    trim(left);
    trim(right);

    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid_t p1 = fork();

    if (p1 == 0) {
        dup2(pipe2[0], STDIN_FILENO);
        dup2(pipe1[1], STDOUT_FILENO);

        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        runCommand(left);
    }

    pid_t p2 = fork();

    if (p2 == 0) {
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        runCommand(right);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
}

void processLine(char *line) {
    char lineCopy[MAXLEN];
    strcpy(lineCopy, line);

    char *pipePtr = strchr(lineCopy, '|');
    char *equalPtr = strchr(lineCopy, '=');

    if (pipePtr) {
        runPipeline(lineCopy);
    } else if (equalPtr) {
        runEqual(lineCopy);
    } else {
        runCommand(lineCopy);
    }

    exit(0);
}

int main() {
    char line[MAXLEN];

    loadQuotes();

    srand(time(NULL));

    while (1) {
        fputs(quotes[rand() % numQuotes], stderr);
        fprintf(stderr, "# ");

        if (fgets(line, MAXLEN, stdin) == NULL)
            break;

        if (strcmp(line, "exit\n") == 0 || strcmp(line, "quit\n") == 0)
            break;

        if (fork() == 0)
            processLine(line);

        wait(NULL);
    }

    return 0;
}
