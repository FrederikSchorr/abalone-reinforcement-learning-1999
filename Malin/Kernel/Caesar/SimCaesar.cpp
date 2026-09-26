/***********************************
/
/   SimCaesar.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 



/*** include ***********************/ 
#include "SimCaesar.h"

#include "../Utl/fstreamExt.h"

#include <afxwin.h>

using namespace caesar;

/*** Definitions *******************/ 
const Value ValueMax = INT_MAX - 1;
const Value ValueMin = INT_MIN + 1;


/*** caesar::Data ******************/ 

/* * * * * * * * * * * * * * * * * */
Data
::Data()
{
  m_nDepth = 3;

  m_nTimeOut = 10.0;

  m_vPoints[Border] = -7;
  m_vPoints[1]      = -5;
  m_vPoints[2]      = -1;
  m_vPoints[3]      = +2;
  m_vPoints[4]      = +5;
  m_vPoints[Center] = +12;

  m_vPoints[Neighbour]  = +2;
  m_vPoints[Kicked]     = +1000;
}


ostream & Data
::write(ostream &os) const
{
  os << m_nDepth << " plies, timeout: " << m_nTimeOut << " sec\n";
  for (int i = 0; i < PointsNum; i++)
    os << m_vPoints[i] << "  ";

  os << endl;

  return os;
}


/*** SimCaesar *********************/ 


/* * * * * * * * * * * * * * * * * */
SimCaesar
::SimCaesar()
{
  m_type = Simulation::caesar;
  m_nCounter = 0;
}


/* * * * * * * * * * * * * * * * * */
void SimCaesar
::load(istream& is)
{
  is >> m_nDepth;
  is >> m_nTimeOut;
  for (int i = 0; i < PointsNum; i++) is >> m_vPoints[i];
}


/* * * * * * * * * * * * * * * * * */
void SimCaesar
::save(ostream& os) const
{
  os << m_nDepth << " " << m_nTimeOut << endl;
  for (int i = 0; i < PointsNum; i++) os << " " << m_vPoints[i];
}



/* * * * * * * * * * * * * * * * * */
void SimCaesar
::modify(Modify &mod)
{
  mod.println("Caesar computer player, brute force & quiesence search");
  mod.modify("Maximal search time in sec (0 = inf)", m_nTimeOut, 0, 10000);
  if (m_nTimeOut == 0)
    mod.modify("Fixed search depth in plies (1-...)", m_nDepth, 1, 128);
  
  
  char *szPoints[Data::PointsNum] = {"Corner", "Border", "1 from border",
    "2 from border", "1 from center", "Center", 
    "Neighbour", "Kick marble"};
  
  for (int i=0; i < Data::PointsNum; i++)
  {
    mod.modify(szPoints[i], m_vPoints[i], -1000, +1000);
  }
}



/* * * * * * * * * * * * * * * * * */
bool SimCaesar
::bIsHuman() const
{ 
  return false; 
}


/* * * * * * * * * * * * * * * * * */
Move SimCaesar
::calcMove(const Situation& sit)
{
  // ? 2 players
  if (sit.m_playerNum != 2) 
    throw SimulationE("This computer player supports only 2-player games");

  // init "global" variables 
  m_bCancel = false;
  m_moveBest.m_count = 0;
  m_pSit = &sit;
  STAT(m_statNew.clear());
  STAT(Statistics statLast);

  Value value;

  // ? timeout
  if (m_nTimeOut > 0)
  {
    m_nDepthMax = 1;
    m_timeEnd = time(NULL) + m_nTimeOut;

    try
    {
      while (true)
      {
        // Statistics
        STAT(statLast = m_statNew);
        STAT(m_statNew.clear(Statistics::evalBoardFinal));
        STAT(m_statNew.clear(Statistics::lookaheadDepthQ));

        value = vEval(sit, 0);
        m_nDepthMax++;
      }
    }
    catch(TimeOutE e)
    {
      STAT(m_statNew = statLast);
      m_nDepthMax--;
    }
  }
  // fixed depth search
  else
  {
    m_nDepthMax = m_nDepth;
    m_timeEnd = LONG_MAX;

    value = vEval(sit, 0);
  }

  // Statistics
  STAT(m_statNew.addCount(Statistics::lookaheadDepth, m_nDepthMax));
  STAT(m_statNew.count(Statistics::evalBoardTotal));
  STAT(m_statNew.count(Statistics::evalBoardFinal));
  STAT(m_stat.addCount(m_statNew));

  if (m_moveBest.m_count <= 0) throw NoMoveFoundE("No move found");
    
  return m_moveBest;
}


/* * * * * * * * * * * * * * * * * */
void SimCaesar
::cancel()
{
  //TRACE("SimCaesar::cancel\n");
  m_bCancel = true; 
}



/* * * * * * * * * * * * * * * * * */
ostream & SimCaesar
::write(ostream& os) const 
{
  Data::write(os); 
  return os;
}



/* * * * * * * * * * * * * * * * * */
Value SimCaesar
::vEval(const FullBoard& boardOri, char nDepth)
{
  // Reserve memory if necessary
  if (m_moves.size() <= nDepth) 
    m_moves.push_back(MoveVector(FullBoard::movesMax));
  Value value;

  // How deep are we ?
  // Quiesence search
  if (nDepth >= m_nDepthMax) boardOri.ejectMoves(m_moves[nDepth]);
  // Regular search
  else if (nDepth > 0) boardOri.allMoves(m_moves[nDepth]);
  // Depth == 0
  else m_pSit->allNewMoves(m_moves[nDepth]);

  // ? no more moves
  if (m_moves[nDepth].size() <= 0)
  {
    value = vEvalBoard(boardOri);
    STAT(m_statNew.add(Statistics::evalBoardTotal));
    STAT(m_statNew.add(Statistics::evalBoardFinal));
    STAT(m_statNew.addCount(Statistics::lookaheadDepthQ, nDepth));
    return value;
  }

  // Time to say goodbye ?
  if (m_bCancel) throw CancelE("Move computation cancelled");
  if (m_nCounter++ >= 1000) 
  {
    if (time(NULL) >= m_timeEnd) throw TimeOutE();
    m_nCounter = 0;
  }

  // Loop through moves
  FullBoard board;
  Value valueBest = ValueMin;
  int nBest = -1;
  for (int i = 0; i < m_moves[nDepth].size(); i++)
  {
    //TRACE("SimCaesar::vEval nDepth = %d, nMove = %d\n", nDepth, i);
    // Move it
    board = boardOri;
    board.moveRaw(m_moves[nDepth][i]);

    // Game is won
    if (board.m_kicked[1 - board.m_playerAct] >= board.m_ejectWon)
    {
      value = ValueMax - 10 * board.m_kicked[board.m_playerAct] - nDepth - 1;
    }
    // Continue recursion
    else 
    {
      value = -1 * vEval(board, nDepth + 1);
    }

    // Store best move
    if (value > valueBest)
    {
      valueBest = value;
      nBest = i;
    }
  }

  // Statistics
  STAT(m_statNew.addCount(Statistics::branchFactorQ, i+1));
  STAT(if (nDepth < m_nDepthMax) \
    m_statNew.addCount(Statistics::branchFactor, i+1));

  // At lowest level, store the best move
  if (nDepth <= 0) 
    m_moveBest = m_moves[nDepth][nBest];

  // Return value
  return valueBest;
}




/* * * * * * * * * * * * * * * * * */
Value SimCaesar
::vEvalBoard(const FullBoard & board)
{
  //TRACE("SimCaesar::vEvalBoard\n"); 

#ifdef _DEBUG
  m_nEvalBoard++;
#endif
  
  Value value = 0;

  Marble playerAct = board.m_playerAct;
  Marble playerEnemy = 1 - board.m_playerAct;

  // Kicked
  if (board.m_kicked[playerAct] > board.m_kicked[playerEnemy])
  {
    value += (2 * board.m_kicked[playerAct] - board.m_kicked[playerEnemy])
      * m_vPoints[Kicked];
  }
  else if (board.m_kicked[playerAct] < board.m_kicked[playerEnemy])
  {
    value += (board.m_kicked[playerAct] - 2 * board.m_kicked[playerEnemy])
      * m_vPoints[Kicked];
  }

  // Loop through positions
  Marble player;
  Value v;
  Dir dir;
  for (Pos p = 1; p <= POS_NUM; p++)
  {
    player = board.m_marble[p];
    if (player == MARBLE_EMPTY) continue;
    
    // Sum up points
    v = m_vPoints[Kernel::nBorderDist[p]];
    for (dir = 0; dir < 3; dir++)
    {
      if (board.m_marble[Kernel::next[p][dir]] == player) 
      {
        v += m_vPoints[Neighbour];
      }
    }

    // Add to total value
    if (player == playerAct) value += v;
    else value -= v;
  }

/* #ifdef _DEBUG
  if ((m_nEvalBoard % 2) == 1)
  {
    FullBoard boardInv = board;
    boardInv.m_playerAct = playerEnemy;
  
    Value valueInv = vEvalBoard(boardInv);
    ASSERT(value == -1 * valueInv);
  }
#endif */

  return value;
}



