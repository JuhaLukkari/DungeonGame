#ifndef _musaLuokka_H_
#define _musaLuokka_H_
#include "SDL_Mixer.h"


//*********************************************************************
//MUSA-luokan esittely        
//Versio 1.0
//BY: Heikki M�ntysaari, 2004

//Jos funktio palauttaa arvon, -1 tarkoittaa ep�onnistumista (paitsi voimakkuuden s��t�)
//*********************************************************************

class Musa {
private:
    Mix_Chunk *data;
    int kanava;                            //Mill� kanavalla kyseist� ��nt� soitetaan

public:
    Musa(char *polku, int kannu);        //Muodostin    
    void Vapauta();                        //Tuhoaja
    int Toista(int maara);                //Toistaa maara monta kertaa ��nen
    int ToistaAika(int aika);            //Toistaa tietyn ajan (ms)
    void Haivyta(int ms);            //H�ivytt�� ��nen tietyssa ajassa (ms)
    int Voimakkuus(int voimakkuus);        //Vaihtaa soittovoimakkuuden (0-128)
    void Lopeta();                        //Lopettaa ��nen toistamisen
    void Keskeyta();                    //Keskeytt�� ��nen toistamisen
    void Jatka();                        //Jatkaa ��nen toistamista
    int ToistaHaivyttaen(int aika, int kerrat);    //Aloittaa ��nen toistamisen h�ivytt�m�ll�

};




//*********************************************************************
//Muodostin
//Lataa ��nitiedoston data-tietotyyppiin
//*********************************************************************
Musa::Musa(char *polku, int kannu) {

    data=Mix_LoadWAV_RW(SDL_RWFromFile(polku, "rb"), kannu);

    if (!data) {    //Lataus ep�onnistui
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
//Alkaa toistaa data-tietotyypiss� olevaa ��nt�
//Parametrina toistokertojen lukum��r�.
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
//Alkaa toistaa data-tietotyypiss� olevaa musiikkia
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
//Voimakkuuden s��t�
//Vaihtaa data:ssa olevan ��nen voimakkuudeksi parametrina annetun arvon (0-128)
//Palauttaa entisen voimakkuuden
//*********************************************************************
int Musa::Voimakkuus(int voimakkuus) {
    //onko arvo rajojen sis�ll�
    if (voimakkuus > 128)
        voimakkuus=128;
    if (voimakkuus < 0)
        voimakkuus=0;
    
    return Mix_VolumeChunk(data, voimakkuus);
}


//**********************************************************************
//Haivyta
//H�ivytt�� ��nen, parametrina h�ivitykseen kuluva aika
//**********************************************************************
void Musa::Haivyta(int ms) {
    Mix_FadeOutChannel(kanava, ms);
}


//*********************************************************************
//Lopettaminen
//Lopettaa ��nen toistamisen
//*********************************************************************

void Musa::Lopeta() {
    Mix_HaltChannel(kanava);
}



//*********************************************************************
//Keskeytt�minen
//Keskeytt�� ��nen toistamisen
//*********************************************************************

void Musa::Keskeyta() {
    Mix_Pause(kanava);
}


//*********************************************************************
//Jatkaminen
//Jatkaa ��nen toistamista
//*********************************************************************

void Musa::Jatka() {
    Mix_Resume(kanava);
}


//*********************************************************************
//Toisto H�ivytt�en
//Ottaa parametrina h�ivytysajan ms:in� ja toistokertojen m��r�n
//*********************************************************************
int Musa::ToistaHaivyttaen(int aika, int kerrat) {
    if(Mix_FadeInChannel(kanava, data, kerrat, aika)==-1) {
        fprintf(stderr, "Virhe (Mix_FadeInChannel @ Musa::ToistaHaivyttaen): %s", Mix_GetError());
        return -1;
    }
    return 0;
} 
#endif