#include <stdio.h>
#include <string.h>
#include "game.h"

void play_game(int);
void show_list(int);

int main(void) {
	
	while (1) {
		int high_menu, low_menu;
		printf("메뉴\n 1. 게임진행\n 2. 기록 보기\n3. 종료\n");
		scanf("%d", &high_menu);
		if (high_menu == 3) break;
		switch(high_menu){
			case 1 :
				printf("\n1. 혼자 하기\n2. 컴퓨터와 대결\n");
				scanf("%d", &low_menu);
				play_game(low_menu);
				break;
			case 2 :
				printf("\n1. 전체 기록 보기\n2. 특정 닉네임 기록 보기\n3. 특정 순위 이상부터 보기\n4. 특정 횟수 기록 보기\n");
				scanf("%d", &low_menu);
				show_list(low_menu);
				break;
			default :
				printf("\n1~3 사이의 번호를 선택해주세요.\n");
		}
	}

	return 0;
}

void play_game(int menu) {
	int number_cnt;
	char str[30] = "\n숫자의 갯수를 정해주세요.\n";
	while (1) {
		printf("%s", str);
		scanf("%d%*c", &number_cnt);
		if (2 <= number_cnt && number_cnt <= 10) break;
		strcpy(str, "\n2~10 사이의 수로 정해주세요.\n");
	}

	switch (menu) {
		case 1: {
			play_alone(number_cnt);
			break;
		}
		case 2: {
			int level;
			printf("\n컴퓨터의 레벨을 선택해주세요.\n");
			scanf("%d", &level);
			// 컴퓨터와 게임
			break;
		}
		default: {
			printf("\n1~2 사이의 번호를 선택해주세요.\n");
		}
	}
}
void show_list(int menu) {
	switch (menu) {
		case 1:
			// 기록
			break;
		case 2:
			// 기록
			break;
		case 3:
			// 기록
			break;
		case 4:
			// 기록
			break;
		default:
			printf("\n1~4 사이의 번호를 선택해주세요.\n");
	}
}