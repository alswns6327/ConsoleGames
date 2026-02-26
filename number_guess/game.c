#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "common.h"
#include "game_io.h"
#include "security.h"
#include "game.h"

typedef enum {
	GAME_ANSWER_WRONG,
	GAME_ANSWER_CORRECT
} GameAnswer;

typedef struct {
	int strike_cnt;
	int ball_cnt;
	GameAnswer answer;
} GuessResult;

static char* make_game_numbers(int number_cnt);
static char* read_number(char* user_input_numbers, int number_cnt);
static GuessResult make_answer(char* game_numbers, char* user_input_numbers, int number_cnt);
static void play_user(int number_cnt, int* tryCount);
static void play_computer(int number_cnt, int* tryCount);
static void make_answer_yes_no(char* answer);
static void game(char* player, int number_cnt, int* tryCount);
static void make_numbers_list(char** list, char* temp, int* visited, int* row, int depth, int number_cnt);

static char* make_game_numbers(int number_cnt) {
	char numbers[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	char* game_numbers = (char*)malloc(sizeof(char) * number_cnt);
	srand((unsigned)time(NULL));
	for (int i = 9; i > 0; i--) {
		int j = rand() % (i + 1);
		char temp = numbers[i];
		numbers[i] = numbers[j];
		numbers[j] = temp;
	}
	for (int i = 0; i < number_cnt; i++) game_numbers[i] = numbers[i] - 48;

	return game_numbers;
}

static char* read_number(char* user_input_numbers, int number_cnt) {
	int c, i=0;
	char pos[10] = { 0 };
	while ((c = getchar()) != '\n' && i < number_cnt) {
		if (c == ' ') continue;
		if (c > 57 || 48 > c) {
			clear_input_buffer();
			break;
		}
		c -= 48;
		if (pos[c]) { // 중복되는 경우
			clear_input_buffer();
			break; 
		}
		user_input_numbers[i++] = c;
		pos[c]++;
	}
	if (i < number_cnt) return NULL;
	if (c != '\n') {
		clear_input_buffer();
		return NULL;
	}
	return user_input_numbers;
}

static GuessResult make_answer(char* game_numbers, char* user_input_numbers, int number_cnt) {
	GuessResult result = { 0, 0, GAME_ANSWER_WRONG };
	char pos[10];
	for (int i = 0; i < 10; i++) pos[i] = - 1;
	for (int i = 0; i < number_cnt; i++) pos[game_numbers[i]] = i;

	for (int i = 0; i < number_cnt; i++) {
		if (pos[user_input_numbers[i]] != -1) {
			if (pos[user_input_numbers[i]] == i) result.strike_cnt++;
			else result.ball_cnt++;
		}
	}
	if (result.strike_cnt == number_cnt) result.answer = GAME_ANSWER_CORRECT;
	return result;
}

static void play_user(int number_cnt, int* tryCount) {
	char* game_numbers = make_game_numbers(number_cnt);
	char* user_input_numbers = (char*)malloc(sizeof(char) * number_cnt);
	*tryCount = 0;
	GuessResult result;
	while (1) {
		printf("숫자 입력 : ");
		if (read_number(user_input_numbers, number_cnt) == NULL) {
			printf("\n%d개의 숫자를 중복 없이 입력해주세요.\n", number_cnt);
			continue;
		}
		result = make_answer(game_numbers, user_input_numbers, number_cnt);
		(*tryCount)++;
		if (result.answer == GAME_ANSWER_CORRECT) {
			printf("\n축하드립니다 %d번 만에 정답을 맞추셨습니다.\n", *tryCount);
			break;
		}
		printf("%dS %dB\n", result.strike_cnt, result.ball_cnt);
	}
	free(game_numbers);
	free(user_input_numbers);
}

void play_alone(int number_cnt) {
	char regame_check[4];
	char save_check[5];
	int tryCount;
	printf("\n숫자 게임에 오신 것을 환영합니다.\n모드 : 혼자\n숫자 갯수: %d\n\n숫자는 붙여서 써도 되고 띄워서 써도 됩니다.\nex) 123 or 1 2 3 or 12 3 모두 동일합니다.\n", number_cnt);
	while (1) {
		play_user(number_cnt, &tryCount);
		printf("\n게임 기록을 저장하시려면 yes를 입력 아니라면 엔터를 눌러주세요.\n");
		fgets(save_check, sizeof(save_check), stdin);
		if (save_check[strlen(save_check) - 1] != '\n') clear_input_buffer();
		else save_check[strlen(save_check) - 1] = '\0';
		if (!strcmp(save_check, "yes")) {
			printf("게임 기록과 함께 저장될 닉네임을 입력해주세요.\n");
			char nickname[22];
			while (1) {
				fgets(nickname, sizeof(nickname), stdin);
				size_t len = strlen(nickname);
				if (nickname[len - 1] != '\n') {
					printf("영어 닉네임은 3~20자 한글 닉네임은 3~10자 사이로 입력해주세요.\n");
					clear_input_buffer();
				}
				else if (len < 4) {
					printf("영어 닉네임은 3~20자 한글 닉네임은 3~10자 사이로 입력해주세요.\n");
				}
				else {
					nickname[len - 1] = '\0';
					break;
				}
			}
			printf("10자 이상 20자 이하의 영문/숫자로 된 password를 입력해주세요.\n");
			char password[22];
			while (1) {
				fgets(password, sizeof(password), stdin);

				size_t len = strlen(password);
				if (password[len - 1] != '\n') {
					printf("10자 이상 20자 이하의 영문/숫자로 된 password를 입력해주세요.\n");
					clear_input_buffer();
					continue;
				}
				if (has_non_ascii(password) || len < 11) {
					printf("10자 이상 20자 이하의 영문/숫자로 된 password를 입력해주세요.\n");
					continue;
				}

				password[len - 1] = '\0';
				break;
			}
			ScoreInfo scoreInfo = { 0 };
			scoreInfo.tryCount = tryCount;
			scoreInfo.numberCnt = number_cnt;
			scoreInfo.password = basic_hash(password);
			strcpy(scoreInfo.nickname, nickname);
			scoreInfo.time = time(NULL);
				
			strftime(scoreInfo.datetime, sizeof(scoreInfo.datetime), "%Y-%m-%d %H:%M:%S", localtime(&scoreInfo.time));
			if (!save_score(scoreInfo)) printf("\n저장에 실패하였습니다.\n");
		}
		printf("\n게임을 한 번 더 하시려면 're'를 입력 끝내시려면 엔터를 눌러주세요.\n");
		fgets(regame_check, sizeof(regame_check), stdin);
		if (regame_check[strlen(regame_check) - 1] != '\n') clear_input_buffer();
		else regame_check[strlen(regame_check) - 1] = '\0';
		if (!strcmp(regame_check, "re")) {
			char str[42] = "\n플레이 할 숫자의 갯수를 입력해주세요.\n";
			while (1) {
				printf("%s", str);
				scanf("%d%*c", &number_cnt);
				if (2 <= number_cnt && number_cnt <= 10) break;
				strcpy(str, "\n2~10 사이의 수로 정해주세요.\n");
			}
			continue;
		} else {
			break;
		}
	}
}

static void play_computer(int number_cnt, int* tryCount) {
	int size = 1;
	for (int i = 0; i < number_cnt; i++) {
		size *= 10 - i;
	}
	char** numbers_list = (char**)malloc(sizeof(char*) * size);
	if (!numbers_list) {
		*tryCount = -1;
		return;
	}
	for (int i = 0; i < size; i++) {
		numbers_list[i] = (char*)malloc(sizeof(char) * number_cnt);
		if (!numbers_list[i]) {
			*tryCount = -1;
			for (int j = 0; j < i; j++) free(numbers_list[j]);
			free(numbers_list);
			return;
		}
	}
	
	int row = 0;
	int visited[10] = { 0 };
	char temp[10];
	make_numbers_list(numbers_list, temp, visited, &row, 0, number_cnt);
	//while (1) {
	//	int random_idx = big_rand() % size;
	//	for (int i = 0; i < number_cnt; i++) printf("%d", numbers_list[random_idx][i]);
	//}
}

static void make_numbers_list(char** list, char* temp, int* visited, int* row, int depth, int number_cnt) {
	if (depth == number_cnt) {
		for (int i = 0; i < number_cnt; i++) list[*row][i] = temp[i];
		(*row)++;
		return;
	}
	for (int i = 0; i < 10; i++) {
		if (!visited[i]) {
			visited[i] = 1;
			temp[depth] = i;
			make_numbers_list(list, temp, visited, row, depth + 1, number_cnt);
			visited[i] = 0;
		}
	}
}

void play_game_with_computer(int level, int number_cnt) {
	printf("\n대결 모드에 오신 것을 환영합니다.\nlevel: %d, 숫자 갯수: %d", level, number_cnt);
	printf("\n응답은 2s 1b과 같이 답해주시면 되며 스트라이크의 갯수가 게임의 숫자 갯수와 동일한 경우 맞춘 것으로 간주합니다.\n");
	printf("\n먼저 맞추시겠어요?(yes or no)\n");
	char answer[5];
	while (1) {
		make_answer_yes_no(answer);
		char* play_order[2];
		int userTryCount;
		int computerTryCount;
		if (!strcmp(answer, "yes")) {
			printf("\n네 그럼 먼저 출제하겠습니다.\n모드 : 대결\n숫자 갯수: %d\n\n숫자는 붙여서 써도 되고 띄워서 써도 됩니다.\nex) 123 or 1 2 3 or 12 3 모두 동일합니다.\n", number_cnt);
			play_order[0] = "user";
			play_order[1] = "computer";
		}
		else {
			printf("\n네 그럼 먼저 시작하겠습니다.\n");
			play_order[0] = "computer";
			play_order[1] = "user";
		}
		int index = 0;
		game(play_order[index++], number_cnt, &userTryCount);
		game(play_order[index], number_cnt, &computerTryCount);

		if (userTryCount == -1 || computerTryCount == -1) {
			printf("\n오류가 발생하였습니다.\n개발자에게 문의 부탁드립니다. 죄송합니다");
			exit(1);
		}
		else if (userTryCount > computerTryCount) printf("\n축하드립니다. %d턴 만큼 더 빠르게 문제를 맞추셨습니다.", userTryCount - computerTryCount);
		else if (userTryCount < computerTryCount) printf("\n%d턴 만큼의 차이로 제가 이겼네요.", userTryCount - computerTryCount);
		else printf("\n아쉽게도 비겼습니다.");

		printf("\n한 번 더 해보시겠습니까?(yes or no)\n");
		make_answer_yes_no(answer);
		if (!strcmp(answer, "yes")) continue;
		else break;
	}
}

static void game(char* player, int number_cnt, int* tryCount) {
	if (!strcmp(player, "user")) play_user(number_cnt, tryCount);
	else play_computer(number_cnt, tryCount);
}

static void make_answer_yes_no(char* answer) {
	size_t len;
	int check;
	while (1) {
		fgets(answer, sizeof(answer), stdout);
		len = strlen(answer);
		check = 1;
		if (answer[len - 1] != '\n') {
			check = 0;
			clear_input_buffer();
		}
		else if (len < 3) check = 0;
		else answer[len - 1] = '\0';

		if (!check) {
			printf("\nyes or no를 입력해주세요.\n");
			continue;
		}

		for (int i = 0; i < len - 1; i++) {
			if (answer[i] < 97) answer[i] += 32;
		}
		if (strcmp(answer, "yes") * strcmp(answer, "no")) {
			printf("\nyes or no를 입력해주세요.\n");
			continue;
		}
		else break;
	}
}