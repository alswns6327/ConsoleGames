#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include "rlutil.h"
#ifdef _WIN32
	#include <windows.h>
#endif

void set_terminal_size(int cols, int lines) {
	char cmd[50];
#ifdef _WIN32
	system("chcp 65001");
	sprintf(cmd, "mode con: cols=%d lines=%d", cols, lines);
	system(cmd);
#else
	sprintf(cmd, "resize -s %d %d > /dev/null", lines, cols);
	if (system(cmd) != 0) {
		printf("\033[8;%d;%dt", lines, cols);
	}
#endif
}

int main(void) {
	
	set_terminal_size(75, 50);
	char admin_check[5] = "admin";
	int admin_index = 0;
	while (1) {
		if (admin_index == 5) {
			admin_index = 0;
			make_map();
		}
		if (kbhit()) {
			int k = getkey();
			if (admin_check[admin_index] == k) {
				admin_index++;
			}
			else admin_index = 0;
		}
		msleep(100);
	}

	return 0;
}