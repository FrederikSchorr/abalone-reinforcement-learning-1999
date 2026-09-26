/***********************************
/
/   Perceptron_n_m_1.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 


/*** include ************************/ 
#include "Perceptron_n_m_1.h"
#include <time.h>
#include <iomanip.h>

#include <afxwin.h>

/*** Perceptron_n_m_1 ***************/ 


/* * * * * * * * * * * * * * * * * */
void Perceptron_n_m_1
::init(int nInputNum, int nHiddenNum, bool bSquashY)
{
  // Store layer dimensions
  m_nInputNum = nInputNum;
  m_nHiddenNum = nHiddenNum;

  // Allocate memory for neuron units
  m_x.assign(nInputNum + 1);
  m_h.assign(nHiddenNum + 1);

  // Allocate memory for weights
  m_v.assign(nInputNum + 1, FloatVector(nHiddenNum));
  m_w.assign(nHiddenNum + 1);

  // Set bias units
  m_x[nInputNum] = bias;
  m_h[nHiddenNum] = bias;

  // ? Apply sigmoidal function to the output
  m_bSquashY = bSquashY;
}



/* * * * * * * * * * * * * * * * * */
void Perceptron_n_m_1
::clear()
{
  m_nInputNum = 0;
  m_nHiddenNum = 0;

  m_x.clear();
  m_h.clear();
  m_y = 0.0;

  m_v.clear();
  m_w.clear();

  m_bSquashY = false;
}


/* * * * * * * * * * * * * * * * * */
float Perceptron_n_m_1
::random(float low, float high) const
{
  float val = high - low; 
  val *= (float) rand(); 
  val /= (float) RAND_MAX; 
  return low + val;
}


/* * * * * * * * * * * * * * * * * */
void Perceptron_n_m_1
::randomize()
{
  // Initialize random generator
  srand(time(NULL));
  
  // Determine bounds;
  ASSERT(m_nHiddenNum != 0);
  ASSERT(m_nInputNum != 0);
  float bound = 0.1 / __max(m_nHiddenNum, m_nInputNum);

  int i,j;

  // Attribute small random values to weights
  // Hidden - output weights
  for (j = 0; j < m_nHiddenNum + 1; j++)
  {
    m_w[j] = random(-bound, bound);  
  }

  // Input - hidden weights
  for (j = 0; j < m_nHiddenNum; j++)
  {
    for (i = 0; i < m_nInputNum + 1; i++)
    {
      m_v[i][j] = random(-bound, bound);
    }
  }
}


/* * * * * * * * * * * * * * * * * */
void Perceptron_n_m_1
::input(const FloatVector & x)
{
  // Check for correct vector size
  if (x.size() != m_nInputNum) 
    throw PerceptronE("Inputvector has incorrect size");

  // Copy vector
  m_x = x;
  ASSERT(m_x[m_nInputNum] == bias);
}


/* * * * * * * * * * * * * * * * * */
void Perceptron_n_m_1
::propagate()
{
  ASSERT(m_x[m_nInputNum] == bias);
  ASSERT(m_h[m_nHiddenNum] == bias);

  int i,j;

  // Calculate the activation levels of the hidden layer
  for (j = 0; j < m_nHiddenNum; j++)
  {
    m_h[j] = 0.0;
    // Sum up inputs * weights
    for (i = 0; i < m_nInputNum/* + 1*/; i++)
    {
      m_h[j] += m_v[i][j] * m_x[i];
    }
    // Apply sigmoid function
    m_h[j] = sigma(m_h[j]);
  }

  // Output layer
  m_y = 0.0;
  for (j = 0; j < m_nHiddenNum/* + 1*/; j++)
  {
    m_y += m_w[j] * m_h[j];
  }
  if (m_bSquashY) m_y = sigma(m_y);
}


/* * * * * * * * * * * * * * * * * */
void Perceptron_n_m_1
::save(ostream & os) const
{
  int i,j;

  long flags = os.setf(ios::showpos | ios::left);
  
  // Dimensions
  os << m_nInputNum << " " << m_nHiddenNum << " " << m_bSquashY << endl;

  // x, h, y
  for (i = 0; i < m_nInputNum + 1; i++) os << m_x[i] << " "; os << endl;
  for (j = 0; j < m_nHiddenNum + 1; j++) os << setw(15) << m_h[j]; os << endl;
  os << setw(15) << m_y << endl << endl;

  // v
  for (i = 0; i < m_nInputNum + 1; i++) 
  {
    for (j = 0; j < m_nHiddenNum; j++) 
    {
      os << setw(15) << m_v[i][j];
    }
    os << endl;
  }
  os << endl;

  // w
  for (j = 0; j < m_nHiddenNum + 1; j++) os << setw(15) << m_w[j]; os << endl << endl;

  os.flags(flags);
}


/* * * * * * * * * * * * * * * * * */
void Perceptron_n_m_1
::load(istream & is)
{
  int i,j;

  // Dimensions
  clear();
  is >> m_nInputNum >> m_nHiddenNum;
  is >> i; m_bSquashY = i > 0;
  init(m_nInputNum, m_nHiddenNum, m_bSquashY);

  // x, h, y
  for (i = 0; i < m_nInputNum + 1; i++) is >> m_x[i];
  for (j = 0; j < m_nHiddenNum + 1; j++) is >> m_h[j];
  is >> m_y;

  // v
  for (i = 0; i < m_nInputNum + 1; i++) 
  {
    for (j = 0; j < m_nHiddenNum; j++) 
    {
      is >> m_v[i][j];
    }
  }

  // w
  for (j = 0; j < m_nHiddenNum + 1; j++) is >> m_w[j];
}



/* * * * * * * * * * * * * * * * * */
ostream & Perceptron_n_m_1
::write(ostream& os) const
{
  os << "# of input units: "    << m_nInputNum << "\n";
  os << "# of hidden units: "   << m_nHiddenNum << "\n";
  os << (m_bSquashY ? "Squashing" : "Not squashing") << " the output unit" << endl;
  return os;
}
