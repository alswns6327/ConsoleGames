#include <stdio.h>

void reset_input_buffer(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

