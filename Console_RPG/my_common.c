#include "my_common.h"
#include <stdio.h>
#include <string.h>

void clear_input_buf(void) {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void get_string(char* buf, int min, int max) {
	size_t len;
	while (1) {
		printf("%d~%d 사이의 길이로 입력해주세요.", min, max);
		fgets(buf, max, stdin);
		len = strlen(buf);
		if (buf[len - 1] != '\n')
			clear_input_buf();
		else if (len <= max || len >= min) {
			buf[len - 1] = '\0';
			break;
		}
	}
}