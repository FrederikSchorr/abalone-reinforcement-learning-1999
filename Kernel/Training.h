/***********************************
/
/   Training.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 

#ifndef TRAINING_H
#define TRAINING_H

#include "Game.h"

/*** Tournament ********************/ 
class Training
{
public:
  //Training(Game & game) : m_game(game){}
  Training() {}
  virtual ~Training() {}

public:
  // Init training session with a script file
  void init(const std::string & sScript);
  // Start the training 
  void run();
  // Cancel training
  void cancel();

private:
  Game m_game;
  std::string m_sFile;
};


/*** exception *********************/ 
class TrainingE : public exception
{
public: TrainingE(const std::string& s) : 
  exception(s.c_str()) { };
};


#endif