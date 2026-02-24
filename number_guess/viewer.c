#include <stdio.h>
#include <stdlib.h>
#include "game_io.h"
#include "common.h"

void view_list_all(int number_cnt) {
	int cnt;
	ScoreInfo* scores = read_scores(&cnt, number_cnt);
	if (!scores) {
		printf("해당 숫자 게임의 기록 파일이 존재하지 않습니다.");
		return;
	}
	qsort(scores, cnt, sizeof(ScoreInfo), compare_ascending);
	printf("----------------------------------------------\n");
	for (int i = 0; i < cnt; i++) {
		printf("%d위 %s, 시도 횟수: %d\n날짜: %s\n\n", i + 1, scores[i].nickname, scores[i].tryCount, scores[i].datetime);
	}

	free(scores);
}

void view_nickname_list(int number_cnt) {

}
void view_specificRank_list(int number_cnt) {

}
void view_specificScore_list(int number_cnt) {

}