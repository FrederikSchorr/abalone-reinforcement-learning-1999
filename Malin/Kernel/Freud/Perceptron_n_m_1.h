/***********************************
/
/   Perceptron_n_m_1.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 


#ifndef PERCEPETRON_N_M_1_H
#define PERCEPETRON_N_M_1_H

/*** include ************************/ 
#include <vector>
#include <math.h>
#include <iostream.h>


/*** definition *********************/ 
typedef std::vector<float> FloatVector;
typedef std::vector<FloatVector> FloatMatrix;
class InputVector;


/*** Perceptron_n_m_1 ***************

                    OUTPUT

                      ()           y
                   /      \  
                 /   w[j]   \
               /   /      \   \
              ()  ()  ()  ()  ()  h[j]   j = 0 ... nHiddenNum
               \     sigma    /            (h[nHiddenNum] = bias = 1)
                \   v[i][j]  /          
                 \ / \  / \ /
                  ()  ()  ()      x[i]   i = 0 ... nInputNum
                                           (x[nInputNum] = bias = 1)
                     INPUT
*/
class Perceptron_n_m_1 
{
public:
  // Initialize unit vectors, weight matrix
  Perceptron_n_m_1() { clear(); }
  virtual ~Perceptron_n_m_1() {}

  // Initialize the net
  virtual void init(int nInputNum, int nHiddenNum, bool bSquashY);
  // Clear and deallocate the net
  virtual void clear();

  // Load net from a istream
  virtual void load(istream&);
  // Save net to a ostream
  virtual void save(ostream&) const;

  // Write to output stream, human readable
  ostream & write(ostream&) const;
  
public:
  // Randomize weigth matrix
  void randomize();
  
  // Copy x to input layer
  void input(const FloatVector &x);
  // Return output in y
  void output(float &y) const { y = m_y; }
  // Propagate input through the net
  void propagate();

  // Return random float 
  float random(float low, float high) const;

protected:
  // The sigmoid function
  float sigma(float x) const;
  // dsigma/dx(sigma^-1(y))
  float dSigma_invSigma(float y) const;

protected:
  // Declare constant bias
  enum { bias = 1 };

protected:
  int m_nInputNum;            // Number of input units (exclusive bias)
  int m_nHiddenNum;           // Number of hidden units (exclusive bias)
  
  FloatVector m_x;            // Input units vector
  FloatVector m_h;            // Hidden units vector
  float m_y;                  // Output unit
  
  FloatMatrix m_v;            // Weight matrix from input to hidden layer
  FloatVector m_w;            // Weight vector from hidden to output layer

  bool m_bSquashY;            // Determines wether sigma is applied to the output

  friend class InputVector;
};

/*** exception **********************/
class PerceptronE : public exception
{
public: 
  PerceptronE() {}
  PerceptronE(const std::string& s) : 
  exception(s.c_str()) { };
};


/* * * * * * * * * * * * * * * * * */
inline float Perceptron_n_m_1
::sigma(float x) const
{
  // The logistic function is used as sigmoid function
  // return 1.0 / (1.0 + exp(-x));

  // Tangens hyperbolicus is used as sigmoid function
  return tanh(x);
}

/* * * * * * * * * * * * * * * * * */
inline float Perceptron_n_m_1
::dSigma_invSigma(float y) const
{
  // The logistic function is used as sigmoid function
  // return y * (1.0 - y);

  // Tangens hyperbolicus is used as sigmoid function
  return 1.0 - y * y;

}
  
#endif