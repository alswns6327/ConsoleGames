#include "map.h"
#include "rlutil.h"
#include "my_common.h"

Tile tiles[35][70];

void make_map(void) {
	cls();
	printf("\n맵 제작을 멈추시려면 esc를 누르세요.");
	printf("\n상단 좌측부터 한 줄 씩 1(이동 가능) 또는 0(이동 불능)을 입력하여\n아래의 틀 안으로 그려 넣어주세요.");
	printf("\n고치시려면 백스페이스로 지우시면 됩니다.\n");
	for (int i = 0; i < 72; i++) printf("-");
	printf("\n");
	for (int i = 0; i < 35; i++) {
		locate(1, i + 7);
		printf("I");
		locate(72, i + 7);
		printf("I\n");
	}
	for (int i = 0; i < 72; i++) printf("-");
	locate(2, 7);
	int col=1, row=1, key;
	while (1) {
		if ((key = getkey()) == KEY_ESCAPE) {
			cls();
			return;
		}
		if (row == 36) {
			locate(1, 57);
			if (key == KEY_ENTER) {
				printf("맵이 저장되었습니다.                                  ");
				break;
			}
			printf("맵을 완성하려면 enter를 눌러주세요.");
		}
		if ((key == 48 || key == 49) && (row != 36)) {
			printf("%c", key == 48 ? '#' : ' ');
			tiles[row - 1][col - 1].type = key - 48;
			if (col == 70) {
				col = 1;
				row += 1;
				locate(2, row + 6);
			}
			else {
				col += 1;
			}
		}
		if (key == 8 && (col > 1 || row > 1)) {
			if (row == 36) {
				locate(1, 57);
				printf("                                                 ");
			}
			if (col == 1) {
				col = 70;
				row -= 1;
			}
			else {
				col -= 1;
			}
			locate(col + 1, row + 6);
			printf("%c", ' ');
			locate(col + 1, row + 6);
		}
	}
	char filename[21];
	get_string(filename, 3, 20);
	FILE* map_fp = fopen(filename, "wb");
	fwrite(tiles, sizeof(tiles), 1, map_fp);
	fclose(map_fp);
	cls();
}