/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 80 /* 80 chars per line, per command */

int main(void) {
  char *args[MAX_LINE / 2 + 1];
  /* command line (of 80) has max of 40 arguments */
  char *command = (char *)malloc(sizeof(char) * MAX_LINE);
  int first_run = 1;
  int should_run = 1;

  while (should_run != 0) {
    printf("osh>");
    fflush(stdout);
    size_t buf = 0;
    // Input operation
    int cmd_line_size = getline(&command, &buf, stdin);
    if(cmd_line_size == 1){
      printf("You didn't input any command. Please retry.\n"); // Only input '\n' means no real command.
      continue;
    }
//    debug:
//    printf("Input: [%s]\n", command);
    char *tmp = (char *)malloc(MAX_LINE * sizeof(char));
    int argc = 0, i = 0, tmp_ptr = 0;
    memset(tmp, 0, MAX_LINE * sizeof(char));

    for (i = 0; i < cmd_line_size - 1; ++i) {
      if (command[i] != ' ')
        tmp[tmp_ptr++] = command[i];
      else {
        args[argc] = (char *)malloc(tmp_ptr + 1);
        strcpy(args[argc], tmp);
        memset(tmp, 0, MAX_LINE * sizeof(char));
        tmp_ptr = 0;
        argc++;
      }
    }
    args[argc] = (char *)malloc(cmd_line_size);
    strcpy(args[argc], tmp);
    argc++;
    size_t idx = argc;
    for(; idx < MAX_LINE / 2 + 1; ++idx) args[idx] = NULL;
    free(tmp);
    int child_status;
//    debug:
//    printf("Arguments: Count: %d\n", argc);
//    for (i = 0; i < argc; ++i)
//      printf("[%s]\n", args[i]);
    if(argc == 1 && strcmp(args[0], "exit") == 0){
      printf("Exit.");
      exit(0);
    }
    pid_t child_pid = fork();
    if(child_pid == 0){
      execvp(args[0], args);
      printf("Command Error.\n");
      exit(0);
    } else {
      pid_t tpid = 0;
      while(tpid != child_pid) tpid = wait(&child_status);
      fflush(stderr);

      if(child_status != 0)
        printf("\n$(%d) ", child_status);
      fflush(stdout);
    }
    /**
     * After reading user input, the steps are:
     * (1) fork a child process
     * (2) the child process will invoke execvp()
     * (3) if command included &, parent will invoke wait()
     */
  }

  return 0;
}