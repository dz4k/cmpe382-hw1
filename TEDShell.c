#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "dynarray.h"
#include "shell_state.h"

// Use following command to compile:
// g++ -g TEDShell.c dynarray.c shell_state.c -o TEDShell

// To run the program
// ./TEDShell

// To run testcases
// ./TEDShell-tester

void readline(char *buffer) {
  fgets(buffer, 100, stdin);
  buffer[strcspn(buffer, "\n")] = 0;
}

enum {
  SHELL_OK,
  SHELL_ERR,
};

#define ERROR() write(STDERR_FILENO, "An error has occurred\n", 23)

int interactive();
int batch(int argc, char *const argv[]);
int runCommand(ShellState *state, char *const input);
char* findExecutable(ShellState *state, char *const cmd);
int runExecutable(char **const argv);

int main(int argc, char *argv[]) {

  char input[100];

  if (argc <= 1)
    return interactive();
  else
    return batch(argc, argv);

  return 0;
}

int interactive() {
  char input[100];
  ShellState state;
  ShellStateInit(&state);

  while (1) {
    printf("TEDShell> ");
    readline(input);
    if (input[0] == EOF) {
      break;
    }
    int status = runCommand(&state, input);
    if (status == SHELL_ERR) {
      ERROR();
    }
  }

  ShellStateFree(&state);
  return 0;
}

int batch(int argc, char *const argv[]) {
  // TODO
  return 0;
}

int runCommand(ShellState *state, char *input) {
  
  // Parse
  DynArray args;
  DynArrayInit(&args);
  char *arg;

  while ((arg = strsep(&input, " ")) != NULL) {
    DynArrayPush(&args, arg);
  }

  // Execute binary
  char* binary = findExecutable(state, (char*) args.array[0]);
  if (binary == NULL) {
    DynArrayFree(&args);
    return SHELL_ERR;
  }
  args.array[0] = binary;

  int status = runExecutable((char**) args.array);

  DynArrayFree(&args);
  return 0;
}

char* findExecutable(ShellState *state, char *const cmd) {
  DynArray path = state->path;

  // Path to the executable binary

  char* binpath = (char*) malloc(sizeof(char) * 512);
  for (int i = 0; i < path.count; i++) {
    int size = sprintf(binpath, "%s/%s", (char*) path.array[i], cmd);
    if (access(binpath, X_OK) == 0) {
      binpath = (char*) realloc(binpath, size);
      return binpath;
    }
  }

  return NULL;
}

int runExecutable(char **const argv) {
  int wstatus;
 
  pid_t pid = fork();
  
  if (pid < 0) return -1;
  else if (pid == 0) execv(argv[0], argv);
  else waitpid(pid, &wstatus, 0);

  return wstatus;
}
