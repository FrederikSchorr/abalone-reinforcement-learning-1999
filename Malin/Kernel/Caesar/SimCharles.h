/***********************************
/
/   SimCharles.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef SIMCHARLES_H
#define SIMCHARLES_H


#include "SimCaesar.h"


/*** SimCharles ********************/ 
class SimCharles : public SimCaesar
{
public:
  SimCharles();
  virtual ~SimCharles() { }

public:
  void modify(Modify &mod);
  Move calcMove(const Situation&); 

private:
  caesar::Value vEvalAlphaBeta(const FullBoard& fullBoard, char nDepth, caesar::Value vAlpha, caesar::Value vBeta);
};

#endif