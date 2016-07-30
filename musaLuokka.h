#ifndef _musaLuokka_H_
#define _musaLuokka_H_
#include "SDL_Mixer.h"


//*********************************************************************
//MUSA-luokan esittely        
//Versio 1.0
//BY: Heikki Mäntysaari, 2004

//Jos funktio palauttaa arvon, -1 tarkoittaa epäonnistumista (paitsi voimakkuuden säätö)
//*********************************************************************

class Musa {
private:
    Mix_Chunk *data;
    int kanava;                            //Millä kanavalla kyseistä ääntä soitetaan

public:
    Musa(char *polku, int kannu);        //Muodostin    
    void Vapauta();                        //Tuhoaja
    int Toista(int maara);                //Toistaa maara monta kertaa äänen
    int ToistaAika(int aika);            //Toistaa tietyn ajan (ms)
    void Haivyta(int ms);            //Häivyttää äänen tietyssa ajassa (ms)
    int Voimakkuus(int voimakkuus);        //Vaihtaa soittovoimakkuuden (0-128)
    void Lopeta();                        //Lopettaa äänen toistamisen
    void Keskeyta();                    //Keskeyttää äänen toistamisen
    void Jatka();                        //Jatkaa äänen toistamista
    int ToistaHaivyttaen(int aika, int kerrat);    //Aloittaa äänen toistamisen häivyttämällä

};




//*********************************************************************
//Muodostin
//Lataa äänitiedoston data-tietotyyppiin
//*********************************************************************
Musa::Musa(char *polku, int kannu) {

    data=Mix_LoadWAV_RW(SDL_RWFromFile(polku, "rb"), kannu);

    if (!data) {    //Lataus epäonnistui
        fprintf(stderr, "Virhe (Mix_LoadWAV @ Musa::Musa): %s", Mix_GetError());        
    }

    //kanava talteen
    kanava=kannu;
}

//*********************************************************************
//Tuhoaja
//Vapauttaa musiikkimuuttujan
//*********************************************************************
void Musa::Vapauta() {
    Mix_FreeChunk(data);
    data=NULL;
}

//*********************************************************************
//Toista
//Alkaa toistaa data-tietotyypissä olevaa ääntä
//Parametrina toistokertojen lukumäärä.
//*********************************************************************

int Musa::Toista(int kerrat) {
    //soitetaan
    if(Mix_PlayChannel(kanava, data, kerrat)==-1) {
        fprintf(stderr, "Virhe (Mix_PlayChannel @ Musa::Toista): %s", Mix_GetError());   //virhe
        return -1;
    }
    return 0;    
}


//*********************************************************************
//ToistaAika
//Alkaa toistaa data-tietotyypissä olevaa musiikkia
//Parametrina toistoaika (ms)
//*********************************************************************
int Musa::ToistaAika(int aika) {
    //soitetaan
    if(Mix_PlayChannelTimed(kanava, data, -1, aika)==-1) {
        fprintf(stderr, "Virhe (Mix_PlayChannelTimed @ Musa::ToistaAika): %s", Mix_GetError());   //virhe
        return -1;
    }
    return 0;
}

//*********************************************************************
//Voimakkuuden säätö
//Vaihtaa data:ssa olevan äänen voimakkuudeksi parametrina annetun arvon (0-128)
//Palauttaa entisen voimakkuuden
//*********************************************************************
int Musa::Voimakkuus(int voimakkuus) {
    //onko arvo rajojen sisällä
    if (voimakkuus > 128)
        voimakkuus=128;
    if (voimakkuus < 0)
        voimakkuus=0;
    
    return Mix_VolumeChunk(data, voimakkuus);
}


//**********************************************************************
//Haivyta
//Häivyttää äänen, parametrina häivitykseen kuluva aika
//**********************************************************************
void Musa::Haivyta(int ms) {
    Mix_FadeOutChannel(kanava, ms);
}


//*********************************************************************
//Lopettaminen
//Lopettaa äänen toistamisen
//*********************************************************************

void Musa::Lopeta() {
    Mix_HaltChannel(kanava);
}



//*********************************************************************
//Keskeyttäminen
//Keskeyttää äänen toistamisen
//*********************************************************************

void Musa::Keskeyta() {
    Mix_Pause(kanava);
}


//*********************************************************************
//Jatkaminen
//Jatkaa äänen toistamista
//*********************************************************************

void Musa::Jatka() {
    Mix_Resume(kanava);
}


//*********************************************************************
//Toisto Häivyttäen
//Ottaa parametrina häivytysajan ms:inä ja toistokertojen määrän
//*********************************************************************
int Musa::ToistaHaivyttaen(int aika, int kerrat) {
    if(Mix_FadeInChannel(kanava, data, kerrat, aika)==-1) {
        fprintf(stderr, "Virhe (Mix_FadeInChannel @ Musa::ToistaHaivyttaen): %s", Mix_GetError());
        return -1;
    }
    return 0;
} 
#endif