/***********************************
/
/   Tournament.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 

#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "Game.h"
#include "Player.h"

/*** Tournament ********************/ 
class Tournament
{
public:
  //Tournament(Game & game) : m_game(game) {}
  Tournament() {}
  virtual ~Tournament() {}

public:
  // Init tournament with given set of players
  void init(const PlayerMap & players);
  // Start the tournament 
  void run();
  // Cancel tournament
  void cancel();

private:
  Game m_game;
  PlayerMap m_players;
  bool m_bCancel;
};

#endif