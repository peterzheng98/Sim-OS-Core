/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <fcntl.h>
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
  char in_redirect[MAX_LINE], out_redirect[MAX_LINE];
  while (should_run != 0) {
    memset(in_redirect, 0, sizeof(in_redirect));
    memset(out_redirect, 0, sizeof(out_redirect));
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
    command[strlen(command) - 1] = 0;
    // for history use
    if(check_dual_exclamation(command)){
      if(first_run){
        printf("No commands in history!\n");
        continue;
      } else {
        modify_dual_exclamation(command, last_command, MAX_LINE - 1);
        strcpy(last_command, command);
      }
    } else {
      first_run = 0;
      strcpy(last_command, command);
    }
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
    // Solve redirect to file problem

    if(argc > 2){
      if(strcmp(args[argc - 2], ">") == 0) {
        strcpy(out_redirect, args[argc - 1]);
        free(args[argc - 1]);
        free(args[argc - 2]);
        args[argc - 1] = args[argc - 2] = NULL;
      } else if(strcmp(args[argc - 2], "<") == 0) {
        strcpy(in_redirect, args[argc - 1]);
        free(args[argc - 1]);
        free(args[argc - 2]);
        args[argc - 1] = args[argc - 2] = NULL;
      }
    }
    pid_t child_pid = fork();
    if(child_pid == 0){
      if(strlen(in_redirect) != 0) {
        printf("In redirect to [%s]\n", in_redirect);
        int errno = access(in_redirect, R_OK); // check whether the file exists
        if(errno != 0){
          printf("Input file: unable to open file [%s] err_code[%d].\n", in_redirect, errno);
          exit(-1);
        }
        int InRedirectFD = open(in_redirect, O_RDONLY);
        int fd = dup2(InRedirectFD, STDIN_FILENO);
        if(fd < 0){
          printf("Redirect fail, exit.(in redirect)");
          exit(-1);
        }

      }
      else if(strlen(out_redirect) != 0) {
        printf("Out redirect to [%s]\n", out_redirect);
        int OutRedirectFD = open(out_redirect, O_APPEND | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        int fd = dup2(OutRedirectFD, STDOUT_FILENO);
        if(fd < 0){
          printf("Redirect fail, exit(out redirect)");
          exit(-1);
        }
      }
      execvp(args[0], args);
      printf("Command Error.\n");
      exit(-1);
    } else {
      pid_t tpid = 0;
      if(background_run != 1)
        while(tpid != child_pid) tpid = wait(&child_status);
      else
        printf("[1] + %d %s Running\n", child_pid, args[0]);
      fflush(stderr);

      if(child_status != 0){
        printf("\nExit code: $(%d)", child_status);
        exit(child_status);
      }
      fflush(stdout);
    }
  }

  return 0;
}