#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <map.h>
#include <monster.h>
#include <player.h>

struct bomb_elem {
    enum compose_type type;
    int timeinit;
    int x;
    int y;
    int range;   //portée de la bombe 0 = pas de portée, 1 = portée classique, 2 = portée maximisée
    struct bomb_elem* bomb_suiv;
};

struct bomb_old {
    struct bomb_elem* prems;
};

//Init of the bomb queue, creating an history of the bomb
struct bomb_old* bomb_init(){

    struct bomb_old* bomb_old = malloc(sizeof(struct bomb_old*));

    if (bomb_old == NULL){
        exit(EXIT_FAILURE);
    }

    bomb_old->prems = NULL;

    return bomb_old;
}

void bomb_free(struct bomb_old* oldbomb) {
    assert(oldbomb);
    free(oldbomb);
}

//Adding a bomb to the bomb queue
struct bomb_elem* add_bomb(struct bomb_old* oldbomb, struct player* player) {
    struct bomb_elem* new_bomb = malloc(sizeof(*new_bomb));
     if (new_bomb == NULL) {
        exit(EXIT_FAILURE);
    }
    new_bomb->type = CELL_BOMB1;
    new_bomb->timeinit = SDL_GetTicks();
    new_bomb->x = bomb_set_pos_x(new_bomb, player);
    new_bomb->y = bomb_set_pos_y(new_bomb, player);

    new_bomb->range = player_get_range(player);

    new_bomb->bomb_suiv = NULL;
    if (oldbomb->prems != NULL){
        struct bomb_elem* bomb_file = oldbomb->prems;
        while (bomb_file->bomb_suiv != NULL)
        {   
            bomb_file = bomb_file->bomb_suiv;
        }	
        bomb_file->bomb_suiv = new_bomb;
    }
    else /* la file est vide */
    {
        oldbomb->prems = new_bomb;
    }
    return new_bomb;
}

void suppr_bomb(struct bomb_old* oldbomb) {
    
    assert(oldbomb);
    if (oldbomb->prems != NULL)
    {
        struct bomb_elem* bombsuppr = oldbomb->prems;
        oldbomb->prems = bombsuppr->bomb_suiv;
        free(bombsuppr);
    }
}

int bomb_set_pos_x(struct bomb_elem* bomb, struct player* player){
    bomb->x = player_get_x(player);
    return bomb->x;
}

int bomb_set_pos_y(struct bomb_elem* bomb, struct player* player){
    bomb->y = player_get_y(player);
    return bomb->y;
}





void bomb_set_type(struct bomb_elem* bomb, enum compose_type type) {
    assert(bomb);
    bomb->type = type;
}

void bomb_statut(struct bomb_old* oldbomb,struct bomb_elem* bomb, struct map* map, struct player* player, struct game* game) {
    int timenow = SDL_GetTicks();
    switch(bomb->type) {
        case CELL_BOMB1 :
            if((timenow-bomb->timeinit)>1000 && (timenow-bomb->timeinit)<2000){
                bomb_set_type(bomb, CELL_BOMB2);
                map_set_cell_type(map,bomb->x,bomb->y,bomb->type);
            }
            break; 
        case CELL_BOMB2 :
            if((timenow-bomb->timeinit)>2000 && (timenow-bomb->timeinit)<3000) {
                bomb_set_type(bomb, CELL_BOMB3);
                map_set_cell_type(map,bomb->x,bomb->y,bomb->type);
            }
            break;
        case CELL_BOMB3 :
            if((timenow-bomb->timeinit)>3000 && (timenow-bomb->timeinit)<4000) {
                bomb_set_type(bomb, CELL_BOMB4);
                map_set_cell_type(map,bomb->x,bomb->y,bomb->type);
            }
            break;
        case CELL_BOMB4 :
            if((timenow-bomb->timeinit)>4000 && (timenow-bomb->timeinit)<5000) {
                /////
               
               int range = bomb->range;
                /////
               bomb_set_type(bomb, CELL_EXPLO);
               map_set_cell_type(map,bomb->x,bomb->y,bomb->type);
               player_inc_nb_bomb(player);
               if (bomb->x==player_get_x(player) && bomb->y==player_get_y(player)){
	                          player_dec_nb_lives(player);
	                         }  
                // Gestion de l'explosion
                





                if (range>=1){ 
                  
                    int x = bomb->x;
                    int y = bomb->y-1;//Nord de la position de la bombe
                
                    enum cell_type type; 
                    if(map_is_inside(map,x,y)){
                        type = map_get_cell_type(map,x,y);
                        switch(type) {
                            case CELL_EMPTY :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;    
                            case CELL_BOX :
                                map_set_cell_type(map,x,y, CELL_BOX_RANGEINC);
                                break;
                            case CELL_MONSTER :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_BONUS :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_DOOR :
                                break;
                            case CELL_SCENERY :
                                break;
                            case CELL_BOMB :
                                break;
                            case CELL_KEY :
                                break;
                        }
                        if (x==player_get_x(player) && y==player_get_y(player)){
	                          player_dec_nb_lives(player);
	                         }  
                    }

                    x = bomb->x;
                    y = bomb->y+1; //Sud de la position de la bombe
                    if(map_is_inside(map,x,y)) {
                        type = map_get_cell_type(map,x,y);
                        switch(type) {
                            case CELL_EMPTY :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;    
                            case CELL_BOX :
                                map_set_cell_type(map,x,y,CELL_BOX_BOMBDEC);
                                break;
                            case CELL_MONSTER :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_BONUS :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_DOOR :
                                break;
                            case CELL_SCENERY :
                                break;
                            case CELL_BOMB :
                                break;
                            case CELL_KEY :
                                break;
                        }
                        if (x==player_get_x(player) && y==player_get_y(player)){
	                        player_dec_nb_lives(player);
	                        }
                    }

                    x = bomb->x-1;
                    y = bomb->y; //Ouest de la position de la bombe
                    if(map_is_inside(map,x,y)) {
                        type = map_get_cell_type(map,x,y);
                        switch(type) {
                            case CELL_EMPTY :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;    
                            case CELL_BOX :
                                map_set_cell_type(map,x,y,CELL_BOX_BOMBINC);
                                break;
                            case CELL_MONSTER :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_BONUS :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_DOOR :
                                break;
                            case CELL_SCENERY :
                                break;
                            case CELL_BOMB :
                                break;
                            case CELL_KEY :
                                break;
                        }
                        if (x==player_get_x(player) && y==player_get_y(player)){
	                        player_dec_nb_lives(player);
	                        }
                    }

                    x = bomb->x+1;
                    y = bomb->y; //Est de la position de la bombe
                    if(map_is_inside(map,x,y)) {
                         type = map_get_cell_type(map,x,y);
                         switch(type) {
                             case CELL_EMPTY :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;    
                            case CELL_BOX :
                                map_set_cell_type(map,x,y,CELL_BOX_RANGEDEC);
                                break;
                            case CELL_MONSTER :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_BONUS :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_DOOR :
                                break;
                            case CELL_SCENERY :
                                break;
                            case CELL_BOMB :
                                break;
                            case CELL_KEY :
                                break;
                        }
                        if (x==player_get_x(player) && y==player_get_y(player)){
	                        player_dec_nb_lives(player);
	                        }
                    }
                }
                
            




            if (range>=2){ 
                  
                    int x = bomb->x;
                    int y = bomb->y-2;//Nord de la position de la bombe
                
                    enum cell_type type; 
                    if(map_is_inside(map,x,y)){
                        type = map_get_cell_type(map,x,y);
                        switch(type) {
                            case CELL_EMPTY :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;    
                            case CELL_BOX :
                                break;
                            case CELL_MONSTER :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_BONUS :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_DOOR :
                                break;
                            case CELL_SCENERY :
                                break;
                            case CELL_BOMB :
                                break;
                            case CELL_KEY :
                                break;
                        }
                        if (x==player_get_x(player) && y==player_get_y(player)){
	                          player_dec_nb_lives(player);
	                         }  
                    }

                    x = bomb->x;
                    y = bomb->y+2; //Sud de la position de la bombe
                    if(map_is_inside(map,x,y)) {
                        type = map_get_cell_type(map,x,y);
                        switch(type) {
                            case CELL_EMPTY :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;    
                            case CELL_BOX :
                                break;
                            case CELL_MONSTER :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_BONUS :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_DOOR :
                                break;
                            case CELL_SCENERY :
                                break;
                            case CELL_BOMB :
                                break;
                            case CELL_KEY :
                                break;
                        }
                        if (x==player_get_x(player) && y==player_get_y(player)){
	                        player_dec_nb_lives(player);
	                        }
                    }

                    x = bomb->x-2;
                    y = bomb->y; //Ouest de la position de la bombe
                    if(map_is_inside(map,x,y)) {
                        type = map_get_cell_type(map,x,y);
                        switch(type) {
                            case CELL_EMPTY :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;    
                            case CELL_BOX :
                                break;
                            case CELL_MONSTER :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_BONUS :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_DOOR :
                                break;
                            case CELL_SCENERY :
                                break;
                            case CELL_BOMB :
                                break;
                            case CELL_KEY :
                                break;
                        }
                        if (x==player_get_x(player) && y==player_get_y(player)){
	                        player_dec_nb_lives(player);
	                        }
                    }

                    x = bomb->x+2;
                    y = bomb->y; //Est de la position de la bombe
                    if(map_is_inside(map,x,y)) {
                         type = map_get_cell_type(map,x,y);
                         switch(type) {
                             case CELL_EMPTY :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;    
                            case CELL_BOX :
                                break;
                            case CELL_MONSTER :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_BONUS :
                                map_set_cell_type(map,x,y,CELL_EXPLO);
                                break;
                            case CELL_DOOR :
                                break;
                            case CELL_SCENERY :
                                break;
                            case CELL_BOMB :
                                break;
                            case CELL_KEY :
                                break;
                        }
                        if (x==player_get_x(player) && y==player_get_y(player)){
	                        player_dec_nb_lives(player);
	                        }
                    }
                }
                
            }
                
            
        
        
        
        
   
        
        
        
        case CELL_EXPLO :
            if((timenow-bomb->timeinit)>5000){
         
            map_set_cell_type(map,bomb->x,bomb->y,CELL_EMPTY);
            suppr_bomb(oldbomb);
            int range=bomb->range;
            // Gestion de l'après explosion
            if (range>=1){    
                int x = bomb->x;
                int y = bomb->y-1; //Nord de la position de la bombe
                enum cell_type type; 
                if(map_is_inside(map,x,y)){
                    type = map_get_cell_type(map,x,y);
                    if(type == (CELL_BOMB & 0xf0)) {
                        map_set_cell_type(map,x,y,CELL_EMPTY);
                    }
                }

                x = bomb->x;
                y = bomb->y+1; //Sud de la position de la bombe
                if(map_is_inside(map,x,y)){
                    type = map_get_cell_type(map,x,y);
                    if(type == (CELL_BOMB & 0xf0)) {
                        map_set_cell_type(map,x,y,CELL_EMPTY);
                    }
                }

                x = bomb->x-1;
                y = bomb->y; //Ouest de la position de la bombe
                if(map_is_inside(map,x,y)){
                    type = map_get_cell_type(map,x,y);
                    if(type == (CELL_BOMB & 0xf0)) {
                        map_set_cell_type(map,x,y,CELL_EMPTY);
                    }
                }

                x = bomb->x+1;
                y = bomb->y; //Est de la position de la bombe
                if(map_is_inside(map,x,y)){
                    type = map_get_cell_type(map,x,y);
                    if(type == (CELL_BOMB & 0xf0)) {
                        map_set_cell_type(map,x,y,CELL_EMPTY);
                    }
                }
            }

            if (range>=2){    
                int x = bomb->x;
                int y = bomb->y-2; //Nord de la position de la bombe
                enum cell_type type; 
                if(map_is_inside(map,x,y)){
                    type = map_get_cell_type(map,x,y);
                    if(type == (CELL_BOMB & 0xf0)) {
                        map_set_cell_type(map,x,y,CELL_EMPTY);
                    }
                }

                x = bomb->x;
                y = bomb->y+2; //Sud de la position de la bombe
                if(map_is_inside(map,x,y)){
                    type = map_get_cell_type(map,x,y);
                    if(type == (CELL_BOMB & 0xf0)) {
                        map_set_cell_type(map,x,y,CELL_EMPTY);
                    }
                }

                x = bomb->x-2;
                y = bomb->y; //Ouest de la position de la bombe
                if(map_is_inside(map,x,y)){
                    type = map_get_cell_type(map,x,y);
                    if(type == (CELL_BOMB & 0xf0)) {
                        map_set_cell_type(map,x,y,CELL_EMPTY);
                    }
                }

                x = bomb->x+2;
                y = bomb->y; //Est de la position de la bombe
                if(map_is_inside(map,x,y)){
                    type = map_get_cell_type(map,x,y);
                    if(type == (CELL_BOMB & 0xf0)) {
                        map_set_cell_type(map,x,y,CELL_EMPTY);
                    }
                }
            }
        
        
        
        
        }
            break;
        default :
            break;
    }
}

int bomb_update(struct bomb_old* oldbomb, struct map* map, struct player* player, struct game* game) {
    if(oldbomb->prems == NULL) {
        return 0;
    } else {
        struct bomb_elem* bomb = oldbomb->prems;
        bomb_statut(oldbomb,bomb,map,player,game);
        while(bomb->bomb_suiv != NULL) {
            bomb_statut(oldbomb,bomb->bomb_suiv,map,player,game);
            bomb = bomb->bomb_suiv;
        }
        return 0;
    }
}