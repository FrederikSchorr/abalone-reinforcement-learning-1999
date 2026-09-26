/***********************************
/
/   FullBoard.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 

#include "FullBoard.h"
#include <afxwin.h>


/*** FullBoard *********************/ 


/* * * * * * * * * * * * * * * * * */
void FullBoard
::allMoves(MoveVector& moves) const
{
  ASSERT(m_marble[0] == MARBLE_BORDER);

  Pos pLoop, p;
  FullMove move;
  Dir dir, dir2;
  char nOwn, nEnemy;

  // Clear move vector
  moves.reserve(movesMax);
  moves.resize(0);

  // Loop through all positions
  Marble playerEnemy = 1 - m_playerAct;
  for (pLoop = 1; pLoop <= POS_NUM; pLoop++)
  {
    // Only treat our marbles
    if (m_marble[pLoop] != m_playerAct) continue;

    // Loop through marble directions
    for (dir = 0; dir < DIR_NUM; dir++)
    {
      // Count my marbles
      move.m_pos[0] = pLoop;
      p = Kernel::next[pLoop][dir];
      nOwn = 1;
      for(;;)
      {
        if (m_marble[p] == m_playerAct) 
        {  
          move.m_pos[nOwn] = p;
          nOwn++;
          p = Kernel::next[p][dir];
          if (nOwn >= 3) break;
        }
        else break;
      }
      // Count enemy marbles
      nEnemy = 0;
      for(;;)
      {
        if (nEnemy >= nOwn - 1) break;
        else if (m_marble[p] == playerEnemy) 
        {
          move.m_posEnemy[nEnemy] = p;
          nEnemy++;
          p = Kernel::next[p][dir];
        }
        else break;
      }

      // Store move
      if (m_marble[p] == MARBLE_EMPTY) 
      {
        move.m_dir = move.m_marbleDir = dir;
        move.m_count = nOwn;
        move.m_countEnemy = nEnemy;
        move.m_bEject = false; 
        moves.push_back(move);
      }
      else if (p == 0 && nEnemy > 0)
      {
        move.m_dir = move.m_marbleDir = dir;
        move.m_count = nOwn;
        move.m_countEnemy = nEnemy;
        move.m_bEject = true; 
        moves.push_back(move);
      }
 
      // Broadside moves, only treat directions 1, 2, 3 => descending positions
      if (nOwn <= 1) continue;
      switch (dir) 
      {
      case 4:
      case 5:
      case 0:
        continue;
      }

      // Loop through move direction
      move.m_marbleDir = dir;
      move.m_countEnemy = 0;
      move.m_bEject = false; 

      for (dir2 = 0; dir2 < DIR_NUM; dir2++)
      {
        // Ignore line directions
        if (((dir - dir2) % 3) == 0) continue;

        if (m_marble[Kernel::next[move.m_pos[0]][dir2]] == MARBLE_EMPTY) 
        {
          if (m_marble[Kernel::next[move.m_pos[1]][dir2]] == MARBLE_EMPTY) 
          {

            move.m_dir = dir2;
            move.m_count = 2;
            moves.push_back(move);

            if (nOwn == 3 && m_marble[Kernel::next[move.m_pos[2]][dir2]] == MARBLE_EMPTY) 
            {
              move.m_count = 3;
              moves.push_back(move);
            }
          }
        }
      }
    }
  }
  ASSERT(moves.size() < movesMax);
}



/* * * * * * * * * * * * * * * * * */
void FullBoard
::ejectMoves(MoveVector& moves) const
{
  Pos pBorder, pAttack, p;
  FullMove move;
  Dir dir;
  int n, n2, i;
  char nOwn, nEnemy;

  // Clear moves vector
  moves.reserve(movesEjectMax);
  moves.resize(0);

  // Loop through all border positions
  Marble playerEnemy = 1 - m_playerAct;
  for (n = 0; ; n++)
  {
    pBorder = Kernel::attack[n][0];
    if (pBorder == 0) break;

    // ? Enemy marble
    if (m_marble[pBorder] != playerEnemy) continue;

    // Loop through directions 
    for (n2 = 1; n2 < 4; n2++)
    {
      dir = Kernel::attack[n][n2];
      if (dir == DIR_NUM) break;

      // Count enemy marbles
      p = Kernel::next[pBorder][dir];
      nEnemy = 1;
      if (m_marble[p] == playerEnemy) 
      {  
        nEnemy++;
        p = Kernel::next[p][dir];
      }
      // Count my own marbles
      nOwn = 0;
      for(;;)
      {
        if (m_marble[p] == m_playerAct) 
        {
          nOwn++;
          pAttack = p;
          p = Kernel::next[p][dir];
          if (nOwn >= 3) break;
        }
        else break;
      }

      // Store moves
      if (nOwn > nEnemy)
      {
        dir = (dir + 3) % DIR_NUM;
        p = pAttack;

        move.m_count = nOwn;
        move.m_countEnemy = nEnemy;
        move.m_marbleDir = dir;
        move.m_dir = dir;
        move.m_bEject = true;
        for (i = 0; i < nOwn; i++) 
        {
          ASSERT(p != 0);
          move.m_pos[i] = p;
          p = Kernel::next[p][dir];
        }
        for (i = 0; i < nEnemy; i++) 
        {
          ASSERT(p != 0);
          move.m_posEnemy[i] = p;
          p = Kernel::next[p][dir];
        }

        moves.push_back(move);

        // 3 :: 1
        if (nOwn - 1 > nEnemy)
        {
          p = Kernel::next[pAttack][dir];
        
          move.m_count = nOwn - 1;
          for (i = 0; i < nOwn - 1; i++) 
          {
            ASSERT(p != 0);
            move.m_pos[i] = p;
            p = Kernel::next[p][dir];
          }
        
          moves.push_back(move);
        }
      }
    }
  }

/*#ifdef _DEBUG
  // Get all moves
  MoveVector movesAll;
  allMoves(movesAll);

  int nEject = 0;
  for (int o = 0; o < movesAll.size(); o++)
  {
    if (movesAll[o].m_bEject)
    {
      nEject++;
      for (int e = 0; e < moves.size(); e++)
      {
        if (movesAll[o].equals(moves[e])) 
          break;
      }
      ASSERT(e < moves.size());
    }
  }
  ASSERT(nEject == moves.size());
#endif*/

  ASSERT(moves.size() < movesEjectMax);
}


/* * * * * * * * * * * * * * * * * */
void FullBoard
::moveRaw(const FullMove& full)
{
  ASSERT(m_playerNum == 2);

  // What kind of a move ?
  if (full.m_count == 1 || full.m_dir != full.m_marbleDir)
  {
    // Broadside move
    Count n;
    for (n = 0; n < full.m_count; n++)
    {
      m_marble[full.m_pos[n]] = MARBLE_EMPTY;
      m_marble[Kernel::next[full.m_pos[n]][full.m_dir]] = m_playerAct;
    }
  }
  // Line move
  else
  {
    if (full.m_countEnemy > 0)
    {
      m_marble[Kernel::next[full.m_posEnemy[full.m_countEnemy - 1]][full.m_dir]] = 
        1 - m_playerAct;
    }
    m_marble[Kernel::next[full.m_pos[full.m_count - 1]][full.m_dir]] = m_playerAct;
    m_marble[full.m_pos[0]] = MARBLE_EMPTY;

    // Eject
    if (full.m_bEject)
    {
      m_kicked[m_playerAct]++;
      m_lost[m_marble[0]]++;
      m_marble[0] = MARBLE_BORDER;
    }
  }

  // Next player
  m_playerAct = 1 - m_playerAct;
}



/* * * * * * * * * * * * * * * * * */
ostream & FullBoard
::write(ostream &os) const
{
  int n, i=0;

  os << "\n          1 2 3 4 5";
  os << "\n         / / / / / 6";
  os << "\n     a- ";
  for (n=1; n < 6; n++) 
    if (m_marble[n] == MARBLE_EMPTY) os << ". ";
    else os << (int)m_marble[n] << " ";
  os << "/ 7 ";
  if (i < m_playerNum) 
    os << "        " << "Player " << i << ": " << (int)m_kicked[i] <<
      " (lost " << (int)m_lost[i] << ")";
    
  os << "\n    b- ";
  for (; n < 12; n++) 
    if (m_marble[n] == MARBLE_EMPTY) os << ". ";
    else os << (int)m_marble[n] << " ";
  os << "/ 8 ";
  if (++i < m_playerNum) 
    os << "       " << "Player " << i << ": " << (int)m_kicked[i] <<
      " (lost " << (int)m_lost[i] << ")";
    
  os << "\n   c- ";
  for (; n < 19; n++) 
    if (m_marble[n] == MARBLE_EMPTY) os << ". ";
    else os << (int)m_marble[n] << " ";
  os << "/ 9 ";
  if (++i < m_playerNum) 
    os << "      " << "Player " << i << ": " << (int)m_kicked[i] <<
      " (lost " << (int)m_lost[i] << ")";
    
  os << "\n  d- ";
  for (; n < 27; n++) 
    if (m_marble[n] == MARBLE_EMPTY) os << ". ";
    else os << (int)m_marble[n] << " ";
  os << "/   ";
  if (++i < m_playerNum) 
    os << "     " << "Player " << i << ": " << (int)m_kicked[i] <<
      " (lost " << (int)m_lost[i] << ")";
    
  os << "\n e- ";
  for (; n < 36; n++) 
    if (m_marble[n] == MARBLE_EMPTY) os << ". ";
    else os << (int)m_marble[n] << " ";
  if (++i < m_playerNum) 
    os << "        " << "Player " << i << ": " << (int)m_kicked[i] <<
      " (lost " << (int)m_lost[i] << ")";
  
  os << "\n  f- ";
  for (; n < 44; n++) 
    if (m_marble[n] == MARBLE_EMPTY) os << ". ";
    else os << (int)m_marble[n] << " ";
  if (++i < m_playerNum) 
    os << "       " << "Player " << i << ": " << (int)m_kicked[i] <<
      " (lost " << (int)m_lost[i] << ")";
    
  os << "\n   g- ";
  for (; n < 51; n++) 
    if (m_marble[n] == MARBLE_EMPTY) os << ". ";
    else os << (int)m_marble[n] << " ";
    
  os << "\n    h- ";
  for (; n < 57; n++) 
    if (m_marble[n] == MARBLE_EMPTY) os << ". ";
    else os << (int)m_marble[n] << " ";
    
  os << "\n     i- ";
  for (; n < 62; n++) 
    if (m_marble[n] == MARBLE_EMPTY) os << ". ";
    else os << (int)m_marble[n] << " ";
  
  os << endl;

  return os;
}