#include<stdlib.h>
#include<stdio.h>
#include "chunk_string.h"

int main() {
	string_chunk* current = NULL;
	char answer;
	FILE* tty = NULL;

	tty = fopen("/dev/tty", "r+");
	current = new_chunk_string();

	while (!feof(stdin)) {
		if (chunk_readline(stdin, current) > 0) {
			fputc('"', tty);
			chunk_printline(tty, current);
			fputs("\"? ", tty);
			answer = fgetc(tty);
			if(answer == 'y' || answer == 'Y') {
				chunk_printline(stdout, current);
				fputc('\n', stdout);
			}
		}
	}
	fclose(tty);
}
