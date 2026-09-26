/***********************************
/
/   Board.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 

#ifndef BOARD_H
#define BOARD_H

#include "Kernel.h"

/*** Board *************************/ 
class Board
{
public:
  Marble m_marble[POS_NUM + 1];
  Count m_kicked[PLAYERS_MAX];
  Count m_lost[PLAYERS_MAX];

  bool equals(const Board &b) const
  {
    return !memcmp(this, &b, sizeof(Board));
  }
  operator=(const Board &b)
  {
    memcpy(this, &b, sizeof(Board));
  }
};


#endif