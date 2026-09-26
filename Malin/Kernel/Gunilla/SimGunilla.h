/***********************************
/
/   SimGunilla.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 


#ifndef SIMGUNILLA_H
#define SIMGUNILLA_H

/*** include ************************/ 
#include "../Simulation.h"
#include "../Freud/Optimistic_TDlambda.h"
#include "../David/Killer.h"
#include "../Emil/Hash.h"
#include "../Emil/HashBoard.h"



/*** SimDavid ***********************/ 
class SimGunilla : public Simulation, protected Optimistic_TDlambda
{
public:
  // Constructor & Destructor
  SimGunilla();
  virtual ~SimGunilla() {}
  
public:
  // virtual functions from Simulation
  void load(istream&);
  void save(ostream&) const;
  ostream & write(ostream& os) const ;
  void modify(Modify &mod);

  void cancel();

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
  // Generates a move during training phase (simulated annealing)
  void evalTraining(const Situation & sit);

  // Generates the best move, hast table, killer moves
  float vEval(const HashBoardF& board, const char nDepth, float vAlpha, float vBeta);

  // Evaluates the board (by evaluating the neural net)
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
  enum { feature00 = 0, feature01, feature02, feature03, 
    feature04, feature05, feature06, featureLast };
  // Determines which kind of feature set shall be used
  int m_nFeatureType;

  // Temperature used by the simulated annealing random generator
  float m_fTemperature;

protected:
  // cancel
  volatile bool m_bCancel;

protected:
  // The killer object
  Killer m_killer;

  // The hash object
  HashTableF m_hash;

  // Determines wether simulation is currently trained
  bool m_bTraining;

  // Statistics
  STAT(Statistics m_statNew);

protected:
  // data used by algorithm
  FullMove m_moveBest;
  char m_nDepthLimit;
  const Situation* m_pSit;
  time_t m_timeEnd;
  int m_nCounter;

  // Temporary (avoid memory reallocation)
  std::vector<MoveVector> m_moves;

protected:
  // Holds type-specific # of features
  static int m_nFeatureNum[];

  // Classifies board positions to 9 classes (0-8)
  static int m_group[];

  // Maps score to 22 classes (0-21, parity)
  static int m_score[7][7];
};

#endif