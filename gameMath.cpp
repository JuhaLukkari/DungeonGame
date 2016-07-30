/*
//gameMath.cpp
//Laskentaa, etäisyyksiä sun muita
//Älä käytä potenssin laskentaan pow-komentoa,hidastaa ihan vitusti
*/
#include <cmath>





double LaskeEtaisyys(int x1,int y1,int x2,int y2,double Etaisyys)
{
  int deltax= x2 - x1;//x-muutos
  int deltay= y2 - y1;//y-muutos
  Etaisyys= sqrt(abs(deltax*deltax)+abs(deltay*deltay));//etäisyys Pythagoraan kaavan tyyliin
  return Etaisyys;//palautetaan etäisyys
}

