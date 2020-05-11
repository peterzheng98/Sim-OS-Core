//
// Created by Wenxin Zheng on 2020/5/4.
//

#ifndef SIM_OS_CORE_LIBFUNC_H
#define SIM_OS_CORE_LIBFUNC_H
#include <stdlib.h>
#include <string.h>

int check_dual_exclamation(char *current_cmd) {
  int idx = 0;
  for (; idx < strlen(current_cmd) - 1; ++idx)
    if (current_cmd[idx] == '!' && current_cmd[idx + 1] == '!')
      return 1;
  return 0;
}

void modify_dual_exclamation(char *current_cmd, char *last_cmd, int limit) {
  while (check_dual_exclamation(current_cmd)) {
    int max_size = strlen(current_cmd) >= limit * 2 ? limit * 2 : strlen(current_cmd);
    int idx = 0;
    for (; idx < max_size - 1; ++idx)
      if (current_cmd[idx] == '!' && current_cmd[idx + 1] == '!')
        break;
    if (idx == max_size - 1)
      return;
    int cmd_new_size = strlen(current_cmd) - 2 + strlen(last_cmd) + 1;
    cmd_new_size = cmd_new_size >= limit * 2 ? limit * 2 : cmd_new_size;
    // this is reversed for '\0'
    char *new_cmd = (char *)malloc(sizeof(char) * (cmd_new_size + 1) * 2);
    int new_cmd_idx = 0, cur_cmd_idx = 0, last_cmd_idx = 0;
    for (; new_cmd_idx < cmd_new_size; ++new_cmd_idx) {
      if (cur_cmd_idx < idx)
        new_cmd[new_cmd_idx] = current_cmd[cur_cmd_idx++];
      else if (cur_cmd_idx >= idx + 1)
        new_cmd[new_cmd_idx] = current_cmd[cur_cmd_idx++];
      else if (last_cmd_idx < strlen(last_cmd))
        new_cmd[new_cmd_idx] = last_cmd[last_cmd_idx++];
      else{
        cur_cmd_idx = idx + 2;
        new_cmd_idx--;
      }
    }
    new_cmd[new_cmd_idx] = 0;
    strcpy(current_cmd, new_cmd);
    free(new_cmd);
  }
  char *temp = (char*) malloc((limit + 1) * sizeof(char));
  strncpy(temp, current_cmd, limit);
  temp[limit] = 0;
  free(current_cmd);
  current_cmd = (char*) malloc((limit + 1) * sizeof(char));;
  strcpy(current_cmd, temp);
}

void search_redirect(char **current_cmd_args, char *in_redirect, char *out_redirect, size_t argc){
  if(argc <= 2) return;
  if(strcmp(current_cmd_args[argc - 2], ">") == 0) {
    out_redirect = (char*) malloc(sizeof(char) * (strlen(current_cmd_args[argc - 1]) + 1));
    strcpy(out_redirect, current_cmd_args[argc - 1]);
    free(current_cmd_args[argc - 1]);
    free(current_cmd_args[argc - 2]);
    current_cmd_args[argc - 1] = current_cmd_args[argc - 2] = NULL;
    return;
  }
  if(strcmp(current_cmd_args[argc - 2], "<") == 0) {
    in_redirect = (char*) malloc(sizeof(char) * (strlen(current_cmd_args[argc - 1]) + 1));
    strcpy(in_redirect, current_cmd_args[argc - 1]);
    free(current_cmd_args[argc - 1]);
    free(current_cmd_args[argc - 2]);
    current_cmd_args[argc - 1] = current_cmd_args[argc - 2] = NULL;
    return;
  }
}

#endif // SIM_OS_CORE_LIBFUNC_H
