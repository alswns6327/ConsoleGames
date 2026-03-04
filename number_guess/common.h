#pragma once

void clear_input_buffer();
int has_non_ascii(char*);
int compare_ascending(const void* score1, const void* score2);
int compare_descending(const void* score1, const void* score2);
int big_rand();
void input_number(int* num, int min, int max);
void make_answer_yes_no(char* answer);