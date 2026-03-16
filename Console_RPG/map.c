#include "map.h"
#include "rlutil.h"

Tile tiles[50][30];

void make_map(void) {
	cls();
	printf("\n맵 제작을 멈추시려면 esc를 누르세요.");
	printf("\n상단 좌측부터 한 줄 씩 1(이동 가능) 또는 0(이동 불능)을 입력하여 아래의 틀 안으로 그려 넣어주세요.");
	printf("\n고치시려면 백스페이스로 지우시면 됩니다.\n");
	for (int i = 0; i < 52; i++) printf("-");
	printf("\n");
	for (int i = 0; i < 30; i++) {
		locate(1, i + 6);
		printf("I");
		locate(52, i + 6);
		printf("I\n");
	}
	for (int i = 0; i < 52; i++) printf("-");
	locate(2, 6);
	int col=1, row=1, key;
	while ((key = getkey()) != KEY_ESCAPE) {
		if (col == 50 && row == 30) {
			locate(1, 57);
			if (key == KEY_ENTER) {
				printf("맵이 저장되었습니다.                                  ");
				break;
			}
			printf("맵을 완성하려면 enter를 눌러주세요.");
		}
		if ((key == 48 || key == 49) && (col != 50 || row != 30)) {
			printf("%c", key == 48 ? '#' : ' ');
			if (col == 50) {
				col = 1;
				row += 1;
				locate(2, row + 5);
			}
			else {
				col += 1;
			}
		}
		if (key == 8 && (col > 1 || row > 1)) {
			locate(1, 57);
			printf("                                                 ");
			if (col == 1) {
				col = 50;
				row -= 1;
			}
			else {
				col -= 1;
			}
			locate(col + 1, row + 5);
			printf("%c", ' ');
			locate(col + 1, row + 5);
		}
	}
}