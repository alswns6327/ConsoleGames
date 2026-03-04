#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_io.h"
#include "common.h"
#include "security.h"

ScoreInfo* get_lists(int number_cnt, int* list_cnt) {
	ScoreInfo* scores = read_scores(list_cnt, number_cnt);
	if (!scores) {
		return NULL;
	}
	qsort(scores, *list_cnt, sizeof(ScoreInfo), compare_ascending);
	return scores;
}

void view_list_all(int number_cnt) {
	int list_cnt;
	ScoreInfo* scores = get_lists(number_cnt, &list_cnt);
	if (list_cnt == 0) {
		printf("\n출력할 기록이 없습니다.\n");
		return;
	}
	printf("----------------------------------------------\n");
	for (int i = 0; i < list_cnt; i++) {
		printf("%d위 %s, 시도 횟수: %d\n날짜: %s\n\n", i + 1, scores[i].nickname, scores[i].tryCount, scores[i].datetime);
	}

	free(scores);
}

void view_nickname_list(int number_cnt) {
	int list_cnt;
	ScoreInfo* scores = get_lists(number_cnt, &list_cnt);
	if (list_cnt == 0) {
		printf("\n출력할 기록이 없습니다.\n");
		return;
	}
	printf("\n검색할 닉네임을 입력해주세요.\n");
	char nickname[22];
	while (1) {
		fgets(nickname, sizeof(nickname), stdin);
		size_t len = strlen(nickname);
		if (nickname[len - 1] != '\n') {
			printf("영어 닉네임은 3~20자 한글 닉네임은 3~10자 사이로 입력해주세요.\n");
			clear_input_buffer();
		}
		else if(len < 3) {
			printf("영어 닉네임은 3~20자 한글 닉네임은 3~10자 사이로 입력해주세요.\n");
		}
		else {
			nickname[len - 1] = '\0';
			break;
		}
	}
	printf("----------------------------------------------\n");
	for (int i = 0; i < list_cnt; i++) {
		if (!strcmp(scores[i].nickname, nickname))
			printf("%d위 %s, 시도 횟수: %d\n날짜: %s\n\n", i + 1, scores[i].nickname, scores[i].tryCount, scores[i].datetime);
	}
	free(scores);
}
void view_specificRank_list(int number_cnt) {
	int list_cnt;
	ScoreInfo* scores = get_lists(number_cnt, &list_cnt);
	if (list_cnt == 0) {
		printf("\n출력할 기록이 없습니다.\n");
		return;
	}
	printf("\n검색할 랭킹을 입력해주세요.\n");
	int ranking;
	input_number(&ranking, 1, list_cnt);
	
	printf("----------------------------------------------\n");
	for (int i = ranking -1; i < list_cnt; i++) {
			printf("%d위 %s, 시도 횟수: %d\n날짜: %s\n\n", i + 1, scores[i].nickname, scores[i].tryCount, scores[i].datetime);
	}

	free(scores);
}
void view_specificScore_list(int number_cnt) {
	int list_cnt;
	ScoreInfo* scores = get_lists(number_cnt, &list_cnt);
	if (list_cnt == 0) {
		printf("\n출력할 기록이 없습니다.\n");
		return;
	}
	int min, max;
	min = scores[0].tryCount, max = scores[list_cnt - 1].tryCount;
	printf("\n검색할 시도 횟수를 입력해주세요.\n");
	int score;
	input_number(&score, min, max);
	
	printf("----------------------------------------------\n");
	for (int i = 0; i < list_cnt; i++) {
		if (scores[i].tryCount > score) break;
		
		if(scores[i].tryCount == score)
			printf("%d위 %s, 시도 횟수: %d\n날짜: %s\n\n", i + 1, scores[i].nickname, scores[i].tryCount, scores[i].datetime);
	}

	free(scores);
}