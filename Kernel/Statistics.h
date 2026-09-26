/***********************************
/
/   Statistics.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   01/99
/   Malin
/  
/***********************************/ 


/*** includes **********************/
#include <iostream.h>
#include <afxwin.h>


/*** definition ********************/
#define STATISTICS

#ifdef STATISTICS
  #define STAT(expr) expr
#else
  #define STAT(expr)
#endif



/*** Statistics ********************/
class Statistics
{
public:
  // Reset all stats
  Statistics() { clear(); }

public:
  // Typedefinitions
  enum Stat {evalBoardTotal = 0, evalBoardFinal, lookaheadDepth,
    lookaheadDepthQ, branchFactor, branchFactorQ, hashFound, 
    numberOfMoves, statLast};

public:
  // Reset all stats
  void clear ();
  // Reset one statistc and its counter
  void clear (Statistics::Stat);
  
  // Write output
  ostream & write (ostream &) const;

  // Increment a statistic
  void add (Statistics::Stat);
  // Add lInc to statistic
  void add (Statistics::Stat, long lInc);
  // Increment counter for a statistic
  void count (Statistics::Stat);
  // All lInc to statistic and increment its counter
  void addCount (Statistics::Stat, long lInc);
  // Add a Statistic object
  void addCount (const Statistics &);


private:
  // The statistics
  long m_lStat[statLast];
  // and their counters
  long m_lCount[statLast];
};


inline ostream &operator<<(ostream &os, const Statistics & stat) { return stat.write(os); }



/* * * * * * * * * * * * * * * * * */
inline void Statistics
::add(Statistics::Stat stat)
{
  ASSERT(stat < statLast);

  m_lStat[stat]++;
}

/* * * * * * * * * * * * * * * * * */
inline void Statistics
::add(Statistics::Stat stat, long lInc)
{
  ASSERT(stat < statLast);

  m_lStat[stat] += lInc;
}


/* * * * * * * * * * * * * * * * * */
inline void Statistics
::count(Statistics::Stat stat)
{
  ASSERT(stat < statLast);

  m_lCount[stat]++;
}

/* * * * * * * * * * * * * * * * * */
inline void Statistics
::addCount(Statistics::Stat stat, long lInc)
{
  ASSERT(stat < statLast);

  m_lStat[stat] += lInc;
  m_lCount[stat]++;
}

/* * * * * * * * * * * * * * * * * */
inline void Statistics
::clear(Statistics::Stat stat)
{
  m_lStat[stat] = 0;
  m_lCount[stat] = 0;
}