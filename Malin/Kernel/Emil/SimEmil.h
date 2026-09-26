/***********************************
/
/   SimEmil.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef SIMEMIL_H
#define SIMEMIL_H


/*** include ***********************/ 
#include "../David/Killer.h"
#include "Hash.h"
#include "HashBoard.h"
#include "../Simulation.h"

/*** Emil data *********************/ 
namespace emil
{

  /* * * * * * * * * * * * * * * * * */
  class Data
  {
  public:
    Data();

  public:
    ostream & write(ostream& os) const;

  public:
    enum Points { Border = 0, Center = 5, Neighbour, Kicked, PointsNum};

    int m_nDepthMin, m_nDepthMax;
    int m_nTimeOut;
    int m_vPoints[PointsNum];
  };
} 

/*** SimEmil *********************/ 
class SimEmil : public Simulation, public emil::Data
{
public:
  // Constructor & Destructor
  SimEmil();
  virtual ~SimEmil() {}
  
public:
  // virtual functions from Simulation
  void load(istream&);
  void save(ostream&) const;
  ostream & write(ostream& os) const ;
  void modify(Modify &mod);

  void cancel();
  // Cleanup temporary objects
  void cleanup ();

  bool bIsHuman() const;
  Move calcMove(const Situation&); 

private:
  // the heart of the algorithm is private
  int vEvalHash(const HashBoardInt& board, const char nDepth, int vAlpha, int vBeta);
  
protected:
  // algorithm functions also accessible to children
  int vEvalBoard(const HashBoardInt& board);

protected:
  // cancel
  volatile bool m_bCancel;

protected:
  // data used by algorithm
  FullMove m_moveBest;
  char m_nDepthLimit;
  const Situation* m_pSit;
  time_t m_timeEnd;
  int m_nCounter;

  Killer m_killer;
  HashTableInt m_hash; 

  // Temporary (avoid memory reallocation)
  std::vector<MoveVector> m_moves;

  STAT(Statistics m_statNew);
};

#endif