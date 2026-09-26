/***********************************
/
/   SimCharles.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 



/*** include ***********************/ 
#include "SimCharles.h"

#include <afxwin.h>

using namespace caesar;

/*** Definitions *******************/ 
const Value ValueMax = INT_MAX - 1;
const Value ValueMin = INT_MIN + 1;


/*** SimCharles ********************/ 

/* * * * * * * * * * * * * * * * * */
SimCharles
::SimCharles()
{
  m_type = Simulation::charles;
}



/* * * * * * * * * * * * * * * * * */
Value SimCharles
::vEvalAlphaBeta(const FullBoard& boardOri, char nDepth, Value vAlpha, Value vBeta)
{
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
    //TRACE("SimCharles::vEval nDepth = %d, nMove = %d\n", nDepth, i);
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
      value = -1 * vEvalAlphaBeta(board, nDepth + 1, 
        -1 * vBeta, -1 * __max(vAlpha, valueBest));
    }

    // Store best move
    if (value > valueBest)
    {
      valueBest = value;
      nBest = i;
      if (valueBest >= vBeta) 
      {
        // Quit move loop
        break;
      }
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
Move SimCharles
::calcMove(const Situation& sit)
{
  // ? 2 players
  if (sit.m_playerNum != 2) 
    throw SimulationE("This computer player supports only 2-player games");

  // init "global" variables 
  m_bCancel = false;
  m_moveBest.m_count = 0;
  m_pSit = &sit;

  Value value;
  STAT(m_statNew.clear());
  STAT(Statistics statLast);


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

        // Yoo, do it
        value = vEvalAlphaBeta(sit, 0, ValueMin, ValueMax);
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

    value = vEvalAlphaBeta(sit, 0, ValueMin, ValueMax);
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
void SimCharles
::modify(Modify &mod)
{
  mod.println("Charles computer player, alpha beta");
  SimCaesar::modify(mod);
}


