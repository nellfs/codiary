#include "storage.h"
#include <stdio.h>

int main() {
    StorageError err = storage_init();
    if (err != STORAGE_OK) {
        fprintf(stderr, "storage initialization error: %s\n",
            storage_strerror(err));
        return 1;
    }
    return 0;
}
