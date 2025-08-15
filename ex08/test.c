#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define DEVICE_PATH "/dev/reverse"
#define READ_BUFFER_SIZE 20
#define CORRECT_WRITE_MSG "psrikamo"
#define REVERSE_CORRECT_MSG "omakirsp"

int main() {
    int fd;
    char read_buffer[READ_BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return EXIT_FAILURE;
    }

    // --- Part 1: Test writing the correct string to the device ---
    printf("\n--- Test : Writing correct string ---\n");

    // Write the correct string
    bytes_written = write(fd, CORRECT_WRITE_MSG, strlen(CORRECT_WRITE_MSG));
    if (bytes_written < 0) {
        perror("Failed to write to the device");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Wrote %zd bytes to device: '%s'\n", bytes_written, CORRECT_WRITE_MSG);

    // The driver returns the length of the string on success
    if (bytes_written == strlen(CORRECT_WRITE_MSG)) {
        printf("✅ Write test (correct string) successful! The write operation succeeded as expected.\n");
    } else {
        printf("❌ Write test (correct string) failed! Expected to write %zu bytes, but wrote %zd.\n", strlen(CORRECT_WRITE_MSG), bytes_written);
    }

    // Close the device
    close(fd);

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return EXIT_FAILURE;
    }

    // --- Part 2: Test reading from the device ---
    printf("--- Test : Reading from device ---\n");

    // Attempt to read from the device
    bytes_read = read(fd, read_buffer, READ_BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("Failed to read from the device");
        close(fd);
        return EXIT_FAILURE;
    }

    // Null-terminate the string for safe printing and comparison
    read_buffer[bytes_read] = '\0';
    printf("Read %zd bytes from device: '%s'\n", bytes_read, read_buffer);

    // Verify the read data
    if (strcmp(read_buffer, REVERSE_CORRECT_MSG) == 0) {
        printf("✅ Read test successful! The returned data matches the expected string.\n");
    } else {
        printf("❌ Read test failed! Expected 'omakirsp', got '%s'\n", read_buffer);
    }

    // Close the device
    close(fd);

    return EXIT_SUCCESS;
}
