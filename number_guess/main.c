#include <stdio.h>
#include <string.h>
#include "game.h"
#include "game_io.h"
#include "common.h"
#include "viewer.h"

void play_game(GameMenu);
void show_list(ShowScoreListMenu);

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
		HighMenu high_menu;
		GameMenu game_menu;
		ShowScoreListMenu list_menu;

		printf("메뉴\n1. 게임진행\n2. 기록\n3. 종료\n");
		scanf("%d%*c", &high_menu);
		if (high_menu == End) break;
		switch(high_menu){
			case Play:
				printf("\n1. 혼자 하기\n2. 컴퓨터와 대결\n");
				while (1) {
					scanf("%d%*c", &game_menu);
					if (game_menu == Alone || game_menu == VersusWithComputer) break;
					else printf("\n1~2 중 하나를 선택해주세요.\n");
				}
				play_game(game_menu);
				break;
			case ShowScoreList :
				printf("\n1. 전체 기록 보기\n2. 특정 닉네임 기록 보기\n3. 특정 순위 이상부터 보기\n4. 특정 기록 보기\n5. 기록 파일 저장소 변경\n6. 기록 삭제\n");
				while (1) {
					scanf("%d%*c", &list_menu);
					if (All <= list_menu && list_menu <= DeleteScore) break;
					else printf("\n1~6 중 하나를 선택해주세요.\n");
				}
				if (ChangeScorePath == list_menu) {
					show_list(list_menu, 0);
					break;
				}
				int number_cnt;
				printf("기록의 게임 플레이 시 지정한 숫자 갯수를 입력해주세요. 2~10\n");
				while (1) {
					scanf("%d%*c", &number_cnt);
					if (2 <= number_cnt && number_cnt <= 10) break;
					else printf("\n2~10 중 하나를 선택해주세요.\n");
				}
				show_list(list_menu, number_cnt);
				break;
			default :
				printf("\n1~3 사이의 번호를 선택해주세요.\n");
		}
	}

	return 0;
}

void play_game(GameMenu game_menu) {
	int number_cnt;
	char str[42] = "\n플레이 할 숫자의 갯수를 입력해주세요.\n";
	while (1) {
		printf("%s", str);
		scanf("%d%*c", &number_cnt);
		if (2 <= number_cnt && number_cnt <= 10) break;
		strcpy(str, "\n2~10 사이의 수로 정해주세요.\n");
	}

	switch (game_menu) {
		case Alone: {
			play_alone(number_cnt);
			break;
		}
		case VersusWithComputer: {
			int level;
			printf("\n컴퓨터의 레벨을 1~4 사이에서 선택해주세요.\n");
			while (1) {
				scanf("%d%*c", &level);
				if (1 <= level && level <= 4) break;
				else printf("\n1~4 중 하나를 선택해주세요.\n");
			}
			play_game_with_computer(level, number_cnt);
			break;
		}
		default: {
			printf("\n1~2 사이의 번호를 선택해주세요.\n");
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
		default:
			printf("\n1~4 사이의 번호를 선택해주세요.\n");
	}
}