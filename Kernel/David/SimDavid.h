/***********************************
/
/   SimDavid.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef SIMDAVID_H
#define SIMDAVID_H

/*** include ************************/ 
#include "../Caesar/SimCaesar.h"
#include "Killer.h"



/*** SimDavid ***********************/ 
class SimDavid : public SimCaesar
{
public:
  SimDavid();
  virtual ~SimDavid() { }

public:
  void modify(Modify &mod);
  Move calcMove(const Situation&); 
  // Cleanup temporary objects
  void cleanup ();

private:
  caesar::Value vEvalAlphaBeta(const FullBoard& fullBoard, char nDepth, caesar::Value vAlpha, caesar::Value vBeta);

protected:
  Killer m_killer;
  
  // Temporary (avoid memory reallocation)
  std::vector<MoveVector> m_moves;
};

#endif