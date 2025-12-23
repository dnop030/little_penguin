#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define DEVICE_PATH "/dev/fortytwo"
#define READ_BUFFER_SIZE 20
#define CORRECT_WRITE_MSG "psrikamo"
#define INCORRECT_WRITE_MSG "wrongmsg"

int main() {
    int fd;
    char read_buffer[READ_BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    printf("--- open device ---\n");
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return EXIT_FAILURE;
    }

    // --- Part 1: Test reading from the device ---
    printf("--- Test 1: Reading from device ---\n");
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
    // if (strcmp(read_buffer, "psrikamo\n") == 0) {
    if (strcmp(read_buffer, CORRECT_WRITE_MSG) == 0) {
        printf("✅ Read test successful! The returned data matches the expected string.\n");
    } else {
        printf("❌ Read test failed! Expected 'psrikamo', got '%s'\n", read_buffer);
    }

    // // --- Part 1.1: Test reading from the device ---
    // printf("--- Test 1.1: Reading from device twice ---\n");
    // // Attempt to read from the device
    // bytes_read = read(fd, read_buffer, READ_BUFFER_SIZE);
    // if (bytes_read <= 0) {
    //     perror("Failed to read from the device");
    //     close(fd);
    //     return EXIT_FAILURE;
    // }

    // // Null-terminate the string for safe printing and comparison
    // read_buffer[bytes_read] = '\0';
    // printf("Read %zd bytes from device: '%s'\n", bytes_read, read_buffer);

    // // Verify the read data
    // // if (strcmp(read_buffer, "psrikamo\n") == 0) {
    // if (strcmp(read_buffer, CORRECT_WRITE_MSG) == 0) {
    //     printf("✅ Read test successful! The returned data matches the expected string.\n");
    // } else {
    //     printf("❌ Read test failed! Expected 'psrikamo', got '%s'\n", read_buffer);
    // }

    // --- Part 2: Test writing the correct string to the device ---
    printf("\n--- Test 2: Writing correct string ---\n");

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

    // --- Part 3: Test writing an incorrect string to the device ---
    printf("\n--- Test 3: Writing incorrect string ---\n");

    // Attempt to write an incorrect string
    bytes_written = write(fd, INCORRECT_WRITE_MSG, strlen(INCORRECT_WRITE_MSG));
    if (bytes_written < 0) {
        // The driver's write function should return -EINVAL, which translates to EFAULT in userspace
        if (errno == EINVAL) {
            printf("✅ Write test (incorrect string) successful! The driver returned the expected error: EINVAL (%d).\n", errno);
        } else {
            perror("Failed to write incorrect string, but received unexpected error");
        }
    } else {
        printf("❌ Write test (incorrect string) failed! The write operation unexpectedly succeeded.\n");
    }

    // Close the device
    printf("--- close device ---\n");
    close(fd);

    return EXIT_SUCCESS;
}
