#include <time.h>
#include <stdio.h>

void datetime_get_iso8601(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%dT%H:%M:%SZ", t);
}


int datetime_get_current_date(char *buffer, size_t size) {
    if (size < 16) {
        if (size > 0) buffer[0] = '\0';
        return -1;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    snprintf(buffer, size, "%02u-%02u-%02u",
             tm.tm_mday, tm.tm_mon + 1, (tm.tm_year + 1900) % 100);
    return 0;
}
