#ifndef MONSTER_H
#define MONSTER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string>
#include "heap.h"
#include "dice.h"

#define DUNGEON_X         80
#define DUNGEON_Y         21
#define MAX_MONSTERS      1

#define haschar(npc, bit) (npc & bit)

#define intellegent 0x00000001
#define telepathic  0x00000002
#define tunnling    0x00000004
#define erratic     0x00000008

class pair_t {
 public:
  uint8_t x;
  uint8_t y;
};

class npc_t {
 public:
  heap_node_t *hn;
  int traits;
  pair_t pos;
  char icon;
  int speed;
  int color;
  int next_turn;
  uint8_t pc;
  uint16_t order;
  uint8_t alive;
  std::string name;
  std::string desc;
  int hp;
  dice dam;
  int boss;
  int light;
};

class npc_d {
 public:
  int traits;
  char icon;
  int speed;
  int color;
  int hp;
  dice dam;
  int rarity;
  int taken;
  int unique;
  std::string name;
  std::string desc;
  int boss;
  int light;
};

class item_d {
 public:
  std::string name;
  std::string desc;
  int speed;
  char icon;
  int color;
  int rarity;
  int artifact;
  int taken;
  dice dam;
};

class item_t {
 public:
  std::string name;
  std::string desc;
  int speed;
  pair_t pos;
  char icon;
  int color;
  int rarity;
  int artifact;
  dice dam;
};

class room_t {
 public:
  pair_t rPos;
  pair_t rSize;
};

class dungeon_t {
 public:
  uint16_t numRooms;
  room_t *rooms;                                 
  uint8_t hardness[DUNGEON_Y][DUNGEON_X];
  uint8_t printChar[DUNGEON_Y][DUNGEON_X];
  uint8_t pcRoom;
  pair_t  pcLoc;
  uint8_t dist_rooms[DUNGEON_Y][DUNGEON_X];
  uint8_t dist_tunnel[DUNGEON_Y][DUNGEON_X];
  uint16_t numMon;
  npc_t *monsters;
  int nummond;
  npc_d *monstersDesc;
  int numitem;
  item_t *items;
  int numitemd;
  item_d *itemDesc;
  item_t *printitem[DUNGEON_Y][DUNGEON_X];
  npc_t *print[DUNGEON_Y][DUNGEON_X];
  uint8_t pc_view[DUNGEON_Y][DUNGEON_X];
  item_t *pcitems[10];
  item_t *pcequip[12];
  int pcLight;
};

typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

class corridor_path_t {
 public:
  heap_node_t *hn;
  uint8_t pos[2];
  int32_t cost;
};

void path_through(dungeon_t *d);
void through_walls(dungeon_t *d);
void gen_items(dungeon_t *d);
void gen_monsters(dungeon_t *d);
int pmon(dungeon_t *d);

#endif
