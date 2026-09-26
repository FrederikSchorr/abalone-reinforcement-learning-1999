/***********************************
/
/   SimRandom.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef SIMRANDOM_H
#define SIMRANDOM_H


/*** include ***********************/ 
#include "SimCaesar.h"


/*** SimRandom *********************/ 
class SimRandom : public Simulation
{
public:
  SimRandom() { m_type = Simulation::random; srand((unsigned)time(NULL)); }
  virtual ~SimRandom() {}
  
public:
  void load(istream& is) {}
  void save(ostream&) const {}

  ostream & write(ostream& os) const { os << "Random player" << endl; return os; }

  void modify(Modify &mod) { mod.println("Random player"); }

  void cancel() {}
  
public:
  bool bIsHuman() const { return false; }
  Move calcMove(const Situation& sit)
  {
    MoveVector moves;
    sit.allNewMoves(moves);
    return moves[Kernel::rand(moves.size())];
  }
};

#endif