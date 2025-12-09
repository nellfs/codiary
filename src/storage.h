#pragma once

typedef enum {
    STORAGE_OK = 0,
    STORAGE_ERR_NOT_FOUND,
    STORAGE_ERR_PERMISSION,
} StorageError;

const char *storage_strerror(StorageError err);

StorageError storage_init(void);

StorageError storage_get_data_dir(const char **out_dir);

// TODO: implement storage save function
StorageError storage_save_data_dir(const char **out_dir);
