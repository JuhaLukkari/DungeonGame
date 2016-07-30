#ifndef gameStuff_H_
#define gameStuff_H_

#include "SDL.h"
#include <iostream>
#include <fstream>
struct item //tiedostossa Type,px,py,strena,stamina,armor,leveli ja nimi 0 = Ase,1=Torso,2=Kypärä,3=kengät/housut
{
	int x;
	int y;
	int TYPE;
    int tex;
	int px;
	int py;
	int strength;
	int stamina;
	int armor;
	int level;
	char name[50];

};

struct palikka
{
	int x;
	int y;
	int tex;
	int px;
	int py;
	int weapon;
	int armor;
	int head;
	int feet;
	bool animate;
	bool nakyy;
	bool chestOpened;
	item standItem[1];
};
struct peluri
{
	int x;
	int y;
	int basestamina;
	int stamina;
	int suuntax;
	int suuntay;
	int px;
	int py;
	int basestrength;
	int strength;
	int CLASS;
	int LEVEL;
	int XP;
	int floor;
	int armor;
	int weapon;
	int torso;
	int head;
	int feet;
	item playerItems[4];
	item InventoryItems[15];
};

struct pahis
{
	int x;
	int y;
	int stamina;
	int px;
	int py;
	int strength;
	int suuntax;
	int suuntay;
	int LEVEL;
};
//ÄÄNIJUTUT
Mix_Chunk *sound[8];		//Pointer to our sound, in memory
	int channel;				//Channel on which our sound is played
	int channel2;
	int audio_rate = 22050;			//Frequency of audio playback
	Uint16 audio_format = AUDIO_S16LSB; 	//Format of the audio we're playing
	int audio_channels = 2;			//2 channels = stereo
	int audio_buffers = 4096;		//Size of the audio buffers in memory
//PELIN JUTTUJA
bool pois=false;
Uint8* nappi;                    // näppäimet
Uint8* nappi2;
SDL_Event tapahtuma;            // tapahtumat 
int STATE=0;               //pelin tila 0=MENU 1=Rata
bool Status=false;         //törmätäänkö
bool PahisStatus=false;    //törmäävätkö viholliset
bool peliMenu=false;
double d;//etäisyyksiä varten
const int MAXP=255;
int MAX_PAL;
const int MAXPA=32;
int MAX_PAH;
const int MAXITAM=32;
int MAX_ITEMS;
int SPEED;
bool Character=false;//ukon kuvan näyttäminen sivussa
int txtIndex;        //mikä teksti kirjoitetaan yläkulmaan
char textbuffer[100];//tekstin sprintfiin varten
bool otettu=false;
int lattiax;
int lattiay;
//Quit flag 
bool quit = false; 
//The timer starting time 
Uint32 start = 0;
Uint32 start2=0;
Uint32 aikaNyt;
Uint32 seuraavaAika;
Uint32 nextTime=0;
Uint32 nextTimePahis=0;
Uint32 *timer;
int homma=0;//animointiin
//The timer start/stop flag 
bool running = true; 


//PINNAT
SDL_Surface *kuva[255],*pelaaja,*kuvaItem[128],*kuvaPahis[48];
SDL_Surface *kuvaHud[8];
SDL_Surface *kuvaLattia,*lattiaTile[25][17];
SDL_Surface *menukuva,*pelimenuKuva;
SDL_Surface * naytto;   //piirto rajapinta 


//STRUCTIT
palikka Palikat[MAXP];
peluri Pelimies;
item Items[63];
item pickItems[MAXITAM];
pahis Pahikset[MAXPA];



#endif