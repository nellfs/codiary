#include "cli.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *join_arguments(int argc, char **argv) {
  int total_len = 0;

  for (int i = 1; i < argc; i++) {
    total_len += strlen(argv[i]) + 1;
  }

  char *result = malloc(total_len);
  if (!result)
    return NULL;

  int offset = 0;

  for (int i = 1; i < argc; i++) {
    offset += snprintf(result + offset, total_len - offset, "%s%s", argv[i],
                       (i < argc - 1 ? " " : ""));
  }

  return result;
}
