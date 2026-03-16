#pragma once

typedef struct {
	int type; // 0은 이동 불가 지역, 1은 이동 가능
	int entity_on;
	int monster_idx;
} Tile;

void make_map(void);