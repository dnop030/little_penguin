// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <errno.h>

// #define TEST_FILE "/sys/kernel/debug/fortytwo/foo"
// #define PAGE_SIZE 4096

// void print_result(const char *test_name, int success) {
//     printf("[%s] %s\n", success ? " PASS " : " FAIL ", test_name);
// }

// int verify_data(char *sent, char *received, size_t size) {
//     return memcmp(sent, received, size) == 0;
// }

// int main() {
//     int fd;
//     char *write_buf = malloc(PAGE_SIZE + 10);
//     char *read_buf = malloc(PAGE_SIZE + 10);
//     ssize_t ret;

//     // 1. ทดสอบอ่านเบื้องต้น (ยังไม่มีการเขียน)
//     printf("--- Test 1: Initial Read ---\n");
//     fd = open(TEST_FILE, O_RDONLY);
//     if (fd < 0) { perror("Open failed"); return 1; }

//     memset(read_buf, 0, 1024);
//     ret = read(fd, read_buf, 1024);
//     // เนื่องจากยังไม่ได้เขียนอะไร foo_buff จะเป็นค่าว่างที่ได้จาก kmalloc
//     printf("Read %zd bytes from empty buffer\n", ret);
//     print_result("Initial Read", ret >= 0);
//     close(fd);

//     // 2. เขียน 1024 bytes และอ่าน 1024 bytes มา verify
//     printf("\n--- Test 2: 1024 Bytes Write/Read ---\n");
//     fd = open(TEST_FILE, O_RDWR);
//     for(int i=0; i<1024; i++) write_buf[i] = (i % 26) + 'A';

//     write(fd, write_buf, 1024);
//     // lseek(fd, 0, SEEK_SET); // กลับไปจุดเริ่มไฟล์
//     // memset(read_buf, 0, 1024);
//     ret = read(fd, read_buf, 1024);

//     print_result("1024 Bytes Verify", verify_data(write_buf, read_buf, 1024));
//     close(fd);

//     // 3. เขียน 1 Page Size (4096) และอ่านมา verify
//     printf("\n--- Test 3: Page Size Write/Read ---\n");
//     fd = open(TEST_FILE, O_RDWR);
//     for(int i=0; i<PAGE_SIZE; i++) write_buf[i] = (i % 10) + '0';

//     write(fd, write_buf, PAGE_SIZE);
//     // lseek(fd, 0, SEEK_SET);
//     memset(read_buf, 0, PAGE_SIZE);
//     ret = read(fd, read_buf, PAGE_SIZE);

//     print_result("Page Size Verify", verify_data(write_buf, read_buf, PAGE_SIZE));
//     close(fd);

//     // 4. เขียน Page Size + 1 ตัว (ควรจะติด Error หรือเขียนได้แค่ Page Size)
//     // ตามโค้ด kernel: if ((*offset) + count >= PAGE_SIZE) return -ENOMEM;
//     printf("\n--- Test 4: Page Size + 1 (Boundary Test) ---\n");
//     fd = open(TEST_FILE, O_RDWR);
//     ret = write(fd, write_buf, PAGE_SIZE + 1);
//     if (ret < 0 && errno == ENOMEM) {
//         printf("Correctly handled overflow: %s\n", strerror(errno));
//         print_result("Boundary Protection", 1);
//     } else {
//         printf("Result: %zd (Expected -ENOMEM)\n", ret);
//         print_result("Boundary Protection", 0);
//     }
//     close(fd);

//     free(write_buf);
//     free(read_buf);
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>

#define DEVICE_PATH "/sys/kernel/debug/fortytwo/foo"
#define PAGE_SIZE 4096

void test_step(const char* msg) {
    printf("\n--- Step %s ---\n", msg);
}

int main() {
    int fd;
    char *read_buf;
    char *write_buf;

    // เตรียม buffer ขนาดใหญ่สำหรับทดสอบ
    read_buf = malloc(PAGE_SIZE * 2);
    write_buf = malloc(PAGE_SIZE);

    // --- 1. อ่านโดยเตรียม buffer ขนาด 2 page size โดยที่ไม่ได้เขียนอะไรลงไป ---
    test_step("1: Read from empty buffer (2 PAGE_SIZE)");
    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) { perror("Open failed"); return 1; }

    ssize_t ret = read(fd, read_buf, PAGE_SIZE * 2);
    printf("Read returned: %zd bytes (Expected: 0 or initial data)\n", ret);
    close(fd);


    // --- 2. เขียน 1 page size และอ่านมา verify ---
    test_step("2: Write/Read 1 PAGE_SIZE");
    fd = open(DEVICE_PATH, O_RDWR);
    memset(write_buf, 'A', PAGE_SIZE); // เติม 'A' เต็ม page

    ret = write(fd, write_buf, PAGE_SIZE);
    printf("Written: %zd bytes\n", ret);

    lseek(fd, 0, SEEK_SET); // กลับไปจุดเริ่มไฟล์
    memset(read_buf, 0, PAGE_SIZE);
    ret = read(fd, read_buf, PAGE_SIZE);
    printf("Read back: %zd bytes\n", ret);

    if (memcmp(write_buf, read_buf, PAGE_SIZE) == 0) printf("Verify: SUCCESS\n");
    else printf("Verify: FAILED\n");
    close(fd);


    // --- 3. เขียน 1/8 page size และอ่านมา verify ---
    test_step("3: Write/Read 1/8 PAGE_SIZE");
    fd = open(DEVICE_PATH, O_RDWR);
    int small_size = PAGE_SIZE / 8;
    memset(write_buf, 'B', small_size);

    ret = write(fd, write_buf, small_size);
    printf("Written: %zd bytes\n", ret);

    lseek(fd, 0, SEEK_SET);
    memset(read_buf, 0, PAGE_SIZE);
    ret = read(fd, read_buf, small_size);

    if (memcmp(write_buf, read_buf, small_size) == 0) printf("Verify: SUCCESS\n");
    else printf("Verify: FAILED\n");
    close(fd);


    // --- 4. ทดสอบ Mutex ด้วย 2 Processes ---
    test_step("4: Concurrency Test (2 Processes)");
    if (fork() == 0) {
        // Child Process
        int fd_c = open(DEVICE_PATH, O_RDWR);
        char *buf_c = malloc(PAGE_SIZE);
        memset(buf_c, 'C', PAGE_SIZE);
        printf("[Child] Writing 'C'...\n");
        write(fd_c, buf_c, PAGE_SIZE);
        printf("[Child] Done.\n");
        close(fd_c);
        exit(0);
    } else {
        // Parent Process
        int fd_p = open(DEVICE_PATH, O_RDWR);
        char *buf_p = malloc(PAGE_SIZE);
        memset(buf_p, 'P', PAGE_SIZE);
        printf("[Parent] Writing 'P'...\n");
        write(fd_p, buf_p, PAGE_SIZE);
        printf("[Parent] Done.\n");

        wait(NULL); // รอ Child ทำงานเสร็จ

        lseek(fd_p, 0, SEEK_SET);
        read(fd_p, buf_p, PAGE_SIZE);
        printf("[Final Result] Data in foo starts with: %c\n", buf_p[0]);
        printf("(If Mutex works, data should be all 'C' or all 'P', not a mix)\n");
        close(fd_p);
    }

    free(read_buf);
    free(write_buf);
    return 0;
}
