/*
//gameMath.cpp
//Laskentaa, et�isyyksi� sun muita
//�l� k�yt� potenssin laskentaan pow-komentoa,hidastaa ihan vitusti
*/
#include <cmath>





double LaskeEtaisyys(int x1,int y1,int x2,int y2,double Etaisyys)
{
  int deltax= x2 - x1;//x-muutos
  int deltay= y2 - y1;//y-muutos
  Etaisyys= sqrt(abs(deltax*deltax)+abs(deltay*deltay));//et�isyys Pythagoraan kaavan tyyliin
  return Etaisyys;//palautetaan et�isyys
}

