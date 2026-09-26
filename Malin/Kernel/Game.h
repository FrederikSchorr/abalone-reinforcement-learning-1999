/***********************************
/
/   Game.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef GAME_H
#define GAME_H

#include "Situation.h"
#include "SmartPtr.h"
#include "Player.h"
#include "History.h"
#include <exception>
#include <string>

/*** exception *********************/ 
class GameE : public exception
{
public: GameE(const std::string& s) : 
  exception(s.c_str()) { };
};


/*** Game **************************/ 
class Game : private Situation
{
public:
  Game();
  Game(SmartPtrPlayer, SmartPtrPlayer);
  Game(const std::string&);
  virtual ~Game();
  
public:
  void load (const std::string&);
  void load (const PlayerMap&);
  void save (const std::string&) const;
  
  void clear ();
  // Cleanup temporary objects
  void cleanup ();
  void players(const PlayerMap &map);
  void ejectWon(int n) { m_ejectWon = n; }
  void randomizeBoard();
  
public:
  Move calcMove();
  FullMove check(const Move& move) const;
  
  FullMove move(const Move& move);
  FullMove takeBackMove();
  Marble computerLoop();

  void cancel();

  Marble playerWon() const;
  bool bIsHuman() const;
  int nPlies() const { return m_liHist.size(); }

  const SmartPtrPlayer spPlayerAct() const 
    { return spPlayer(m_playerAct); }
  const PlayerMap& players() const;
  const SmartPtrPlayer spPlayer(Marble m) const;

  Count playerNum() const { return m_playerNum; }
  Count playerAct() const { return m_playerAct; }
  
  const Situation& sit() const { return *this; }
  const FullBoard& fullBoard() const { return *this; }
  
  const time_t* timePlayers() const { return m_timePlayers; }
  time_t timePlayerAct() const;

private:
  PlayerMap m_mapPlayer;
  time_t m_timePlayers[PLAYERS_MAX];
  time_t m_timeStart;
};


#endif