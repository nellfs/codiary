#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

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

  // simple value use for now

  char *text = join_arguments(argc, argv);
  storage_create_file(text);

  return 0;
}

static char *join_arguments(int argc, char **argv) {
    int total_len = 0;

    for (int i = 1; i < argc; i++) {
        total_len += strlen(argv[i]) + 1;
    }

    char *result = malloc(total_len);
    if (!result) return NULL;

    int offset = 0;

    for (int i = 1; i < argc; i++) {
        offset += snprintf(result + offset,
                           total_len - offset,
                           "%s%s",
                           argv[i],
                           (i < argc - 1 ? " " : ""));
    }

    return result;
}
