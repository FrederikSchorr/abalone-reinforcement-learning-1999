/***********************************
/
/   Hash.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 



/*** include ***********************/ 
#pragma warning( disable : 4786 )

#include "Hash.h"
#include "HashBoard.h"
#include <afxwin.h>


/*** Instantiation *****************/ 

#pragma warning( disable : 4660 )
template HashTableInt;
template HashTableF;
#pragma warning( default : 4660 )


/* * * * * * * * * * * * * * * * * */
template<class Value> HashTable<Value>
::HashTable(int nTableSize, int nTableNum, int nHashValueMax)
{
  m_nTableSize = nTableSize;
  m_nHashValueMax = nHashValueMax;
  m_nTableNum = nTableNum;
  
  init();
  //clear();
}

/* * * * * * * * * * * * * * * * * */
template<class Value> void HashTable<Value>
::clear()
{
  // Mark all entries as empty
  HashElement elEmpty;
  elEmpty.m_hashValue   = HashElement::empty;
  elEmpty.m_value       = 0;
  elEmpty.m_nCountDown  = 0;
  elEmpty.m_quality     = exact;

  for (int i = 0; i < m_nTableSize; i++)
  {
    for (int e = 0; e < m_nTableNum; e++)
    {
      m_table[i][e] = elEmpty;
    }
  }
}


/* * * * * * * * * * * * * * * * * */
template<class Value> HashTable<Value>
::~HashTable()
{
  //LOG(*this);
}

/* * * * * * * * * * * * * * * * * */
template<class Value> void HashTable<Value>
::store(const HashBoard<Value> &board, char nCountDown, Value value, Quality quality)
{
  ASSERT(board.hashIndex() < m_nTableSize);
  ASSERT(quality == exact || quality == lesser || quality == greater);
  
  ElementVector & vector = m_table[board.hashIndex()];

  for (short i = 0; i < m_nTableNum; i++)
  {
    HashElement & hash = vector[i];

    // ? no more entries
    if (hash.m_hashValue == HashElement::empty) break;
    // ? existing entry
    else if(hash.m_hashValue == board.hashValue())
    {
      if (nCountDown >= hash.m_nCountDown)
      {
        // ? old value better than new one
        if (nCountDown == hash.m_nCountDown)
        {
          if (quality == greater && hash.m_quality == greater && 
            hash.m_value >= value) return;
          if (quality == lesser && hash.m_quality == lesser && 
            hash.m_value <= value) return;
        }
        hash.m_value = value;
        hash.m_quality = quality;
        hash.m_nCountDown = nCountDown;
      }
      // Stored value is the better one, do not change it
      return;
    }
    /*else if(hash.m_hashValue == board.hashValue() &&
      hash.m_nCountDown == nCountDown)
    {
      if (quality == greater && hash.m_quality == greater && 
        hash.m_value >= value) return;
      else if (quality == lesser && hash.m_quality == lesser && 
        hash.m_value <= value) return;
      //else
      hash.m_value = value;
      hash.m_quality = quality;
      return;
    }*/
  }

  // Board not found in hashtable
  if (i >= m_nTableNum) 
  {
    //TRACE("HashTable::store replace entry at %d\n", board.hashIndex());
    i = rand() % m_nTableNum;
    if (vector[i].m_nCountDown > 0) i = rand() % m_nTableNum;
  }
  
  // Store new entry
  HashElement &hash = vector[i];
  hash.m_hashValue = board.hashValue();
  hash.m_value = value;
  hash.m_nCountDown = nCountDown;
  hash.m_quality = quality;
}



/* * * * * * * * * * * * * * * * * */
template<class Value> bool HashTable<Value>
::bFind(const HashBoard<Value> &board, char nCountDown, Value & value, Quality & quality) const
{
  ASSERT(board.hashIndex() < m_nTableSize);

  for (int i = 0; i < m_nTableNum; i++)
  {
    const HashElement &hash = m_table[board.hashIndex()][i];

    // ? no more entries
    if (hash.m_hashValue == HashElement::empty) break;
    // ? existing entry
    else if(hash.m_hashValue == board.hashValue())
    {
      if (hash.m_nCountDown >= nCountDown)
      //if (hash.m_nCountDown == nCountDown)
      {
        // store return values
        value = hash.m_value;
        quality = hash.m_quality;
        ASSERT(quality == exact || quality == lesser || quality == greater);
        return true;
      }
      else return false;
    }
  }

  // not found
  return false;
}


/* * * * * * * * * * * * * * * * * */
template<class Value> ostream& HashTable<Value>
::write(ostream &os) const
{
  for (int i = 0; i < m_nTableSize; i++)
  {
    os << i << ": ";
    for (int e = 0; e < m_nTableNum; e++)
    {
      if (m_table[i][e].m_hashValue == HashElement::empty) break;
      os << "(" << (int) m_table[i][e].m_nCountDown << "," << 
        m_table[i][e].m_quality << ")  ";
    }
    os << "\n";
  }

  return os;
}


/* * * * * * * * * * * * * * * * * */
template<class Value> void HashTable<Value>
::init()
{

  m_table.resize(m_nTableSize, ElementVector(m_nTableNum));

  m_hashI[0] = 1;
  m_hashV[0] = 1;

  for (Pos p = 1; p <= POS_NUM; p++)
  {
    m_hashI[p] = (m_hashI[p - 1] * 3) % m_nTableSize;
    m_hashV[p] = (m_hashV[p - 1] * 5) % m_nHashValueMax;
  }
}
