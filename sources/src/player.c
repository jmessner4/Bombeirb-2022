/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

////////

#include <time.h>
#include <monster.h>
/////////

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <game.h>


struct player {
	int x, y;
	enum direction direction;
	int range;
	int bombs;
	int lives;
	int key;

};


struct player* player_init(int bombs, int lives) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = NORTH;
	player->bombs = bombs;
	player->lives = lives;
	player->key = 0;
	player->range = 2;
	return player;
}



void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}




void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}
int player_get_life(struct player* player) {
	assert(player != NULL);
	return player->lives;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}


void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

int player_get_nb_key(struct player* player) {
	assert(player);
	return player->key;
}

void player_key_inc(struct player* player) {
	assert(player);
	player->key +=1;
}

void player_key_dec(struct player* player) {
	assert(player);
	player->key -=1;
}

int player_get_range(struct player* player) {
	assert(player);
	return player->range;
}

void player_range_inc(struct player* player) {
	assert(player);
	if (player_get_range(player)<2){
	   player->range +=1;
    }
}

void player_range_dec(struct player* player) {
	assert(player);
    if (player_get_range(player)>0){
	   player->range -=1;
    }
}


int player_get_nb_lives(struct player* player) {
	assert(player);
	return player->lives;
}

void player_dec_nb_lives(struct player* player) {
	assert(player);
	player->lives -= 1;
}


void player_bonus_reaction(enum compose_type type, struct player* player) {
      
	switch(type){
		case CELL_BOX_BOMBINC: 
		    player_inc_nb_bomb(player);    
			break;

		case CELL_BOX_BOMBDEC: 
		    player_dec_nb_bomb(player);    
			break;
		
		case CELL_BOX_RANGEINC: 
		    player_range_inc(player); 
			break;

		case CELL_BOX_RANGEDEC: 
		    player_range_dec(player); 
			break;
		
		default : 
		    break;
	    }
   }
		
			
		






static int player_move_aux(struct player* player, struct map* map, struct game* game,int x, int y) {
	int level=0;
	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		level = map_get_bomb_type(map,x,y);
		if(level == CELL_PRINCESS) {
			endgame(game,1,x,y-2);
		}
		return 0;
		break;

	case CELL_BOX:
		switch (player->direction){
		case NORTH:
			if(!map_is_inside(map,x,y-1)){
				return 0;
			}
			else if(map_get_cell_type(map,x,y-1) == CELL_EMPTY){
				map_set_cell_type(map, x, y-1, CELL_BOX);
				map_set_cell_type(map, x, y, CELL_EMPTY);
				return 1;
			} else {
				return 0;
			}

		case SOUTH:
			if(!map_is_inside(map,x,y+1)){
				return 0;
			}
			else if(map_get_cell_type(map,x,y+1) == CELL_EMPTY){
				map_set_cell_type(map, x, y+1, CELL_BOX);
				map_set_cell_type(map, x, y, CELL_EMPTY);
				return 1;
			} else {
				return 0;
			}
		
		case WEST:
			if(!map_is_inside(map,x-1,y)){
				return 0;
			} else if(map_get_cell_type(map,x-1,y) == CELL_EMPTY){
				map_set_cell_type(map, x-1, y, CELL_BOX);
				map_set_cell_type(map, x, y, CELL_EMPTY);
				return 1;
			} else {
				return 0;
			}

		case EAST:
			if(!map_is_inside(map,x+1,y)){
				return 0;
			}
			else if(map_get_cell_type(map,x+1,y) == CELL_EMPTY){
				map_set_cell_type(map, x+1, y, CELL_BOX);
				map_set_cell_type(map, x, y, CELL_EMPTY);
				return 1;
			}
			else {
				return 0;
			}
		}
		break;

	case CELL_KEY:
	    player_key_inc(player);
		map_set_cell_type(map, x, y, CELL_EMPTY);
		break;

	case CELL_BONUS:
	     player_bonus_reaction(map_get_bomb_type(map, x, y),player);
		 map_set_cell_type(map, x, y, CELL_EMPTY);
	    
		break;

	case CELL_MONSTER:
	    player_dec_nb_lives(player);
		window_display_image(sprite_get_number(player_get_nb_lives(player)), x, y);
	
		break;
	
	case CELL_BOMB:
		if(map_get_bomb_type(map,x,y) == CELL_EXPLO) {
			player_dec_nb_lives(player);
			return 0;
		} else {
			return 1;
		}
		break;
	
	case CELL_DOOR:
		switch(map_get_bomb_type(map,x,y) & 0x01) {
			case 0:
				if(player_get_nb_key(player)>0) {
					map_open_door(map,x,y);
					player_key_dec(player);
				}
				switch(map_get_bomb_type(map,x,y)) {
					case 49:
						level = 0;
						break;
					case 51:
						level = 1;
						break;
					case 53:
						level = 2;
						break;
					case 55:
						level = 3;
						break;
					case 57:
						level = 4;
						break;
					case 59:
						level = 5;
						break;
					case 61:
						level = 6;
						break;
					case 63:
						level = 7;
						break;
					default:
						break;
				}
				level_inc(game, level);
				break;
			case 1:
				switch(map_get_bomb_type(map,x,y)) {
					case 49:
						level = 0;
						break;
					case 51:
						level = 1;
						break;
					case 53:
						level = 2;
						break;
					case 55:
						level = 3;
						break;
					case 57:
						level = 4;
						break;
					case 59:
						level = 5;
						break;
					case 61:
						level = 6;
						break;
					case 63:
						level = 7;
						break;
					default:
						break;
				}
				level_inc(game,level);
				return 1;
		}
	case CELL_EMPTY:
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map, struct game* game) {
	int x = player->x;
	int y = player->y;
	int move = 0; 

	switch (player->direction) {
	case NORTH:
		if (player_move_aux(player, map,game, x, y - 1)) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, game,x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map,game, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, game,x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		if(map_get_cell_type(map,x,y) != CELL_BOMB && map_get_cell_type(map,x,y) != CELL_DOOR){
			map_set_cell_type(map, x, y, CELL_EMPTY);
		} else if (map_get_bomb_type(map,x,y) == CELL_EXPLO){
			player_dec_nb_lives(player);
		}
	}
	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}


