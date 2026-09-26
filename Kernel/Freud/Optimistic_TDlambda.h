/***********************************
/
/   Optimistic_TDlambda.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 


#ifndef OPTIMISTIC_TDLAMBDA_H
#define OPTIMISTIC_TDLAMBDA_H

/*** include ************************/ 
#include "Perceptron_n_m_1.h"


/*** definition *********************/ 



/*** Optimistic_TDlambda ************/ 
class Optimistic_TDlambda : private Perceptron_n_m_1
{
public:
  Optimistic_TDlambda() { clear(); }
  virtual ~Optimistic_TDlambda() {}

  void init(int nInputNum, int nHiddenNum,    
    bool bSquashY, bool bOnline, 
    float lambda, float stepInput = 0.0, float stepHidden = 0.0);
  virtual void clear();

  // Load net from a istream
  virtual void load(istream&);
  // Save net to a ostream
  virtual void save(ostream&) const;

  // Write to output stream, human readable
  ostream & write(ostream&) const;

private:
  // Overrides Perceptron_n_m_1::init
  virtual void init(int nInputNum, int nHiddenNum, bool bSquashY);

public:
  // Start new trajectory
  void learnBegin(const FloatVector & x);
  // Next state in trajectory, update weights
  void learnNext(const FloatVector & x);
  // Last state in trajectory, final reward, update weights
  void learnEnd(const FloatVector & x, float y);

public:
  // How many states have been considered
  long lLearnStates() const { return m_lLearnStates; }
  // How many trajectories have been run through
  long lLearnTrajec() const { return m_lLearnTrajec; }

public:
  // Randomize weigth matrix
  void randomize() { Perceptron_n_m_1::randomize(); }
  
  // Copy x to input layer
  void input(const FloatVector &x) { Perceptron_n_m_1::input(x); }
  // Return output in y
  void output(float &y) const { Perceptron_n_m_1::output(y); }
  // Propagate input through the net
  void propagate() { Perceptron_n_m_1::propagate(); }

public:
  void lambda(float f) { m_lambda = f; }
  float fLambda() { return m_lambda; }
  void stepInput(float f) { m_stepInput = f; }
  float fStepInput() { return m_stepInput; }
  void stepHidden(float f) { m_stepHidden = f; }
  float fStepHidden() { return m_stepHidden; }

  int nInputNum() { return m_nInputNum; }
  int nHiddenNum() { return m_nHiddenNum; }

  bool bSquashY() { return m_bSquashY; }
  void squashY(bool b) { m_bSquashY = b; }
  bool bOnline() { return m_bOnline; }
  void online(bool b) { m_bOnline = b; }


private:
  // Adjust weights according to elig. and the error ( = difference)
  void adjustWeights(FloatMatrix & v, FloatVector & w, float difference);

  // Update elig. according to actual neuron activation levels
  void updateElig();
  // Set eligibilities to 0.0
  void resetElig();

private:
  float m_lambda;           // The decay factor 
  float m_stepInput;        // Step size for the input layer
  float m_stepHidden;       // Step size for the hidden layer

  FloatMatrix m_zv;         // Eligibility matrix for i-h weights
  FloatVector m_zw;         // Eligibility vector for h-o weights

  long m_lLearnStates;      // # of considered states (input vectors)
  long m_lLearnTrajec;      // # of trajectories

  float m_yLast;            // Output from last state in trajectory

  bool m_bOnline;           // Determines wether on- of offline algo. is used

  FloatMatrix m_vOff;       // Offline weight matrix from input to hidden layer
  FloatVector m_wOff;       // Offline weight vector from hidden to output layer
};


#endif