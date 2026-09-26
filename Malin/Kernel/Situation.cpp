/***********************************
/
/   Situation.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 
 



/*** includes **********************/
#pragma warning (disable : 4786)

#include "Situation.h"
#include <afxwin.h>


/*** init. globals *****************/
Marble g_marbleBoard2[POS_NUM + 1] = 
  {          MARBLE_BORDER,
           0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,   
      -1, -1,  0,  0,  0, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1,  1,  1,  1, -1, -1,
         1,  1,  1,  1,  1,  1,
           1,  1,  1,  1,  1
  };

Marble g_marbleBoard3[POS_NUM + 1] = 
  {          MARBLE_BORDER,
           1,  1, -1,  2,  2,
         1,  1, -1, -1,  2,  2,   
       1,  1, -1, -1, -1,  2,  2,
     1,  1, -1, -1, -1, -1,  2,  2,
   1,  1, -1, -1, -1, -1, -1,  2,  2,
     1, -1, -1, -1, -1, -1, -1,  2,
      -1, -1, -1, -1, -1, -1, -1,
         0,  0,  0,  0,  0,  0,
           0,  0,  0,  0,  0
  };

Marble g_marbleBoard4[POS_NUM + 1] = 
  {          MARBLE_BORDER,
           2,  2,  2,  2, -1,
         2,  2,  2,  2, -1,  3,   
      -1, -1, -1, -1, -1,  3,  3,
     1, -1, -1, -1, -1, -1,  3,  3,
   1,  1, -1, -1, -1, -1, -1,  3,  3,
     1,  1, -1, -1, -1, -1, -1,  3,
       1,  1, -1, -1, -1, -1, -1,
         1, -1,  0,  0,  0,  0,
          -1,  0,  0,  0,  0
  };




/* * * * * * * * * * * * * * * * * */
Situation
::Situation()
{
  clear();
}


/* * * * * * * * * * * * * * * * * */
Situation
::Situation(Count playerNum)
{
  load(playerNum);
}



/* * * * * * * * * * * * * * * * * */
void Situation
::load(Count playerNum)
{
  clear();

  /* playerNum & Act */
  m_playerNum = playerNum;
  m_playerAct = 0;
  m_ejectWon = 6;
  
  /* Kicked, lost */
  for (int n = 0; n < PLAYERS_MAX; n++)
  {
    m_kicked[n] = 0;
    m_lost[n] = 0;
  }
  
  /* Marbles */
  if (m_playerNum == 2) 
    memcpy (m_marble, g_marbleBoard2, POS_NUM + 1);
  else if (m_playerNum == 3) 
    memcpy (m_marble, g_marbleBoard3, POS_NUM + 1);
  else if (m_playerNum == 4) 
    memcpy (m_marble, g_marbleBoard4, POS_NUM + 1);
  else
    throw SituationE("Cannot initialize the board");
}

/* * * * * * * * * * * * * * * * * */
FullMove Situation
::check(const Move &move) const
{

  /* Check move parameters */
  if (move.m_count < 0 || move.m_count > SEL_MAX ||
    move.m_dir < 0 || move.m_dir > DIR_NUM)
    throw MoveE("Invalid move parameters");

  /* Check if marbles correspond to player */
  Count n;
  for (n = 0; n < move.m_count; n++)
  {
    if (move.m_pos[n] <= 0 || move.m_pos[n] > POS_NUM)
      throw MoveE("Marble outside of playboard");

    if (m_marble[move.m_pos[n]] != m_playerAct)
    {
      throw MoveE("Marble(s) not belonging to you: " + 
        Move::sPos(move.m_pos[n]));
    }
  }


  /*** Check Selection ***/
  FullMove full;
  for (n = 0; n < move.m_count; n++)
  {
    full.m_pos[n] = move.m_pos[n];
  }
  full.m_count = move.m_count;
  full.m_dir = move.m_dir;

  /* 2 marbles */
  if (full.m_count == 2)
  {
    /* Sort marbles in descending order*/
    if (full.m_pos[0] < full.m_pos[1])
    {
      Pos pSwap = full.m_pos[0];
      full.m_pos[0] = full.m_pos[1];
      full.m_pos[1] = pSwap;
    }

    /* Look for the marble direction */
    Dir dir;
    for (dir = 0; dir < DIR_NUM; dir++)
    {
      /* 1 next 0 ? */
      if (Kernel::next[full.m_pos[0]][dir] == full.m_pos[1]) break;
    }
    if (dir == DIR_NUM) throw MoveE("The 2 marbles are not adjacent");
    full.m_marbleDir = dir;
  }

  /* 3 marbles */
  else if (full.m_count == 3)
  {
    /* Sort the three marbles, according to pos (descending) */
    Count i, p;
    Pos pSwap;
    for (i = 0; i < 2; i++)
    {
      for (p = 0; p < 2; p++)
      {
        if (full.m_pos[p] < full.m_pos[p + 1])
        {
          pSwap = full.m_pos[p];
          full.m_pos[p] = full.m_pos[p + 1];
          full.m_pos[p + 1] = pSwap;
        }
      }
    }
    /* 2 next to 1, 1 next to 0 */
    for (Dir dir = 0; dir < DIR_NUM; dir++)
    {
      if (Kernel::next[full.m_pos[0]][dir] == full.m_pos[1] &&
        Kernel::next[full.m_pos[1]][dir] == full.m_pos[2]) break;
    }
    if (dir == DIR_NUM) throw MoveE("The 3 marbles are not in a line");
    full.m_marbleDir = dir;
  }


  /*** Check broadside ***/
  for (n = 0; n < full.m_count; n++)
  {
    if (m_marble[Kernel::next[full.m_pos[n]][full.m_dir]] != MARBLE_EMPTY) break;
  }
  if (n == full.m_count)
  {
    /* Found broadside move */
    full.m_countEnemy = 0;
    full.m_bEject = false;
    return full;
  }


  /*** Check for line move ***/
  /* 1 marble */
  if (full.m_count == 1) full.m_marbleDir = full.m_dir;

  /* Inverse direction */
  if (full.m_count > 1 &&
    full.m_dir == ((full.m_marbleDir + DIR_NUM/2) % DIR_NUM))
  {
    Pos pSwap;
    
    pSwap = full.m_pos[0];
    full.m_pos[0] = full.m_pos[full.m_count - 1];
    full.m_pos[full.m_count - 1] = pSwap;

    full.m_marbleDir = full.m_dir;
  }

  /* Illegal direction */
  if (full.m_dir != full.m_marbleDir) 
  {
    throw MoveE("Broadside move impossible");
  }

  /* Extend selection */
  for (n = full.m_count ; n < SEL_MAX; n++)
  {
    Pos p = Kernel::next[full.m_pos[n-1]][full.m_dir];
    if (m_marble[p] == m_playerAct)
    {
      full.m_pos[n] = p;
      full.m_count = n + 1;
    }
  }

  /* Ejecting yourself */
  Pos p = Kernel::next[full.m_pos[full.m_count - 1]][full.m_dir];
  if (p == 0) throw MoveE("Why would you want to eject yourself ?");
  
  /* Count enemy marbles */
  for (full.m_countEnemy = 0; full.m_countEnemy < full.m_count; full.m_countEnemy++)
  {
    if (m_marble[p] == m_playerAct) throw MoveE("You cannot push yourself");
    if (m_marble[p] == MARBLE_EMPTY || p == 0) break;
    full.m_posEnemy[full.m_countEnemy] = p;
    p = Kernel::next[p][full.m_dir];
  }

  /* Moving too many marbles */
  if (full.m_countEnemy >= full.m_count)
    throw MoveE("Illegal sumito");

  /* Eject ? */
  if (p == 0) full.m_bEject = true;
  else full.m_bEject = false;

  return full;
}



/* * * * * * * * * * * * * * * * * */
void Situation
::move(const FullMove &full)
{
  /* full is suspected to be a valid move */

  /* Check history */
  History hist;
  hist.m_board = (Board)*this;
  hist.m_playerAct = m_playerAct;
  hist.m_fMove = full;

  HistoryList::iterator it;
  for (it = m_liHist.begin(); it != m_liHist.end(); it++)
  {
    if (hist.equals(*it)) throw MoveRepeatedE("You may not repeat a move: " 
      + full.string());
  }


  /* What kind of a move ? */
  if (full.m_count == 1 || full.m_dir != full.m_marbleDir)
  {
    /* Broadside move */
    Count n;
    for (n = 0; n < full.m_count; n++)
    {
      m_marble[full.m_pos[n]] = MARBLE_EMPTY;
      m_marble[Kernel::next[full.m_pos[n]][full.m_dir]] = m_playerAct;
    }
  }
  else
  {
    /* Enemy marbles */
    if (full.m_countEnemy > 0)
    {
      Pos p;
      for (Count n = full.m_countEnemy - 1; n >= 0; n--)
      {
        p = full.m_posEnemy[n];
        m_marble[Kernel::next[p][full.m_dir]] = m_marble[p];
      }
    }

    /* Line move */
    m_marble[full.m_pos[0]] = MARBLE_EMPTY;
    m_marble[Kernel::next[full.m_pos[full.m_count - 1]][full.m_dir]] = m_playerAct;

    /* Eject */
    if (full.m_bEject)
    {
      m_kicked[m_playerAct]++;
      m_lost[m_marble[0]]++;
      m_marble[0] = MARBLE_BORDER;
    }
  }

  /* Add to history */
  m_liHist.push_back(hist);

  /* Ok, next player */
  Pos p;
  Count n;
  for (n = 0; n < m_playerNum; n++)
  {
    m_playerAct = (m_playerAct + 1) % m_playerNum;
    /* ? any marbles left */
    for (p = 1; p <= POS_NUM; p++)
    {
      if (m_marble[p] == m_playerAct) break;
    }
    if (p <= POS_NUM) 
    {
      /* Yes, marble found */
      break;
    }
  }
  if (n >= m_playerNum)
  {
    /* No player has any marbles left */
    throw SituationE("No player has any marbles left");
  }
}



/* * * * * * * * * * * * * * * * * */
void Situation
::clear()
{
  m_playerNum = 0;
  m_playerAct = 0;
  m_ejectWon = 0;
  m_liHist.clear();

  for (Pos p = 1; p <= POS_NUM; p++) m_marble[p] = MARBLE_EMPTY;
  for (int n = 0; n < PLAYERS_MAX; n++) 
  {
    m_kicked[n] = 0;
    m_lost[n] = 0;
  }
}


/* * * * * * * * * * * * * * * * * */
Marble Situation
::playerWon() const
{
  for (Marble n = 0; n < m_playerNum; n++)
  {
    if (m_kicked[n] >= m_ejectWon) return n;
  }
  return MARBLE_EMPTY;
}


/* * * * * * * * * * * * * * * * * */
FullMove Situation
::takeBackMove()
{
  /* ? History empty */
  if (m_liHist.empty()) throw SituationE("No more moves can be taken back");
  
  /* Get last history entry */
  History hist = *m_liHist.rbegin();
  m_liHist.pop_back();

  loadBoard(hist.m_board);
  m_playerAct = hist.m_playerAct;
  return hist.m_fMove;
}


/* * * * * * * * * * * * * * * * * */
Situation::Situation(const Situation &sit)
{
  clear();
  loadBoard(sit);
  m_ejectWon = sit.m_ejectWon;
  m_playerAct = sit.m_playerAct;
  m_playerNum = sit.m_playerNum;
  m_liHist = sit.m_liHist;
}


/* * * * * * * * * * * * * * * * * */
void Situation::loadBoard(const Board &board)
{
  memcpy(m_marble, board.m_marble, sizeof(Marble) * (POS_NUM + 1));
  memcpy(m_kicked, board.m_kicked, sizeof(Count) * PLAYERS_MAX);
  memcpy(m_lost, board.m_lost, sizeof(Count) * PLAYERS_MAX);
}



/* * * * * * * * * * * * * * * * * */
void Situation
::allNewMoves(MoveVector& moves) const
{
  // Compact history list
  HistoryList::const_iterator it;
  HistoryList liHist;
  for (it = m_liHist.begin(); it != m_liHist.end(); it++)
  {
    if (it->m_playerAct == m_playerAct && it->m_board.equals(*this))
      liHist.push_back(*it);
  }

  // ? no history cases
  if (liHist.empty()) 
  {
    allMoves(moves);
    return;
  }

  // Retrieve all moves 
  TRACE("Situation::allNewMoves historic cases\n");
  MoveVector movesAll(FullBoard::movesMax);
  allMoves(movesAll);

  // Eliminate all historic moves
  moves.reserve(FullBoard::movesMax);
  moves.resize(0);
  // Loop through all moves 
  bool bFound;
  for (int i = 0; i < movesAll.size(); i++)
  {
    bFound = false;
    // Loop through compact history
    for (it = liHist.begin(); it != liHist.end(); it++)
    {
      if (it->m_fMove.equals(movesAll[i])) 
      {
        // Found a historic case
        bFound = true;
        break;
      }
    }
    if (!bFound) moves.push_back(movesAll[i]);
  }
}
