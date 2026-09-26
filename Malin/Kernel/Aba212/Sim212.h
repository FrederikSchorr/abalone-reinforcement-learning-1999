/***********************************
/
/   Sim212.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef SIM212_H
#define SIM212_H


/*** include ***********************/ 

#include "../Simulation.h"

namespace aba212
{
  class COMPPLAYER;
}


/*** Sim212 ************************/ 
class Sim212 : public Simulation
{
public:
  Sim212(int nDepth);
  Sim212();
  virtual ~Sim212();
  
public:
  static void init();

public:
  void load(istream&);
  void save(ostream&) const;

  ostream & write(ostream& o) const { o << "Aba2 player"; return o; }

  void modify(Modify &mod);

  void cancel();
  
public:
  bool bIsHuman() const;
  Move calcMove(const Situation&) ; 

private:
  aba212::COMPPLAYER *m_pCp;
  
};

#endif