#include "SDL.h"
#include "SDL_TTF.h"
#include "SDL_mixer.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include "gameMath.h"
#include "gameDefines.h"
#include "gameStuff.h"
#include "musaLuokka.h"
#pragma comment(lib, "SDLmain.lib")  
#pragma comment(lib, "SDL.lib")  
#pragma comment(lib, "SDL_ttf.lib")
#pragma comment(lib, "SDL_mixer.lib")
using namespace std; 
TTF_Font* font3 = TTF_OpenFont("German.ttf", 24);//fontti
//Tekstit
SDL_Color foregroundColor2 = { 128, 0, 0 };//taustaväri
/*
DiabloPeli (c) Juha Lukkari 2010
Pitää olla Project->settings Multithreaded dll ja 8 bytes struct homma
BUGEJA!
Jos pahis seuraa pelaajaa seinän toisella puolella kulkien pos suuntiin niin menee seinistä läpi kun nopeus suurempi kuin 1
"Kaluston" siirtäminen samaan kuvatiedostoon ja muutenkin kuvat ladattaisiiin kerran eikä sataa kertaa joka structin  osalle
mikäli toimisi näin tai ylipäänsä näkyvillä aina noin 10 palikkaa niin enempää ei tarvita?
*/ 

ifstream MapFile;   //kartta
ifstream EntityFile;//vihollisten määrä alotuspaikka ym.
ifstream ItemFile;//kamat
ofstream SaveGameFile;//tallennus
ifstream LoadGameFile;//tallennuksen lataus

int Game_Init(const char *filename, const char *filename2,bool saved);
//////////Radan lataus///////////////
void  LoadMapData(const char *filename, const char *filename2,bool saved)
{			
         int i=0;
		 MAX_PAL=0;
		 MAX_PAH=0;
		 int oldx=Pelimies.x;
		 int oldy=Pelimies.y;
		 MAX_ITEMS=0;
         MapFile.clear(); //Ilman tätä ei toimi!!!
		 EntityFile.clear();

		MapFile.open(filename,ios::binary);//avataan tiedostot
		EntityFile.open(filename2,ios::binary);

	    while(MapFile.peek() !=EOF) //Ladataan palikoiden paikat
		{			
			MapFile>>Palikat[i].x;
			MapFile>>skipws;
			MapFile>>Palikat[i].y;
			MapFile>>skipws;
			MapFile>>Palikat[i].tex;
			MapFile>>skipws;
			MAX_PAL++;
			i++; 
		}
		 i=0;			 
		MapFile.close();//sulkeminen on pakollinen muuten ei päästä enää käsiksi tiedostoon
        while(EntityFile.peek() !=EOF ) //Ladataan alkupaikka ja viholliset ja itemit
		{			
			EntityFile>>Pelimies.x;
			EntityFile>>skipws;
			EntityFile>>Pelimies.y;
			EntityFile>>skipws;
			EntityFile>>MAX_PAH;
			EntityFile>>skipws;
			EntityFile>>MAX_ITEMS;
			EntityFile>>skipws;			
		}
		if(saved==true)//jos initoidaan tallennus
		{
			Pelimies.x=oldx;
			Pelimies.y=oldy;
		}
		EntityFile.close();//suljetaan tiedosto				
}
void LoadItems(char *filename)//itemeitten lataus
{
	int a=0;
	ItemFile.open(filename,ios::binary);
	while(ItemFile.peek() !=EOF)
	{
		ItemFile>>Items[a].TYPE;
		ItemFile>>skipws;
		ItemFile>>Items[a].px;
		ItemFile>>skipws;
		ItemFile>>Items[a].py;
		ItemFile>>skipws;
		ItemFile>>Items[a].strength;
		ItemFile>>skipws;
		ItemFile>>Items[a].stamina;
		ItemFile>>skipws;
		ItemFile>>Items[a].armor;
		ItemFile>>skipws;
		ItemFile>>Items[a].level;
		ItemFile>>skipws;
		a++;
	}
	ItemFile.close();
	
}
void SaveGame(char *filename)//tähän seivaus
{
	SaveGameFile.open(filename,ios::out| ios::binary);
	SaveGameFile<<Pelimies.floor;
	SaveGameFile<<endl;
	SaveGameFile<<Pelimies.x;
    SaveGameFile<<endl;
    SaveGameFile<<Pelimies.y;
    SaveGameFile<<endl;
    SaveGameFile<<Pelimies.CLASS;
    SaveGameFile<<endl;
    SaveGameFile<<Pelimies.LEVEL;
    SaveGameFile<<endl;
    SaveGameFile<<Pelimies.XP;
    SaveGameFile<<endl;
    SaveGameFile<<Pelimies.basestamina;
    SaveGameFile<<endl;
    SaveGameFile<<Pelimies.basestrength;
    SaveGameFile<<endl;
    SaveGameFile<<Pelimies.weapon;
    SaveGameFile<<endl;
	SaveGameFile<<Pelimies.torso;
    SaveGameFile<<endl;
	SaveGameFile<<Pelimies.head;
    SaveGameFile<<endl;
	SaveGameFile<<Pelimies.feet;
    SaveGameFile<<endl;
	
	SaveGameFile.close();
}
void LoadGame(char *filename)//pelin lataus
{
	LoadGameFile.open(filename,ios::in| ios::binary);
    
    LoadGameFile>>Pelimies.floor;
	LoadGameFile>>skipws;
	LoadGameFile>>Pelimies.x;
	LoadGameFile>>skipws;
    LoadGameFile>>Pelimies.y;
	LoadGameFile>>skipws;
	LoadGameFile>>Pelimies.CLASS;
	LoadGameFile>>skipws;
	LoadGameFile>>Pelimies.LEVEL;
	LoadGameFile>>skipws;
	LoadGameFile>>Pelimies.XP;
	LoadGameFile>>skipws;
	LoadGameFile>>Pelimies.basestamina;
    LoadGameFile>>skipws;
    LoadGameFile>>Pelimies.basestrength;
    LoadGameFile>>skipws;
    LoadGameFile>>Pelimies.weapon;
    LoadGameFile>>skipws;
	LoadGameFile>>Pelimies.torso;
    LoadGameFile>>skipws;
	LoadGameFile>>Pelimies.head;
    LoadGameFile>>skipws;
	LoadGameFile>>Pelimies.feet;
    LoadGameFile>>skipws;
	LoadGameFile.close();

	if(Pelimies.floor==1)//vaihdetaan rata latauksen mukaan
		Game_Init("level1.map","level1.ent",true);
	else if(Pelimies.floor==2)
		Game_Init("level2.map","level2.ent",true);
	else
		STATE=0; //jos ei ole sellaista rataa olemassa heitetään alkumenuun

}
int Tormays(int x1,int y1,int x2,int y2,int leveys1,int korkeus1,int leveys2,int korkeus2)
{
  Status=false;//törmäämätön kunnes toisin todistetaan
	  if(y1 < y2 + korkeus2){
		  if(y1 + korkeus1 > y2){
			  if(x1 < x2 + leveys2){
				  if(x1 + leveys1 > x2){
                    Pelimies.x = Pelimies.x+Pelimies.suuntax;
                    Pelimies.y = Pelimies.y +Pelimies.suuntay;
					Status=true;//Törmäys
				  }
			  }
		  }
	  }
  return Status;	
}
int PahisTormays(int x1,int y1,int x2,int y2,int leveys1,int korkeus1,int leveys2,int korkeus2,int i)
{
  PahisStatus=false;//törmäämätön kunnes toisin todistetaan
	  if(y1 <= y2 + korkeus2){
		  if(y1 + korkeus1 >= y2){
			  if(x1 <= x2 + leveys2){
				  if(x1 + leveys1 >= x2){
                    Pahikset[i].x = Pahikset[i].x+Pahikset[i].suuntax;
                    Pahikset[i].y = Pahikset[i].y +Pahikset[i].suuntay;
					PahisStatus=true;//Törmäys
				  }
			  }
		  }
	  }
  return PahisStatus;	
}
void PiirraKuva(SDL_Surface *kuva, SDL_Surface *naytto, int kuvax, int kuvay, int leveys, int korkeus, int nayttox, int nayttoy)
{
    SDL_Rect kuvaalue; // alue, mikä kuvasta piirretään
    kuvaalue.x = kuvax;
    kuvaalue.y = kuvay;
    kuvaalue.h = korkeus;
    kuvaalue.w = leveys;

    SDL_Rect nayttoalue; // alue näytöllä, jolle kuva piirretään
    nayttoalue.x = nayttox;
    nayttoalue.y = nayttoy;

    SDL_BlitSurface(kuva, &kuvaalue, naytto, &nayttoalue);
} 

//pelin initointi
int Game_Init(const char *filename, const char *filename2,bool saved)
{
	LoadMapData(filename,filename2,saved);//ladataan kartta
    int w=7;
	int a=0;
	int wi=rand() %6;
	int ai=rand() %6;
	for(int s=0;s<MAX_PAL;s++)//seinät
	{
		if(Palikat[s].tex==0)//seinät
			kuva[s]=SDL_LoadBMP("muuri.bmp"); 
		if(Palikat[s].tex==1)//lopetus
		   kuva[s]=SDL_LoadBMP("down.bmp");
		if(Palikat[s].tex==2)//ase ständi
		{
			kuva[s]=SDL_LoadBMP("wstand.bmp");//tässä nyt joku että aseet ei mee oikein
			if(w<13)//tiedoston rivi
			{
				Palikat[s].standItem[0]=Items[wi+7];
				Palikat[s].weapon=wi+7;
				w++;
			}
		}
		if(Palikat[s].tex==3)//arkku
			kuva[s]=SDL_LoadBMP("arkku.bmp");
		    Palikat[s].chestOpened=false;
		if(Palikat[s].tex==4)//soihtu
		{
			kuva[s]=SDL_LoadBMP("soihtu.bmp");
			Palikat[s].animate=true;
		}
		if(Palikat[s].tex==5)//armorit ja kypärät
		{
			kuva[s]=SDL_LoadBMP("astand.bmp");
			if(a<7)//tiedoston rivi
			{
				Palikat[s].standItem[0]=Items[ai];
				if(Items[ai].TYPE==1)
					Palikat[s].armor=ai;
                else if(Items[ai].TYPE==2)
					Palikat[s].head=ai;
				else if(Items[ai].TYPE==3)
					Palikat[s].feet=ai;
				a++;
			}
		}
		SDL_SetColorKey(kuva[s], SDL_SRCCOLORKEY, SDL_MapRGB(kuva[s]->format,0,0,0));
	}
	for(int f=0;f<MAX_ITEMS-1;f++)//itemeitä
	{
			kuvaItem[f]=SDL_LoadBMP("avain1.bmp");
			pickItems[f].x=rand() %1000;//arvotaan paikat
			pickItems[f].y=rand() %600;
		for(int k=0;k<MAX_PAL;k++)//jos ollaan seinän sisällä,siiretään
		{
			Tormays(Palikat[k].x,Palikat[k].y,pickItems[f].x,pickItems[f].y,40,40,22,22);
			while(Status==true)
			{
				pickItems[f].x++;
				pickItems[f].y++;
				Tormays(Palikat[k].x,Palikat[k].y,pickItems[f].x,pickItems[f].y,40,40,22,22);
			}
			pickItems[f].tex=0;
		}
		SDL_SetColorKey(kuvaItem[f], SDL_SRCCOLORKEY, SDL_MapRGB(kuvaItem[f]->format,0,0,0)); 	
	}
	for(int j=0;j<MAX_PAH-1;j++)//pahiksia
	{
		kuvaPahis[j]=SDL_LoadBMP("pahis1.bmp");
		if(j>(MAX_PAH/2))
		kuvaPahis[j]=SDL_LoadBMP("pahis2.bmp");//uusia pahiksia
		if(j>(MAX_PAH-5) && Pelimies.floor>1)
			kuvaPahis[j]=SDL_LoadBMP("luupahis.bmp");
		Pahikset[j].x=rand() %1000;//arvotaan paikat
		Pahikset[j].y=rand() %600;
		Pahikset[j].stamina=200;
		Pahikset[j].strength=1;
		Pahikset[j].LEVEL=1;
		if(j>(MAX_PAH/2))
		{
			Pahikset[j].stamina=300;
			Pahikset[j].strength=2;
			Pahikset[j].LEVEL=2;
		}
		if(j>(MAX_PAH-3)&& Pelimies.floor>1)
		{
			Pahikset[j].stamina=400;
			Pahikset[j].strength=2;
			Pahikset[j].LEVEL=3;
		}
		for(int b=0;b<MAX_PAL;b++)//jos sijoitettu seinän sisään, siiretään
		{
            Tormays(Palikat[b].x,Palikat[b].y,Pahikset[j].x,Pahikset[j].y,40,40,32,32);
			while(Status==true)
			{
				Pahikset[j].x++;
				Pahikset[j].y++;
				Tormays(Palikat[b].x,Palikat[b].y,Pahikset[j].x,Pahikset[j].y,40,40,22,22);
			}
		}
		SDL_SetColorKey(kuvaPahis[j], SDL_SRCCOLORKEY, SDL_MapRGB(kuvaPahis[j]->format,0,0,0)); 	
	}
	for(int y=0;y<17;y++)//lattian lataus
	{
		for(int x=0;x<25;x++)
		{
			lattiaTile[x][y]=SDL_LoadBMP("lattia1.bmp");
		}
	}
	kuvaLattia=SDL_LoadBMP("lattia.bmp");
	SDL_SetColorKey(kuvaLattia,SDL_SRCCOLORKEY,SDL_MapRGB(kuvaLattia->format,0,0,0));
	kuvaHud[0]=SDL_LoadBMP("hpbar.bmp");//hela-palkki
	kuvaHud[1]=SDL_LoadBMP("human.bmp");//ihmisen silhuetti
	kuvaHud[2]=SDL_LoadBMP("diablo01.bmp");//aseet
	kuvaHud[3]=SDL_LoadBMP("diablo01.bmp");//torso
	kuvaHud[4]=SDL_LoadBMP("diablo01.bmp");//kypärä
	if(Pelimies.CLASS==1)
    pelaaja=SDL_LoadBMP("ukkot.bmp");//lataa pelaajan kuvat
	if(Pelimies.CLASS==2)
		pelaaja=SDL_LoadBMP("munkki.bmp");
	SDL_SetColorKey(pelaaja, SDL_SRCCOLORKEY, SDL_MapRGB(pelaaja->format,0,0,0)); 

	if(saved==true)//jos initoidaan ladattu määrätään kamat tiedostosta
	{
		Pelimies.playerItems[0]=Items[Pelimies.weapon];
	    Pelimies.playerItems[1]=Items[Pelimies.torso];//ei vittu toimi
		Pelimies.playerItems[2]=Items[Pelimies.head];
		Pelimies.playerItems[3]=Items[Pelimies.feet];
	}
	//ÄÄNTEN LATAUS
	sound[0] = Mix_LoadWAV("sword3.wav");//hakkaus
	if(sound[0] == NULL) 
		printf("Unable to load WAV file: %s\n", Mix_GetError());
	sound[1] = Mix_LoadWAV("Ahh.wav");//tappo
	if(sound[1] == NULL) 
		printf("Unable to load WAV file: %s\n", Mix_GetError());
	sound[2] = Mix_LoadWAV("fire-1.wav");//soihdun lähellä
	if(sound[2] == NULL) 
		printf("Unable to load WAV file: %s\n", Mix_GetError());
    sound[3]=Mix_LoadWAV("rock4_crash.wav");//dingaus
	if(sound[3] == NULL) 
		printf("Unable to load WAV file: %s\n", Mix_GetError());
    sound[4]=Mix_LoadWAV("footsteps.wav");//kävely
	if(sound[4] == NULL) 
		printf("Unable to load WAV file: %s\n", Mix_GetError());

return 1;
}
int Game_Menu()
{
	
        nappi2=SDL_GetKeyState(NULL); 
     
			   if(nappi2[SDLK_F1])//warriori
			   {
				   Pelimies.CLASS=1;
			       Pelimies.basestrength=2;
				   Pelimies.basestamina=100;
				   
			   }
			   else if(nappi2[SDLK_F2])//munkki
			   {
				   Pelimies.CLASS=2;
				   Pelimies.basestrength=1;
				   Pelimies.basestamina=150;
			   }
			   if(nappi2[SDLK_l])
				   LoadGame("Save.sav");
			   if(Pelimies.CLASS==0)//warriori oletuksena
				   Pelimies.CLASS=1;
			   if( nappi2[SDLK_1])//tällä mennään peliin
			   {
				   STATE=1;//peli käyntiin
				   Game_Init("level1.map","level1.ent",false);//Alusta
				   Pelimies.floor=1;				  
			   }
               else if( nappi2[SDLK_2])//tällä mennään peliin
			   {
				   STATE=1;//peli käyntiin
				   Game_Init("level2.map","level2.ent",false);//Alusta
				   Pelimies.floor=2;
			   }
			if(Pelimies.floor==2)
			{
				STATE=1;
                Game_Init("level2.map","level2.ent",false);//Alusta
			}
	  
			  
		      PiirraKuva(menukuva,naytto,0,0,1280,1024,0,0);//menu 
	          SDL_Flip(naytto);



return 1;
}
int Game_Reset()//Pelin resetointi
{
 // Game_Init(filename,filename2);
  
  Pelimies.stamina=Pelimies.basestamina+Pelimies.playerItems[0].stamina+Pelimies.playerItems[1].stamina;
  Pelimies.stamina=Pelimies.stamina+Pelimies.playerItems[2].stamina+Pelimies.playerItems[3].stamina;
  Pelimies.armor=Pelimies.playerItems[1].armor + Pelimies.playerItems[2].armor + Pelimies.playerItems[3].armor;
  Pelimies.strength=Pelimies.basestrength+Pelimies.playerItems[0].strength;
  txtIndex=0;
  d=0;
    return 1; 
}

//radan pyöritys,for loopit pois ja loopit samanlailla ku toi homma

Uint32 Game_RUN()
{
	    if(homma<INTERVAL)//Tällä hidastetaan animaatiota!!Pienempi INTERVAL sitä nopeampi
		{
			homma++;
		}
		else
			homma=0;
        SDL_PollEvent(&tapahtuma);
            if ( tapahtuma.type == SDL_QUIT )  {  STATE = 0;
			}  //poistumistapahtuma
            if ( tapahtuma.type == SDL_KEYDOWN )  {
				  
                    if ( tapahtuma.key.keysym.sym == SDLK_BACKSPACE ) {
						STATE = 0;
						Pelimies.floor=0;//ei jäädä jumittaa jos halutaan menuun
						
				}                   
            } 
			nappi = SDL_GetKeyState(NULL);//näppäinten kattominen
		//	PELAAJAN TILANTEEN MUUTTUMINEN
			//vanha if(Pelimies.py==0)
			//		Pelimies.py=22;
			//	else
			//		Pelimies.py=0;
			
			
			if ( nappi[SDLK_UP] )
			{ 
				Pelimies.y -= SPEED;
				Pelimies.suuntay=SPEED;
				Pelimies.px=0;
				seuraavaAika=start+100;
			    if(homma==0)
				{
					if(Pelimies.py==0 || Pelimies.py==22 || Pelimies.py==44)
						Pelimies.py+=22;
					if(Pelimies.py==66)
						Pelimies.py=0;
				}
				
			}
			if ( nappi[SDLK_DOWN] ) 
			{
				Pelimies.y += SPEED; 
                Pelimies.suuntay=-SPEED;
				Pelimies.px=21;
				if(homma==0)
				{
					if(Pelimies.py==0 || Pelimies.py==22 || Pelimies.py==44)
					  Pelimies.py+=22;
					if(Pelimies.py==66)
					  Pelimies.py=0;
				}
			}
			if ( nappi[SDLK_LEFT] )
			{ 
				Pelimies.x -= SPEED; 
				Pelimies.suuntax=SPEED;
				Pelimies.px=42;
				if(homma==0)
				{
				if(Pelimies.py==0 || Pelimies.py==22 || Pelimies.py==44)
					Pelimies.py+=22;
				if(Pelimies.py==66)
					Pelimies.py=0;
				}
			}
			if ( nappi[SDLK_RIGHT] )
			{ 
				Pelimies.x += SPEED; 
				Pelimies.suuntax=-SPEED;
				Pelimies.px=63;
				if(homma==0)
				{
				if(Pelimies.py==0 || Pelimies.py==22 || Pelimies.py==44)
					Pelimies.py+=22;
				if(Pelimies.py==66)
					Pelimies.py=0;
				}
			} 
			if(nappi [SDLK_F10] && peliMenu==false)//menu päälle
			{
              PiirraKuva(pelimenuKuva,naytto,0,0,200,300,512,350);
			   peliMenu=true;
			   
			}
			if( nappi[SDLK_1] && peliMenu==true){//tallennetaan peli
				SaveGame("Save.sav");
				peliMenu=false;}
			else if( nappi[SDLK_2] && peliMenu==true){//ladataan peli
				LoadGame("Save.sav");
				peliMenu=false;}
			else if( nappi[SDLK_3] && peliMenu==true){//lopetetaan peli
				STATE=0;
				peliMenu=false;}
            else if(nappi[SDLK_4] && peliMenu==true)//menu pois
				peliMenu=false;

			if (nappi[SDLK_c] && Character==false)
				Character=true;
            if (nappi[SDLK_v] && Character==true)
				Character=false;
			if(Pelimies.XP>150*Pelimies.LEVEL)/////LEVELIN KASVU
			{
				Pelimies.LEVEL++;
				Pelimies.basestrength+=2;
				Pelimies.basestamina+=50;
				channel = Mix_PlayChannel(-1, sound[3], 0);//NICEDING.wav ei kuulu
			}
			
		SDL_FillRect(naytto, NULL, SDL_MapRGB(naytto->format, 0, 0, 0));//Pimeys
	//lattian piirto
		for(int y=0;y<17;y++)
		{
			for(int x=0;x<25;x++)
			{
               if(LaskeEtaisyys(Pelimies.x,Pelimies.y,(x*40),(y*40),d)<80)
				PiirraKuva(lattiaTile[x][y],naytto,40,0,40,40,(x*40),(y*40));
			}
		}
		PiirraKuva(pelaaja,naytto,Pelimies.px,Pelimies.py,22,22,Pelimies.x,Pelimies.y);//piirretään pelaaja,px ja py määrää mikä osa
		PiirraKuva(kuvaHud[0],naytto,0,0,Pelimies.stamina,15,10,740);//helat
		Pelimies.strength=Pelimies.basestrength+Pelimies.playerItems[0].strength;//aseen ottoa varten,mites muut kamat?
        
		for(int k=0;k<MAX_ITEMS-1;k++)//itemit ja niihin liittyvät hommat
		{
			if(LaskeEtaisyys(Pelimies.x,Pelimies.y,pickItems[k].x,pickItems[k].y,d)<100)
			{
				PiirraKuva(kuvaItem[k],naytto,0,0,22,22,pickItems[k].x,pickItems[k].y);
			}
			 Tormays(Pelimies.x,Pelimies.y,pickItems[k].x,pickItems[k].y,22,22,22,22);
			 if(Status==true && pickItems[k].tex==0)
			 {
				 pickItems[k].x=-10;
				 Pelimies.stamina+=15;
				 txtIndex=1;
			 }		
		} 
		
		for(int p=0;p<MAX_PAH-1;p++)//pahisten piirto ja niihin liittyvät hommat
		{
		
		     Tormays(Pelimies.x,Pelimies.y,Pahikset[p].x,Pahikset[p].y,22,22,22,22);
			 if(Status==true)
			 {
				 if(Pelimies.suuntax != Pahikset[p].suuntax || Pelimies.suuntay !=Pahikset[p].suuntay)//pelaaja hyökkää päälle
				 {
					 Pahikset[p].stamina-=Pelimies.strength;
					 if(Mix_Playing(channel)==0)
				     channel = Mix_PlayChannel(2, sound[0], 0);
				 }
				 Pelimies.stamina-=Pahikset[p].strength;	 
			 }
			for(int i=0;i<MAX_PAH-1;i++)//vihollisten törmääminen toisiinsa
			{
				if(i!=p)
				PahisTormays(Pahikset[i].x,Pahikset[i].y,Pahikset[p].x,Pahikset[p].y,22,22,22,22,i);
			}
			if(LaskeEtaisyys(Pelimies.x,Pelimies.y,Pahikset[p].x,Pahikset[p].y,d)<140)
			{				
				PiirraKuva(kuvaPahis[p],naytto,0,0,20,22,Pahikset[p].x,Pahikset[p].y);//näytetään silmät
			}
			if(LaskeEtaisyys(Pelimies.x,Pelimies.y,Pahikset[p].x,Pahikset[p].y,d)<80)
			{ 	
					//ja hyökätään
					if(Pahikset[p].x<Pelimies.x)
					{
						Pahikset[p].x+=1;
						Pahikset[p].px=65;
						Pahikset[p].suuntax=-1;
						if(homma==0)
						{
							if(Pahikset[p].py==0)
								Pahikset[p].py=22;
							else
								Pahikset[p].py=0;
						}
					}
					if(Pahikset[p].x>Pelimies.x)
					{
						Pahikset[p].x-=1;
						Pahikset[p].px=87;
						Pahikset[p].suuntax=1;
						if(homma==0)
						{
							if(Pahikset[p].py==0)
								Pahikset[p].py=22;
							else
								Pahikset[p].py=0;
						}
					}
					if(Pahikset[p].y<Pelimies.y)
					{
						Pahikset[p].y+=1;
						Pahikset[p].px=20;
						Pahikset[p].suuntay=-1;
						if(homma==0)
						{
						if(Pahikset[p].py==0)
							Pahikset[p].py=22;
						else
							Pahikset[p].py=0;
						}
					}
					if(Pahikset[p].y>Pelimies.y)
					{
						Pahikset[p].y-=1;
						Pahikset[p].px=42;
						Pahikset[p].suuntay=1;
						if(homma==0)
						{
						if(Pahikset[p].py==0)
							Pahikset[p].py=22;
						else
							Pahikset[p].py=0;
						}
					}
				
				 PiirraKuva(kuvaPahis[p],naytto,Pahikset[p].px,Pahikset[p].py,22,22,Pahikset[p].x,Pahikset[p].y);//näytetään kokonaan
				//HUOM. Korkeus ja leveys -1 jotta tappelu toimisi oikein
				PahisTormays(Pahikset[p].x,Pahikset[p].y,Pelimies.x,Pelimies.y,20,20,20,20,p);
               
				if(Pahikset[p].LEVEL==1 || Pahikset[p].LEVEL==2)
				txtIndex=2;//teksti hyökkäyksestä
                if(Pahikset[p].LEVEL==3)
				txtIndex=5;//teksti hyökkäyksestä	
			}
			if(Pahikset[p].stamina<0)
			{
				Pahikset[p].x=-10;
				txtIndex=3;
				channel = Mix_PlayChannel(-1, sound[1], 0);
				if(Pahikset[p].LEVEL==1)
				   Pelimies.XP+=10;
				else if(Pahikset[p].LEVEL==2)
					    Pelimies.XP+=25;
				else if(Pahikset[p].LEVEL==3)
					    Pelimies.XP+=40;

				Pahikset[p].stamina=1;//jotta teksti 3 ei jäisi ikuisesti
			}			
			if(Pelimies.stamina<0){
				channel = Mix_PlayChannel(-1, sound[1], 0);
				STATE=0;}
		}           
		for(int a=0;a<MAX_PAL-1;a++)//seiniin ja muihin kiinteisiin liittyvät hommat
		{
			lattiax=Palikat[a].x;
			lattiay=Palikat[a].y;
			Tormays(Pelimies.x,Pelimies.y,Palikat[a].x,Palikat[a].y,22,22,39,39);//törmätäänkö?             
			if( Status==true && Palikat[a].tex==1)
			{				
				STATE=0;
				Pelimies.floor++;
			}
            if( Status==true && Palikat[a].tex==3)//jos avataan arkku
			{				
				Palikat[a].chestOpened=true;
				
			}
			if( Status==true && Palikat[a].tex==2)//aseiden otto
			{				
			   txtIndex=4;
			   sprintf(textbuffer,"You wish to take weapon %i strength?",Palikat[a].standItem[0].strength);
			   item VanhaItem[1];//vanha ase muistiin
				  VanhaItem[0]=Pelimies.playerItems[0];
			   if(nappi[SDLK_SPACE])
			   {
				   SDL_Delay(300);
				    Pelimies.playerItems[0]=Palikat[a].standItem[0];
					Pelimies.weapon=Palikat[a].weapon;
				   	Palikat[a].standItem[0]=VanhaItem[0];		
			   }
			}
			if( Status==true && Palikat[a].tex==5)//armorien otto
			{				
			   txtIndex=4;
			   sprintf(textbuffer,"You wish to take armor %i stamina %d armor?",Palikat[a].standItem[0].stamina,Palikat[a].standItem[0].armor);
			   item Vanha2Item[2];
			   
				  Vanha2Item[0]=Pelimies.playerItems[1];//vanha armori muistiin
				  Vanha2Item[1]=Pelimies.playerItems[2];//vanha kypärä muistiin
			   if(nappi[SDLK_SPACE])
			   {
				   SDL_Delay(300);
				   if(Palikat[a].standItem[0].TYPE==1)
				   {
						Pelimies.playerItems[1]=Palikat[a].standItem[0];
						Pelimies.torso=Palikat[a].armor;
				   		Palikat[a].standItem[0]=Vanha2Item[0];
				   }
				   if(Palikat[a].standItem[0].TYPE==2)
				   {
						Pelimies.playerItems[2]=Palikat[a].standItem[0];
						Pelimies.head=Palikat[a].head;
				   		Palikat[a].standItem[0]=Vanha2Item[1];
				   }
					Pelimies.armor= Pelimies.playerItems[1].armor+ Pelimies.playerItems[2].armor + Pelimies.playerItems[3].armor;
					Pelimies.stamina = Pelimies.basestamina + Pelimies.playerItems[1].stamina+Pelimies.playerItems[2].stamina + Pelimies.playerItems[3].stamina;
			   } 
			}
			for(int s=0;s<MAX_PAH-1;s++)
			{
				PahisTormays(Pahikset[s].x,Pahikset[s].y,Palikat[a].x,Palikat[a].y,22,22,39,39,s);
			}
           //Tässä on pelin valaisun efekti,lisää paloja vois lisätä ja tehdä smoothimman valaistuksen 
			if(LaskeEtaisyys(Pelimies.x,Pelimies.y,Palikat[a].x,Palikat[a].y,d)<160)//tummemmpana
			{
				Palikat[a].nakyy=true;
				if(Palikat[a].tex==4 && LaskeEtaisyys(Pelimies.x,Pelimies.y,Palikat[a].x,Palikat[a].y,d)>100)//jos palikka on animoitava esim soihtu
				{
					if(Palikat[a].py==40)
						Palikat[a].py=0;
					else
						Palikat[a].py=40;

					PiirraKuva(kuva[a],naytto,0,Palikat[a].py,40,40,Palikat[a].x,Palikat[a].y);
				}
				else if(Palikat[a].tex==3)
				{
                    if(Palikat[a].chestOpened==false)//jos arkku on avattu
					PiirraKuva(kuva[a],naytto,0,0,40,40,Palikat[a].x,Palikat[a].y);
					if(Palikat[a].chestOpened==true)//jos arkku on avattu
					PiirraKuva(kuva[a],naytto,40,0,40,40,Palikat[a].x,Palikat[a].y);
				}
				else
                PiirraKuva(kuva[a],naytto,40,0,40,40,Palikat[a].x,Palikat[a].y);
				for(int h=0;h<MAX_PAL-1;h++)//soihdut ei oikeen skulaa oikein
				{
					if(Palikat[a].tex==4 && (LaskeEtaisyys(Palikat[a].x,Palikat[a].y,Palikat[h].x,Palikat[h].y,d)<60))//piiretään viereiset vaaleammiksi
                        PiirraKuva(kuva[h],naytto,0,Palikat[h].py,40,40,Palikat[h].x,Palikat[h].y);
				}
			}
		
			if(LaskeEtaisyys(Pelimies.x,Pelimies.y,Palikat[a].x,Palikat[a].y,d)<100)//valoisampana
			{
				Palikat[a].nakyy=true;
				if(Palikat[a].tex==4)//jos palikka on animoitava esim soihtu
				{
					if(Palikat[a].py==40)
						Palikat[a].py=0;
					else
						Palikat[a].py=40;

					PiirraKuva(kuva[a],naytto,40,Palikat[a].py,40,40,Palikat[a].x,Palikat[a].y);
				    channel = Mix_PlayChannel(-1, sound[2], 0);
				}
				else
                PiirraKuva(kuva[a],naytto,0,0,40,40,Palikat[a].x,Palikat[a].y);
				if(Palikat[a].chestOpened==true)//jos arkku on avattu
					PiirraKuva(kuva[a],naytto,40,0,40,40,Palikat[a].x,Palikat[a].y);
			}				           
		}
		if(Character==true)//piiretään ukon kamat
		{
			PiirraKuva(kuvaHud[1],naytto,0,0,169,347,800,10);
            PiirraKuva(kuvaHud[2],naytto,Pelimies.playerItems[0].px,0,70,94,760,160);//ase
		    PiirraKuva(kuvaHud[3],naytto,Pelimies.playerItems[1].px,Pelimies.playerItems[1].py,68,94,850,120);
		    PiirraKuva(kuvaHud[4],naytto,Pelimies.playerItems[2].px,Pelimies.playerItems[2].py,70,85,850,20);		
		}
		if(peliMenu==true)
			 PiirraKuva(pelimenuKuva,naytto,0,0,200,300,200,150);
			for(int n=0;n<MAX_PAH-1;n++)//ettei mennä seiniin
			{
			    Pahikset[n].suuntax=0;
				Pahikset[n].suuntay=0;
			}
		
        Pelimies.suuntax=0;
		Pelimies.suuntay=0;	
		return 1;
}
int main(int argc, char *argv[]) 
{
	bool paapois=false;
	
	TTF_Init();//tekstiä varten
    	TTF_Font* font = TTF_OpenFont("German.ttf", 24);//fontti
//Tekstit
SDL_Color foregroundColor = { 128, 0, 0 };//taustaväri
     SDL_Surface *infoSurface[8];
	 SDL_Surface *characterSurface[8];
    TTF_Font* font2 = TTF_OpenFont("German.ttf", 18);//fontti2    
SDL_Rect infoLocation = {10,10,0,0};
 SDL_Rect textLocation = { 820, 0, 0, 0 };
 SDL_Rect characterLocation={780,400,0,0};
 SDL_Rect characterLocation2={780,450,0,0};
 SDL_Rect characterLocation3={780,500,0,0};
 SDL_Rect characterLocation4={780,550,0,0};
 SDL_Rect characterLocation5={780,350,0,0};
 SDL_Rect characterLocation6={780,600,0,0};
	infoSurface[0] = TTF_RenderText_Solid(font2, " ",
      foregroundColor);
infoSurface[1] = TTF_RenderText_Solid(font2, POTIONTEXT,foregroundColor);
infoSurface[2] = TTF_RenderText_Solid(font2, ROTATTACKTEXT,foregroundColor);
infoSurface[3] = TTF_RenderText_Solid(font2, ATTACKDEADTEXT,foregroundColor);
infoSurface[5]=TTF_RenderText_Solid(font2,UNDEADATTACKTEXT,foregroundColor);
SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Character",
      foregroundColor);
 
    if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER) < 0 )  {
        fprintf(stderr, "SDL:n alustus ei onnistunut: %s\n", SDL_GetError()); // virheestä tiedot tiedostoon
        return 0; // lopetetaan ohjelma
    }
		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, audio_channels, audio_buffers) != 0) {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}
		Mix_AllocateChannels(3);
		Musa Tausta("Pelimusa2.wav",1);
	//PELIIN LIITTYVIÄ ALKU ASETUKSIA
	SPEED=1;
	STATE=0;
   LoadItems("Items.txt");//ladataan itemit
	Pelimies.floor=1;
	Pelimies.basestamina=250;
  Pelimies.basestrength=2;
  Pelimies.LEVEL=1;
  Pelimies.playerItems[0]=Items[7];//ase
  Pelimies.weapon=7;
  Pelimies.playerItems[1]=Items[3];//paita
  Pelimies.torso=3;
  Pelimies.playerItems[2]=Items[0];//kypärä
  Pelimies.head=0;
  Pelimies.playerItems[3]=Items[0];//housut
  Pelimies.feet=0;
    naytto = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE|SDL_FULLSCREEN|SDL_DOUBLEBUF);
      menukuva=SDL_LoadBMP("menu2.bmp");//ladataan menun kuva 
	  pelimenuKuva=SDL_LoadBMP("ingamemenu.bmp");
	   
	  //ÄÄNTEN ASETUS
//Pää-looppi    
while(paapois==false)
{
	
	 SDL_PollEvent(&tapahtuma);
            if ( tapahtuma.type == SDL_QUIT )  {  paapois = true;  }  //poistumistapahtuma
            if ( tapahtuma.type == SDL_KEYDOWN )  {
                    if ( tapahtuma.key.keysym.sym == SDLK_ESCAPE ) { paapois = true; }                   
            }
			
			  Game_Menu();
			 
			  Tausta.Toista(-1);//toistetaan loopilla radassa
			  Tausta.Keskeyta();
			  while(STATE==1)
			  {
				  	start=SDL_GetTicks();//alotetaan ajan otto
				    Tausta.Jatka();
				Game_RUN();//Peli käynnissä
				if(Character==true)//jos profiili on päällä
				{
					SDL_BlitSurface(textSurface, NULL, naytto, &textLocation);//menu teksti paneelin päälle
					
					if(Pelimies.CLASS==1)
					sprintf(textbuffer,"Level %i   Warrior",Pelimies.LEVEL);
					else if(Pelimies.CLASS==2)
					sprintf(textbuffer,"Level %i   Monk",Pelimies.LEVEL);

					characterSurface[5]=TTF_RenderText_Solid(font2,textbuffer,foregroundColor);
					SDL_BlitSurface(characterSurface[5],NULL,naytto,&characterLocation5);

					sprintf(textbuffer,"Strength:%i Base:%d",Pelimies.strength,Pelimies.basestrength);
					characterSurface[0]=TTF_RenderText_Solid(font2, textbuffer,foregroundColor);
					SDL_BlitSurface(characterSurface[0],NULL,naytto,&characterLocation);//strena
					sprintf(textbuffer,"Stamina: %i Base:%d",Pelimies.stamina,Pelimies.basestamina);
					characterSurface[1]=TTF_RenderText_Solid(font2, textbuffer,foregroundColor);
					SDL_BlitSurface(characterSurface[1],NULL,naytto,&characterLocation2);//stamina
					sprintf(textbuffer,"Armor: %i",Pelimies.armor);
					characterSurface[2]=TTF_RenderText_Solid(font2, textbuffer,foregroundColor);
					SDL_BlitSurface(characterSurface[2],NULL,naytto,&characterLocation3);//armori
					sprintf(textbuffer,"Experience: %i",Pelimies.XP);
					characterSurface[3]=TTF_RenderText_Solid(font2,textbuffer,foregroundColor);
					SDL_BlitSurface(characterSurface[3],NULL,naytto,&characterLocation4);
					sprintf(textbuffer,"Next Level: %i",Pelimies.LEVEL*150);
					characterSurface[4]=TTF_RenderText_Solid(font2,textbuffer,foregroundColor);
					SDL_BlitSurface(characterSurface[4],NULL,naytto,&characterLocation6);
					infoSurface[4]=TTF_RenderText_Solid(font2," ",foregroundColor);				
				}
				infoSurface[4]=TTF_RenderText_Solid(font2,textbuffer,foregroundColor);
				SDL_BlitSurface(infoSurface[txtIndex], NULL, naytto, &infoLocation);//info hyökkäyksestä
				SDL_Flip(naytto);//käännetään puskuri
				SDL_Delay(10);
			  }	
               Game_Reset();//tapahtuu kummalisuuksia jos tätä ei ole !? eli ei ole staminaa sun muita juttuja			  	 
 }
   for(int o=0;o<255;o++)
   {
    SDL_FreeSurface(kuva[o]);
   }
   Mix_FreeChunk(sound[0]);
   Mix_FreeChunk(sound[1]);
   Mix_FreeChunk(sound[2]);
   Tausta.Vapauta();
   TTF_Quit();
	Mix_CloseAudio();
    SDL_Quit(); // "suljetaan" SDL
    return 0;
} 