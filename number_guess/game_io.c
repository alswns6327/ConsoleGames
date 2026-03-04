#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"
#include "game_io.h"
#include "security.h"


#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
    // Windows: C:\Users\Name\AppData\Local 경로를 바로 가져옴
    #define DIR_ENV_NAME "LOCALAPPDATA"
#else 
    #define MKDIR(path) mkdir(path, 0755)
    // Linux/macOS: /home/name 경로를 가져옴
    #define DIR_ENV_NAME "HOME"
    #include <unistd.h> // Mac, Linux
#endif
typedef struct {
    char config_base[256];
    char config_folder_path[256];
    char config_file_path[256];
    char score_folder_path[256];
    char score_file_path[256];
} FileInfo;

FileInfo fileInfo;

int folder_exists(char* folder_path) {
    struct stat info;

    // stat 함수는 성공 시 0, 실패(파일 없음 등) 시 -1 반환
    if (stat(folder_path, &info) == 0) {
        // 존재하는 경우, 그게 '폴더(Directory)'인지 확인
        if (info.st_mode & S_IFDIR) {
            return 1; // 폴더로 존재함
        }
    }
    return 0; // 존재하지만 폴더가 아님 (예: 같은 이름의 파일)
}

int file_exists(char* file_path) {
    struct stat info;
    // stat 함수가 0을 반환하면 일단 무언가 존재한다는 뜻
    if (stat(file_path, &info) == 0) {
        // 그게 '일반 파일(Regular File)'인지 확인
        if (info.st_mode & S_IFREG) {
            return 1; // 파일이 존재함
        }
    }
    return 0; // 존재하지 않거나 폴더임
}

void init_config_file_base_path() {
    strcpy(fileInfo.config_base, getenv(DIR_ENV_NAME));
    sprintf(fileInfo.config_folder_path, "%s/ConsoleGame/", fileInfo.config_base);
    sprintf(fileInfo.config_file_path, "%sconfig", fileInfo.config_folder_path);
}

void check_config_folder() {
    if (folder_exists(fileInfo.config_folder_path)) return;
    MKDIR(fileInfo.config_folder_path);
}

void check_config_file() {
    FILE* fp;
    if (file_exists(fileInfo.config_file_path)) {
        fp = fopen(fileInfo.config_file_path, "rb");
        if (fp == NULL) {
            printf("\n오류가 발생하였습니다.\n");
            exit(1);
        }
        fread(&fileInfo, sizeof(FileInfo), 1, fp);
        return;
    }
    else {
        fp = fopen(fileInfo.config_file_path, "wb");
        if (fp == NULL) {
            printf("\n오류가 발생하였습니다.\n");
            exit(1);
        }
        sprintf(fileInfo.score_folder_path, "%s/ConsoleGame/", fileInfo.config_base);
        sprintf(fileInfo.score_file_path, "%sscore", fileInfo.config_folder_path);
        fwrite(&fileInfo, sizeof(FileInfo), 1, fp);
    }
    fclose(fp);
}

void change_location(void) {
    printf("\n전체 경로를 입력해주세요.\n");
    char location[256];
    size_t len;
    while (1) {
        fgets(location, sizeof(location), stdin);
        len = strlen(location) - 1;
        if (location[len] == '\n') {
            location[len] = '\0';
            break;
        }
        printf("\n경로의 길이가 너무 깁니다.\n256자 내로 설정해주세요.\n");
        clear_input_buffer();
    }
    if (!folder_exists(location)) {
        printf("\n존재하지 않는 경로입니다.\n");
        return;
    }

    FILE* config_fp = fopen(fileInfo.config_file_path, "wb");
    printf("\n게임 기록 파일이 %s/ConsoleGame/ 경로에 저장되며 기존 기록은 옮겨지고 기존 경로 파일은 삭제됩니다.\n", location);
    char new_score_folder_path[256];
    sprintf(new_score_folder_path, "%s/ConsoleGame/", location);
    if (!folder_exists(new_score_folder_path)) MKDIR(new_score_folder_path);
    char new_score_file_path[256];
    char old_score_file_path[256];
    int errorCheck = 0;
    for (int i = 2; i < 11; i++) {
        sprintf(new_score_file_path, "%sscore_%d", new_score_folder_path, i);
        sprintf(old_score_file_path, "%s_%d", fileInfo.score_file_path, i);
        if (file_exists(old_score_file_path)) {
            FILE* old_score_fp = fopen(old_score_file_path, "rb");
            fseek(old_score_fp, 0, SEEK_END);
            int count = ftell(old_score_fp) / sizeof(ScoreInfo);
            ScoreInfo* scores = (ScoreInfo*)malloc(sizeof(ScoreInfo) * count);
            rewind(old_score_fp);
            fread(scores, sizeof(ScoreInfo), count, old_score_fp);
            FILE* new_score_fp = fopen(new_score_file_path, "wb");
            if (!new_score_fp) {
                printf("\n파일을 옮기는 과정에서 문제가 발생하였습니다.\n");
                fclose(old_score_fp);
                for (int j = 2; j < i; j++) {
                    sprintf(new_score_file_path, "%sscore_%d", new_score_folder_path, i);
                    if (file_exists(old_score_file_path)) {
                        if (remove(old_score_file_path)) errorCheck++;
                    }
                }
                rmdir(new_score_folder_path);
                return;
            }
            fwrite(scores, sizeof(ScoreInfo), count, new_score_fp);
            free(scores);
            fclose(old_score_fp);
            fclose(new_score_fp);
        }
    }
    for (int i = 2; i < 11; i++) {
        sprintf(old_score_file_path, "%s_%d", fileInfo.score_file_path, i);
        if (file_exists(old_score_file_path)) {
            if (remove(old_score_file_path)) errorCheck++;
        }
    }
    if (errorCheck) {
        printf("\n에러 원인: %s\n", strerror(errno));
        printf("\n이전 파일이 자동 삭제가 실패하여 %s 경로에 남아있으니 원하시는 경우 별도 삭제가 필요합니다.\n", fileInfo.score_file_path);
    }
    else printf("정상 처리 되었습니다.\n");
    sprintf(fileInfo.score_folder_path, "%s", new_score_folder_path);
    sprintf(fileInfo.score_file_path, "%sscore", fileInfo.score_folder_path);
    fwrite(&fileInfo, sizeof(FileInfo), 1, config_fp);
    fclose(config_fp);
}

int save_score(ScoreInfo scoreInfo) {
    char score_file_path[256];
    sprintf(score_file_path, "%s_%d", fileInfo.score_file_path, scoreInfo.numberCnt);
    FILE* score_fp; 
    if(file_exists(score_file_path)) score_fp = fopen(score_file_path, "ab");
    else score_fp = fopen(score_file_path, "wb");
    
    if (!score_fp) {
        printf("\n저장에 실패하였습니다.\n");
        return 0;
    }
    fwrite(&scoreInfo, sizeof(ScoreInfo), 1, score_fp);
    fclose(score_fp);
    return 1;
}

ScoreInfo* read_scores(int * cnt, int number_cnt) {
    char score_file_path[256];
    sprintf(score_file_path, "%s_%d", fileInfo.score_file_path, number_cnt);
    FILE* score_fp = fopen(score_file_path, "rb");
    if (!score_fp) return NULL;
    fseek(score_fp, 0, SEEK_END);
    *cnt = ftell(score_fp) / sizeof(ScoreInfo);
    if (*cnt == 0) {
        fclose(score_fp);
        return NULL;
    }
    ScoreInfo* scores = (ScoreInfo*)malloc(sizeof(ScoreInfo) * (*cnt));
    if (!scores) {
        fclose(score_fp);
        return NULL;
    }
    rewind(score_fp);
    fread(scores, sizeof(ScoreInfo), *cnt, score_fp);
    fclose(score_fp);
    return scores;
}

void delete_score(int number_cnt) {
    int list_cnt;
    ScoreInfo* scores = read_scores(&list_cnt, number_cnt);
    if (list_cnt == 0) {
        printf("\n삭제할 기록이 없습니다.\n");
        return;
    }

    printf("삭제할 기록의 닉네임을 입력해주세요.\n");
    char nickname[22];
    while (1) {
        fgets(nickname, sizeof(nickname), stdin);
        size_t len = strlen(nickname);
        if (nickname[len - 1] != '\n') {
            printf("영어 닉네임은 3~20자 한글 닉네임은 3~10자 사이로 입력해주세요.\n");
            clear_input_buffer();
        }
        else if (len < 3) {
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
        if (has_non_ascii(password) || len < 10) {
            printf("10자 이상 20자 이하의 영문/숫자로 된 password를 입력해주세요.\n");
            continue;
        }

        password[len - 1] = '\0';
        break;
    }
    long long encryp_password = basic_hash(password);
    int idx = -1;
    for (int i = 0; i < list_cnt; i++) {
        if (scores[i].password == encryp_password && strcmp(scores[i].nickname, nickname) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("\n정보와 일치하는 기록이 존재하지 않습니다.\n");
        return;
    }

    qsort(scores, list_cnt, sizeof(ScoreInfo), compare_ascending);
    printf("%d위 %s, 시도 횟수: %d\n날짜: %s의 기록을 삭제하시겠습니까(yes or no)\n", idx + 1, scores[idx].nickname, scores[idx].tryCount, scores[idx].datetime);
    char delCheck[5];
    make_answer_yes_no(delCheck);
    if (!strcmp(delCheck, "no")) {
        printf("삭제에 취소합니다.\n");
        free(scores);
        return;
    }

    char score_file_path[256];
    sprintf(score_file_path, "%s_%d", fileInfo.score_file_path, number_cnt);
    FILE* score_fp = fopen(score_file_path, "wb");
    if (score_fp) {
        for (int i = 0; i < list_cnt; i++) {
            if(idx != i) fwrite(&scores[i], sizeof(ScoreInfo), 1, score_fp);
        }
        fclose(score_fp);
        printf("삭제에 성공하였습니다.\n");
    }
    free(scores);
}