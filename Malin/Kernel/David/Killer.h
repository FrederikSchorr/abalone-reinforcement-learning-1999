/***********************************
/
/   Killer.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef KILLER_H
#define KILLER_H

/*** include ************************/ 
#include <vector>
#include <iostream.h>
#include "../Move.h"

/*** definition *********************/ 


/*** HitsVector *********************/ 
typedef std::vector<short> HitsVector;
typedef std::vector<HitsVector> MoveHitsVector; // vector[ply][moveId] = hits


/*** SimDavid ***********************/ 
class Killer
{
public:
  Killer() { clear(); }

public:
  // Empty all move tables
  void init();
  // Clear up killer object (deallocate tables)
  void clear();
  // Sort the given move vector according to table[nDepth]
  void sort(char nDepth, MoveVector& moves);
  // Normalize all tables
  void normalize();
  // Add a move to table[nDepth]
  void add(char nDepth, const FullMove& move);

  //void initDepth(char nDepth);

public:
  static int nMoveId(const FullMove& move);

private:
  // Return the nDepth's move table (and be sure it exists)
  HitsVector & hitsVector(char nDepth);

private:
  // The vector of move tables
  MoveHitsVector m_vec;

public:
  ostream& write(ostream& os) const;
};

ostream& operator<<(ostream& os, Killer& killer);

#endif