#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

struct stat stat_buffer;

int main(int argc, char *argv[]) {
    if (lstat(argv[1], &stat_buffer) < 0) {     //  stat() segue os links
        fprintf(stderr, "Error in stat: %s\n", argv[1]);
        exit(1);
    }
    char *type;
    if (S_ISREG(stat_buffer.st_mode))
        type = "regular";
    else if (S_ISDIR(stat_buffer.st_mode))
        type = "directory";
    else if (S_ISLNK(stat_buffer.st_mode))
        type = "link";
    else if (S_ISCHR(stat_buffer.st_mode))
        type = "char device";
    else if (S_ISBLK(stat_buffer.st_mode))
        type = "block device";
    else if (S_ISLNK(stat_buffer.st_mode))
        type = "link";
    else
        type ="unknown";
    printf("%s is a %s\n", argv[1], type);
}