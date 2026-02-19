#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"

typedef struct {
	int tryCount;
	char nickname[21];
	unsigned long long password;
} ScoreInfo;

void init_config_file_base_path();
void check_config_folder();
void check_config_file();

int save_score(ScoreInfo);