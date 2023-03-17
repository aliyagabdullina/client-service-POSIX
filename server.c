#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_NAME "/shm_server"
#define SHM_SIZE 4096

int main() {
    // создаем разделяемую память
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    // меняем размер разделяемой памяти
    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    void *ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // считываем случайные числа из разделяемой памяти и выводим их
    int *num_ptr = (int *) ptr;
    for (int i = 0; i < 10; i++) {
        printf("%d ", *num_ptr);
        num_ptr++;
    }
    printf("\n");

    // отсоединяем разделяемую память
    if (munmap(ptr, SHM_SIZE) == -1) {
        perror("munmap");
        return 1;
    }

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    // удаляем разделяемую память
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        return 1;
    }

    return 0;
}
