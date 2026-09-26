/***********************************
/
/   Player.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 

#ifndef PLAYER_H
#define PLAYER_H

#include "Simulation.h"
#include "SmartPtr.H"

/*** exception *********************/ 
class PlayerE : public exception
{
public: PlayerE(const std::string& s) : 
  exception(s.c_str()) { };
};


/*** Player ************************/ 
class Player: public SmartObject
{
public:
  // Create a new player of a given type
  Player(Simulation::Type);
  // Create a new player from a given simulation object, name and description
  Player(const SmartPtrSim& spSim, const std::string& sName, const std::string& sDesc);
  // Create a new player from a file
  Player(const std::string& sFile);
  // Destructor
  virtual ~Player() {}

public:
  // Initialize all simulation objects
  static void init();
  
public:
  // Load player from file
  void load(const std::string& sFile);
  // Save player to a given file
  void save(const std::string& sFile) const;
  // Save player to <m_sName>.ap in the player directory
  void save() const;
  // Save player in the directory "player directory/m_sName/" and append nIndex to the filename
  void save(int nIndex) const;

  // Write to output stream, human readable
  ostream & write(ostream&) const;

public:
  // Cancel all (possibly ongoing) calculations
  void cancel() { m_spSim->cancel(); }
  // Cleanup temporary objects
  void cleanup () { m_spSim->cleanup(); }


public:
  // Use a Modify object to change player settings
  void modify(Modify& mod);
  
public:
  // ? human player
  bool bIsHuman() const;
  // The players name
  const std::string& sName() const { return m_sName; }
  // It's description
  const std::string& sDesc() const { return m_sDesc; }
  // A const pointer to the simulation object
  const SmartPtrSim spSim() const { return m_spSim; }
  // Reference to the statistics object
  const Statistics & statistics() { return m_spSim->statistics(); }
  
  // Calculate the best move for this situation
  Move calcMove(const Situation& sit);

public:
  // A new game begins, prepare training algorithm
  void learnBegin(const FullBoard & board) { m_spSim->learnBegin(board); }
  // A move was chosen, feed it into the training algo.
  void learnNext(const FullBoard & board) { m_spSim->learnNext(board); }
  // Game over, finish training algo.
  void learnEnd(const FullBoard & board) { m_spSim->learnEnd(board); }
  // ? how many games have been used for training
  long lLearnGames() { return m_spSim->lLearnGames(); }

public:
  // Returns a description string for each simulation type supported
  static const char *sSimDesc(Simulation::Type type);
  
private:
  // The simulation object
  SmartPtrSim m_spSim;
  // The players name
  std::string m_sName;
  // The players description string
  std::string m_sDesc;
};

inline ostream &operator<<(ostream &os, const Player & player) { return player.write(os); }

typedef SmartPtr<Player> SmartPtrPlayer;
typedef std::map<Count, SmartPtrPlayer> PlayerMap;

#endif