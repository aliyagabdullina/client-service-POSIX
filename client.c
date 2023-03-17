#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHM_NAME "/shm_client"
#define SHM_SIZE 4096

int main() {
    srand(time(NULL)); // генератор случайных чисел

    // открываем разделяемую память
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRWXU | S_IRWXG);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
  
    void *ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // генерируем случайные числа и записываем их в разделяемую память
    int *num_ptr = (int *) ptr;
    for (int i = 0; i < 10; i++) {
        int num = rand() % 100;
        *num_ptr = num;
        num_ptr++;
    }

    // отсоединяем разделяемую память
    if (munmap(ptr, SHM_SIZE) == -1) {
        perror("munmap");
        return 1;
    }
  
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
