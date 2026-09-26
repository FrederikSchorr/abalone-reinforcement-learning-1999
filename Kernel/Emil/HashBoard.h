/***********************************
/
/   HashBoard.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef HASHBOARD_H
#define HASHBOARD_H

/*** include ************************/ 

#include "../FullBoard.h"
#include "../Move.h"

/*** definition *********************/ 
template <class Value> class HashTable;


/*** HashBoard **********************/ 
template <class Value> class HashBoard : public FullBoard
{
public:
  HashBoard(const HashTable<Value>& table);
  HashBoard(const HashTable<Value>& table, const FullBoard& full);

public:
  void calcHash();
  int hashValue() const { return m_hashValue; }
  int hashIndex() const { return m_hashIndex; }

  void moveRaw(const FullMove&);

private:
  void marbleRemove(Pos p);
  void marbleSet(Pos p, Marble marble);

private:
  unsigned int m_hashIndex;
  unsigned int m_hashValue;

  const HashTable<Value> & m_table;
};

typedef HashBoard<int> HashBoardInt;
typedef HashBoard<float> HashBoardF;

#endif