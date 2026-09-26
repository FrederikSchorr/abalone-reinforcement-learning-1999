/***********************************
/
/   History.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef HISTORY_H
#define HISTORY_H

#include "Move.h"
#include "Board.h"

/*** History ***********************/ 
class History
{
public: 
  Board m_board;
  FullMove m_fMove;
  Count m_playerAct;

public:
  bool equals(History& hist) const
  {
    return m_fMove.equals(hist.m_fMove) && 
      m_board.equals(hist.m_board) &&
      m_playerAct == hist.m_playerAct;
  }
};

/* * * * * * * * * * * * * * * * * */
typedef std::list<History> HistoryList;

#endif