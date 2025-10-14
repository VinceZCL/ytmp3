#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

const char* filename = "test.txt";

size_t getLines(FILE* file);
char* getArray(char** arr, size_t size, FILE* file);
void* process(char* link);

int main() {
    size_t size;
    FILE *file;

    file = fopen(filename, "r");
    if (file == NULL) {
        char err[50];
        snprintf(err, sizeof(err), "Error opening file: %s\n", filename);
        perror(err);
        return 1;
    }

    size = getLines(file);
    printf("Lines collected from %s: %ld\n\n", filename, size);

    char* links[size];
    for (size_t i = 0; i < size; i++) {
        links[i] = (char*)malloc(100 * sizeof(char));
        if (!links[i]) {
            perror("Malloc failed");
            fclose(file);
            return 1;
        }
    }

    getArray(links, size, file);
    for (size_t i = 0; i < size; i++) {
        process(links[i]);
    }

    for (size_t i = 0; i < size; i++) {
        free(links[i]);
    }
    fclose(file);
    return 0;
}

size_t getLines(FILE* file) {
    size_t lines = 0;
    char c[100];

    while (fgets(c, 100, file)) {
        if (strlen(c) > 1) {
            lines++;
        }
    }

    rewind(file);
    return lines;
}

char* getArray(char** arr, size_t size, FILE* file) {
    char link[100];
    size_t i = 0;
    while (fgets(link, 100, file) && i < size) {
        link[strcspn(link, "\n")] = '\0';
        strcpy(arr[i], link);
        i++;
    }
    return (char*) arr;
}

void* process(char* link) {
    printf("Link is being processed: %s\n", link);
    char command[512];
    snprintf(command, sizeof(command), "yt-dlp -x --audio-format mp3 --embed-thumbnail %s", link);
    int status = system(command);
    if (status == 0) {
        printf("Processing succeed: %s\n\n", link);
    } else {
        printf("Processing failed: %s\n\n", link);
    }
}
