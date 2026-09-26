/***********************************
/
/   FullBoard.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 

#ifndef FULLBOARD_H
#define FULLBOARD_H

#include "Board.h"
#include "Move.h"


/*** FullBoard *********************/ 
class FullBoard : public Board
{
public:
  enum { movesMax = 130, movesEjectMax = 25 };

public:
  ostream & write(ostream & os) const;

public:
  void allMoves(MoveVector&) const;
  void ejectMoves(MoveVector&) const;

  virtual void moveRaw(const FullMove&);

public:
  Count m_playerNum;
  Marble m_playerAct;
  Count m_ejectWon;
};

inline ostream& operator<<(ostream& os, const FullBoard &full) { return full.write(os); }

#endif