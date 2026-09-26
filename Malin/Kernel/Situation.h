/***********************************
/
/   Situation.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 

#ifndef SITUATION_H
#define SITUATION_H

#include <exception>
#include <string>
#include "FullBoard.h"
#include "History.h"

/*** exception *********************/ 
class SituationE : public exception
{
public: SituationE(const std::string& s) : 
  exception(s.c_str()) { };
};


/*** Situation *********************/ 
class Situation : public FullBoard
{
public:
  Situation();
  Situation(Count);
	Situation(const Situation &sit);

public:
  void load(Count);
  void clear();

  void loadBoard(const Board &board);

public:
  Marble playerWon() const;

public:
  FullMove check(const Move& move) const;
  void move(const FullMove& fullMove);

  FullMove takeBackMove();

public:
  void allNewMoves(MoveVector&) const;
  
public:
  HistoryList m_liHist;
};

#endif