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

#include "libfunc.h"

#define MAX_LINE 80 /* 80 chars per line, per command */

int main(void) {
  char *args[MAX_LINE / 2 + 1];
  /* command line (of 80) has max of 40 arguments */
  char *command = (char *)malloc(sizeof(char) * MAX_LINE);
  char *last_command = (char *)malloc(sizeof(char) * MAX_LINE);
  int first_run = 1;
  int should_run = 1;
  int background_run = 0;

  while (should_run != 0) {
    background_run = 0;
    printf("osh>");
    fflush(stdout);
    size_t buf = 0;
    // Input operation
    int cmd_line_size = getline(&command, &buf, stdin);
    if(cmd_line_size == 1){
      printf("You didn't input any command. Please retry.\n"); // Only input '\n' means no real command.
      continue;
    }
    // for history use
    if(check_dual_exclamation(command)){
      if(first_run){
        printf("No commands in history!\n");
        continue;
      } else {
        modify_dual_exclamation(command, last_command, MAX_LINE);
      }
    } else {
      first_run = 0;
      strcpy(last_command, command);
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
    if(strcmp(tmp, "&") != 0) {
      args[argc] = (char *)malloc(cmd_line_size);
      strcpy(args[argc], tmp);
      argc++;
    } else background_run = 1;
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
    if(argc == 1 && strcmp(args[0], "clear") == 0){
      printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
      continue;
    }
    pid_t child_pid = fork();
    if(child_pid == 0){
      execvp(args[0], args);
      printf("Command Error.\n");
      exit(0);
    } else {
      pid_t tpid = 0;
      if(background_run != 1)
        while(tpid != child_pid) tpid = wait(&child_status);
      else
        printf("[1] + %d %s Running\n", child_pid, args[0]);
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