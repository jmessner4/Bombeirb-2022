/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <monster.h>
#include <map.h>

// Abstract data type
struct game;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the monster of the current game
struct monster** game_get_monster(struct game* game);

// Return the first element for the bomb queue
struct bomb_old* game_get_old_bomb(struct game* game);

int game_get_current_level(struct game* game);

void level_inc(struct game* game,int level_new);

// Return the current map
struct map* game_get_current_map(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);

//endgame procedure
void endgame(struct game* game, int i, int x, int y);

#endif /* GAME_H_ */
