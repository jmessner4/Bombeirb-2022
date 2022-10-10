/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>

/////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <player.h>

/////////

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <map.h>

struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
	struct monster** monster;
	struct bomb_old* oldbomb;
};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->levels = 8;
	game->level = 0;
	game->maps = malloc((game->levels)*sizeof(struct map*));

	//game->maps[0] = map_get_static();
	game->maps[0] = map_get_level('0');
	game->maps[1] = map_get_level('1');
	game->maps[2] = map_get_level('2');
	game->maps[3] = map_get_level('3');
	game->maps[4] = map_get_level('4');
	game->maps[5] = map_get_level('5');
	game->maps[6] = map_get_level('6');
	game->maps[7] = map_get_level('7');

	game->player = player_init(3,3);

	// Set default location of the player
	player_set_position(game->player, 1, 0);

	game->monster = malloc(4*sizeof(struct monster*));

	game->monster[0] = monster_init();
	monster_set_position(game->monster[0], 2, 3);
	monster_set_speed(game->monster[0], SDL_GetTicks());

	game->monster[1] = monster_init();
	monster_set_position(game->monster[1],3 , 4);
	monster_set_speed(game->monster[1], SDL_GetTicks());

	game->monster[2] = monster_init();
	monster_set_position(game->monster[2], 4, 2);
	monster_set_speed(game->monster[2], SDL_GetTicks());

	game->monster[3] = monster_init();
	monster_set_position(game->monster[3], 5, 5);
	monster_set_speed(game->monster[3], SDL_GetTicks());
	

	
	


	game->oldbomb = bomb_init();
	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	///////////////
	for (int i = 0; i < 4; i++)
		monster_free(game->monster[i]);
    
	////////////////
	bomb_free(game->oldbomb);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	int i = game->level;
	return game->maps[i];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;


}

//////////////////////////////
struct monster** game_get_monster(struct game* game) {
	assert(game);
	return game->monster;
}
//////////////////////////

struct bomb_old* game_get_old_bomb(struct game* game) {
	assert(game);
	return game->oldbomb;
}

int game_get_current_level(struct game* game) {
	assert(game);
	return game->level;
}

void level_inc(struct game* game, int level_new) {
	assert(game);
	game->level = level_new;
}



void game_banner_display(struct game* game) {
	assert(game);
	struct player* player = game_get_player(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 9 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

    x = white_bloc + SIZE_BLOC;
///////////////
	window_display_image(sprite_get_number(player_get_nb_lives(player)), x, y);
///////////////


	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_bomb(player)), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	
	int range= player_get_range(player);
	    
			if (range <= 2) {
	        window_display_image(sprite_get_number(range), x, y);
	    
	    } else {
		      window_display_image(sprite_get_number(2), x, y);   
	    }

	
	x = 3 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(sprite_get_key(), x, y);

	x = 3 * white_bloc + 8 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_key(player)), x, y);
    
	x = 3 * white_bloc + 9 * SIZE_BLOC;

}

void game_display(struct game* game) {
	assert(game);

	window_clear();
	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);
	int i;
	for (i = 0; i < 4; i++)
	    //if (game->monster[i] !=NULL){
	    monster_display(game->monster[i]);
			//}
	window_refresh();
}



static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	struct bomb_old* oldbomb = game_get_old_bomb(game);
	//FILE* save;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map,game);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map,game);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map,game);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map,game);
				break;
			case SDLK_SPACE:  //Action pour placer une bombe
				if(player_get_nb_bomb(player)>0) {
					player_dec_nb_bomb(player);
					add_bomb(oldbomb, player);
					map_set_cell_type(map,player_get_x(player),player_get_y(player),CELL_BOMB1);
				}
				break;
			case SDLK_s:
			/* 	save = fopen("data/save.txt", "r+");
				char* fichier;
				fscanf(save, "%s",fichier);
				if(fichier == " "){
					fwrite(game,game_get_current_map(game),game_get_player(game),save);
					fclose(save);
				} else {
					struct game* game_save; 
					fscanf(save, "%p ", game_save);
					struct map* map_save; 
					fscanf(save, "%p ", map_save);
					struct player* player;
					fscanf(save, "%p ", map_save);
				} */
				break;
			
		//	case SDLK_p :


			default:
				break;
			}

			break;
		}
	}
	return 0;
}




/* void adaptation_number_of_monsters(struct game* game){
    struct monster** monster = game_get_monster(game);
		if (game_get_current_level(game)>=1){
		  game->monster[1] = monster_init();
	    monster_set_position(game->monster[1],1 , 6);
	    monster_set_speed(game->monster[1], SDL_GetTicks());
		}
	  if (game_get_current_level(game)>=2){
	    game->monster[2] = monster_init();
	    monster_set_position(game->monster[2], 8, 7);
	    monster_set_speed(game->monster[2], SDL_GetTicks());
		}
	
	  if (game_get_current_level(game)>=3){
	    game->monster[3] = monster_init();
	    monster_set_position(game->monster[3], 3, 2);
	    monster_set_speed(game->monster[3], SDL_GetTicks());
		}

} */
	
		    






///////////


static short monster_random_directions(struct game* game) {
	struct monster** monster = game_get_monster(game);
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
  int randomly_changing_variable;
	
    srand(time(0));	  
	  randomly_changing_variable=rand()%4;

	  if (randomly_changing_variable==0) {
			
	  	monster_set_current_way(monster[0], NORTH);
		  monster_move(monster[0], map, player);
      
			//if (game_get_current_level(game)>=1){
				//adaptation_number_of_monsters(game);
			  monster_set_current_way(monster[1], SOUTH);
		    monster_move(monster[1], map, player);
			//}
      
      //if (game_get_current_level(game)>=2){
				//adaptation_number_of_monsters(game);
			  monster_set_current_way(monster[2], EAST);
		    monster_move(monster[2], map, player);
			//}
      
			//if (game_get_current_level(game)>=3){
				//adaptation_number_of_monsters(game);
			  monster_set_current_way(monster[3], WEST);
		    monster_move(monster[3], map, player);
			//}
		//break;
			
			}
	  else if (randomly_changing_variable==1) {
		  monster_set_current_way(monster[0], SOUTH);
		  monster_move(monster[0], map, player);
			
			//if (game_get_current_level(game)>=1){
			//adaptation_number_of_monsters(game);
			monster_set_current_way(monster[1], EAST);
		  monster_move(monster[1], map, player);
			//}
      
			//if (game_get_current_level(game)>=2){
			//adaptation_number_of_monsters(game);
			monster_set_current_way(monster[2], WEST);
		  monster_move(monster[2], map, player);
			//}

			//if (game_get_current_level(game)>=3){
			//adaptation_number_of_monsters(game);
			monster_set_current_way(monster[3], NORTH);
		  monster_move(monster[3], map, player);
			//}
		//break;
		
			
			}
	  else if (randomly_changing_variable==2) {
		  monster_set_current_way(monster[0], EAST);
		  monster_move(monster[0], map, player);
			
			//if (game_get_current_level(game)>=1){
			//adaptation_number_of_monsters(game);
			monster_set_current_way(monster[1], WEST);
		  monster_move(monster[1], map, player);
			//}
      
			//if (game_get_current_level(game)>=2){
      //adaptation_number_of_monsters(game);
			monster_set_current_way(monster[2], NORTH);
		  monster_move(monster[2], map, player);
			//}
     
      //if (game_get_current_level(game)>=3){		 
		  //adaptation_number_of_monsters(game);
			monster_set_current_way(monster[3], SOUTH);
		  monster_move(monster[3], map, player);
			//}
			
		//break;
				
		}
	  else if (randomly_changing_variable==3) {
		  monster_set_current_way(monster[0], WEST);
		  monster_move(monster[0], map, player);
			

			//if (game_get_current_level(game)>=1){
			//adaptation_number_of_monsters(game);
			monster_set_current_way(monster[1], NORTH);
		  monster_move(monster[1], map, player);
			//}
      
			//if (game_get_current_level(game)>=2){
			//adaptation_number_of_monsters(game);
			monster_set_current_way(monster[2], SOUTH);
		  monster_move(monster[2], map, player);
			//}

			//if (game_get_current_level(game)>=3){
      //adaptation_number_of_monsters(game);
			monster_set_current_way(monster[3], EAST);
		  monster_move(monster[3], map, player);
			//}
			
		//break;
		
				
		}

			
		//break;
			
	return 0;
	}
///////////

 
int game_update(struct game* game) {
	struct bomb_old* oldbomb = game_get_old_bomb(game);

	struct map* map = game_get_current_map(game);
	struct player* player = game_get_player(game);

	struct monster** monster = game_get_monster(game);
	struct monster* first_monster = game->monster[0];
  
	int level=game_get_current_level(game);
	if ((SDL_GetTicks()-monster_get_speed(first_monster))>(1000/(1+2*level))){
        monster_random_directions(game);
				monster_set_speed(first_monster, SDL_GetTicks());

	}
	bomb_update(oldbomb, map, player,game);
	if (input_keyboard(game))
		return 1; // exit game
///////////////
	if(map_get_bomb_type(map,0,6) == CELL_END) {
		sleep(8);
		return 1;
	}
	if(player_get_life(player) == 0) {
		int x = player_get_x(player);
		int y = player_get_y(player);
		endgame(game,0,x,y);
	}

	return 0;

	
	
}

void endgame(struct game* game,int i, int x, int y) {
	struct map* map = game_get_current_map(game);
	if(i==1) {
		map_set_cell_type(map,x,y, CELL_FLAG);
		map_set_cell_type(map,6,6,CELL_WIN);
	} else if (i==0) {
		map_set_cell_type(map,0,6, CELL_END);
	}
}
