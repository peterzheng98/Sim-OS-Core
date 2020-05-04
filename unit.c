//
// Created by Wenxin Zheng on 2020/5/4.
//

#include "libfunc.h"
#include <assert.h>
#include <stdio.h>

void test_check_func(){
  printf("\t Test: !!");
  assert(check_dual_exclamation("!!") == 1);
  printf("\tPassed\n\t Test: !-!");
  assert(check_dual_exclamation("!-!") == 0);
  printf("\tPassed\n\t Test: ~!!");
  assert(check_dual_exclamation("~!!") == 1);
  printf("\tPassed\n\t Test: sedt!!sdgwea");
  assert(check_dual_exclamation("sedt!!sdgwea") == 1);
  printf("\tPassed\n\t Test: w!w!w");
  assert(check_dual_exclamation("w!w!w") == 0);
  printf("\tPassed\n");

}

void test_modify_func_itn(char *current_cmd, char *last_cmd, char* expected_cmd, int limit){
  printf("\t Test: [%s], [%s] -> [%s]\t", current_cmd, last_cmd, expected_cmd);
  char *rcur, *rlast, *rexpected;
  rcur = (char*) malloc(sizeof(char) * limit);
  rlast = (char*) malloc(sizeof(char) * limit);
  rexpected = (char*) malloc(sizeof(char) * limit);
  strcpy(rcur, current_cmd);
  strcpy(rlast, last_cmd);
  strcpy(rexpected, expected_cmd);
  modify_dual_exclamation(rcur, rlast, limit);
  printf("Result: [%s]", rcur);
  assert(strcmp(rcur, expected_cmd) == 0);
  printf("\tPassed\n");
}

void test_modify_func(){
  test_modify_func_itn("!!+!!", "abc", "abc+a", 5);
  test_modify_func_itn("ab!!c", "ab", "ababc", 6);
  test_modify_func_itn("ab!!c", "abdfg", "ababd", 5);
  test_modify_func_itn("!!", "abcde", "abcde", 5);
  test_modify_func_itn("!!+!!", "abc", "abc+abc", 9);
}

int main(){
  printf("Starting Testing...\n");
  test_check_func();
  printf("[TEST] Check func Passed\n");
  test_modify_func();
  printf("[TEST] Modify func Passed\n");
}