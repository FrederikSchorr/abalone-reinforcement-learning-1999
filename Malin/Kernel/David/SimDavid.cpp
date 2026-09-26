/***********************************
/
/   SimDavid.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 



/*** include ***********************/ 
#include "SimDavid.h"

#include <afxwin.h>

using namespace caesar;


/*** Definitions *******************/ 
const Value ValueMax = INT_MAX - 1;
const Value ValueMin = INT_MIN + 1;


/*** SimDavid **********************/ 

/* * * * * * * * * * * * * * * * * */
SimDavid
::SimDavid()
{
  m_type = Simulation::david;
}



/* * * * * * * * * * * * * * * * * */
Value SimDavid
::vEvalAlphaBeta(const FullBoard& boardOri, char nDepth, Value vAlpha, Value vBeta)
{
  // Reserve memory
  if (m_moves.size() <= nDepth) m_moves.push_back(MoveVector(FullBoard::movesMax));
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

  // Sort moves
  m_killer.sort(nDepth, m_moves[nDepth]);

  // Loop through moves
  FullBoard board;
  Value valueBest = ValueMin;
  int nBest = -1;
  int nEqual = 1;
  for (int i = 0; i < m_moves[nDepth].size(); i++)
  {
    // Move it
    board = boardOri;
    board.moveRaw(m_moves[nDepth][i]);

    // Game is won
    if (board.m_kicked[1 - board.m_playerAct] >= board.m_ejectWon)
      value = ValueMax - 10 * board.m_kicked[board.m_playerAct] - nDepth - 1;
    // Continue recursion
    else
      value = -1 * vEvalAlphaBeta(board, nDepth + 1, -1 * vBeta, 
        -1 * __max(vAlpha, (nDepth == 0 ? valueBest-1 : valueBest)));

    // Store best move
    ASSERT(ValueMin < value && value < ValueMax);
    if (value > valueBest)
    {
      valueBest = value;
      nBest = i;
      nEqual = 1;
      if (valueBest >= vBeta) 
      {
        // Quit move loop
        break;
      }
    }
    else if (nDepth == 0 && value == valueBest)
    {
      if (rand() % ++nEqual == 0) nBest = i;
    }

  }
  ASSERT(nBest >= 0);

  // Statistics
  STAT(m_statNew.addCount(Statistics::branchFactorQ, i+1));
  STAT(if (nDepth < m_nDepthMax) \
    m_statNew.addCount(Statistics::branchFactor, i+1));

  // Remeber move for further calculations
  m_killer.add(nDepth, m_moves[nDepth][nBest]);

  // At lowest level, store the best move
  if (nDepth <= 0) m_moveBest = m_moves[nDepth][nBest];

  // Return value
  return valueBest;
}




/* * * * * * * * * * * * * * * * * */
Move SimDavid
::calcMove(const Situation& sit)
{
  // ? 2 players
  if (sit.m_playerNum != 2) 
    throw SimulationE("This computer player supports only 2-player games");

  // init "global" variables 
  m_bCancel = false;
  m_moveBest.m_count = 0;
  m_pSit = &sit;
  srand(Kernel::rand());
  STAT(Statistics statLast);
  STAT(m_statNew.clear());
 
  // Init. killer object 
  m_killer.init();

  Value value;

  // timeout
  if (m_nTimeOut > 0) m_timeEnd = time(NULL) + m_nTimeOut;
  else m_timeEnd = LONG_MAX;

  // if m_nDepth = 0 equiv with infinity
  try
  {
    m_nDepthMax = 1;       
    while (true)
    {
      // Statistics
      STAT(statLast = m_statNew);
      STAT(m_statNew.clear(Statistics::evalBoardFinal));
      STAT(m_statNew.clear(Statistics::lookaheadDepthQ));

      // Evaluate position
      m_killer.normalize();
      value = vEvalAlphaBeta(sit, 0, ValueMin, ValueMax);

      // max depth attained
      if (m_nDepthMax == m_nDepth) break;
      m_nDepthMax++;
    }
  }
  catch(TimeOutE e)
  {
    STAT(m_statNew = statLast);
    m_nDepthMax--;
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
void SimDavid
::modify(Modify &mod)
{
  mod.println("David computer player, alpha beta, killer moves");
  mod.modify("Maximal search depth in plies (0 = inf)", m_nDepth, 0, 128);
  mod.modify("Maximal search time in sec (0 = inf)", m_nTimeOut, 0, 10000);
  
  char *szPoints[Data::PointsNum] = {"Corner", "Border", "1 from border",
    "2 from border", "1 from center", "Center", 
    "Neighbour", "Kick marble"};
  
  for (int i=0; i < Data::PointsNum; i++)
  {
    mod.modify(szPoints[i], m_vPoints[i], -1000, +1000);
  }
}


/* * * * * * * * * * * * * * * * * */
void SimDavid
::cleanup()
{
  SimCaesar::cleanup();
  m_killer.clear();
}



