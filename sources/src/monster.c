#include <SDL/SDL_image.h>
#include <assert.h>
/////////

#include <monster.h>
#include <game.h>

//////////
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>


struct monster {
  int x,y,speed;
  enum direction direction;
	

};

struct monster* monster_init() {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->direction = EAST;

	return monster;
}



void monster_set_position(struct monster *monster, int x, int y) {
	assert(monster);
	monster->x = x;
	monster->y = y;
	
}


void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}



int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}



int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}




void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}




int monster_get_speed(struct monster* monster) {
	assert(monster != NULL);
	return monster->speed;
}


int monster_set_speed(struct monster* monster, int t) {
	assert(monster != NULL);
	return monster->speed=t;
}



static int monster_move_aux(struct monster* monster, struct map* map, int x, int y, struct player* player) {

	if (!map_is_inside(map, x, y))
		return 0;
  
	if (x==player_get_x(player) && y==player_get_y(player)){
	    player_dec_nb_lives(player);
	}

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX: 
        return 0;
		break;

    case CELL_BONUS:
		return 0;
        break;

	case CELL_MONSTER:
		return 0;
        break;

    case CELL_DOOR: 
        return 0;
		break;

    case CELL_KEY:
		    return 0;
      break;

	case CELL_BOMB:
	   
		return 0;
        break;


    

	default:
		break;
	}

    return 1;
}
         


int monster_move(struct monster* monster, struct map* map, struct player* player) {
  	
		int x = monster->x;
 	  int y = monster->y;
		 

	  int move = 0; 

	switch (monster->direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1, player)) {
			monster->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1, player)) {
			monster->y++;
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y, player)) {
			monster->x--;
			move = 1;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y, player)) {
			monster->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		/////////////
		map_set_cell_type(map, monster_get_x(monster), monster_get_y(monster), CELL_MONSTER);
	  ///////////// 
	}
	return move;
}






void monster_display(struct monster* monster) {
 	assert(monster);
 	window_display_image(sprite_get_monster(monster->direction),
		monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
 }









