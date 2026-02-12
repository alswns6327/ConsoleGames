#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"

void init_config_file_base_path();
void check_config_forder();
void check_config_file();