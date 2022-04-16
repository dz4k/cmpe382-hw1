
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtins.h"
#include "dynarray.h"
#include "error.h"
#include "parser.h"
#include "shell_state.h"

// Use following command to compile:
// g++ -g *.c -o TEDShell

// To run the program
// ./TEDShell

// To run testcases
// ./TEDShell-tester

int readline(FILE *file, char *buffer) {
  if (fgets(buffer, 100, file) == NULL) return 1;
  buffer[strcspn(buffer, "\n")] = 0;
  return 0;
}

int interactive();
int batch(int argc, char *const argv[]);
int runLine(ShellState *state, char *const input);
int runCommand(ShellState *state, Command *cmd);
char *findExecutable(ShellState *state, char *const cmd);
int runExecutable(char *const exe, char **const argv, char *const redirectStdout);

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
    if (readline(stdin, input) != 0) {
      break;
    }
    int status = runLine(&state, input);
  }

  ShellStateFree(&state);
  return 0;
}

int batch(int argc, char *const argv[]) {
  char input[100];
  ShellState state;
  ShellStateInit(&state);

  if (argc != 2) {
    ERROR();
    exit(1);
  }

  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    ERROR();
    exit(1);
  }
  
  while (readline(file, input) == 0) {
    int status = runLine(&state, input);
  }

  return 0;
}

int runLine(ShellState *state, char *input) {

  // Parse
  Command *cmd = parseCommand(input);

  if (cmd == NULL) return 0;

  if (cmd->parseError) {
    ERROR();
    return 1;
  }

  int status = runCommand(state, cmd);

  CommandFree(cmd);
  return status;
}

int runCommand(ShellState *state, Command *cmd) { 
  int pid = 0;
  if(cmd -> parallelWith != NULL){
    pid = fork();
   if(pid==0){
     runCommand(state, cmd -> parallelWith);
     exit(0);
   }
  }
  for (int i = 0; builtins[i].name != NULL; i++) {
    if (strcmp((char *)cmd->args.array[0], builtins[i].name) == 0) {
      return builtins[i].builtin(state, &cmd->args);
    }
  }

  // Execute binary
  char *binary = findExecutable(state, (char *)cmd->args.array[0]);
  if (binary == NULL) {
    ERROR();
    return 1;
  }


  int result = runExecutable(binary, (char **)cmd->args.array, cmd->redirectStdout);
  int status = 0;
  if(pid !=0){
  waitpid(pid, &status, 0);
  }
  return result;
} 

char *findExecutable(ShellState *state, char *const cmd) {
  DynArray path = state->path;

  // Path to the executable binary
  char *binpath = (char *)malloc(sizeof(char) * 512);
  for (int i = 0; i < path.count; i++) {
    int size = sprintf(binpath, "%s/%s", (char *)path.array[i], cmd);
    if (access(binpath, X_OK) == 0) {
      binpath = (char *)realloc(binpath, size);
      return binpath;
    }
  }

  return NULL;
}

int runExecutable(char *const exe, char **const argv, char *const redirectStdout) {
  int wstatus;

  pid_t pid = fork();

  if (pid < 0) {
    return -1;
  } else if (pid == 0) {
    // Child process
    if (redirectStdout != NULL) {
      dup2(fileno(fopen(redirectStdout, "w")), STDOUT_FILENO);
    }
    
    execv(exe, argv);
  } else {
    // Parent process
    waitpid(pid, &wstatus, 0);
  }

  return wstatus;
}
