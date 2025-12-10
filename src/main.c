#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <threads.h>
#include <time.h>

static char *join_arguments(int argc, char **argv) {
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

int main(int argc, char **argv) {
  StorageError err = storage_init();

  if (err != STORAGE_OK) {
    fprintf(stderr, "storage initialization error: %s\n",
            storage_strerror(err));
    return 1;
  }
  if (argc <= 1) {
    fprintf(stderr, "please provide an argument. (use -h to see options)\n");
    return 1;
  }

  if (err != STORAGE_OK) {
    fprintf(stderr, "error creating diary file: %s\n", storage_strerror(err));
    return 1;
  }

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  char filename[16];

  snprintf(filename, sizeof(filename), "%02u-%02u-%02u", tm.tm_mday,
           tm.tm_mon + 1, (tm.tm_year + 1900) % 100);

  char *text = join_arguments(argc, argv);

  storage_create_file(filename, text);

  return 0;
}
