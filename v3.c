#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

const char* filename = "test.txt";

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int thread;
    char* link;
    int line;
} ThreadArg;

size_t getLines(FILE* file);
void getArray(char** arr, size_t size, FILE* file);
void* process(void* arg);
void move_cursor(int row, int col);
void clear_line();

int main() {
    size_t size;
    FILE *file;
    
    printf("\033[H\033[J");
    
    file = fopen(filename, "r");
    if (file == NULL) {
        char err[50];
        snprintf(err, sizeof(err), "Error opening file: %s\n", filename);
        perror(err);
        return 1;
    }

    size = getLines(file);
    printf("Lines collected from %s: %ld\n", filename, size);

    char* links[size];
    for (size_t i = 0; i < size; i++) {
        links[i] = (char*) malloc(100*sizeof(char));
        if (!links[i]) {
            perror("Malloc failed");
            fclose(file);
            return 1;
        }
    }

    getArray(links, size, file);
    fclose(file);

    pthread_t threads[size];
    ThreadArg args[size];

    for (size_t i = 0; i < size; i++) {
        args[i].thread = (int) i+1;
        args[i].link = links[i];
        args[i].line = (int) i+1;
        pthread_create(&threads[i], NULL, process, &args[i]);
    }

    for (size_t i = 0; i < size; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_lock(&print_mutex);
    move_cursor((int) size+1, 1);
    printf("All %ld processes completed", size);
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);

    for (size_t i = 0; i < size; i++) {
        free(links[i]);
    }
    return 0;
}

void move_cursor(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

void clear_line() {
    printf("\033[2K\r");
    fflush(stdout);
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

void getArray(char** arr, size_t size, FILE* file) {
    char link[100];
    size_t i = 0;
    while (fgets(link, 100, file) && i < size) {
        link[strcspn(link, "\n")] = '\0';
        strcpy(arr[i], link);
        i++;
    }
}

void* process(void* arg) {
    ThreadArg* t_arg = (ThreadArg*) arg;
    char cmd[512];

    snprintf(cmd, sizeof(cmd), "yt-dlp -x --audio-format mp3 --embed-thumbnail \"%s\" 2>&1", t_arg->link);

    pthread_mutex_lock(&print_mutex);
    move_cursor(t_arg->line, 1);
    clear_line();

    printf("[Thread %d] Starting: %s", t_arg->thread, t_arg->link);
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);

    FILE* fp = popen(cmd, "r");
    if (!fp) {
        pthread_mutex_lock(&print_mutex);
        move_cursor(t_arg->line, 1);
        clear_line();
        printf("[Thread %d] Failed to process: %s", t_arg->thread, t_arg->link);
        fflush(stdout);
        pthread_mutex_unlock(&print_mutex);
        return NULL;
    }

    char line[300];
    while (fgets(line, sizeof(line), fp)) {
        pthread_mutex_lock(&print_mutex);
        move_cursor(t_arg->line, 1);
        clear_line();
        line[strcspn(line, "\n")] = '\0';
        printf("\033[%d;1H\033[2K%s\n", t_arg->line, line);
        fflush(stdout);
        pthread_mutex_unlock(&print_mutex);
    }

    int status = pclose(fp);
    int success = 0;

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        success = 1;
    }

    pthread_mutex_lock(&print_mutex);
    move_cursor(t_arg->line, 1);
    clear_line();
    if (success) {
        printf("[Thread %d] Success: %s\n", t_arg->thread, t_arg->link);
    } else {
        printf("[Thread %d] Failure: %s\n", t_arg->thread, t_arg->link);
    }

    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);
}
