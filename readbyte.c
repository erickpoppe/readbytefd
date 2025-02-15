#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "hacking.h"

#define FILENAME "/tmp/notas"

int main(int argc, char *argv[]) {
    int fd;
    char note_buffer[100];
    int length;
    unsigned char byte;

    fd = open(FILENAME, O_RDONLY);
    if(fd == -1)
        fatal("in main() while opening file for reading");

    // Move forward 5 bytes from the current position
    off_t posit = lseek(fd, 5, SEEK_CUR);
    printf("[DEBUG] Initial file descriptor position: %ld\n", posit);

    length = 0;
    byte = 0;

    // Read byte by byte until newline
    while (byte != '\n') {
        if (read(fd, &byte, 1) != 1) {
            return -1;
        }
        length++;
    }

    printf("[DEBUG] Length: %d\n", length);
    posit = lseek(fd, 0, SEEK_CUR);  // Current position after reading until newline
    printf("[DEBUG] Final file descriptor position after reading: %ld\n", posit);

    // Make sure we're not seeking before the start of the file
    if (length <= posit) {
        posit = lseek(fd, -length, SEEK_CUR);
        if (posit == -1) {
            perror("lseek error");
            return -1;
        }
    }

    printf("[DEBUG] Final file descriptor position after seeking back: %ld\n", posit);

    // Read the data into note_buffer
    if (read(fd, note_buffer, length) != length) {
        perror("Read error");
        return -1;
    }

    // Null-terminate the buffer to print it correctly as a string
    note_buffer[length] = '\0';

    printf("[DEBUG] Note buffer: %s\n", note_buffer);

    close(fd);
    return 0;
}
