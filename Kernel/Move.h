/***********************************
/
/   Move.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 

#ifndef MOVE_H
#define MOVE_H

#include "Kernel.h"

/*** Move **************************/ 
class Move 
{
public:
  void set(const std::string&);
  std::string string() const;

public:
  static Pos pos(const std::string&);
  static std::string sPos(Pos p) ;
  
public:
  Pos m_pos[SEL_MAX];
  Count m_count;
  Dir m_dir;
  
  friend ostream &operator<<(ostream &os, const Move move) ;
};


/*** FullMove **********************/ 
class FullMove : public Move
{
public:
  bool equals (const FullMove& full) const;

public:
  Pos m_posEnemy[PUSH_ENEMY_MAX];
  Count m_countEnemy;
  Dir m_marbleDir;
  bool m_bEject;
};


/*** exception *********************/ 
class MoveE : public exception
{
public: MoveE(const std::string& s) : 
  exception(s.c_str()) { };
};

class NoMoveFoundE : public MoveE
{
public: NoMoveFoundE(const std::string& s) : 
  MoveE(s.c_str()) { };
};

class MoveRepeatedE : public MoveE
{
public: MoveRepeatedE(const std::string& s) : 
  MoveE(s.c_str()) { };
};




/*** MoveVector ********************/ 
typedef std::vector<FullMove> MoveVector;

/*class MoveVector
{
public:
  MoveVector() { nLast = 0; }

public:
  FullMove & operator[](int nIndex) { return m_vec[nLast]; }
  void push_back(const FullMove & move) 
  { 
    if (nLast >= Size) throw MoveE("MoveVector too small");
    m_vec[nLast++] = move; 
  }
  FullMove & back() { return m_vec[nLast-1]; }
  void clear() { nLast = 0; }
  int size() { return nLast; }

public:
  enum { Size = 90 };

private:
  int nLast;
  FullMove m_vec[Size];
};
*/

#endif