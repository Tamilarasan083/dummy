#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

void countFilesAndDirectories(const char *path, int *fileCount, int *dirCount) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp))) {
        // Ignore the special entries "." and ".."
        if (entry->d_type == DT_DIR) {
            if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || 
                (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
                continue;
            }
            (*dirCount)++;
            // Construct new path from our base path
            char newPath[1024];
            snprintf(newPath, sizeof(newPath), "%s/%s", path, entry->d_name);
            countFilesAndDirectories(newPath, fileCount, dirCount); // Recursively count in subdirectories
        } else {
            (*fileCount)++;
        }
    }
    closedir(dp);
}

void listFilesAndDirectories(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp))) {
        // Ignore the special entries "." and ".."
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || 
            (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
            continue;
        }
        printf("%s/%s\n", path, entry->d_name);
        if (entry->d_type == DT_DIR) {
            // Construct new path from our base path
            char newPath[1024];
            snprintf(newPath, sizeof(newPath), "%s/%s", path, entry->d_name);
            listFilesAndDirectories(newPath); // Recursively list subdirectories
        }
    }
    closedir(dp);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        return 1;
    }

    int fileCount = 0, dirCount = 0;
    countFilesAndDirectories(argv[1], &fileCount, &dirCount);

    printf("Total files: %d\n", fileCount);
    printf("Total directories: %d\n", dirCount);

    printf("Listing all files and directories:\n");
    listFilesAndDirectories(argv[1]);

    return 0;
}
