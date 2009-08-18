.PHONY: clean

ask: ask.c chunk_string.h chunk_string.o
	$(CC) -o ask ask.c chunk_string.h chunk_string.o

chunk_string.o: chunk_string.h chunk_string.c
	$(CC) -c -o chunk_string.o chunk_string.c

clean:
	$(RM) ask
	$(RM) chunk_string.o
