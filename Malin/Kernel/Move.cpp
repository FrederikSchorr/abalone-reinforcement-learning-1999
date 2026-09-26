/***********************************
/
/   Move.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 




/*** includes **********************/

#include "Move.h"

/*** globals ***********************/
const char g_szPos[POS_NUM + 1][3] = 
{
  "  ",
  "a1", "a2", "a3", "a4", "a5",
  "b1", "b2", "b3", "b4", "b5", "b6",
  "c1", "c2", "c3", "c4", "c5", "c6", "c7",
  "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", 
  "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9",
  "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9",
  "g3", "g4", "g5", "g6", "g7", "g8", "g9",
  "h4", "h5", "h6", "h7", "h8", "h9",
  "i5", "i6", "i7", "i8", "i9"
};


/* * * * * * * * * * * * * * * * * */
void Move
::set(const std::string& s)
{
  /* Get first marble */
  Count n = 0;
  Pos p1 = pos(s.substr(n, 2)); n += 2;
  
  /* a1-... or a1,... */
  char c = s.at(n); n ++;
  Pos p2, pNext, pFar;
  if (c == '-')
  {
    /* More then one marble */
    p2 = pos(s.substr(n, 2)); n += 2;
    
    /* Search in all directions  */
    for (Dir dir = 0; dir < DIR_NUM; dir++)
    {
      pNext = Kernel::next[p1][dir];
      if (pNext == p2)
      {
        m_pos[0] = p1;
        m_pos[1] = p2;
        m_count = 2;
        break;
      }
      pFar = Kernel::next[pNext][dir];
      if (pFar == p2)
      {
        m_pos[0] = p1;
        m_pos[1] = pNext;
        m_pos[2] = p2;
        m_count = 3;
        break;
      }
    }
    /* ? no direction found */
    if (dir == DIR_NUM) throw MoveE("Marbles not adjacent");

    c = s.at(n); n ++;
  }
  else 
  {
    /* One marble */
     m_pos[0] = p1;
     m_count = 1;
  }

  /* Komma */
  if (c != ',') throw MoveE(std::string("Illegal character: '") + c + "'");

  /* Direction */
  Pos pDir = pos(s.substr(n, 2)); n += 2;
  
  /* Relative to first marble */
  for (Dir dir = 0; dir < DIR_NUM; dir++)
  {
    if (Kernel::next[p1][dir] == pDir)
    {
      m_dir = dir;
      return;
    }
  }
  
  /* Relative to last marble */
  if (m_count == 1) throw MoveE("No direction found");
  for (dir = 0; dir < DIR_NUM; dir++)
  {
    if (Kernel::next[p2][dir] == pDir)
    {
      m_dir = dir;
      return;
    }
  }
  
  /* No direction found */
  throw MoveE("No direction found");
}


/* * * * * * * * * * * * * * * * * */
Pos Move
::pos(const std::string& s) 
{
  for (Pos p = 0; p <= POS_NUM; p++)
  {
    if (s == g_szPos[p]) return p;
  }
  throw MoveE("Illegal position: " + s);
}


/* * * * * * * * * * * * * * * * * */
std::string Move
::sPos(Pos p)
{
  return g_szPos[p];
}


/* * * * * * * * * * * * * * * * * */
ostream &operator<<(ostream &os, const Move move) 
{
  os << g_szPos[move.m_pos[0]];

  if (move.m_count > 1) 
  {
    os << "-" << g_szPos[move.m_pos[move.m_count - 1]];
  }

  os << " , " << g_szPos[Kernel::next[move.m_pos[0]][move.m_dir]];

  return os;
}


/* * * * * * * * * * * * * * * * * */
std::string Move
::string() const
{
  std::string s;

  s = g_szPos[m_pos[0]];

  if (m_count > 1) 
  {
    s += "-";
    s += g_szPos[m_pos[m_count - 1]];
  }

  s += " , ";
  s += g_szPos[Kernel::next[m_pos[0]][m_dir]];

  return s;
}


/* * * * * * * * * * * * * * * * * */
bool FullMove
::equals(const FullMove &full) const
{
  int n;
#ifdef _DEBUG
  if (full.m_dir != full.m_marbleDir)
  {
    for (n = 1; n < full.m_count; n++)
    { 
      if (full.m_pos[n-1] < full.m_pos[n]) 
       throw MoveE("Fullmove not correctly sorted");
    }

    for (n = 0; n < full.m_count; n++) 
      if (full.m_pos[n] == 0)
        throw MoveE("Fullmove contains illegal positions");

    for (n = 0; n < full.m_countEnemy; n++) 
      if (full.m_posEnemy[n] == 0)
        throw MoveE("Fullmove contains illegal positions");

  }
#endif

  if (m_count     != full.m_count ||
    m_dir         != full.m_dir ||
    m_countEnemy  != full.m_countEnemy ||
    m_bEject      != full.m_bEject) return false;

  if (m_count > 1 && (m_marbleDir != full.m_marbleDir)) return false;
  
  for (n = 0; n < m_count; n++)
  {
    if (m_pos[n] != full.m_pos[n]) return false;
  }
  for (n = 0; n < m_countEnemy; n++)
  {
    if (m_posEnemy[n] != full.m_posEnemy[n]) return false;
  }

  return true;
}