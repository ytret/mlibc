#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define TEST_FILE "getdelim.tmp"

int main(void) {
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	// We have to open the file for writing and then reading separately,
	// as mlibc doesn't allow us to do both at the same time (yet).
	fp = fopen(TEST_FILE, "w");
	assert(fp);
	fputs("foo\nbar\nbaz\nquux\n", fp);
	fclose(fp);

	fp = fopen(TEST_FILE, "r");
	assert(fp);
	while ((read = getline(&line, &len, fp)) != -1) {
		printf("read line of length %zu, capacity %zu\n", read, len);
	}
	assert(!ferror(fp));
	free(line);
	fclose(fp);

	size_t nchars = 10000;
	fp = fopen(TEST_FILE, "w");
	assert(fp);
	for (int i = 0; i < nchars; i++)
		fputc('a', fp);
	fputc('b', fp);
	fclose(fp);

	line = NULL;
	len = 0;
	fp = fopen(TEST_FILE, "r");
	assert(fp);
	while ((read = getdelim(&line, &len, 'b', fp)) != -1) {
		printf("read line of length %zu, capacity %zu\n", read, len);
		assert(read == nchars + 1);
	}

	assert(len > nchars + 1);
	assert(!ferror(fp));
	assert(feof(fp));

	assert(getdelim(&line, &len, 'b', fp) == -1);
	assert(feof(fp));

	free(line);
	fclose(fp);

	// Delete the file
	unlink(TEST_FILE);
}
