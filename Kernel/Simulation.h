/***********************************
/
/   Simulation.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef SIMULATION_H
#define SIMULATION_H

#pragma warning (disable : 4786)

#include "Modify.h"
#include "SmartPtr.h"
#include "Move.h"
#include "Situation.h"
#include "Statistics.h"


/*** exception *********************/ 
class SimulationE : public exception
{
public: SimulationE(const std::string& s) : 
  exception(s.c_str()) { };
};



/*** Simulation ********************/ 
class Simulation : public SmartObject
{
public:
  virtual ~Simulation() {}

public:
  // Load and save simulation from and to iostream
  virtual void load(istream&) = 0;
  virtual void save(ostream&) const = 0;

  // Write to output stream, human readable
  virtual ostream & write(ostream&) const = 0;

  // Return wether move can be calculated or not
  virtual bool bIsHuman() const = 0;
  // The main routine, calculating the best move
  virtual Move calcMove(const Situation&) = 0;

  // Use modify object to interrogate user and update object
  virtual void modify(Modify& mod) = 0;

  // Throws CancelE in the calculating thread
  virtual void cancel() {}

  // Cleanup temporary objects
  void cleanup () {}

  // A new game begins, prepare training algorithm
  virtual void learnBegin(const FullBoard & board) {}
  // A move was chosen, feed it into the training algo.
  virtual void learnNext(const FullBoard & board) {}
  // Game over, finish training algo.
  virtual void learnEnd(const FullBoard & board) {}
  // ? how many games have been used for training
  virtual long lLearnGames() { return 0L; }

  // Reference to the statistics object
  const Statistics & statistics() { return m_stat; }

public:
  // Distinguish the different simulation types
  enum Type {unknown = 0, human, aba212, boub, caesar, charles, 
    david, emil, random, freud, gunilla, last};

public:
  Type type() const { return m_type; }

protected:
  Type m_type;
  Statistics m_stat;
};

typedef SmartPtr<Simulation> SmartPtrSim;

#endif