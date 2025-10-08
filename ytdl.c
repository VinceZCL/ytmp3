#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>

void *download(void *link);
int burner();
void hide();

int main() {

	hide();
	int c = burner();

	if (c != -1) {
		if (c != 0) {
			char links[c][256];

			FILE *f = fopen("yt.txt", "r");
			int i = 0;
				char link[256];
				while ( fgets(link, 256, f) ) {
					strcpy(link, strtok(link, "&"));
					strcpy(links[i], link);
					i++;
				}

			int nlinks = sizeof(links) / sizeof(links[0]);

			for (int j=0;j<nlinks;j++) {
				pthread_create(NULL, NULL, download, links[j]);
			}
			pthread_exit(NULL);

			fclose(f);
		} else {
			printf("No links");
			return -1;
		}
	} else {
		printf("File error");
		return -1;
	}
}

int burner() {
	char burner[256];
	FILE *bf = fopen("yt.txt", "r");
	if (bf) {
		int c = 0;
		while (fgets(burner, 256, bf)) {
			c++;
		}
		fclose(bf);
		return c;
	} else {
		return -1;
	}
}

void *download(void *link) {
	char command[256];
	strcpy(command, "\"C:/Program Files/Git/git-bash.exe\" -c 'yt-dlp -x --audio-format mp3 --embed-thumbnail ");
	strcat(command, link);
	strcat(command, "'");
	system(command);
}

void hide() {
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
}
