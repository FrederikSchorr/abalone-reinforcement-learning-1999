/***********************************
/
/   Killer.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 



/*** include ***********************/ 
#pragma warning (disable : 4786)

#include "Killer.h"
#include "../Simulation.h"
#include <algorithm>

#include <afxwin.h>

using namespace std;


/*** Definitions *******************/ 
const int KILLER_NUM = 6;
const int MOVEID_MAX = (1 << 12);



/*** Compare ***********************/ 
class Compare
{
public:
  Compare(const HitsVector& hits) : m_hits(hits) 
  {
#ifdef _DEBUG
    if (m_hits.size() != MOVEID_MAX) 
      throw SimulationE("Unexpected hits vector size");
#endif
  }
  
  bool operator() (const FullMove& m1, const FullMove& m2) const
  { 
    return m_hits[Killer::nMoveId(m1)] > m_hits[Killer::nMoveId(m2)];
  }

private:
  const HitsVector& m_hits;
};



/*** Killer ************************/ 


/* * * * * * * * * * * * * * * * * */
void Killer
::init()
{
  TRACE("Killer::init\n");

  // Reserve memory for a 12 ply lookahead
  m_vec.reserve(12);

  // Loop through all move tables
  for (int i = 0; i < m_vec.size(); i++)
  {
    // Set all counters to 0
    fill(m_vec[i].begin(), m_vec[i].end(), 0);
  }

  // Reset size
  m_vec.resize(0);
}





/* * * * * * * * * * * * * * * * * */
void Killer
::clear()
{
  TRACE("Killer::clear\n");
  
  // Clear the move table vector
  m_vec.clear();
}





/* * * * * * * * * * * * * * * * * */
void Killer
::normalize()
{
  TRACE("Killer::normalize\n");

  // Loop through tables
  for (int nPly = 0; nPly < m_vec.size(); nPly++)
  {
    HitsVector& hits = m_vec[nPly];

    // copy vector to map
    typedef greater<int> great;
    multimap<int, int, great> moveId;   // pair<hits, moveId>
    for(int n = 0; n < MOVEID_MAX; n++)
    {
      if (hits[n] > 0) moveId.insert(make_pair(hits[n], n));
    }

    // clear hits and restore only the best moves with values 6,5,...,1
    fill(hits.begin(), hits.end(), 0);
    multimap<int, int, great>::iterator it;
    for (it = moveId.begin(), n = 0; n < KILLER_NUM && it != moveId.end(); n++, it++)
    {
      hits[it->second] = KILLER_NUM - n;
    }
  }
}



/* * * * * * * * * * * * * * * * * */
void Killer
::sort(char nDepth, MoveVector &moves)
{
  // Select correct MoveHits map (or insert new one)
  HitsVector& hits = hitsVector(nDepth);

  // Do only sort as much as necessary
  MoveVector::iterator itStart = moves.begin();
  MoveVector::iterator itEnd = moves.end();
  MoveVector::iterator itMiddle = itStart + 
    __min(__min(hits.size(), 
    moves.size()),
    KILLER_NUM);

  Compare compare(hits);

  // Sort it!
  partial_sort(itStart, itMiddle, itEnd, compare);
}



/* * * * * * * * * * * * * * * * * */
int inline Killer
::nMoveId(const FullMove &move)
{
  ASSERT((*move.m_pos | (move.m_dir << 6) | (move.m_marbleDir << 9)) < MOVEID_MAX);

  return *move.m_pos |          // 62, 6 bits
    (move.m_dir << 6) |         // 6,  3 bits
    (move.m_marbleDir << 9);    // 6,  3 bits
    //(move.m_count << 9) |
}


/* * * * * * * * * * * * * * * * * */
::ostream& Killer
::write(::ostream &os) const
{
  os << "\n";

  // Loop through all tables
  for (int nPly = 0; nPly < m_vec.size(); nPly++)
  {
    const HitsVector& hits = m_vec[nPly];

    os << "Ply " << nPly << ": ";

    // Show priorities
    for (int n = 0; n < MOVEID_MAX; n++)
    {
      if (hits[n] > 0) os << n << "," << hits[n] << "  ";
    }
    os << "\n";
  }

  return os;
}


/* * * * * * * * * * * * * * * * * */
::ostream& operator<<(::ostream& os, Killer& killer)
{
  return killer.write(os);
}


/* * * * * * * * * * * * * * * * * */
void Killer
::add(char nDepth, const FullMove& move) 
{
  hitsVector(nDepth)[nMoveId(move)]++;
}


/* * * * * * * * * * * * * * * * * */
HitsVector & Killer
::hitsVector(char nDepth) 
{
  // ? table big enough
  if (nDepth >= m_vec.size())
  {
    // No, blow up table
    m_vec.push_back(HitsVector(MOVEID_MAX));
  }

  return m_vec[nDepth];
}
