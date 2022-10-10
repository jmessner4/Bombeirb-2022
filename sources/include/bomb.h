#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>


//Création de la file des bombes
struct bomb_elem;
//Pointeur qui redirige vers la bombe précédente dans la file
struct bomb_old;

struct bomb_old* bomb_init();

void bomb_free(struct bomb_old* oldbomb);

struct bomb_elem* add_bomb(struct bomb_old* oldbomb, struct player* player);

void suppr_bomb(struct bomb_old* oldbomb);

int bomb_set_pos_x(struct bomb_elem* bomb, struct player* player);

int bomb_set_pos_y(struct bomb_elem* bomb, struct player* player);

void bomb_set_type(struct bomb_elem* bomb, enum compose_type type);

void bomb_statut(struct bomb_old* oldbomb, struct bomb_elem* bomb, struct map* map, struct player* player, struct game* game);

int bomb_update(struct bomb_old* oldbomb, struct map* map, struct player* player, struct game* game);