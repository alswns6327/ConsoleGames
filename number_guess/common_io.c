#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"


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

int forder_exists() {
    struct stat info;

    // stat 함수는 성공 시 0, 실패(파일 없음 등) 시 -1 반환
    if (stat(fileInfo.config_folder_path, &info) == 0) {
        // 존재하는 경우, 그게 '폴더(Directory)'인지 확인
        if (info.st_mode & S_IFDIR) {
            return 1; // 폴더로 존재함
        }
    }
    return 0; // 존재하지만 폴더가 아님 (예: 같은 이름의 파일)
}

int file_exists() {
    struct stat info;
    // stat 함수가 0을 반환하면 일단 무언가 존재한다는 뜻
    if (stat(fileInfo.config_file_path, &info) == 0) {
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

void check_config_forder() {
    if (forder_exists()) return;
    mkdir(fileInfo.config_folder_path);
}

void check_config_file() {
    FILE* fp;
    if (file_exists()) {
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