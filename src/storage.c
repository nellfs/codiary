#include "storage.h"
#include <stdlib.h>

const char *storage_strerror(StorageError err) {
    switch (err) {
        case STORAGE_OK:             return "ok";
        case STORAGE_ERR_NOT_FOUND:  return "directory not found";
        case STORAGE_ERR_PERMISSION: return "permission denied";
    }
    return "Unknown error";
}

StorageError storage_init(void) {
    const char *dir;
    StorageError err = storage_get_data_dir(&dir);
    if (err != STORAGE_OK) {
        return err;
    }
    return STORAGE_OK;
};

StorageError storage_get_data_dir(const char **out_dir) {
    const char *home = "HOME";
    const char *location = getenv(home);
    if (!location) {
        return STORAGE_ERR_NOT_FOUND;
    }

    *out_dir = location;
    return STORAGE_OK;
}
