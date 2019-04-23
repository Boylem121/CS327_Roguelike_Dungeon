#include <limits.h>
#include <stdio.h>

#include "monster.h"

static int32_t corridor_path_cmp(const void *key, const void *with) {
  return ((corridor_path_t *) key)->cost - ((corridor_path_t *) with)->cost;
}


void path_through(dungeon_t *d)
{
  static corridor_path_t path[DUNGEON_Y][DUNGEON_X], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

  if (!initialized) {
    for (y = 0; y < DUNGEON_Y; y++) {
      for (x = 0; x < DUNGEON_X; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }
  
  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

  path[d->monsters[0].pos.y][d->monsters[0].pos.x].cost = 0;

  heap_init(&h, corridor_path_cmp, NULL);

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      if (d->hardness[y][x] == 0) {
        path[y][x].hn = heap_insert(&h, &path[y][x]);
      } else {
        path[y][x].hn = NULL;
      }
    }
  }

  while ((p = (corridor_path_t *)  heap_remove_min(&h))) {

    p->hn = NULL;

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost > p->cost)) {

      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost =  p->cost + 1;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn);
    }

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x]].cost > p->cost)) {

      path[p->pos[dim_y] - 1][p->pos[dim_x]].cost =  p->cost + 1;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x]].hn);
    }

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost > p->cost)) {

      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost =  p->cost + 1;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn);
    }

    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost > p->cost)) {

      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost = p->cost + 1;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn);
    }

    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1 ].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x]+ 1  ].cost > p->cost)) {

      path[p->pos[dim_y] + 1][p->pos[dim_x]+ 1].cost = p->cost + 1;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]+ 1].hn);
    }

    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x]].cost > p->cost)) {

      path[p->pos[dim_y] + 1][p->pos[dim_x]].cost = p->cost + 1;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]].hn);
    }

    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1 ].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x]- 1  ].cost > p->cost)) {

      path[p->pos[dim_y] + 1][p->pos[dim_x]- 1].cost = p->cost + 1;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]- 1].hn);
    }

    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost > p->cost)) {

      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost = p->cost + 1;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] - 1].hn);
    }    

  }

  
  
  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      d->dist_rooms[y][x] = path[y][x].cost;
      /*
      if(path[y][x].cost == 0){
	printf("@");
      }else if(path[y][x].cost != INT_MAX){
	printf("%i", (path[y][x].cost)%10);
      }else if(d->hardness[y][x] == 0 && path[y][x].cost == INT_MAX){
	printf("X");
      }else{
	printf(" ");
	}*/
      } 
      // printf("\n");
  }
 
}


void through_walls(dungeon_t *d)
{
  static corridor_path_t path[DUNGEON_Y][DUNGEON_X], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

  if (!initialized) {
    for (y = 0; y < DUNGEON_Y; y++) {
      for (x = 0; x < DUNGEON_X; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }
  
  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

  path[d->monsters[0].pos.y][d->monsters[0].pos.x].cost = 0;

  heap_init(&h, corridor_path_cmp, NULL);

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      if (d->hardness[y][x] != 255) {
        path[y][x].hn = heap_insert(&h, &path[y][x]);
      } else {
        path[y][x].hn = NULL;
      }
    }
  }

  while ((p = (corridor_path_t *) heap_remove_min(&h))) {

    p->hn = NULL;

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost > p->cost + 1 + (d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85))) {

      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost =  p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn);
    }

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x]].cost > p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85)) {

      path[p->pos[dim_y] - 1][p->pos[dim_x]].cost =  p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x]].hn);
    }

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost > p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85)) {

      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost =  p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn);
    }

    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost > p->cost + 1 +  d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85)) {

      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost = p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn);
    }

    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1 ].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x]+ 1  ].cost > p->cost + 1 +  d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85)) {

      path[p->pos[dim_y] + 1][p->pos[dim_x]+ 1].cost = p->cost + 1 +  d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]+ 1].hn);
    }

    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x]].cost > p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85)) {

      path[p->pos[dim_y] + 1][p->pos[dim_x]].cost = p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]].hn);
    }

    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1 ].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x]- 1  ].cost > p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85)) {

      path[p->pos[dim_y] + 1][p->pos[dim_x]- 1].cost = p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]- 1].hn);
    }

    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost > p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85)) {

      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost = p->cost + 1 + d->hardness[p->pos[dim_y]][p->pos[dim_x]]/85;

      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] - 1].hn);
    }    

  }


  
  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      d->dist_tunnel[y][x] = path[y][x].cost;
      /*
      if(path[y][x].cost == 0){
	printf("@");
      }else if(path[y][x].cost != INT_MAX){
	printf("%i", (path[y][x].cost)%10);
      }else{
	printf("X");
      }
      */
    }
    // printf("\n");
  }

  /*
  printf("\n");
  printf("\n");
  printf("\n");

  
  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      if(d->hardness[y][x] == 255){
	printf("X");
      }else if(path[y][x].cost == 0){
	printf("@");
      }else{
	printf("%i", (d->hardness[y][x]/85) + 1);
      }
    }
    printf("\n");
  }
  */
  
}

void gen_items(dungeon_t *d){

  int m;
  int md;
  int r;
  uint32_t xP;
  uint32_t yP;
  int openSpot = 0;

  for(yP = 0; yP < DUNGEON_Y; yP++){
    for(xP = 0; xP < DUNGEON_X; xP++){
      d->printitem[yP][xP] = NULL;
    }
  }

  for(yP = 0; yP < DUNGEON_Y; yP++){
    for(xP = 0; xP < DUNGEON_X; xP++){
      if(d->printChar[yP][xP] == '.' || d->printChar[yP][xP] == '>' || d->printChar[yP][xP] == '<'){
	openSpot++;
      }
    }
  }

  if(openSpot < d->numitem){
    d->numitem = openSpot;
  }

  for(m = 0; m < d->numitem; m++){
    do{

      r = rand()%d->numRooms;

      xP = rand()%d->rooms[r].rSize.x;
      yP = rand()%d->rooms[r].rSize.y;
   
      d->items[m].pos.y = d->rooms[r].rPos.y + yP;
      d->items[m].pos.x = d->rooms[r].rPos.x + xP;
      
    }while(d->printitem[d->items[m].pos.y][d->items[m].pos.x] != NULL);

    do{

      md = rand() % d->numitemd;

    }while(d->itemDesc[md].taken);

    int rc = rand()%100;

    if(rc < d->itemDesc[md].rarity){

      if(d->itemDesc[md].artifact){
	d->itemDesc[md].taken = 1;
      }

      d->items[m].icon = d->itemDesc[md].icon;
      d->items[m].color = d->itemDesc[md].color;
      d->items[m].name = d->itemDesc[md].name;
      d->items[m].desc = d->itemDesc[md].desc;
      d->items[m].dam = d->itemDesc[md].dam;
      d->items[m].speed = d->itemDesc[md].speed;

      d->printitem[d->items[m].pos.y][d->items[m].pos.x] = &d->items[m];
    }else{
      m--;
    }

  }
      
}


void gen_monsters(dungeon_t *d){

  int m;
  int md;
  int r;
  uint32_t xP;
  uint32_t yP;
  int openSpot = 0;

  for(yP = 0; yP < DUNGEON_Y; yP++){
    for(xP = 0; xP < DUNGEON_X; xP++){
      if(d->printChar[yP][xP] == '.' || d->printChar[yP][xP] == '>' || d->printChar[yP][xP] == '<'){
	openSpot++;
      }
    }
  }

  openSpot =  openSpot - (d->rooms[d->pcRoom].rSize.x * d->rooms[d->pcRoom].rSize.y);

  if(openSpot < d->numMon){
    d->numMon = openSpot + 1;
  }

  for(m = 1; m < d->numMon; m++){

    do{
      do{
	r = rand()%d->numRooms;
      }while(r == d->pcRoom);

      xP = rand()%d->rooms[r].rSize.x;
      yP = rand()%d->rooms[r].rSize.y;
   
      d->monsters[m].pos.y = d->rooms[r].rPos.y + yP;
      d->monsters[m].pos.x = d->rooms[r].rPos.x + xP;
      
    }while(d->print[d->monsters[m].pos.y][d->monsters[m].pos.x] != NULL);

    do{

      md = rand() % d->nummond;

    }while(d->monstersDesc[md].taken);

    int rc = rand()%100;

    if(rc < d->monstersDesc[md].rarity){

      

      if(d->monstersDesc[md].unique){
	d->monstersDesc[md].taken = 1;
      }

    

      d->monsters[m].speed = d->monstersDesc[md].speed;
      d->monsters[m].next_turn = 0;
      d->monsters[m].pc = 0;
      d->monsters[m].order = m;
      d->monsters[m].alive = 1;
      d->monsters[m].icon = d->monstersDesc[md].icon;
      d->monsters[m].color = d->monstersDesc[md].color;
      d->monsters[m].traits = d->monstersDesc[md].traits;
      //d->monsters[m].name = d->monstersDesc[md].name;
      d->monsters[m].desc = d->monstersDesc[md].desc;
      d->monsters[m].hp = d->monstersDesc[md].hp;
      d->monsters[m].dam = d->monstersDesc[md].dam;
      d->monsters[m].boss = d->monstersDesc[md].boss;
      d->monsters[m].light = d->monstersDesc[md].light;

      d->print[d->monsters[m].pos.y][d->monsters[m].pos.x] = &d->monsters[m];
    }else{
      m--;
    }

  }

  // std::cout << d->monstersDesc[0].icon << "jhfkgjfgj" << std::endl;

  /*

  for(m = 1; m < d->numMon; m++){
    d->monsters[m].traits = rand()%16;
    do{
      do{
	r = rand()%d->numRooms;
      }while(r == d->pcRoom);

      xP = rand()%d->rooms[r].rSize.x;
      yP = rand()%d->rooms[r].rSize.y;
   
      d->monsters[m].pos.y = d->rooms[r].rPos.y + yP;
      d->monsters[m].pos.x = d->rooms[r].rPos.x + xP;
      
    }while(d->print[d->monsters[m].pos.y][d->monsters[m].pos.x] != NULL);

    d->monsters[m].speed = (rand()%16)+5;
    d->monsters[m].next_turn = 0;
    d->monsters[m].pc = 0;
    d->monsters[m].order = m;
    d->monsters[m].alive = 1;

    d->print[d->monsters[m].pos.y][d->monsters[m].pos.x] = &d->monsters[m];

    switch(d->monsters[m].traits){

    case 0:
      d->monsters[m].icon = '0';
      break;
    case 1:
      d->monsters[m].icon = '1';
      break;
    case 2:
      d->monsters[m].icon = '2';
      break;
    case 3:
      d->monsters[m].icon = '3';
      break;
    case 4:
      d->monsters[m].icon = '4';
      break;
    case 5:
      d->monsters[m].icon = '5';
      break;
    case 6:
      d->monsters[m].icon = '6';
      break;
    case 7:
      d->monsters[m].icon = '7';
      break;
    case 8:
      d->monsters[m].icon = '8';
      break;
    case 9:
      d->monsters[m].icon = '9';
      break;
    case 10:
      d->monsters[m].icon = 'A';
      break;
    case 11:
      d->monsters[m].icon = 'B';
      break;
    case 12:
      d->monsters[m].icon = 'C';
      break;
    case 13:
      d->monsters[m].icon = 'D';
      break;
    case 14:
      d->monsters[m].icon = 'E';
      break;
    case 15:
      d->monsters[m].icon = 'F';
      break;
    }
  }
  */

      
}

static int32_t monster_cmp(const void *key, const void *with) {
  if(((npc_t *) key)->next_turn - ((npc_t *) with)->next_turn != 0){
    return ((npc_t *) key)->next_turn - ((npc_t *) with)->next_turn;
  }else{
    return((npc_t *) key)->order - ((npc_t *) with)->order;
  }
}

static void move_tunnel(dungeon_t *d, npc_t *c){
  
  int8_t x ,y;
  uint8_t dir = 9;
  int cost = INT_MAX;
  int i = 0;
  uint8_t tunnel = 0;

  int pX = c->pos.x;
  int pY = c->pos.y;

  through_walls(d);

  if(d->printChar[pY][pX] == ' '){
    tunnel = 1;
  }

  for(y = -1; y < 2; y++){
    for(x = -1; x < 2; x++){
      if(d->dist_tunnel[c->pos.y + y][c->pos.x + x] < cost){
	cost = d->dist_tunnel[c->pos.y + y][c->pos.x + x];
	dir = i;
      }
      i++;
    }
  }

  d->print[c->pos.y][c->pos.x] = NULL;

  switch(dir){

  case 0:
    c->pos.y--;
    c->pos.x--;
    break;
  case 1:
    c->pos.y--;
    break;
  case 2:
    c->pos.y--;
    c->pos.x++;
    break;
  case 3:
    c->pos.x--;
    break;
  case 4:
    break;
  case 5:
    c->pos.x++;
    break;
  case 6:
    c->pos.y++;
    c->pos.x--;
    break;
  case 7:
    c->pos.y++;
    break;
  case 8:
    c->pos.y++;
    c->pos.x++;
    break;
  }


  if(tunnel){
    if(d->hardness[pY][pX] < 85){

    d->printChar[pY][pX] = '#';

    if(c->pos.x == 0){
      c->pos.x = 1;
    }else if(c->pos.x == DUNGEON_X-1){
      c->pos.x = DUNGEON_X - 2;
    }

    if(c->pos.y == 0){
      c->pos.y = 1;
    }else if(c->pos.y == DUNGEON_Y-1){
      c->pos.y = DUNGEON_Y - 2;
    }

    if(d->print[c->pos.y][c->pos.x] != NULL){
      // printf("%c\n", d->print[c->pos.y][c->pos.x]->icon);
      if(d->print[c->pos.y][c->pos.x] != &d->monsters[0]){
	d->print[c->pos.y][c->pos.x]->pos.x = pX;
	d->print[c->pos.y][c->pos.x]->pos.y = pY;
	d->print[pY][pX] = d->print[c->pos.y][c->pos.x];
	d->print[c->pos.y][c->pos.x] = c;
	
      }else{
	c->pos.x = pX;
	c->pos.y = pY;
	d->print[c->pos.y][c->pos.x] = c;
	d->monsters[0].hp = d->monsters[0].hp - c->dam.roll();
	if(d->monsters[0].hp < 0){
	  d->monsters[0].alive = 0;
	}
      }
    }else{
      d->print[c->pos.y][c->pos.x] = c;
    }
    }else{
      d->hardness[pY][pX] -= 85;
      c->pos.y = pY;
      c->pos.x = pX;
      d->print[pY][pX] = c;
    }

  }else{

   

    if(c->pos.x == 0){
      c->pos.x = 1;
    }else if(c->pos.x == DUNGEON_X-1){
      c->pos.x = DUNGEON_X - 2;
    }

    if(c->pos.y == 0){
      c->pos.y = 1;
    }else if(c->pos.y == DUNGEON_Y-1){
      c->pos.y = DUNGEON_Y - 2;
    }

    if(d->print[c->pos.y][c->pos.x] != NULL){
      // printf("%c\n", d->print[c->pos.y][c->pos.x]->icon);
      if(d->print[c->pos.y][c->pos.x] != &d->monsters[0]){
	d->print[c->pos.y][c->pos.x]->pos.x = pX;
	d->print[c->pos.y][c->pos.x]->pos.y = pY;
	d->print[pY][pX] = d->print[c->pos.y][c->pos.x];
	d->print[c->pos.y][c->pos.x] = c;
	
      }else{
	c->pos.x = pX;
	c->pos.y = pY;
	d->print[c->pos.y][c->pos.x] = c;
	d->monsters[0].hp = d->monsters[0].hp - c->dam.roll();
	if(d->monsters[0].hp < 0){
	  d->monsters[0].alive = 0;
	}
      }
    }else{
      d->print[c->pos.y][c->pos.x] = c;
    }

  }
}

static void move_smart(dungeon_t *d, npc_t *c){
  
  int8_t x ,y;
  uint8_t dir = 9;
  int cost = INT_MAX;
  int i = 0;
  int pX = c->pos.x;
  int pY = c->pos.y;

  path_through(d);

  for(y = -1; y < 2; y++){
    for(x = -1; x < 2; x++){
      if(d->dist_rooms[c->pos.y + y][c->pos.x + x] < cost){
	cost = d->dist_rooms[c->pos.y + y][c->pos.x + x];
	dir = i;
      }
      i++;
    }
  }

  d->print[c->pos.y][c->pos.x] = NULL;

  switch(dir){

  case 0:
    c->pos.y--;
    c->pos.x--;
    break;
  case 1:
    c->pos.y--;
    break;
  case 2:
    c->pos.y--;
    c->pos.x++;
    break;
  case 3:
    c->pos.x--;
    break;
  case 4:
    break;
  case 5:
    c->pos.x++;
    break;
  case 6:
    c->pos.y++;
    c->pos.x--;
    break;
  case 7:
    c->pos.y++;
    break;
  case 8:
    c->pos.y++;
    c->pos.x++;
    break;
  }

  if(c->pos.x == 0){
      c->pos.x = 1;
    }else if(c->pos.x == DUNGEON_X-1){
      c->pos.x = DUNGEON_X - 2;
    }

    if(c->pos.y == 0){
      c->pos.y = 1;
    }else if(c->pos.y == DUNGEON_Y-1){
      c->pos.y = DUNGEON_Y - 2;
    }

    if(d->print[c->pos.y][c->pos.x] != NULL){
      // printf("%c\n", d->print[c->pos.y][c->pos.x]->icon);
      if(d->print[c->pos.y][c->pos.x] != &d->monsters[0]){
	d->print[c->pos.y][c->pos.x]->pos.x = pX;
	d->print[c->pos.y][c->pos.x]->pos.y = pY;
	d->print[pY][pX] = d->print[c->pos.y][c->pos.x];
	d->print[c->pos.y][c->pos.x] = c;
	
      }else{
	c->pos.x = pX;
	c->pos.y = pY;
	d->print[c->pos.y][c->pos.x] = c;
	d->monsters[0].hp = d->monsters[0].hp - c->dam.roll();
	if(d->monsters[0].hp < 0){
	  d->monsters[0].alive = 0;
	}
      }
    }else{
      d->print[c->pos.y][c->pos.x] = c;
    }

  }

static void tellepathic_tunnel(dungeon_t *d, npc_t *c){

  d->print[c->pos.y][c->pos.x] = NULL;

  int pX = c->pos.x;
  int pY = c->pos.y;

  if(c->pos.x > d->monsters[0].pos.x){
    c->pos.x--;
  }else if(c->pos.x < d->monsters[0].pos.x){
    c->pos.x++;
  }else{
    if(c->pos.y > d->monsters[0].pos.y){
      c->pos.y--;
    }else if(c->pos.y < d->monsters[0].pos.y){
      c->pos.y++;
    }
  }

  if(d->printChar[c->pos.y][c->pos.x] != '.' && d->printChar[c->pos.y][c->pos.x] != '#' && d->printChar[c->pos.y][c->pos.x] != '<' && d->printChar[c->pos.y][c->pos.x] != '>'){

    if(haschar(c->traits, tunnling)){

      if(d->hardness[pY][pX] < 85){

	if(d->printChar[pY][pX] == ' '){
	  d->printChar[pY][pX] = '#';
	}

      }else{

	c->pos.y = pY;
	c->pos.x = pX;

	d->hardness[pY][pX] -= 85;
      }
    }else{
      c->pos.y = pY;
      c->pos.x = pX;
    }

	
  }else{
    if(d->printChar[pY][pX] == ' '){
      if(d->hardness[pY][pX] < 85){

	if(d->printChar[pY][pX] == ' '){
	  d->printChar[pY][pX] = '#';
	}

      }else{

	c->pos.y = pY;
	c->pos.x = pX;

	d->hardness[pY][pX] -= 85;
      }
    }
  }

  
  if(c->pos.x == 0){
    c->pos.x = 1;
  }else if(c->pos.x == DUNGEON_X-1){
    c->pos.x = DUNGEON_X - 2;
  }

  if(c->pos.y == 0){
    c->pos.y = 1;
  }else if(c->pos.y == DUNGEON_Y-1){
    c->pos.y = DUNGEON_Y - 2;
  }

    if(d->print[c->pos.y][c->pos.x] != NULL){
      // printf("%c\n", d->print[c->pos.y][c->pos.x]->icon);
      if(d->print[c->pos.y][c->pos.x] != &d->monsters[0]){
	d->print[c->pos.y][c->pos.x]->pos.x = pX;
	d->print[c->pos.y][c->pos.x]->pos.y = pY;
	d->print[pY][pX] = d->print[c->pos.y][c->pos.x];
	d->print[c->pos.y][c->pos.x] = c;
	
      }else{
	c->pos.x = pX;
	c->pos.y = pY;
	d->print[c->pos.y][c->pos.x] = c;
	d->monsters[0].hp = d->monsters[0].hp - c->dam.roll();
	if(d->monsters[0].hp < 0){
	  d->monsters[0].alive = 0;
	}
      }
    }else{
      d->print[c->pos.y][c->pos.x] = c;
    }

}


static void move_erratic(dungeon_t *d, npc_t *c){

  uint8_t err = rand()%2;

  if(err){

    uint8_t dir = rand()%8;

    d->print[c->pos.y][c->pos.x] = NULL;

    int pX = c->pos.x;
    int pY = c->pos.y;

    switch(dir){

    case 0:
      c->pos.y--;
      c->pos.x--;
      break;
    case 1:
      c->pos.y--;
      break;
    case 2:
      c->pos.y--;
      c->pos.x++;
      break;
    case 3:
      c->pos.x++;
      break;
    case 4:
      c->pos.y++;
      c->pos.x++;
      break;
    case 5:
      c->pos.y++;
      break;
    case 6:
      c->pos.y++;
      c->pos.x--;
      break;
    case 7:
      c->pos.x--;
      break;
    }

    if(d->printChar[c->pos.y][c->pos.x] != '.' && d->printChar[c->pos.y][c->pos.x] != '#' && d->printChar[c->pos.y][c->pos.x] != '<' && d->printChar[c->pos.y][c->pos.x] != '>'){

      if(haschar(c->traits, tunnling)){

	if(d->hardness[pY][pX] < 85){

	  if(d->printChar[pY][pX] == ' '){
	    d->printChar[pY][pX] = '#';
	  }

	}else{

	  c->pos.y = pY;
	  c->pos.x = pX;

	  d->hardness[pY][pX] -= 85;
	}
      }else{
	c->pos.y = pY;
	c->pos.x = pX;
      }

	
    }else{
      if(d->printChar[pY][pX] == ' '){
	if(d->hardness[pY][pX] < 85){

	  if(d->printChar[pY][pX] == ' '){
	    d->printChar[pY][pX] = '#';
	  }

	}else{

	  c->pos.y = pY;
	  c->pos.x = pX;

	  d->hardness[pY][pX] -= 85;
	}
      }
    }

    
    if(c->pos.x == 0){
      c->pos.x = 1;
    }else if(c->pos.x == DUNGEON_X-1){
      c->pos.x = DUNGEON_X - 2;
    }

    if(c->pos.y == 0){
      c->pos.y = 1;
    }else if(c->pos.y == DUNGEON_Y-1){
      c->pos.y = DUNGEON_Y - 2;
    }

    if(d->print[c->pos.y][c->pos.x] != NULL){
      // printf("%c\n", d->print[c->pos.y][c->pos.x]->icon);
      if(d->print[c->pos.y][c->pos.x] != &d->monsters[0]){
	d->print[c->pos.y][c->pos.x]->pos.x = pX;
	d->print[c->pos.y][c->pos.x]->pos.y = pY;
	d->print[pY][pX] = d->print[c->pos.y][c->pos.x];
	d->print[c->pos.y][c->pos.x] = c;
	
      }else{
	c->pos.x = pX;
	c->pos.y = pY;
	d->print[c->pos.y][c->pos.x] = c;
	d->monsters[0].hp = d->monsters[0].hp - c->dam.roll();
	if(d->monsters[0].hp < 0){
	  d->monsters[0].alive = 0;
	}
      }
    }else{
      d->print[c->pos.y][c->pos.x] = c;
    }


  }else{

      int newTrait = c->traits & ~(1UL << 3);
    
      switch(newTrait){
    
      case 0:
	//printf("0\n");
	break;
      case 1:
	//printf("1\n");
	break;
      case 2:
	//printf("2\n");
	break;
      case 3:
	move_smart(d, c);
	break;
      case 4:
	//printf("4\n");
	break;
      case 5:
	//printf("5\n");
	break;
      case 6:
	tellepathic_tunnel(d, c);
	break;
      case 7:
	move_tunnel(d, c);
	break;
      }

  }
  

}

static void move_monster(dungeon_t *d, npc_t *c){
  
  switch(c->traits){
    
  case 0:
    //printf("0\n");
    break;
  case 1:
    //printf("1\n");
    break;
  case 2:
    //printf("2\n");
    break;
  case 3:
    move_smart(d, c);
    break;
  case 4:
    //printf("4\n");
    break;
  case 5:
    //printf("5\n");
    break;
  case 6:
    tellepathic_tunnel(d, c);
    break;
  case 7:
    move_tunnel(d, c);
    break;
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    move_erratic(d, c);
    break;
  }
   
}

static void expunge_items(dungeon_t *d){

  clear();
  
  int i = 1;
  int s = 0;
  char key;
  do{
    for(i = 0; i < 10; i++){

      if(d->pcitems[i] == NULL){
	if(i == s){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%i: %s", i, " ");
	if(i == s){
	  attroff(A_BLINK);
	}
      }else{
	if(i == s){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%i: %s", i, d->pcitems[i]->name.c_str());
	if(i == s){
	  attroff(A_BLINK);
	}
      }

    }

    refresh();
  
    key = getch();

    if(key == 3){
      if(s > 0){
	s--;
      }
    }

    if(key == 2){
      if(s < 11){
	s++;
      }
    }
    clear();
  }while(key != 27 && key != 10);

  d->pcitems[s] = NULL;

    
}

static void drop_items(dungeon_t *d){

  clear();
  
  if(d->printitem[d->monsters[0].pos.y][d->monsters[0].pos.x] == NULL){

  int i = 1;
  int s = 0;
  char key;
  do{
    for(i = 0; i < 10; i++){

      if(d->pcitems[i] == NULL){
	if(i == s){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%i: %s", i, " ");
	if(i == s){
	  attroff(A_BLINK);
	}
      }else{
	if(i == s){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%i: %s", i, d->pcitems[i]->name.c_str());
	if(i == s){
	  attroff(A_BLINK);
	}
      }

    }

    refresh();
  
    key = getch();

    if(key == 3){
      if(s > 0){
	s--;
      }
    }

    if(key == 2){
      if(s < 11){
	s++;
      }
    }
    clear();
  }while(key != 27 && key != 10);

  d->printitem[d->monsters[0].pos.y][d->monsters[0].pos.x] = d->pcitems[s];
  d->pcitems[s] = NULL;

  }else{
    char key;
    printw("Space Already Occupied Move to an Open Space to Drop");
    while(key != 27){
    
    key = getch();
    }
  }

    
}

static void take_off_items(dungeon_t *d){

  clear();

  int s = 0;
  int i = 0;
  char key;
  while(d->pcitems[i] != NULL){
    i++;
  }
  if(i < 10){

  


  do{
   
    for(i = 0; i < 12; i++){

      if(d->pcequip[i] != NULL){
	if(i == s){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%c: %s", i + 97 , d->pcequip[i]->name.c_str());
	if(i == s){
	  attroff(A_BLINK);
	}
      }else{
	if(i == s){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%c:", i + 97);
	if(i == s){
	  attroff(A_BLINK);
	}
      }
    }  

    refresh();
  
    key = getch();

    if(key == 3){
      if(s > 0){
	s--;
      }
    }

    if(key == 2){
      if(s < 11){
	s++;
      }
    }
    clear();
    if(d->pcequip[s] == NULL && key == 10){
      clear();
      printw("Spot Already Empty Pick Another");
      while(key != 27){
	key = getch();
      }
      key = 90;
      clear();
    }
  }while(key != 27 && key != 10);

  i = 0;

  while(d->pcitems[i] != NULL){
    i++;
  }
  if(key != 27){
  d->monsters[0].speed -= d->pcequip[s]->speed;
  }
  d->pcitems[i] = d->pcequip[s];
  d->pcequip[s] = NULL;
  }else{
    printw("Must Drop Item First Inventory Full");
    while(key != 27){
      key = getch();
    }
  }

  if(s == 9){
    d->pcLight--;
  }

    
}

static void wear_items(dungeon_t *d){

  clear();

  int s = 0;
  int i = 1;
  char key;

  do{
   
    for(i = 0; i < 12; i++){

      if(d->pcequip[i] != NULL){
	if(i == s){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%c: %s", i + 97 , d->pcequip[i]->name.c_str());
	if(i == s){
	  attroff(A_BLINK);
	}
      }else{
	if(i == s){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%c:", i + 97);
	if(i == s){
	  attroff(A_BLINK);
	}
      }
    }  

    refresh();
  
    key = getch();

    if(key == 3){
      if(s > 0){
	s--;
      }
    }

    if(key == 2){
      if(s < 11){
	s++;
      }
    }
    clear();
  }while(key != 27 && key != 10);

  int se = 0;

  while(key != 27){

    for(i = 0; i < 10; i++){
      if(d->pcitems[i] == NULL){
	if(se == i){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%i: %s", i, " ");
	if(se == i){
	  attroff(A_BLINK);
	}
      }else{
	if(se == i){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%i: %s, %i", i, d->pcitems[i]->name.c_str());
	if(se == i){
	  attroff(A_BLINK);
	}
      }
    }

    refresh();

    key = getch();

    i = 0;
    int pl = 1;

    item_t *temp;

    if(key == 10){
      if(d->pcitems[se] == NULL){
	clear();
	printw("Spot Blank Pick Item");
	while(key != 27){
	  
	  key = getch();
	}
      }else{
	switch(s){
	case 0:
	  if(d->pcitems[se]->icon != '|'){
	    pl = 0;
	  }
	  break;
	case 1:
	  if(d->pcitems[se]->icon != ')'){
	    pl = 0;
	  }
	  break;
	case 2:
	  if(d->pcitems[se]->icon != '}'){
	    pl = 0;
	  }
	  break;
	case 3:
	  if(d->pcitems[se]->icon != '['){
	    pl = 0;
	  }
	  break;
	case 4:
	  if(d->pcitems[se]->icon != ']'){
	    pl = 0;
	  }
	  break;
	case 5:
	  if(d->pcitems[se]->icon != '('){
	    pl = 0;
	  }
	  break;
	case 6:
	  if(d->pcitems[se]->icon != '{'){
	    pl = 0;
	  }
	  break;
	case 7:
	  if(d->pcitems[se]->icon != '\\'){
	    pl = 0;
	  }
	  break;
	case 8:
	  if(d->pcitems[se]->icon != '"'){
	    pl = 0;
	  }
	  break;
	case 9:
	  if(d->pcitems[se]->icon != '_'){
	    pl = 0;
	  }else{
	    d->pcLight++;
	  }
	  break;
	case 10:
	  if(d->pcitems[se]->icon != '='){
	    pl = 0;
	  }
	  break;
	case 11:
	  if(d->pcitems[se]->icon != '='){
	    pl = 0;
	  }
	  break;
	}
	if(pl){
	  if(d->monsters[0].speed + d->pcitems[se]->speed > 0){
	    if(d->pcequip[s] != NULL){
	      d->monsters[0].speed -= d->pcequip[s]->speed;
	      temp = d->pcitems[se];
	      d->pcitems[se] = NULL;
	      while(d->pcitems[i] != NULL){
		i++;
	      }
	      d->pcitems[i] = d->pcequip[s];
	      d->pcequip[s] = temp;
	  
	    }else{
	      d->pcequip[s] = d->pcitems[se];
	      d->pcitems[se] = NULL;
	    }
	    d->monsters[0].speed += d->pcequip[s]->speed;
	  }else{
	    clear();
	    printw("Speed Below Zero Can't Equip");
	    while(key != 27){
	  
	      key = getch();
	    }
	  }

	  key = 27;
	}else{
	  clear();
	  printw("Wrong Item put in Another Slot");
	  while(key != 27){
	  
	    key = getch();
	  }
	}
      
      }
    }

    if(key == 3){
      if(se > 0){
	se--;
      }
    }

    if(key == 2){
      if(se < 10){
	se++;
      }
    }
    clear();
  }
    
}

static void list_equip(dungeon_t *d){

  clear();
  
  int i = 1;
  
    for(i = 0; i < 12; i++){
      if(d->pcequip[i] != NULL){
	mvprintw(i, 0, "%c: %s", i + 97 , d->pcequip[i]->name.c_str());
      }else{
	mvprintw(i, 0, "%c:", i + 97);
      }
    }  

    refresh();
    
    char key;

    do{
     key = getch();
    }while(key != 27);

}

static void list_items(dungeon_t *d){

  clear();
  
  int i = 1;
  
    for(i = 0; i < 10; i++){

      if(d->pcitems[i] == NULL){
	mvprintw(i, 0, "%i: %s", i, " ");
      }else{
	mvprintw(i, 0, "%i: %s", i, d->pcitems[i]->name.c_str());
      }

    }
    
    char key;

    do{
     key = getch();
    }while(key != 27);

}

static void list_monster(dungeon_t *d){

  clear();
  
  int i = 1;
  int k = i;
  int max = 25;
  int key_check;
  char *c = (char * ) " ";

  if(max > d->numMon){
    max = d->numMon;
  }

  do{
  
    for(; i < max; i++){

      mvprintw(i-k, 0, "%c: %s", d->monsters[i].icon, c);

      if(d->monsters[i].pos.y > d->monsters[0].pos.y){
	printw("%i South ", d->monsters[i].pos.y - d->monsters[0].pos.y);
      }else if(d->monsters[i].pos.y < d->monsters[0].pos.y){
	printw("%i North ", d->monsters[0].pos.y - d->monsters[i].pos.y);
      }

      if(d->monsters[i].pos.x > d->monsters[0].pos.x){
	printw("%i East ", d->monsters[i].pos.x - d->monsters[0].pos.x);
      }else if(d->monsters[i].pos.x < d->monsters[0].pos.x){
	printw("%i West ", d->monsters[0].pos.x - d->monsters[i].pos.x);
      }
    
    }

    refresh();
  
    key_check = getch();

    if(key_check == 259){
      if(k - 24 < 1){
	max = max - (k-1);
	k = 1;
      }else{
	k = k - 24;
	max = max - 24;
      }
    }

    if(key_check == 258){
      if(max + 24 > d->numMon){
	k = k + (d->numMon - max);
	max = d->numMon;
      }else{
	k = k + 24;
	max = max + 24;
      }
    }

    i = k;
    clear();

  }while(key_check != 27);

}

static void inspect_items(dungeon_t *d){

  clear();
  
  int i = 1;
  int s = 0;
  char key;
  do{
    for(i = 0; i < 10; i++){

      if(d->pcitems[i] == NULL){
	if(i == s){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%i: %s", i, " ");
	if(i == s){
	  attroff(A_BLINK);
	}
      }else{
	if(i == s){
	  attron(A_BLINK);
	}
	mvprintw(i, 0, "%i: %s", i, d->pcitems[i]->name.c_str());
	if(i == s){
	  attroff(A_BLINK);
	}
      }

    }

    refresh();
  
    key = getch();

    if(key == 3){
      if(s > 0){
	s--;
      }
    }

    if(key == 2){
      if(s < 11){
	s++;
      }
    }
    clear();
  }while(key != 27 && key != 10);

  while(key != 27){

    clear();

    printw("Name: %s \n\n",  d->pcitems[s]->name.c_str());
    printw("Icon: %c \n\n",  d->pcitems[s]->icon);
    printw("Description: %s",   d->pcitems[s]->desc.c_str());
  
    key = getch();
  };

    
}

static void inspect_monster(dungeon_t *d){
  
  
  int x;
  int y;

  int px;
  int py;
  
  int key;

  int cx = d->monsters[0].pos.x;
  int cy = d->monsters[0].pos.y;

  for(y = d->pcLight * -1; y <= d->pcLight; y++){
    for(x = d->pcLight * -1; x <= d->pcLight; x++){
      if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x];
      }
    }
  }

  do{

    px = cx;
    py = cy;

    for(y = d->pcLight * -1; y <= d->pcLight; y++){
      for(x = d->pcLight * -1; x <= d->pcLight; x++){
	if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	  if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] == NULL){
	    d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x];
	  }else{
	    d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon;
	  }
	}
      }
    }

    clear();
    for(y = 0; y < DUNGEON_Y; y++){
      for(x = 0; x < DUNGEON_X; x++){

	if(d->pc_view[y][x] != (char) NULL){
	  mvaddch(y , x, d->pc_view[y][x]);
	}else if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
	  int x2;
	  int y2;
	  for(y2 = -1; y2 <= 1; y2++){
	    for(x2 = -1; x2 <= 1; x2++){
	      d->pc_view[y + y2][x + x2] = d->printChar[y + y2][x + x2];
	    }
	  }
	}else{
	  mvaddch(y , x, ' ');
	}
	//printf("\n");
      }
    }

    for(y = 0; y < DUNGEON_Y; y++){
      for(x = 0; x < DUNGEON_X; x++){
	if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
	  int x2;
	  int y2;
	  for(y2 = -1; y2 <= 1; y2++){
	    for(x2 = -1; x2 <= 1; x2++){
	      if(y + y2 < DUNGEON_Y && x + x2 < DUNGEON_X && y + y2 >= 0 && x + x2 >= 0){
		attron(A_BOLD);
		if(d->print[y + y2][x + x2] != (int) NULL){
		  attron(COLOR_PAIR(d->print[y + y2][x + x2]->color));
		  mvaddch(y + y2, x + x2, d->print[y + y2][x + x2]->icon);
		  attroff(COLOR_PAIR(d->print[y + y2][x + x2]->color));
		}else{
		  if(d->printitem[y + y2][x + x2] != (int) NULL){
		    attron(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
		    mvaddch(y + y2 , x + x2, d->printitem[y + y2][x + x2]->icon);
		    attroff(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
		  }else{
		    mvaddch(y + y2, x + x2, d->printChar[y + y2][x + x2]);
		  }
		}
		attroff(A_BOLD);
	      }
	    }
	  }

	}
      }
    }

    for(y = d->pcLight * -1; y <= d->pcLight; y++){
      for(x = d->pcLight * -1; x <= d->pcLight; x++){
	if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	  attron(A_BOLD);
	  if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] != (int) NULL){
	    attron(COLOR_PAIR(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	    mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon);
	    attroff(COLOR_PAIR(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	  }else{
	    if(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] != (int) NULL){
	      attron(COLOR_PAIR(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	      mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon);
	      attroff(COLOR_PAIR(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	    }else{
	      mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]);
	    }
	  }
	  attroff(A_BOLD);
	}
      }
    }
    refresh();

    attron(A_BLINK);
    mvaddch(cy , cx, '*');
    attroff(A_BLINK);
    refresh();

    key = getch();

    switch(key){
    case 'y':
    case '7':
      cy--;
      cx--;
      break;
    case 'k':
    case '8':
      cy--;
      break;
    case 'u':
    case '9':
      cy--;
      cx++;
      break;
    case 'l':
    case '6':
      cx++;
      break;
    case 'n':
    case '3':
      cy++;
      cx++;
      break;
    case 'j':
    case '2':
      cy++;
      break;
    case 'b':
    case '1':
      cy++;
      cx--;
      break;
    case 'h':
    case '4':
      cx--;
      break;
    default:
      break;
    }

    if(cx < 1 || cx > DUNGEON_X - 2){
      cx = px;
    }

    if(cy < 1 || cy > DUNGEON_Y - 2){
      cy = py;
    }

    if(key == 'L' && d->print[cy][cx] == NULL && d->printitem[cy][cx] == NULL){
      clear();
      printw("Empty spot pick agin");
      do{
	key = getch();
      }while(key != 27);
      key = 5;
    }

  }while(key != 'L' && key != 27);

  if(key != 27){
    clear();
    if(d->print[cy][cx] != NULL){

      printw("Icon: %c \n\n",  d->print[cy][cx]->icon);
      printw("Description: %s",  d->print[cy][cx]->desc.c_str());

    }else if(d->printitem[cy][cx] != NULL){

      printw("Icon: %c \n\n",  d->printitem[cy][cx]->icon);
      printw("Description: %s",  d->printitem[cy][cx]->desc.c_str());

    }

    do{
	key = getch();
    }while(key != 27);

  }
}

static void teleport(dungeon_t *d){

  int x;
  int y;

  int px;
  int py;
  
  int key;

  int cx = d->monsters[0].pos.x;
  int cy = d->monsters[0].pos.y;

  for(y = d->pcLight * -1; y <= d->pcLight; y++){
    for(x = d->pcLight * -1; x <= d->pcLight; x++){
      if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x];
      }
    }
  }

  do{

    px = cx;
    py = cy;

    clear();

    for(y = 0; y < DUNGEON_Y; y++){
      for(x = 0; x < DUNGEON_X; x++){
	if(d->print[y][x] != (int) NULL){
	  attron(COLOR_PAIR(d->print[y][x]->color));
	  mvaddch(y , x, d->print[y][x]->icon);
	  attroff(COLOR_PAIR(d->print[y][x]->color));
	}else{
	  if(d->printitem[y][x] != (int) NULL){
	    attron(COLOR_PAIR(d->printitem[y][x]->color));
	    mvaddch(y , x, d->printitem[y][x]->icon);
	    attroff(COLOR_PAIR(d->printitem[y][x]->color));
	  }else{
	    mvaddch(y , x, d->printChar[y][x]);
	  }
	}
	//printf("\n");
      }
    }
    refresh();

    attron(A_BLINK);
    mvaddch(cy , cx, '*');
    attroff(A_BLINK);
    refresh();

    key = getch();

    switch(key){
    case 'y':
    case '7':
      cy--;
      cx--;
      break;
    case 'k':
    case '8':
      cy--;
      break;
    case 'u':
    case '9':
      cy--;
      cx++;
      break;
    case 'l':
    case '6':
      cx++;
      break;
    case 'n':
    case '3':
      cy++;
      cx++;
      break;
    case 'j':
    case '2':
      cy++;
      break;
    case 'b':
    case '1':
      cy++;
      cx--;
      break;
    case 'h':
    case '4':
      cx--;
      break;
    default:
      break;
    }

    if(cx < 1 || cx > DUNGEON_X - 2){
      cx = px;
    }

    if(cy < 1 || cy > DUNGEON_Y - 2){
      cy = py;
    }

  }while(key != 'g' && key != 'r');

  if(key == 'g'){

    d->print[d->monsters[0].pos.y][d->monsters[0].pos.x] = NULL;

    d->monsters[0].pos.y = cy;
    d->monsters[0].pos.x = cx;

    if(d->print[d->monsters[0].pos.y][d->monsters[0].pos.x]  != NULL){
      d->print[d->monsters[0].pos.y][d->monsters[0].pos.x]->alive = 0; 
    }

    d->print[d->monsters[0].pos.y][d->monsters[0].pos.x] = &d->monsters[0];

	
    for(y = -2; y < 3; y++){
      for(x = -2; x < 3; x++){
	if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	  if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] == NULL){
	    d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x];
	  }else{
	    d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon;
	  }
	}
      }
    }

  }

  if(key == 'r'){

    d->print[d->monsters[0].pos.y][d->monsters[0].pos.x] = NULL;

    d->monsters[0].pos.y = rand()%(DUNGEON_Y - 1) + 1;
    d->monsters[0].pos.x = rand()%(DUNGEON_X - 1) + 1;

    if(d->print[d->monsters[0].pos.y][d->monsters[0].pos.x]  != NULL){
      d->print[d->monsters[0].pos.y][d->monsters[0].pos.x]->alive = 0; 
    }

    d->print[d->monsters[0].pos.y][d->monsters[0].pos.x] = &d->monsters[0];

	
  for(y = d->pcLight * -1; y <= d->pcLight; y++){
    for(x = d->pcLight * -1; x <= d->pcLight; x++){
	if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	  if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] == NULL){
	    d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x];
	  }else{
	    d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon;
	  }
	}
      }
    }

  
  }


  clear();


	
  for(y = d->pcLight * -1; y <= d->pcLight; y++){
    for(x = d->pcLight * -1; x <= d->pcLight; x++){
      if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] == NULL){
	  d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x];
	}else{
	  d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon;
	}
      }
    }
  }
	
  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){

      if(d->pc_view[y][x] != (char) NULL){
	mvaddch(y , x, d->pc_view[y][x]);
      }else if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
	int x2;
	int y2;
	for(y2 = -1; y2 <= 1; y2++){
	  for(x2 = -1; x2 <= 1; x2++){
	    d->pc_view[y + y2][x + x2] = d->printChar[y + y2][x + x2];
	  }
	}
      }else{
	mvaddch(y , x, ' ');
      }
      //printf("\n");
    }
  }

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
	int x2;
	int y2;
	for(y2 = -1; y2 <= 1; y2++){
	  for(x2 = -1; x2 <= 1; x2++){
	    if(y + y2 < DUNGEON_Y && x + x2 < DUNGEON_X && y + y2 >= 0 && x + x2 >= 0){
	      attron(A_BOLD);
	      if(d->print[y + y2][x + x2] != (int) NULL){
		attron(COLOR_PAIR(d->print[y + y2][x + x2]->color));
		mvaddch(y + y2, x + x2, d->print[y + y2][x + x2]->icon);
		attroff(COLOR_PAIR(d->print[y + y2][x + x2]->color));
	      }else{
		if(d->printitem[y + y2][x + x2] != (int) NULL){
		  attron(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
		  mvaddch(y + y2 , x + x2, d->printitem[y + y2][x + x2]->icon);
		  attroff(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
		}else{
		  mvaddch(y + y2, x + x2, d->printChar[y + y2][x + x2]);
		}
	      }
	      attroff(A_BOLD);
	    }
	  }
	}

      }
    }
  }

  for(y = d->pcLight * -1; y <= d->pcLight; y++){
    for(x = d->pcLight * -1; x <= d->pcLight; x++){
      if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	attron(A_BOLD);
	if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] != (int) NULL){
	  attron(COLOR_PAIR(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	  mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon);
	  attroff(COLOR_PAIR(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	}else{
	  if(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] != (int) NULL){
	    attron(COLOR_PAIR(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	    mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon);
	    attroff(COLOR_PAIR(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	  }else{
	    mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]);
	  }
	}
	attroff(A_BOLD);
      }
    }
  }
  refresh();

  int i = 0;
    if(d->printitem[d->monsters[0].pos.y][d->monsters[0].pos.x] != NULL){
      while(d->pcitems[i] != NULL){
	i++;
      }
      if(i < 10){
      d->pcitems[i] = d->printitem[d->monsters[0].pos.y][d->monsters[0].pos.x];
      d->printitem[d->monsters[0].pos.y][d->monsters[0].pos.x] = NULL;
      }
    }

}

static int move_pc(dungeon_t *d, npc_t *p){

  int key;

  int pX = p->pos.x;
  int pY = p->pos.y;
  
  key = getch();

  if(key != 'f' && key != 'm' && key != 't'){

  d->print[p->pos.y][p->pos.x] = NULL;

  }

  switch(key){
  case 'y':
  case '7':
    p->pos.y--;
    p->pos.x--;
    break;
  case 'k':
  case '8':
    p->pos.y--;
    break;
  case 'u':
  case '9':
    p->pos.y--;
    p->pos.x++;
    break;
  case 'l':
  case '6':
    p->pos.x++;
    break;
  case 'n':
  case '3':
    p->pos.y++;
    p->pos.x++;
    break;
  case 'j':
  case '2':
    p->pos.y++;
    break;
  case 'b':
  case '1':
    p->pos.y++;
    p->pos.x--;
    break;
  case 'h':
  case '4':
    p->pos.x--;
    break;
  case '<':
    if(d->printChar[p->pos.y][p->pos.x] == '<'){
      return 1;
    }else{
      return 2;
    }
    break;
  case '>':
    if(d->printChar[p->pos.y][p->pos.x] == '>'){
      return 1;
    }else{
      return 2;
    }
    break;
  case '.':
  case '5':
  case ' ':
  case 'g':
    break;
  case 'm':
    list_monster(d);
    break;
  case 'i':
    list_items(d);
    break;
  case 'w':
    wear_items(d);
    break;
  case 't':
  take_off_items(d);
    break;
  case 'd':
    drop_items(d);
    break;
  case 'x':
    expunge_items(d);
    break;
  case 'e':
    list_equip(d);
    break;
  case 'L':
    inspect_monster(d);
    break;
  case 'I':
    inspect_items(d);
    break;
  case 'q':
    return 4;
    break;
  case 'f':
    break;
  default:
    return 2;
    break;
  }


  int out = 0;


  if(key == 'g' || key == 'f' || key == 'm'){
    if(key == 'g'){
    teleport(d);
    return 6;
    }
  }else{

    if(d->printChar[p->pos.y][p->pos.x] != '.' && d->printChar[p->pos.y][p->pos.x] != '#' && d->printChar[p->pos.y][p->pos.x] != '<' && d->printChar[p->pos.y][p->pos.x] != '>'){
      p->pos.y = pY;
      p->pos.x = pX;
      if(key != '.' && key != '5' && key != ' '){
	out = 1;
      }
    }


    if(key != 'd'){
    int i = 0;
    if(d->printitem[p->pos.y][p->pos.x] != NULL){
      while(d->pcitems[i] != NULL){
	i++;
      }
      if(i < 10){
      d->pcitems[i] = d->printitem[p->pos.y][p->pos.x];
      d->printitem[p->pos.y][p->pos.x] = NULL;
      }
    }
    }

    if(d->print[p->pos.y][p->pos.x] != NULL){
      int i;
      for(i = 0; i < 12; i++){
	if(d->pcequip[i] != NULL){
	  d->print[p->pos.y][p->pos.x]->hp = d->print[p->pos.y][p->pos.x]->hp - d->pcequip[i]->dam.roll();
	}
      }
      d->print[p->pos.y][p->pos.x]->hp = d->print[p->pos.y][p->pos.x]->hp - p->dam.roll();
      if(d->print[p->pos.y][p->pos.x]->hp < 0){
	if(d->print[p->pos.y][p->pos.x]->boss){
	  return 7;
	}
	d->print[p->pos.y][p->pos.x]->alive = 0;
	d->print[p->pos.y][p->pos.x] = NULL;
      }else{
	p->pos.y = pY;
	p->pos.x = pX;
  
      }
      
    }
  
    d->print[p->pos.y][p->pos.x] = p;

    if(out){
      return 2;
    }

  }

    if(key == 'm' || key == 'i' || key == 'w' || key == 't' || key == 'd' || key == 'x' || key == 'e' || key == 'I' || key == 'L'){
      return 3;
    }

    if(key == 'f'){
      return 5;
    }

  

  return 0;
}

int pmon(dungeon_t *d)
{
  const char *tombstone =
  "\n\n\n\n                /\"\"\"\"\"/\"\"\"\"\"\"\".\n"
  "               /     /         \\             __\n"
  "              /     /           \\            ||\n"
  "             /____ /   Rest in   \\           ||\n"
  "            |     |    Pieces     |          ||\n"
  "            |     |               |          ||\n"
  "            |     |   A. Luser    |          ||\n"
  "            |     |               |          ||\n"
  "            |     |     * *   * * |         _||_\n"
  "            |     |     *\\/* *\\/* |        | TT |\n"
  "            |     |     *_\\_  /   ...\"\"\"\"\"\"| |"
  "| |.\"\"....\"\"\"\"\"\"\"\".\"\"\n"
  "            |     |         \\/..\"\"\"\"\"...\"\"\""
  "\\ || /.\"\"\".......\"\"\"\"...\n"
  "            |     |....\"\"\"\"\"\"\"........\"\"\"\"\""
  "\"^^^^\".......\"\"\"\"\"\"\"\"..\"\n"
  "            |......\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"......"
  "..\"\"\"\"\"....\"\"\"\"\"..\"\"...\"\"\".\n\n"
  "            You're dead.  Better luck in the next life.\n\n\n";
  

  npc_t *p;
  heap_t h;
  uint32_t m;
  int x;
  int y;
  int key_check;

  for(y = d->pcLight * -1; y <= d->pcLight; y++){
    for(x = d->pcLight * -1; x <= d->pcLight; x++){
      if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	if(d->print[d->pcLoc.y + y][d->pcLoc.x + x] == NULL){
	  d->pc_view[d->pcLoc.y + y][d->pcLoc.x + x] = d->printChar[d->pcLoc.y + y][d->pcLoc.x + x];
	}else{
	  d->pc_view[d->pcLoc.y + y][d->pcLoc.x + x] = d->print[d->pcLoc.y + y][d->pcLoc.x + x]->icon;
	}
      }
    }
  }
 
  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){

      if(d->pc_view[y][x] != (char) NULL){
	mvaddch(y , x, d->pc_view[y][x]);
      }else if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
	int x2;
	int y2;
	for(y2 = -1; y2 <= 1; y2++){
	  for(x2 = -1; x2 <= 1; x2++){
	    d->pc_view[y + y2][x + x2] = d->printChar[y + y2][x + x2];
	  }
	}
      }else{
	mvaddch(y , x, ' ');
      }
      //printf("\n");
    }
  }

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
	int x2;
	int y2;
	for(y2 = -1; y2 <= 1; y2++){
	  for(x2 = -1; x2 <= 1; x2++){
	    if(y + y2 < DUNGEON_Y && x + x2 < DUNGEON_X && y + y2 >= 0 && x + x2 >= 0){
	      attron(A_BOLD);
	      if(d->print[y + y2][x + x2] != (int) NULL){
		attron(COLOR_PAIR(d->print[y + y2][x + x2]->color));
		mvaddch(y + y2, x + x2, d->print[y + y2][x + x2]->icon);
		attroff(COLOR_PAIR(d->print[y + y2][x + x2]->color));
	      }else{
		if(d->printitem[y + y2][x + x2] != (int) NULL){
		  attron(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
		  mvaddch(y + y2 , x + x2, d->printitem[y + y2][x + x2]->icon);
		  attroff(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
		}else{
		  mvaddch(y + y2, x + x2, d->printChar[y + y2][x + x2]);
		}
	      }
	      attroff(A_BOLD);
	    }
	  }
	}

      }
    }
  }

  for(y = d->pcLight * -1; y <= d->pcLight; y++){
    for(x = d->pcLight * -1; x <= d->pcLight; x++){
      if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	attron(A_BOLD);
	if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] != (int) NULL){
	  attron(COLOR_PAIR(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	  mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon);
	  attroff(COLOR_PAIR(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	}else{
	  if(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] != (int) NULL){
	    attron(COLOR_PAIR(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	    mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon);
	    attroff(COLOR_PAIR(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	  }else{
	    mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]);
	  }
	}
	attroff(A_BOLD);
      }
    }
  }
  refresh();

  heap_init(&h, monster_cmp, NULL);

  for (m = 0; m < d->numMon; m++) {
    d->monsters[m].hn = heap_insert(&h, &d->monsters[m]);
  }

  while ((p = (npc_t *) heap_remove_min(&h))) {

    if(p->alive){
      
      if(p->order == 0){

	do{
	  key_check = move_pc(d, p);
	  
	  if(key_check == 1){
	    return 1;
	  }
	  if(key_check == 3){

	    for(y = d->pcLight * -1; y <= d->pcLight; y++){
	      for(x = d->pcLight * -1; x <= d->pcLight; x++){
		if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
		  if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] == NULL){
		    d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x];
		  }else{
		    d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon;
		  }
		}
	      }
	    }

	    clear();
	    for(y = 0; y < DUNGEON_Y; y++){
	      for(x = 0; x < DUNGEON_X; x++){

		if(d->pc_view[y][x] != (char) NULL){
		  mvaddch(y , x, d->pc_view[y][x]);
		}else if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
		  int x2;
		  int y2;
		  for(y2 = -1; y2 <= 1; y2++){
		    for(x2 = -1; x2 <= 1; x2++){
		      d->pc_view[y + y2][x + x2] = d->printChar[y + y2][x + x2];
		    }
		  }
		}else{
		  mvaddch(y , x, ' ');
		}
		//printf("\n");
	      }
	    }

	    for(y = 0; y < DUNGEON_Y; y++){
	      for(x = 0; x < DUNGEON_X; x++){
		if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
		  int x2;
		  int y2;
		  for(y2 = -1; y2 <= 1; y2++){
		    for(x2 = -1; x2 <= 1; x2++){
		      if(y + y2 < DUNGEON_Y && x + x2 < DUNGEON_X && y + y2 >= 0 && x + x2 >= 0){
			attron(A_BOLD);
			if(d->print[y + y2][x + x2] != (int) NULL){
			  attron(COLOR_PAIR(d->print[y + y2][x + x2]->color));
			  mvaddch(y + y2, x + x2, d->print[y + y2][x + x2]->icon);
			  attroff(COLOR_PAIR(d->print[y + y2][x + x2]->color));
			}else{
			  if(d->printitem[y + y2][x + x2] != (int) NULL){
			    attron(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
			    mvaddch(y + y2 , x + x2, d->printitem[y + y2][x + x2]->icon);
			    attroff(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
			  }else{
			    mvaddch(y + y2, x + x2, d->printChar[y + y2][x + x2]);
			  }
			}
			attroff(A_BOLD);
		      }
		    }
		  }

		}
	      }
	    }

	    for(y = d->pcLight * -1; y <= d->pcLight; y++){
	      for(x = d->pcLight * -1; x <= d->pcLight; x++){
		if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
		  attron(A_BOLD);
		  if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] != (int) NULL){
		    attron(COLOR_PAIR(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
		    mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon);
		    attroff(COLOR_PAIR(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
		  }else{
		    if(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] != (int) NULL){
		      attron(COLOR_PAIR(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
		      mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon);
		      attroff(COLOR_PAIR(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
		    }else{
		      mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]);
		    }
		  }
		  attroff(A_BOLD);
		}
	      }
	    }
	    refresh();
	  }

	  if(key_check == 4){
	    endwin();
	    return 0;
	  }

	  if(key_check == 5){
	    clear();

	    for(y = 0; y < DUNGEON_Y; y++){
	      for(x = 0; x < DUNGEON_X; x++){
		if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
		  int x2;
		  int y2;
		  for(y2 = -1; y2 <= 1; y2++){
		    for(x2 = -1; x2 <= 1; x2++){
		      if(y + y2 < DUNGEON_Y && x + x2 < DUNGEON_X && y + y2 >= 0 && x + x2 >= 0){
			attron(A_BOLD);
			if(d->print[y + y2][x + x2] != (int) NULL){
			  attron(COLOR_PAIR(d->print[y + y2][x + x2]->color));
			  mvaddch(y + y2, x + x2, d->print[y + y2][x + x2]->icon);
			  attroff(COLOR_PAIR(d->print[y + y2][x + x2]->color));
			}else{
			  if(d->printitem[y + y2][x + x2] != (int) NULL){
			    attron(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
			    mvaddch(y + y2 , x + x2, d->printitem[y + y2][x + x2]->icon);
			    attroff(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
			  }else{
			    mvaddch(y + y2, x + x2, d->printChar[y + y2][x + x2]);
			  }
			}
			attroff(A_BOLD);
		      }
		    }
		  }

		}
	      }
	    }

	    for(y = 0; y < DUNGEON_Y; y++){
	      for(x = 0; x < DUNGEON_X; x++){
		if(d->print[y][x] != (int) NULL){
		  attron(COLOR_PAIR(d->print[y][x]->color));
		  mvaddch(y , x, d->print[y][x]->icon);
		  attroff(COLOR_PAIR(d->print[y][x]->color));
		}else{
		  if(d->printitem[y][x] != (int) NULL){
		    attron(COLOR_PAIR(d->printitem[y][x]->color));
		    mvaddch(y , x, d->printitem[y][x]->icon);
		    attroff(COLOR_PAIR(d->printitem[y][x]->color));
		  }else{
		    mvaddch(y , x, d->printChar[y][x]);
		  }
		}
		//printf("\n");
	      }
	    }
	    refresh();
	  }

	  if(key_check == 7){
	    endwin();
	    printf("!!!!!!!!!!!YOU WIN!!!!!!!!!!!/n");
	    return 0;
	  }
	  
	}while(key_check);

	
	//sleep(1);
	clear();

	
	for(y = d->pcLight * -1; y <= d->pcLight; y++){
	  for(x = d->pcLight * -1; x <= d->pcLight; x++){
	    if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	      if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] == NULL){
		d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x];
	      }else{
		d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon;
	      }
	    }
	  }
	}
	
	for(y = 0; y < DUNGEON_Y; y++){
	  for(x = 0; x < DUNGEON_X; x++){

	    if(d->pc_view[y][x] != (char) NULL){
	      mvaddch(y , x, d->pc_view[y][x]);
	    }else if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
	      int x2;
	      int y2;
	      for(y2 = -1; y2 <= 1; y2++){
		for(x2 = -1; x2 <= 1; x2++){
		  d->pc_view[y + y2][x + x2] = d->printChar[y + y2][x + x2];
		}
	      }
	    }else{
	      mvaddch(y , x, ' ');
	    }
	    //printf("\n");
	  }
	}

	for(y = 0; y < DUNGEON_Y; y++){
	  for(x = 0; x < DUNGEON_X; x++){
	    if((d->printitem[y][x] != NULL && d->printitem[y][x]->icon == '_') || (d->print[y][x] != NULL && d->print[y][x]->light == 1)){
	      int x2;
	      int y2;
	      for(y2 = -1; y2 <= 1; y2++){
		for(x2 = -1; x2 <= 1; x2++){
		  if(y + y2 < DUNGEON_Y && x + x2 < DUNGEON_X && y + y2 >= 0 && x + x2 >= 0){
		    attron(A_BOLD);
		    if(d->print[y + y2][x + x2] != (int) NULL){
		      attron(COLOR_PAIR(d->print[y + y2][x + x2]->color));
		      mvaddch(y + y2, x + x2, d->print[y + y2][x + x2]->icon);
		      attroff(COLOR_PAIR(d->print[y + y2][x + x2]->color));
		    }else{
		      if(d->printitem[y + y2][x + x2] != (int) NULL){
			attron(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
			mvaddch(y + y2 , x + x2, d->printitem[y + y2][x + x2]->icon);
			attroff(COLOR_PAIR(d->printitem[y + y2][x + x2]->color));
		      }else{
			mvaddch(y + y2, x + x2, d->printChar[y + y2][x + x2]);
		      }
		    }
		    attroff(A_BOLD);
		  }
		}
	      }

	    }
	  }
	}

	for(y = d->pcLight * -1; y <= d->pcLight; y++){
	  for(x = d->pcLight * -1; x <= d->pcLight; x++){
	    if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	      attron(A_BOLD);
	      if(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] != (int) NULL){
		attron(COLOR_PAIR(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
		mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon);
		attroff(COLOR_PAIR(d->print[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
	      }else{
		if(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] != (int) NULL){
		  attron(COLOR_PAIR(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
		  mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->icon);
		  attroff(COLOR_PAIR(d->printitem[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]->color));
		}else{
		  mvaddch(d->monsters[0].pos.y + y , d->monsters[0].pos.x + x, d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x]);
		}
	      }
	      attroff(A_BOLD);
	    }
	  }
	}
	refresh();

	for(y = d->pcLight * -1; y <= d->pcLight; y++){
	  for(x = d->pcLight * -1; x <= d->pcLight; x++){
	    if(d->monsters[0].pos.y + y < DUNGEON_Y && d->monsters[0].pos.x + x < DUNGEON_X && d->monsters[0].pos.y + y >= 0 && d->monsters[0].pos.x + x >= 0){
	      d->pc_view[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x] = d->printChar[d->monsters[0].pos.y + y][d->monsters[0].pos.x + x];
	    }
	  }
	}

      }else{
	move_monster(d, p);
      }
	
      p->next_turn += 1000/p->speed;
    
      d->monsters[p->order].hn = heap_insert(&h, &d->monsters[p->order]);

    }else if(p->order == 0){

      endwin();
      printf("%s\n", tombstone);
      return 0;
    }
  }


  return 0;
    
}




