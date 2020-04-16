#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libunix.h"

// read entire file into buffer.  return it.   zero pads to a 
// multiple of 4.
//
// make sure to cleanup!
uint8_t *read_file(unsigned *size, const char *name) {
    uint8_t *buf = 0;
    *size = 0;

    struct stat st;
    if (stat(name, &st) != 0) return 0;
    int fd = open(name, O_RDONLY);
    if (fd < 0) return 0;
    *size = st.st_size + 4;
    buf = (uint8_t*)calloc(*size, sizeof(uint8_t));
    ssize_t bytes_read = read(fd, buf, st.st_size);
    if (bytes_read != st.st_size) {
        free(buf);
	buf = 0;
        *size = 0;
    }
    close(fd);
    return buf;
}
