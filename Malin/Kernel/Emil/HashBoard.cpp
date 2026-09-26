/***********************************
/
/   HashBoard.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 



/*** include ***********************/ 
#include "HashBoard.h"
#include "Hash.h"

#include <afxwin.h>

/*** Definitions *******************/ 

template HashBoardInt;
template HashBoardF;


/*** HashBoard *********************/ 

/* * * * * * * * * * * * * * * * * */
template <class Value> HashBoard<Value>
::HashBoard(const HashTable<Value> & table)
:FullBoard(), m_table(table)
{
  m_hashIndex = 0;
  m_hashValue = 0;
}

/* * * * * * * * * * * * * * * * * */
template <class Value> HashBoard<Value>
::HashBoard(const HashTable<Value> & table, const FullBoard & full)
:FullBoard(full), m_table(table)
{
  calcHash();
}



/* * * * * * * * * * * * * * * * * */
template<class Value> void HashBoard<Value>
::moveRaw(const FullMove& full)
{
  // Be sure to stay > 0
  m_hashIndex += m_table.m_nTableSize << 3;
  m_hashValue += m_table.m_nHashValueMax << 3;

  // What kind of a move ?
  if (full.m_count == 1 || full.m_dir != full.m_marbleDir)
  {
    // Broadside move
    Count n;
    for (n = 0; n < full.m_count; n++)
    {
      marbleRemove(full.m_pos[n]);
      marbleSet(Kernel::next[full.m_pos[n]][full.m_dir], m_playerAct);
    }
  }
  // Line move
  else
  {
    if (full.m_countEnemy > 0)
    {
      marbleRemove(full.m_posEnemy[0]);
      marbleSet(Kernel::next[full.m_posEnemy[full.m_countEnemy - 1]] [full.m_dir],
        1 - m_playerAct);
    }

    marbleRemove(full.m_pos[0]);
    marbleSet(Kernel::next[full.m_pos[full.m_count - 1]] [full.m_dir], m_playerAct);

    // Eject
    if (full.m_bEject)
    {
      ASSERT(Kernel::next[full.m_posEnemy[full.m_countEnemy - 1]] [full.m_dir] == 0);
      
      m_kicked[m_playerAct]++;
      m_lost[m_marble[0]]++;
      
      marbleRemove(0);
      m_marble[0] = MARBLE_BORDER;
    }
  }

 // Next player
  m_hashIndex -= m_playerAct;
  m_hashValue -= m_playerAct;

  m_playerAct = 1 - m_playerAct;

  m_hashIndex += m_playerAct;
  m_hashValue += m_playerAct;

  // Normalize hash index and value
  m_hashIndex = m_hashIndex % m_table.m_nTableSize;
  m_hashValue = m_hashValue % m_table.m_nHashValueMax;

#ifdef _DEBUG 
  {
    unsigned int hashIndex = m_hashIndex;
    unsigned int hashValue = m_hashValue;
    calcHash();
    ASSERT(hashIndex == m_hashIndex);
    ASSERT(hashValue == m_hashValue);
  }
#endif
}



/* * * * * * * * * * * * * * * * * */
template<class Value> void HashBoard<Value>
::marbleRemove(Pos p)
{
  ASSERT(m_marble[p] >= 0);

  m_hashIndex -= (m_marble[p] + 1) * m_table.m_hashI[p];
  m_hashValue -= (m_marble[p] + 1) * m_table.m_hashV[p];

  m_marble[p] = MARBLE_EMPTY;
}

/* * * * * * * * * * * * * * * * * */
template<class Value> void HashBoard<Value>
::marbleSet(Pos p, Marble marble)
{
  m_hashIndex += (marble + 1) * m_table.m_hashI[p];
  m_hashValue += (marble + 1) * m_table.m_hashV[p];

  ASSERT(m_marble[p] < 0);
  m_marble[p] = marble;
}



/* * * * * * * * * * * * * * * * * */
template<class Value> void HashBoard<Value>
::calcHash()
{
  m_hashIndex = m_playerAct;
  m_hashValue = m_playerAct;

  for (Pos p = 1; p <= POS_NUM; p++)
  {
    m_hashIndex += (m_marble[p] + 1) * m_table.m_hashI[p];  
    m_hashValue += (m_marble[p] + 1) * m_table.m_hashV[p];  
  }

  m_hashIndex = m_hashIndex % m_table.m_nTableSize;
  m_hashValue = m_hashValue % m_table.m_nHashValueMax; 
}