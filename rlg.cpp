#include <ctime>
#include <cstring>
#include <endian.h>
#include <fstream>
#include <iostream>
#include <string>
#include "monster.h"
#include "dice.h"


using namespace std;

#define ROOM_MIN_X        4
#define ROOM_MIN_Y        3
#define ROOM_MAX_X        20
#define ROOM_MAX_Y        15

void init_dungeon(dungeon_t *d){
  uint32_t x;
  uint32_t y;


  d->numRooms = 6;
  while(rand()%2){
    d->numRooms++;
  }
  d->rooms = (room_t*)  malloc(sizeof(*d->rooms)* d->numRooms);

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      if(y == 0 || y == DUNGEON_Y - 1){
	d->printChar[y][x] = '-';
      }else if(x == 0 || x == DUNGEON_X - 1){
	d->printChar[y][x] = '|';
      }else{
	d->printChar[y][x] = ' ';
      }
    }
  }

  
  for(x = 0; x < 10; x++){
    d->pcitems[x] = NULL;
  }

  for(x = 0; x < 12; x++){
    d->pcequip[x] = NULL;
  }

}

int checkRoom(dungeon_t *d, uint32_t r){

  uint8_t x;
  uint8_t y;

  for(y = 0; y <= d->rooms[r].rSize.y + 1; y++){
    for(x = 0; x <= d->rooms[r].rSize.x + 1; x++){
      if(d->printChar[d->rooms[r].rPos.y + y - 1][d->rooms[r].rPos.x + x - 1] == '.'){
	return 1;
      }

    }

  }
  return 0;
}

void gen_rooms(dungeon_t *d){

  uint32_t r;
  uint32_t x;
  uint32_t y;

  for(r = 0; r < d->numRooms; r++){

    do{

      do{
	d->rooms[r].rPos.x = (rand()%DUNGEON_X)+1;
        d->rooms[r].rSize.x = (rand()%(ROOM_MAX_X-ROOM_MIN_X))+ROOM_MIN_X;
      }while(d->rooms[r].rPos.x + d->rooms[r].rSize.x > DUNGEON_X - 1);

      do{
	d->rooms[r].rPos.y = (rand()%DUNGEON_Y)+1;
        d->rooms[r].rSize.y = (rand()%(ROOM_MAX_Y-ROOM_MIN_Y))+ROOM_MIN_Y;
      }while(d->rooms[r].rPos.y + d->rooms[r].rSize.y > DUNGEON_Y - 1); 
      
    }while(checkRoom(d, r) == 1); 

    for(y = 0; y < d->rooms[r].rSize.y; y++){
      for(x = 0; x < d->rooms[r].rSize.x; x++){
	d->printChar[d->rooms[r].rPos.y + y][d->rooms[r].rPos.x + x] = '.';
      }
    }
  }

}

void gen_halls(dungeon_t *d){
  int diff;
  uint32_t r;
  uint32_t x;
  uint32_t y;

  for(r = 0; (int) r < d->numRooms - 1; r++){
    diff = d->rooms[r].rPos.y - d->rooms[r+1].rPos.y;
    if(diff < 0){
      for(y = d->rooms[r].rPos.y; y <= d->rooms[r+1].rPos.y; y++){
	if(d->printChar[y][d->rooms[r].rPos.x] == ' '){
	  d->printChar[y][d->rooms[r].rPos.x] = '#';
	}
      }
    }else{
      for(y = d->rooms[r].rPos.y; y >= d->rooms[r+1].rPos.y; y--){
	if(d->printChar[y][d->rooms[r].rPos.x] == ' '){
	  d->printChar[y][d->rooms[r].rPos.x] = '#';
	}
      }
    }
    diff = d->rooms[r].rPos.x - d->rooms[r+1].rPos.x;
    if(diff < 0){
      for(x = d->rooms[r].rPos.x; x <= d->rooms[r+1].rPos.x; x++){
	if(d->printChar[d->rooms[r+1].rPos.y][x] == ' '){
	  d->printChar[d->rooms[r+1].rPos.y][x] = '#';
	}
      }
    }else{
      for(x = d->rooms[r].rPos.x; x >= d->rooms[r+1].rPos.x; x--){
	if(d->printChar[d->rooms[r+1].rPos.y][x] == ' '){
	  d->printChar[d->rooms[r+1].rPos.y][x] = '#';
	}
      }
    }

  }

}

void gen_stairs(dungeon_t *d){

  uint32_t num = rand()%3;
  uint32_t i;
  uint32_t r;
  uint32_t xP;
  uint32_t yP;

  for(i = 0; i <= num; i++){ 
    do{
    r = rand()%d->numRooms;
    }while(r == d->pcRoom);
    xP = rand()%d->rooms[r].rSize.x;
    yP = rand()%d->rooms[r].rSize.y;

  d->printChar[d->rooms[r].rPos.y + yP][d->rooms[r].rPos.x + xP] = '>';
  }

  num = rand()%3;

  for(i = 0; i <= num; i++){
    do{
      r = rand()%d->numRooms;
    }while(r == d->pcRoom);
    xP = rand()%d->rooms[r].rSize.x;
    yP = rand()%d->rooms[r].rSize.y;

    d->printChar[d->rooms[r].rPos.y + yP][d->rooms[r].rPos.x + xP] = '<';
  }

}

void gen_pc(dungeon_t *d){

  uint32_t r = rand()%d->numRooms;
  r = d->pcRoom;
  uint32_t xP = rand()%d->rooms[r].rSize.x;
  uint32_t yP = rand()%d->rooms[r].rSize.y;

  d->pcLoc.x = d->rooms[r].rPos.x + xP;
  d->pcLoc.y = d->rooms[r].rPos.y + yP;

  //d->printChar[d->rooms[r].rPos.y + yP][d->rooms[r].rPos.x + xP] = '#';

  uint32_t x;
  uint32_t y;

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      d->print[y][x] = NULL;
      d->pc_view[y][x] = (uint8_t) NULL;
    }
  }

  d->print[d->pcLoc.y][d->pcLoc.x] = &d->monsters[0];

  d->monsters[0].pos.x = d->pcLoc.x;
  d->monsters[0].pos.y = d->pcLoc.y;

  d->monsters[0].traits = 16;
  d->monsters[0].icon = '@';
  d->monsters[0].speed = 10;
  d->monsters[0].next_turn = 0;
  d->monsters[0].pc = 1;
  d->monsters[0].order = 0;
  d->monsters[0].alive = 1;
  d->monsters[0].hp = 5000;
  dice dam(10, 2, 10);
  d->monsters[0].dam = dam;
  d->pcLight = 2;

}

void gen_hardness(dungeon_t *d){

  uint32_t x;
  uint32_t y;

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      if(y == 0 || y == DUNGEON_Y - 1){
	d->hardness[y][x] =  255;
      }else if(x == 0 || x == DUNGEON_X - 1){
	d->hardness[y][x] = 255;
      }else if(d->printChar[y][x] != ' '){
	d->hardness[y][x] = 0;
      }else{
	d->hardness[y][x] = rand()%254 + 1;
      }
    }
  }

}

void gen_dungeon(dungeon_t *d)
{
  gen_rooms(d);
  gen_halls(d);
  gen_stairs(d);
  gen_pc(d);
  gen_hardness(d);
}

void render_dungeon(dungeon_t *d){
  uint32_t x;
  uint32_t y;

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      //printf("%c", d->printChar[y][x]);
      mvaddch(y, x, d->printChar[y][x]);
      
    }
    //printf("\n");
  }

  refresh();

}

uint16_t countUp(dungeon_t *d){
  uint32_t x, y;
  uint16_t i = 0;

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      if(d->printChar[y][x] == '<'){
	i++;
      }
    }
  }
  return i;
} 

uint16_t countDown(dungeon_t *d){
  uint32_t x, y;
  uint16_t i = 0;

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      if(d->printChar[y][x] == '>'){
	i++;
      }
    }
  }
  return i;
}  

uint32_t calc_fileSize(dungeon_t *d){

  uint32_t i = 1708 + (4*d->numRooms) + (2*countUp(d)) + (2*countDown(d));

  return i;
} 

int save(dungeon_t *d){
  FILE *f;

  uint8_t x;
  uint8_t y;
  uint8_t r;

  uint16_t numUp = countUp(d);
  uint16_t numDown = countDown(d);
  

  char marker[13] = "RLG327-S2019";
  uint32_t version = 0;
  uint32_t fileSize = calc_fileSize(d);

  char *home = getenv("HOME");
  char *path;
  path = (char *)  malloc(strlen(home)+17);
  strcpy(path, home);
  strcat(path, "/.rlg327/dungeon");
  
  if(!(f = fopen(path, "w"))){
    fprintf(stderr, "Failed to open file");
    return -3;
  }
  
  free(path);

  if(fwrite(&marker, 12, 1, f) != 1){
    fprintf(stderr, "Failed to write to file");
    return -3;
  }
      
  version = htobe32(version);
  fwrite(&version, 4, 1, f);

  fileSize = htobe32(fileSize);
  fwrite(&fileSize, 4, 1, f);

  fwrite(&d->pcLoc.x, 1, 1, f);
  fwrite(&d->pcLoc.y, 1, 1, f);

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      fwrite(&d->hardness[y][x], 1, 1, f);
    }
  }

  d->numRooms = htobe16(d->numRooms);
  fwrite(&d->numRooms, 2, 1, f);

  d->numRooms = be16toh(d->numRooms);

  for(r = 0; r < d->numRooms; r++){
    fwrite(&d->rooms[r].rPos.x, 1, 1, f);
    fwrite(&d->rooms[r].rPos.y, 1, 1, f);
    fwrite(&d->rooms[r].rSize.x, 1, 1, f);
    fwrite(&d->rooms[r].rSize.y, 1, 1, f);
  }

  numUp = htobe16(numUp);
  fwrite(&numUp, 2, 1, f);

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      if(d->printChar[y][x] == '<'){
	fwrite(&x, 1, 1, f);
	fwrite(&y, 1, 1, f);
      }
    }
  }

  numDown = htobe16(numDown);
  fwrite(&numDown, 2, 1, f);

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      if(d->printChar[y][x] == '>'){
	fwrite(&x, 1, 1, f);
	fwrite(&y, 1, 1, f);
      }
    }
  }

  fclose(f);

  return 0;
}

int load(dungeon_t *d){
  
  FILE *f;

  uint8_t x;
  uint8_t y;
  uint8_t r;

  uint16_t numUp;
  uint16_t numDown;
  

  char marker[13];
  uint32_t version;
  uint32_t fileSize;
  
  char *home = getenv("HOME");
  char *path;
  path = (char *)  malloc(strlen(home)+17);
  strcpy(path, home);
  strcat(path, "/.rlg327/dungeon");
  
  if(!(f = fopen(path, "r"))){
    fprintf(stderr, "Failed to open file");
    return -3;
  }
  

  free(path);

  fread(&marker, 12, 1, f);

  fread(&version, 4, 1, f);
  version =  be32toh(version);

  fread(&fileSize, 4, 1, f);
  fileSize = be32toh(fileSize);

  fread(&d->pcLoc.x, 1, 1, f);

  fread(&d->pcLoc.y, 1, 1, f);

  for(y = 0; y < 21; y++){
    for(x = 0; x < 80; x++){
      fread(&d->hardness[y][x], 1, 1, f);
    }
  }

  for(y = 0; y < 21; y++){
    for(x = 0; x < 80; x++){
      if(d->hardness[y][x] == 0){
	d->printChar[y][x] = '#';
      }
    }
  }

  fread(&d->numRooms, 2, 1, f);
  d->numRooms = be16toh(d->numRooms);

  d->rooms = (room_t *) malloc(sizeof(*d->rooms)* d->numRooms);

  for(r = 0; r < d->numRooms; r++){
    fread(&d->rooms[r].rPos.x, 1, 1, f);
    fread(&d->rooms[r].rPos.y, 1, 1, f);
    fread(&d->rooms[r].rSize.x, 1, 1, f);
    fread(&d->rooms[r].rSize.y, 1, 1, f);
  }
  
  for(r = 0; r < d->numRooms; r++){
    for(y = 0; y < d->rooms[r].rSize.y; y++){
      for(x = 0; x < d->rooms[r].rSize.x; x++){
	d->printChar[d->rooms[r].rPos.y + y][d->rooms[r].rPos.x + x] = '.';
      }
    }
  }

  d->printChar[d->pcLoc.y][d->pcLoc.x] = '@';

  for(y = 0; y < DUNGEON_Y; y++){
    for(x = 0; x < DUNGEON_X; x++){
      d->print[y][x] = NULL;
      d->pc_view[y][x] = (uint8_t) NULL;
    }
  }

  d->print[d->pcLoc.y][d->pcLoc.x] = &d->monsters[0];

  d->monsters[0].pos.x = d->pcLoc.x;
  d->monsters[0].pos.y = d->pcLoc.y;

  d->monsters[0].traits = 16;
  d->monsters[0].icon = '@';
  d->monsters[0].speed = 10;
  d->monsters[0].next_turn = 0;
  d->monsters[0].pc = 1;
  d->monsters[0].order = 0;
  d->monsters[0].alive = 1;

  fread(&numUp, 2, 1, f);
  numUp = be16toh(numUp);

  for(r = 0; r < numUp; r++){
    fread(&x, 1, 1, f);
    fread(&y, 1, 1, f);
    d->printChar[y][x] = '<';
  }
     
  fread(&numDown, 2, 1, f);
  numDown = be16toh(numDown);

  for(r = 0; r < numDown; r++){
    fread(&x, 1, 1, f);
    fread(&y, 1, 1, f);
    d->printChar[y][x] = '>';
  }

  fclose(f);

  marker[12] = '\0';

  printf("Marker: %s Version: %i Size: %i bytes\n", marker, version, fileSize);

  return 0;
  
}

void usage(char *name){
  fprintf(stderr, "Usage: %s [--save] [--load] <--seed [seed num]> <--nummon [num of mon]>\n", name);

  exit(-1);
}

void init_terminal(){
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();

  init_pair(0, COLOR_WHITE, COLOR_BLACK);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
}

int main(int argc, char *argv[]){

  dungeon_t d;
  uint32_t seed = 0;
  uint32_t do_load = 0, do_save = 0;
  uint32_t i;
  int nummon = 10;
  d.numMon = 10;

  string line;
  string name;
  string desc;
  string color;
  string speed;
  string abil;
  string hp;
  string dam;
  string symb;
  string rrty;

  d.numitem = 10;

  

  for(i = 1; (int) i < argc; i++){

    if(argv[i][0] == '-'){

      if(!strcmp(argv[i], "--load")){
	do_load = 1;
      }else if(!strcmp(argv[i], "--save")){
	do_save = 1;
      }else if(!strcmp(argv[i], "--nummon")){
	nummon = atoi(argv[i+1]);
	d.numMon = nummon+1;
	i++;
      }else if(!strcmp(argv[i], "--numitem")){
	d.numitem = atoi(argv[i+1]);
	i++;
      }else if(!strcmp(argv[i], "--seed")){
	seed = atoi(argv[i+1]);
	i++;
      }else{
	usage(argv[0]);
      }
    }else{
      usage(argv[0]);
    }
  }
  
  d.items = (item_t *)  malloc(sizeof(item_t) * (d.numitem));
  d.monsters = (npc_t *)  malloc(sizeof(npc_t) * (nummon));
  
   
  if(!seed){
    seed = time(NULL);
  }
  srand(seed);

  
  int count = 0;;

  char *home = getenv("HOME");
  char *path;
  path = (char *)  malloc(strlen(home)+17);
  strcpy(path, home);
  strcat(path, "/.rlg327/monster_desc.txt");

  ifstream cfile(path);

  while(getline(cfile, line)){
    if(line == "BEGIN MONSTER"){
      while(getline(cfile, line)){
	if(line == "END"){
	  count++;
	}
      }
    }
  }

  d.monstersDesc = (npc_d *) malloc(sizeof(npc_d) * count);

  d.nummond = count;

  count = 0;

  ifstream infile(path);

  getline(infile, line);

  if(line != "RLG327 MONSTER DESCRIPTION 1"){
    cout << "Error in Monster File"  << endl;
    return 1;
  }

  while(getline(infile, line)){
    if(line == "BEGIN MONSTER"){

      d.monstersDesc[count].taken = 0;
      d.monstersDesc[count].unique = 0;
      d.monstersDesc[count].traits = 0;
      d.monstersDesc[count].boss = 0;
      d.monstersDesc[count].light = 0;

      infile >> line;

      do{
   
	if(line == "NAME"){
	  infile.get();
	  getline(infile, line);
	  d.monstersDesc[count].name = line;
	}

	if(line == "COLOR"){
	  infile.get();
	  infile >> line;

	  if(line == "BLACK"){
	    d.monstersDesc[count].color = 0;
	  }

	  if(line == "RED"){
	    d.monstersDesc[count].color = 1;
	  }

	  if(line == "GREEN"){
	    d.monstersDesc[count].color = 2;
	  }

	  if(line == "YELLOW"){
	    d.monstersDesc[count].color = 3;
	  }

	  if(line == "BLUE"){
	    d.monstersDesc[count].color = 4;
	  }

	  if(line == "MAGENTA"){
	    d.monstersDesc[count].color = 5;
	  }

	  if(line == "CYAN"){
	    d.monstersDesc[count].color = 6;
	  }

	  
	  getline(infile, line);
	  
	  
	  
	}
	
	if(line == "SPEED"){
	  infile.get();
	  getline(infile, line);
	  int base;
	  int sides;
	  int number;
      
	  int temp =  line.find('+');

	  base = atoi(line.substr(0, temp).c_str());
	  int temp2 =  line.find('d');
	  sides = atoi(line.substr(temp, temp2).c_str());
	  number = atoi(line.substr(temp2 + 1).c_str());

	  dice speed(base, sides, number);


	  d.monstersDesc[count].speed = speed.roll();
	}

	if(line == "ABIL"){
	  infile.get();
	  getline(infile, line);
	  
	  if(line.find("SMART") != string::npos){
	    d.monstersDesc[count].traits ^= (1u);

	  } 
	  if(line.find("TELE") != string::npos){
	    d.monstersDesc[count].traits ^= (1u << 1);

	  }
	  if(line.find("TUNNEL") != string::npos){
	    d.monstersDesc[count].traits ^= (1u << 2);

	  }
	  if(line.find("ERRATIC") != string::npos){
	    d.monstersDesc[count].traits ^= (1u) << 3;

	  }
	  if(line.find("UNIQ") != string::npos){
	    d.monstersDesc[count].unique = 1;

	  }
	  if(line.find("BOSS") != string::npos){
	    d.monstersDesc[count].boss = 1;
	  }
	  if(line.find("LIGHT") != string::npos){
	    d.monstersDesc[count].light = 1;
	  }
	}

	if(line == "HP"){
	  infile.get();
	  getline(infile, line);
	  int base;
	  int sides;
	  int number;
      
	  int temp =  line.find('+');

	  base = atoi(line.substr(0, temp).c_str());
	  int temp2 =  line.find('d');
	  sides = atoi(line.substr(temp, temp2).c_str());
	  number = atoi(line.substr(temp2 + 1).c_str());

	  dice hp(base, sides, number);


	  d.monstersDesc[count].hp = hp.roll();
	}

	if(line == "DAM"){
	  infile.get();
	  getline(infile, line);
	  int base;
	  int sides;
	  int number;
      
	  int temp =  line.find('+');

	  base = atoi(line.substr(0, temp).c_str());
	  int temp2 =  line.find('d');
	  sides = atoi(line.substr(temp, temp2).c_str());
	  number = atoi(line.substr(temp2 + 1).c_str());

	  dice dam(base, sides, number);

	  d.monstersDesc[count].dam = dam;
	}

	if(line == "SYMB"){
	  infile.get();
	  getline(infile, line);
	  d.monstersDesc[count].icon = line[0];
	}

	if(line == "RRTY"){
	  infile.get();
	  getline(infile, line);
	  d.monstersDesc[count].rarity = atoi(line.c_str());
	}
	
	if(line == "DESC"){
	  infile.get();
	  getline(infile, line);
	  
	  do{
	    desc += line + "\n";
	    getline(infile, line);
	  }while(line != ".");
	  d.monstersDesc[count].desc = desc;
	}
	infile >> line;
      } while(line != "END");

      

      count++;

     
	
      desc = "";
    }
  }
  
  count = 0;

  home = getenv("HOME");
  path = (char *)  malloc(strlen(home)+17);
  strcpy(path, home);
  strcat(path, "/.rlg327/object_desc.txt");

  ifstream cfile2(path);

  while(getline(cfile2, line)){
    if(line == "BEGIN OBJECT"){
      while(getline(cfile2, line)){
	if(line == "END"){
	  count++;
	}
      }
    }
  }

  d.itemDesc = (item_d *) malloc(sizeof(item_d) * count);

  d.numitemd = count;

  count = 0;

  ifstream infile2(path);

  getline(infile2, line);

  if(line != "RLG327 OBJECT DESCRIPTION 1"){
    cout << "Error in Monster File"  << endl;
    return 1;
  }

  while(getline(infile2, line)){
    if(line == "BEGIN OBJECT"){

      d.itemDesc[count].artifact = 0;
      d.itemDesc[count].taken = 0;

      infile2 >> line;

      do{

	if(line == "NAME"){
	  infile2.get();
	  getline(infile2, line);
	  d.itemDesc[count].name = line;
	  
	}

	if(line == "SPEED"){
	  infile2.get();
	  getline(infile2, line);
	  int base;
	  int sides;
	  int number;
      
	  int temp =  line.find('+');

	  base = atoi(line.substr(0, temp).c_str());
	  int temp2 =  line.find('d');
	  sides = atoi(line.substr(temp, temp2).c_str());
	  number = atoi(line.substr(temp2 + 1).c_str());

	  dice speed(base, sides, number);

	  d.itemDesc[count].speed = speed.roll();
	}

	if(line == "DAM"){
	  infile2.get();
	  getline(infile2, line);
	  int base;
	  int sides;
	  int number;
      
	  int temp =  line.find('+');

	  base = atoi(line.substr(0, temp).c_str());
	  int temp2 =  line.find('d');
	  sides = atoi(line.substr(temp, temp2).c_str());
	  number = atoi(line.substr(temp2 + 1).c_str());

	  dice dam(base, sides, number);

	  d.itemDesc[count].dam = dam;
	}

	if(line == "COLOR"){
	  infile2.get();
	  infile2 >> line;

	  if(line == "BLACK"){
	    d.itemDesc[count].color = 0;
	  }

	  if(line == "RED"){
	    d.itemDesc[count].color = 1;
	  }

	  if(line == "GREEN"){
	    d.itemDesc[count].color = 2;
	  }

	  if(line == "YELLOW"){
	    d.itemDesc[count].color = 3;
	  }

	  if(line == "BLUE"){
	    d.itemDesc[count].color = 4;
	  }

	  if(line == "MAGENTA"){
	    d.itemDesc[count].color = 5;
	  }

	  if(line == "CYAN"){
	    d.itemDesc[count].color = 6;
	  }
	  
	  getline(infile2, line);
	  
	}

	if(line == "TYPE"){
	  infile2.get();
	  infile2 >> line;

	  if(line == "WEAPON"){
	    d.itemDesc[count].icon = '|';
	  }
	  if(line == "OFFHAND"){
	    d.itemDesc[count].icon = ')';
	  }
	  if(line == "RANGED"){
	    d.itemDesc[count].icon = '}';
	  }
	  if(line == "ARMOR"){
	    d.itemDesc[count].icon = '[';
	  }
	  if(line == "HELMET"){
	    d.itemDesc[count].icon = ']';
	  }
	  if(line == "CLOAK"){
	    d.itemDesc[count].icon = '(';
	  }
	  if(line == "GLOVES"){
	    d.itemDesc[count].icon = '{';
	  }
	  if(line == "BOOTS"){
	    d.itemDesc[count].icon = '\\';
	  }
	  if(line == "RING"){
	    d.itemDesc[count].icon = '=';
	  }
	  if(line == "AMULET"){
	    d.itemDesc[count].icon = '"';
	  }
	  if(line == "LIGHT"){
	    d.itemDesc[count].icon = '_';
	  }
	  if(line == "SCROLL"){
	    d.itemDesc[count].icon = '~';
	  }
	  if(line == "BOOK"){
	    d.itemDesc[count].icon = '?';
	  }
	  if(line == "FLASK"){
	    d.itemDesc[count].icon = '!';
	  }
	  if(line == "GOLD"){
	    d.itemDesc[count].icon = '$';
	  }
	  if(line == "AMMUNITION"){
	    d.itemDesc[count].icon = '/';
	  }
	  if(line == "FOOD"){
	    d.itemDesc[count].icon = ',';
	  }
	  if(line == "WAND"){
	    d.itemDesc[count].icon = '-';
	  }
	  if(line == "CONTAINER"){
	    d.itemDesc[count].icon = '%';
	  }
	}

	if(line == "RRTY"){
	  infile.get();
	  getline(infile2, line);
	  d.itemDesc[count].rarity = atoi(line.c_str());
	}

	if(line == "ART"){
	  infile2.get();
	  infile2 >> line;
	  if(line == "TRUE"){
	    d.itemDesc[count].artifact = 1;
	  }
	}

	if(line == "DESC"){
	  infile2.get();
	  getline(infile2, line);
	  
	  do{
	    desc += line + "\n";
	    getline(infile2, line);
	  }while(line != ".");
	  d.itemDesc[count].desc = desc;
	}
	  	
	infile2 >> line;
      } while(line != "END");
     
      count++;
      desc = "";

    }
  }
  

  init_dungeon(&d);

  if(do_load){
    printf("Loading\n");
    load(&d);
  }

  if(do_save){
    printf("Saving\n");
    if(argc != 3){
      printf("Using seed: %u\n", seed);
      gen_dungeon(&d);
    }
    save(&d);
  }
  
  if(!do_load && !do_save){
    gen_dungeon(&d);
  }

  init_terminal();

  //render_dungeon(&d);

  gen_monsters(&d);

  gen_items(&d);

  while(pmon(&d)){
    init_dungeon(&d);
    gen_dungeon(&d);
    //    render_dungeon(&d);
    gen_monsters(&d);
    gen_items(&d);
  }

  //endwin();

  return 0;
}
