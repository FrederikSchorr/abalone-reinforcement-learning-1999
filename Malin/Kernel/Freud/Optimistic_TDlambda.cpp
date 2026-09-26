/***********************************
/
/   Optimistic_TDlambda.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 



/*** include ************************/ 
#include "Optimistic_TDLambda.h"
#include <iomanip.h>

#include <afxwin.h>

/*** definition *********************/ 



/*** Optimistic_TDlambda ************/ 

/* * * * * * * * * * * * * * * * * */
void Optimistic_TDlambda
::init(int nInputNum, int nHiddenNum, 
  bool bSquashY, bool bOnline,
  float lambda, float stepInput,  float stepHidden) 
{
  init(nInputNum, nHiddenNum, bSquashY);

  // Save constants
  m_lambda = lambda;
  m_stepInput = (stepInput == 0.0 ? 1.0 / nInputNum : stepInput);
  m_stepHidden = (stepHidden == 0.0 ? 1.0 / nHiddenNum : stepHidden);

  // Initialize counters
  m_lLearnStates = 0L;
  m_lLearnTrajec = 0L;

  // bOnline
  m_bOnline = bOnline;
}


/* * * * * * * * * * * * * * * * * */
void Optimistic_TDlambda
::init(int nInputNum, int nHiddenNum, bool bSquashY)
{
  Perceptron_n_m_1::init(nInputNum, nHiddenNum, bSquashY);

  // Allocate memory for eligibility matrix
  m_zv.assign(nInputNum + 1, FloatVector(nHiddenNum));
  m_zw.assign(nHiddenNum + 1);
}


/* * * * * * * * * * * * * * * * * */
void Optimistic_TDlambda
::clear()
{
  Perceptron_n_m_1::clear();
  
  m_zv.clear();
  m_zw.clear();

  m_lambda = 0.0;
  m_stepInput = 0.0;
  m_stepHidden = 0.0;

  m_yLast = 0.0;

  m_lLearnStates = 0L;
  m_lLearnTrajec = 0L;

  m_bOnline = true;

  m_vOff.clear();
  m_wOff.clear();
}


/* * * * * * * * * * * * * * * * * */
void Optimistic_TDlambda
::learnBegin(const FloatVector & x)
{
  // Start a new trajectory, reset all elig.
  resetElig();

  // ? Offline
  if (!m_bOnline) 
  {
    m_vOff = m_v;
    m_wOff = m_w;
  }
  
  // Evaluate input x(0) and store result J(x(0),r(0)) in m_yLast
  input(x);
  propagate();
  output(m_yLast);

  // Form gradient and store results in eligibilities
  // z(0) = grad(J(x(0),r(0)))
  updateElig();

  // Keep counters accurate
  m_lLearnStates++;
  m_lLearnTrajec++;
}

/* * * * * * * * * * * * * * * * * */
void Optimistic_TDlambda
::learnNext(const FloatVector & x)
{
  // Assume this is the kth call to learnNext (starting with 0)
  // The input vector (state) is x(k+1)         ... m_x
  // The net contains the weights r(k)          ... m_v, m_w
  // The eligibilities contain z(k)             ... m_zv, m_zw
  // m_yLast contains J(x(k),r(k))
  // The offline weights are denoted by r'(k)   ... m_vOff, m_wOff

  ASSERT(m_lLearnTrajec > 0);

  // ? Online
  if (m_bOnline)
  {
    // Compute J(x(k+1),r(k))
    input(x);
    propagate();
    float y;
    output(y);

    // r(k+1) = r(k) + stepSize * (J(x(k+1),r(k)) - J(x(k),r(k))) * z(k)
    adjustWeights(m_v, m_w, y - m_yLast);

    // Store J(x(k+1),r(k+1)) in m_yLast
    propagate();
    output(m_yLast);

    // z(k+1) = lambda * z(k) + grad(J(x(k+1),r(k+1))
    updateElig();
  }
  // Offline
  else 
  {
    // r(k+1) = r(k) = r

    // Compute J(x(k+1),r)
    input(x);
    propagate();
    float y;
    output(y);

    // r'(k+1) = r'(k) + stepSize * (J(x(k+1),r) - J(x(k),r)) * z(k)
    adjustWeights(m_vOff, m_wOff, y - m_yLast);

    // Store J(x(k+1),r) in m_yLast
    m_yLast = y;

    // z(k+1) = lambda * z(k) + grad(J(x(k+1),r)
    updateElig();
  }
  
  m_lLearnStates++;
}

/* * * * * * * * * * * * * * * * * */
void Optimistic_TDlambda
::learnEnd(const FloatVector & x, float yTarget)
{
  ASSERT(m_lLearnTrajec > 0);

  // ? online
  if (m_bOnline)
  {
    // r(k+1) = r(k) + stepSize * (J*(x(k+1)) - J(x(k),r(k))) * z(k)
    adjustWeights(m_v, m_w, yTarget - m_yLast);

    // J(x(k+1),r(k+1))
    input(x);
    propagate();
  }
  // Offline
  else
  {
    // r'(k+1) = r'(k) + stepSize * (J*(x(k+1)) - J(x(k),r)) * z(k)
    adjustWeights(m_vOff, m_wOff, yTarget - m_yLast);

    // Perform update
    m_v = m_vOff;
    m_w = m_wOff;

    // J(x(k+1),r)
    input(x);
    propagate();
  }
}




/* * * * * * * * * * * * * * * * * */
void Optimistic_TDlambda
::adjustWeights(FloatMatrix & v, FloatVector & w, float difference)
{
  int i,j;

  // r := r + stepSize * difference * eligibility
  // hidden layer
  for (j = 0; j < m_nHiddenNum + 1; j++)
  {
    w[j] += m_stepHidden * difference * m_zw[j];
  }

  // input layer
  for (j = 0; j < m_nHiddenNum; j++)
  {
    for (i = 0; i < m_nInputNum + 1; i++)
    {
      v[i][j] += m_stepInput * difference * m_zv[i][j];
    }
  }
}


/* * * * * * * * * * * * * * * * * */
 void Optimistic_TDlambda
::updateElig()
{
  int i,j;
  float tempY;
  float tempH;

  /* z := lambda * z + grad(J(x,r)) */
  /* Hidden layer */
  if (m_bSquashY) tempY = dSigma_invSigma(m_y) + 0.1;
  for (j = 0; j < m_nHiddenNum + 1; j++)
  {
    /* dgrad(J(x,r)) / dw[j] = h[j] */
    if (m_bSquashY)
      m_zw[j] = m_lambda * m_zw[j] + tempY * m_h[j];
    else
      m_zw[j] = m_lambda * m_zw[j] +         m_h[j];
  }

  /* Input layer */
  for (j = 0; j < m_nHiddenNum; j++)
  {
    if (m_bSquashY) 
      tempH = m_w[j] * tempY * (dSigma_invSigma(m_h[j]) + 0.1);
    else 
      tempH = m_w[j] *         dSigma_invSigma(m_h[j]);

    for (i = 0; i < m_nInputNum + 1; i++)
    {
      /* dgrad(J(x,r)) / dv[i][j] = w[j] * sigma'(sigma^-1(h[j])) * x[i] */
      m_zv[i][j] = m_lambda * m_zv[i][j] + tempH * m_x[i];
    }
  }
}



/* * * * * * * * * * * * * * * * * */
void Optimistic_TDlambda
::resetElig()
{
  int i,j;

  // Hidden
  for (j = 0; j < m_nHiddenNum + 1; j++)
  {
    m_zw[j] = 0.0;
  }

  // Input
  for (j = 0; j < m_nHiddenNum; j++)
  {
    for (i = 0; i < m_nInputNum + 1; i++)
    {
      m_zv[i][j] = 0.0;
    }
  }
}




/* * * * * * * * * * * * * * * * * */
void Optimistic_TDlambda
::save(ostream &os) const
{
  Perceptron_n_m_1::save(os);

  // lambda, stepsizes
  os << m_bOnline << " " << m_lambda << " " << m_stepInput << " " << m_stepHidden << endl;

  // yLast
  os << setw(15) << m_yLast << endl;

  // lLearn...
  os << m_lLearnStates << " " << m_lLearnTrajec << endl << endl;
}


/* * * * * * * * * * * * * * * * * */
void Optimistic_TDlambda
::load(istream &is)
{
  Perceptron_n_m_1::load(is);

  // lambda, stepsizes
  int i;
  is >> i; m_bOnline = (i > 0);
  is >> m_lambda >> m_stepInput >> m_stepHidden ;

  // yLast
  is >> m_yLast ;

  // lLearn...
  is >> m_lLearnStates >> m_lLearnTrajec ;
}


/* * * * * * * * * * * * * * * * * */
ostream & Optimistic_TDlambda
::write(ostream &os) const
{
  Perceptron_n_m_1::write(os);
  
  os << (m_bOnline ? "Online" : "Offline") << " algorithm\n";
  os << "Lambda(decay factor): "  << m_lambda << "\n";
  os << "Input-units stepsize: "  << m_stepInput;
  os << ", hidden-units: "        << m_stepHidden << "\n";
  os << "Learned trajectories: "  << m_lLearnTrajec;
  os << ", learned states: "      << m_lLearnStates << endl;

  return os;
}
