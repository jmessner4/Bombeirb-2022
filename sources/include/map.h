/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,      	//  0011 0000
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70, 	   		// 	0111 0000
                  
};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_MONSTER,
	BONUS_LIFE
};

enum scenery_type {
	SCENERY_STONE = 1,    // 0001 
	SCENERY_TREE  = 2,    // 0010 
	SCENERY_PRINCESS = 4,  // 0100
	SCENERY_FLAG = 8, //1000
	SCENERY_END = 9, //1001
	SCENERY_WIN = 10 // 1010
};
enum bomb_type {
	BOMB1=0,
	BOMB2,
	BOMB3,
	BOMB4,
	EXPLO,
};

enum niveau {
	NIV0 = 1,
	NIV1 = 3,
	NIV2 = 5,
	NIV3 = 7,
	NIV4 = 9,
	NIV5 = 11,
	NIV6 = 13,
	NIV7 = 15,
	NIV8 = 17,
};

enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,
	CELL_FLAG = CELL_SCENERY | SCENERY_FLAG,
	CELL_END = CELL_SCENERY | SCENERY_END,
	CELL_WIN = CELL_SCENERY | SCENERY_WIN,

    CELL_BOX_RANGEINC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
    CELL_BOX_RANGEDEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
	CELL_BOX_BOMBINC  = CELL_BONUS | BONUS_BOMB_NB_INC,
    CELL_BOX_BOMBDEC  = CELL_BONUS | BONUS_BOMB_NB_DEC,
    CELL_BOX_LIFE     = CELL_BONUS | BONUS_MONSTER,
    CELL_BOX_MONSTER  = CELL_BONUS | BONUS_LIFE,

	CELL_BOMB1 = CELL_BOMB | BOMB1,
	CELL_BOMB2 = CELL_BOMB | BOMB2,
	CELL_BOMB3 = CELL_BOMB | BOMB3,
	CELL_BOMB4 = CELL_BOMB | BOMB4,
	CELL_EXPLO = CELL_BOMB | EXPLO,

	CELL_OP_DOOR_0 = CELL_DOOR | NIV0,
	CELL_OP_DOOR_1 = CELL_DOOR | NIV1,
	CELL_OP_DOOR_2 = CELL_DOOR | NIV2,
	CELL_OP_DOOR_3 = CELL_DOOR | NIV3,
	CELL_OP_DOOR_4 = CELL_DOOR | NIV4,
	CELL_OP_DOOR_5 = CELL_DOOR | NIV5,
	CELL_OP_DOOR_6 = CELL_DOOR | NIV6,
	CELL_OP_DOOR_7 = CELL_DOOR | NIV7,
};

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);

// Return the type of a bomb
enum compose_type map_get_bomb_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum compose_type type);

void map_open_door(struct map* map, int x, int y);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static();

// Display a bomb on the screen
void display_bomb(struct map* map, int x, int y, unsigned char type);

// Display the map on the screen
void map_display(struct map* map);

struct map* map_get_level(char L);

#endif /* MAP_H_ */
