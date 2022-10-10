/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>
//#include <game.h>

struct player;
struct game;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number, int lives);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);
int player_get_life(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player* player);
void player_inc_nb_bomb(struct player* player);
void player_dec_nb_bomb(struct player* player);


// Get, Increase, Decrease the range of the player's bomb
int  player_get_range(struct player* player);
void player_range_inc(struct player* player);
void player_range_dec(struct player* player);

//Defines the player's reaction to bonus
void player_bonus_reaction(enum compose_type type, struct player* player);

// Get, Decrease the number of lives of the player 
int  player_get_nb_lives(struct player* player);
void player_dec_nb_lives(struct player* player);

int player_get_nb_key(struct player* player);
void player_key_inc(struct player* player);
void player_key_dec(struct player* player);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map, struct game* game);

// Display the player on the screen
void player_display(struct player* player);

#endif /* PLAYER_H_ */
