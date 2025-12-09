#include "storage.h"
#include <stdio.h>

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

  storage_create_file("michael.txt");
  if (err != STORAGE_OK) {
    fprintf(stderr, "error creating diary file: %s\n", storage_strerror(err));
    return 1;
  }

  // simple value use for now
  if (argv[1]) {
    printf("param");
  }

  return 0;
}
