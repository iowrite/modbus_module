#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>

#define BUF_SIZE 256


int main() {
    int fd = open("/dev/pts/2", O_RDWR|O_NOCTTY);  
    printf("fd = %d\n", fd);
    if (fd == -1) {
        perror("open serial port");
        return 1;
    }
    

    
    uint8_t read_buf[BUF_SIZE];
    while (1) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        
        struct timeval timeout = { .tv_sec = 1, .tv_usec = 0 };
        if (select(fd + 1, &fds, NULL, NULL, &timeout) > 0) {
            ssize_t n = read(fd, read_buf, BUF_SIZE);
            if (n > 0) {
                for(int i = 0; i < n; i++)
                {
                    printf("%x ", read_buf[i]);
                }
                printf("\n");
            }
        }
    }
    
    close(fd);
    return 0;
}