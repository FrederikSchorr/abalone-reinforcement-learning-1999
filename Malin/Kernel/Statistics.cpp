/***********************************
/
/   Statistics.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   01/99
/   Malin
/  
/***********************************/ 


/*** includes **********************/
#include "Statistics.h"
#include <math.h>

/*** definition ********************/
#define AVERAGE_NUMBER_MOVES 58


/*** Statistics ********************/

/* * * * * * * * * * * * * * * * * */
void Statistics
::clear()
{
  // Reset all statistics
  for (int i = 0; i < statLast; i++) 
  {
    m_lStat[i] = 0;
    m_lCount[i] = 0;
  }
}


/* * * * * * * * * * * * * * * * * */
ostream & Statistics
::write(ostream & os) const
{
#ifdef STATISTICS
  char sz[128];

  // evalBoardTotal
  if (m_lCount[evalBoardTotal] > 0)
  {
    float fEvalBoardTotal = (float)m_lStat[evalBoardTotal] / m_lCount[evalBoardTotal];
    float fDepth = (float)m_lStat[lookaheadDepth] / m_lCount[lookaheadDepth];
    // Branching factor R
    float R = (float)pow(fEvalBoardTotal, 1.0/fDepth);
    // Complexity
    float c = (float)(log(R) / log(AVERAGE_NUMBER_MOVES));  

    sprintf(sz, "E %6.0fa(%6.0ff),%4.1fR = %2.0f^%5.3f  ", 
      fEvalBoardTotal,
      (float)m_lStat[evalBoardFinal] / m_lCount[evalBoardFinal],
      R, (float)AVERAGE_NUMBER_MOVES, c);
    os << sz;
  }

  // lookaheadDepth
  if (m_lCount[lookaheadDepth] > 0)
  {
    sprintf(sz, "D %1.2f(%.2fq)  ", 
      (float)m_lStat[lookaheadDepth] / m_lCount[lookaheadDepth],
      (float)m_lStat[lookaheadDepthQ] / m_lCount[lookaheadDepthQ]);
    os << sz;
  } 

  // branchFactor
  if (m_lCount[branchFactor] > 0)
  {
    sprintf(sz, "B %4.1f(%4.1fq)  ", 
      (float)m_lStat[branchFactor] / m_lCount[branchFactor],
      (float)m_lStat[branchFactorQ] / m_lCount[branchFactorQ]);
    os << sz;
  } 

  // hashFound
  if (m_lCount[hashFound] > 0)
  {
    sprintf(sz, "H %2.0f%%  ", 
      100.0 * (float)m_lStat[hashFound] / m_lCount[hashFound]);
    os << sz;
  } 

  // numberOfMoves
  if (m_lCount[numberOfMoves] > 0)
  {
    sprintf(sz, "M %2.0f  ", 
      (float)m_lStat[numberOfMoves] / m_lCount[numberOfMoves]);
    os << sz;
  } 

#else // STATISTICS
  os << "No statistics available";
#endif
  return os;
}


/* * * * * * * * * * * * * * * * * */
void Statistics
::addCount(const Statistics & stat)
{
  // Add all statistics
  for (int i = 0; i < statLast; i++) 
  {
    m_lStat[i] += stat.m_lStat[i];
    m_lCount[i] += stat.m_lCount[i];
  }
}