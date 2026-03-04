#include <stdio.h>
#include <string.h>
#include "game.h"
#include "game_io.h"
#include "common.h"
#include "viewer.h"

void play_game(GameMenu);
void show_list(ShowScoreListMenu, int);

typedef enum {
	Play=1, ShowScoreList, End
} HighMenu;

typedef enum {
	Alone=1, VersusWithComputer
} GameMenu;

typedef enum {
	All=1, NickName, SpecificRank, SpecificScore, ChangeScorePath, DeleteScore
} ShowScoreListMenu;

int main(void) {
	init_config_file_base_path();
	check_config_folder();
	check_config_file();
	while (1) {
		int high_menu, game_menu, list_menu;

		printf("메뉴\n1. 게임진행\n2. 기록\n3. 종료\n");
		input_number(&high_menu, 1, 3);
		if (high_menu == End) break;
		switch(high_menu){
			case Play:
				printf("\n1. 혼자 하기\n2. 컴퓨터와 대결\n");
				input_number(&game_menu, 1, 2);
				play_game(game_menu);
				break;
			case ShowScoreList :
				printf("\n1. 전체 기록 보기\n2. 특정 닉네임 기록 보기\n3. 특정 순위 이상부터 보기\n4. 특정 기록 보기\n5. 기록 파일 저장소 변경\n6. 기록 삭제\n");
				input_number(&list_menu, 1, 6);
				
				if (ChangeScorePath == list_menu) {
					show_list(list_menu, 0);
					break;
				}
				int number_cnt;
				printf("기록의 게임 플레이 시 지정한 숫자 갯수를 입력해주세요. 2~10\n");
				input_number(&number_cnt, 2, 10);
				show_list(list_menu, number_cnt);
				break;
		}
	}

	return 0;
}

void play_game(GameMenu game_menu) {
	int number_cnt;
	printf("\n플레이 할 숫자의 갯수를 입력해주세요.\n");
	input_number(&number_cnt, 2, 10);
	switch (game_menu) {
		case Alone: {
			play_alone(number_cnt);
			break;
		}
		case VersusWithComputer: {
			int level;
			printf("\n컴퓨터의 레벨을 1~4 사이에서 선택해주세요.\n");
			input_number(&level, 1, 4);
			play_game_with_computer(level, number_cnt);
			break;
		}
	}
}
void show_list(ShowScoreListMenu list_menu, int number_cnt) {
	switch (list_menu) {
		case All:
			view_list_all(number_cnt);
			break;
		case NickName:
			view_nickname_list(number_cnt);
			break;
		case SpecificRank:
			view_specificRank_list(number_cnt);
			break;
		case SpecificScore:
			view_specificScore_list(number_cnt);
			break;
		case ChangeScorePath:
			change_location();
			break;
		case DeleteScore:
			delete_score(number_cnt);
	}
}