/***********************************
/
/   SimBoub.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef SIMBOUB_H
#define SIMBOUB_H


/*** include ***********************/ 
#include "Boub.h"
#include "../Simulation.h"


/*** SimBoub ************************/ 
class SimBoub : public Simulation
{
public:
  SimBoub(int nDepth);
  SimBoub(boub::COMPPLAYER&);
  SimBoub();
  virtual ~SimBoub() {}
  
public:
  static void init();

public:
  void load(istream&);
  void save(ostream&) const;

  ostream & write(ostream& o) const { m_cp.write(o); return o; }

  void modify(Modify &mod);

  void cancel();
  
public:
  bool bIsHuman() const;
  Move calcMove(const Situation&) ; 

  const boub::COMPPLAYER& data() const { return m_cp; }
  
private:
  boub::COMPPLAYER m_cp;

private:
  static boub::COMPPLAYER m_cpDefault;
  static char *m_szPoints[boub::POI_LAST];
 
  friend class GenomeBoub;
  
};

#endif