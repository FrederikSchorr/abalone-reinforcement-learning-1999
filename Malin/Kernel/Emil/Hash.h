/***********************************
/
/   Hash.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef HASH_H
#define HASH_H

/*** include ************************/
#pragma warning( disable : 4786 )

#include "../Kernel.h" 
#include <list>
#include <vector>
#include <iostream.h>

/*** definition *********************/ 
template<class Value> class HashBoard;


/*** HashTable **********************/ 
template <class Value> class HashTable
{
public:
  // Constructor, other possible tablesizes 4423, 9941, 11213, 21701
  HashTable(int nTableSize = 21701, int nTableNum = 4, int nHashValueMax = 72117691);
  virtual ~HashTable();

public:
  // Indicates quality of stored value
  typedef enum {exact = 0, lesser, greater} Quality;
  
public:
  void store(const HashBoard<Value> &, char nCountDown, Value value, Quality quality);
  bool bFind(const HashBoard<Value> &, char nCountDown, Value & value, Quality & quality) const;

  void clear();

private:
  void init();

public:
  ostream& write(ostream& os) const;

private:
  // The hashtable entry class
  template<class Value> class HashElement_
  {
  public: 
    HashElement_()
    {
      m_hashValue = empty;
      m_value = 0;
      m_nCountDown = 0;
      m_quality = exact;
    }
  public:
    enum {empty = 0xffffffff};
  public:
    unsigned int m_hashValue;
    Value m_value;
    char m_nCountDown;
    Quality m_quality : 4;
  };
  typedef HashElement_<Value> HashElement;
  typedef std::vector<HashElement> ElementVector;
  typedef std::vector<ElementVector> ElementMatrix;

private:
  // The hash table
  ElementMatrix m_table; // [tableSize][tableNum]

  int m_nTableSize;
  int m_nHashValueMax;
  int m_nTableNum;

private:
  // Used to calculate the hashindex and hashvalue
  unsigned int m_hashI[POS_NUM+1];
  unsigned int m_hashV[POS_NUM+1];

friend class HashBoard<Value>;
};

typedef HashTable<int> HashTableInt;
typedef HashTable<float> HashTableF;

inline ostream& operator<<(ostream& os, HashTableInt& hash) { return hash.write(os); }
inline ostream& operator<<(ostream& os, HashTableF& hash)   { return hash.write(os); }

#endif