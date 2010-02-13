#include "chunk_string.h"

int chunk_readline_count(FILE* stream, string_chunk* chunk, int read) {
	char temp;
	int i=0;

	for (i=0; i < CHUNK_SIZE; i++) {
		if (feof(stream)) {
			chunk->value[i]='\0';
			return read;
		}
		temp = fgetc(stream);
		if (temp == EOF || temp == '\0' || temp == '\n') {
			chunk->value[i]='\0';
			return read;
		}
		read++;
		chunk->value[i]=temp;
	}

	if (feof(stream)) {
		chunk->next = NULL;
		return read;
	}

	temp = fgetc(stream);
	if (temp == EOF || temp == '\0' || temp == '\n') {
		chunk->next = NULL;
		return read;
	}

	ungetc(temp, stream);
	if (chunk->next == NULL) {
		/* Only allocate a new chunk if we don't already have one to blow over. */
		/* With this, this ends up being almost vector like. */
		chunk->next = malloc(sizeof(string_chunk));
		chunk->next->next = NULL;
	}
	if (chunk->next == NULL) {
		return read;
	}

	return chunk_readline_count(stream, chunk->next, read);
}

int chunk_readline(FILE* stream, string_chunk* chunk) {
	return chunk_readline_count(stream, chunk, 0);
}

void chunk_printline(FILE* stream, string_chunk* chunk) {
	int i=0;
	for (i=0; i < CHUNK_SIZE; i++) {
		if (chunk->value[i] == '\0') {
			return;
		}
		fputc(chunk->value[i], stream);
	}

	if (chunk->next != NULL) {
		chunk_printline(stream, chunk->next);
	}
}

string_chunk* new_chunk_string() {
	string_chunk* ret = NULL;
	ret = malloc(sizeof(string_chunk));
	ret->next = NULL;

	return ret;
}

void chunk_string_copy(string_chunk* src, string_chunk* dst) {
	string_chunk* currentSrc = src;
	string_chunk* currentDst = dst;
	int i;

	while (currentSrc != NULL) {
		for (i = 0; i < CHUNK_SIZE; i++) {
			currentDst->value[i] = currentSrc->value[i];
			if (currentDst->value[i] == '\0') goto found_null;
		}

		if (currentSrc->next != NULL && currentDst->next == NULL) {
			currentDst->next = malloc(sizeof(string_chunk));
			currentDst->next->next = NULL;
		} else if (currentSrc->next == NULL && currentDst->next != NULL) {
			/* Here we have currentSrc ending on a multiple of CHUNK_SIZE*/
			/* This means src is delimited by a NULL next. */
			/* currentDst, though, is bigger than that. */
			/* In order to fix this, we put a null character as the first char of currentDst->next */
			/* The only other option would have been to orphan the rest of the string */
			currentDst->next->value[0] = '\0';
		}

		currentSrc = currentSrc->next;
		currentDst = currentDst->next;
	}
found_null:
	return;
}
