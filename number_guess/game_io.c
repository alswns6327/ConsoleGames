#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"
#include "game_io.h"


#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
    // Windows: C:\Users\Name\AppData\Local 경로를 바로 가져옴
    #define DIR_ENV_NAME "LOCALAPPDATA"
#else 
    #define MKDIR(path) mkdir(path, 0755)
    // Linux/macOS: /home/name 경로를 가져옴
    #define DIR_ENV_NAME "HOME"
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

void change_location(char* location) {
    if (!folder_exists(location)) {
        printf("\n존재하지 않는 경로입니다.\n");
        return;
    }

    FILE* config_fp = fopen(fileInfo.config_file_path, "wb");
    printf("\n게임 기록 파일이 %s/ConsoleGame/ 경로에 저장되며 기존 기록은 옮겨지고 기존 경로 파일은 삭제됩니다.\n", location);
    sprintf(fileInfo.score_folder_path, "%s/ConsoleGame/", location);
    char new_score_file_path[256];
    sprintf(new_score_file_path, "%sscore", fileInfo.score_folder_path);
    if (file_exists(fileInfo.score_file_path)) {
        FILE* score_fp = fopen(fileInfo.score_file_path, "rb");
        fseek(score_fp, 0, SEEK_END);
        int count = ftell(score_fp) / sizeof(ScoreInfo);
        ScoreInfo* scores = (ScoreInfo*)malloc(sizeof(ScoreInfo) * count);
        rewind(score_fp);
        fread(scores, sizeof(ScoreInfo), count, score_fp);
        FILE* new_score_fp = fopen(new_score_file_path, "wb");
        if (!new_score_fp) {
            printf("\n파일을 옮기는 과정에서 문제가 발생하였습니다.\n");
            fclose(score_fp);
            return;
        }
        fwrite(scores, sizeof(ScoreInfo), count, new_score_fp);
        free(scores);
        fclose(score_fp);
        fclose(new_score_fp);
        if (remove(fileInfo.score_file_path))
            printf("\n이전 파일이 자동 삭제가 실패하여 %s 경로에 남아있으니 원하시는 경우 별도 삭제가 필요합니다.\n", fileInfo.score_file_path);
    }
    sprintf(fileInfo.score_file_path, "%s", new_score_file_path);
    fwrite(&fileInfo, sizeof(FileInfo), 1, config_fp);
    fclose(config_fp);
}

int save_score(ScoreInfo scoreInfo) {
    FILE* score_fp = fopen(fileInfo.score_file_path, "ab");
    if (!score_fp) {
        printf("\n저장에 실패하였습니다.\n");
        return 0;
    }
    fwrite(&scoreInfo, sizeof(ScoreInfo), 1, score_fp);
    fclose(score_fp);
    return 1;
}