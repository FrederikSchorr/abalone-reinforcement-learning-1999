/***********************************
/
/   SimFreud.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 


#ifndef SIMFREUD_H
#define SIMFREUD_H

/*** include ************************/ 
#include "../Simulation.h"
#include "Optimistic_TDlambda.h"
#include "../David/Killer.h"



/*** SimDavid ***********************/ 
class SimFreud : public Simulation, protected Optimistic_TDlambda
{
public:
  // Constructor & Destructor
  SimFreud();
  virtual ~SimFreud() {}
  
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

public:
  // A new game begins, prepare training algorithm
  void learnBegin(const FullBoard & board);
  // A move was chosen, feed it into the training algo.
  void learnNext(const FullBoard & board);
  // Game over, finish training algo.
  void learnEnd(const FullBoard & board);
  // ? how many games have been used for training
  long lLearnGames() { return Optimistic_TDlambda::lLearnTrajec(); }

protected:
  // the heart of the algorithm 
  float vEval(const FullBoard& board, const char nDepth, float vAlpha, float vBeta);
  float vEvalBoard(const FullBoard & board);

  // Translates a board to an input vector
  void boardToInput(const FullBoard & board, FloatVector & x);

protected:
  // Simulation parameters
  // TimeOut, min & max search depth
  int m_nTimeOut;
  int m_nDepthMax;
  int m_nDepthMin;

protected:
  enum { feature00 = 0, feature01, feature02, feature03, feature04, featureLast };
  // Determines which kind of feature set shall be used
  int m_nFeatureType;
  static int m_nFeatureNum[];

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

  // The killer object
  Killer m_killer;

  // Temporary (avoid memory reallocation)
  std::vector<MoveVector> m_moves;

  // Statistics
  STAT(Statistics m_statNew);
};

#endif