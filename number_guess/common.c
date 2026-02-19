#include <stdio.h>

void clear_input_buffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

int has_non_ascii(char* str) {
	while (*str) {
		if ((unsigned)*str++ > 127) return 1;
	}
	return 0;
}