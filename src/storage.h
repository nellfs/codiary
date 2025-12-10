#pragma once

typedef enum {
    STORAGE_OK,
    STORAGE_ERR_NOT_FOUND,
    STORAGE_ERR_MALLOC_FAIL,
    STORAGE_ERR_PATH_TOO_LONG,
    STORAGE_ERR_INVALID_PATH,
    STORAGE_ERR_PERMISSION,
    STORAGE_ERR_INVALID_ARGUMENT,
    STORAGE_ERR_NO_PERMISSION,
    STORAGE_FAILED_TO_CREATE_DIRECTORY,
} StorageError;

const char *storage_strerror(StorageError err);

StorageError storage_init(void);

StorageError storage_alloc_app_dir(const char **out_dir);

StorageError storage_create_file(const char *filename, const char *text);

// StorageError storage_save_file(const char **out_dir);

void storage_free_dir(const char *dir);
