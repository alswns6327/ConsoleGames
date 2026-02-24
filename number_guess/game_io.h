#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"
#include <time.h>

typedef struct {
	unsigned long long password;
	time_t time;
	int tryCount;
	int numberCnt;
	char nickname[21];
	char datetime[20];
} ScoreInfo;

void init_config_file_base_path();
void check_config_folder();
void check_config_file();
void change_location(char* location);
int save_score(ScoreInfo);
ScoreInfo* read_scores(int * cnt, int number_cnt);