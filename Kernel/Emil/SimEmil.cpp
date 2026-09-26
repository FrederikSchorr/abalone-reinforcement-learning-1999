/***********************************
/
/   SimEmil.cpp
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

#include "SimEmil.h"
#include "Hash.h"

#include "../Utl/fstreamExt.h"

#include <afxwin.h>

using namespace emil;

/*** Definitions *******************/ 
const int ValueMax = INT_MAX - 1;
const int ValueMin = INT_MIN + 1;




/*** emil::Data ******************/ 

/* * * * * * * * * * * * * * * * * */
Data
::Data()
{
  m_nDepthMin = 4;
  m_nDepthMax = 0;

  m_nTimeOut = 5.0;

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
  os << m_nDepthMin << "," << m_nDepthMax << 
    " plies, timeout: " << m_nTimeOut << " sec\n";
  for (int i = 0; i < PointsNum; i++)
    os << m_vPoints[i] << "  ";

  os << endl;

  return os;
}


/*** SimEmil *********************/ 


/* * * * * * * * * * * * * * * * * */
SimEmil
::SimEmil()
{
  m_type = Simulation::emil;
  m_nCounter = 0;
}


/* * * * * * * * * * * * * * * * * */
void SimEmil
::load(istream& is)
{
  is >> m_nDepthMin;
  is >> m_nDepthMax;
  is >> m_nTimeOut;
  for (int i = 0; i < PointsNum; i++) is >> m_vPoints[i];
}


/* * * * * * * * * * * * * * * * * */
void SimEmil
::save(ostream& os) const
{
  os << m_nDepthMin << " " << m_nDepthMax << " "<< m_nTimeOut << endl;
  for (int i = 0; i < PointsNum; i++) os << " " << m_vPoints[i];
}



/* * * * * * * * * * * * * * * * * */
void SimEmil
::modify(Modify &mod)
{
  mod.println("Emil computer player, alpha-beta brute, killer moves, hash table");

  mod.modify("Maximal search depth in plies (0 = inf)", m_nDepthMax, 0, 128);
  mod.modify("Maximal search time in sec (0 = inf)", m_nTimeOut, 0, 10000);
  mod.modify("Minimal search depth in plies (1-8)", m_nDepthMin, 1, 8);

  if (m_nDepthMax != 0 && m_nDepthMax < m_nDepthMin) 
    throw SimulationE("Maximal depth may not be below minimal depth");
  
  char *szPoints[Data::PointsNum] = {"Corner", "Border", "1 from border",
    "2 from border", "1 from center", "Center", 
    "Neighbour", "Kick marble"};
  
  for (int i=0; i < Data::PointsNum; i++)
  {
    mod.modify(szPoints[i], m_vPoints[i], -1000, +1000);
  }
}



/* * * * * * * * * * * * * * * * * */
bool SimEmil
::bIsHuman() const
{ 
  return false; 
}



/* * * * * * * * * * * * * * * * * */
void SimEmil
::cancel()
{
  //TRACE("SimEmil::cancel\n");
  m_bCancel = true; 
}



/* * * * * * * * * * * * * * * * * */
ostream & SimEmil
::write(ostream& os) const 
{
  Data::write(os); 
  return os;
}


/* * * * * * * * * * * * * * * * * */
int SimEmil
::vEvalHash(const HashBoardInt& boardOri, const char nDepth, int vAlpha, int vBeta)
{
  // Look in HashTableInt
  int value;
  HashTableInt::Quality quality;
  if (nDepth > 0 && m_hash.bFind(boardOri, m_nDepthLimit - nDepth, value, quality)) 
  {
    switch(quality)
    {
    // Exact
    case HashTableInt::exact:
      STAT(m_statNew.addCount(Statistics::hashFound, 1));        
      return value;
    // Greater
    case HashTableInt::greater:
      if (value >= vBeta) 
      {
        STAT(m_statNew.addCount(Statistics::hashFound, 1));        
        return value;
      }
    // lesser
    default:
      if (value <= vAlpha) 
      {
        STAT(m_statNew.addCount(Statistics::hashFound, 1));        
        return value;
      }
    }
  }
  // Board position not found in hash table
  STAT(m_statNew.addCount(Statistics::hashFound, 0));

  // Load moves vector
  if (m_moves.size() <= nDepth) 
    m_moves.push_back(MoveVector(FullBoard::movesMax));

  // How deep are we ?
  // Quiesence search
  if (nDepth >= m_nDepthLimit) boardOri.ejectMoves(m_moves[nDepth]);
  // Regular search
  else if (nDepth > 0) 
  {
    boardOri.allMoves(m_moves[nDepth]);
    STAT(m_statNew.addCount(Statistics::numberOfMoves, m_moves[nDepth].size()));
  }
  // Depth == 0
  else m_pSit->allNewMoves(m_moves[nDepth]);

  // No more moves found
  if (m_moves[nDepth].size() <= 0)
  {
    value = vEvalBoard(boardOri);
    m_hash.store(boardOri, 0, value, HashTableInt::exact);
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
  int valueBest = ValueMin;
  int nBest = -1;
  int nEqual = 1;

  for (int i = 0; i < m_moves[nDepth].size(); i++)
  {
    // Move it
    HashBoardInt board(boardOri);
    board.moveRaw(m_moves[nDepth][i]);

    // ? game won
    if (board.m_kicked[1 - board.m_playerAct] >= board.m_ejectWon)
      value = ValueMax - 10 * board.m_kicked[board.m_playerAct] - nDepth - 1;
    // Continue recursion
    else
      value = -1 * vEvalHash(board, nDepth + 1, -1 * vBeta, 
        -1 * __max(vAlpha, (nDepth == 0 ? valueBest-1 : valueBest)));

    // Store best move
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
  
  STAT(m_statNew.addCount(Statistics::branchFactorQ, i+1));

  // Move has proved to be a killer
  m_killer.add(nDepth, m_moves[nDepth][nBest]);

  // Quiescence
  if (nDepth >= m_nDepthLimit)
  {
    // Store board value in hash table,
    // if value <= alpha no statement about the quality is possible
    if (valueBest > vAlpha)
      m_hash.store(boardOri, 0, valueBest, HashTableInt::greater);
  }
  // Regular
  else if (nDepth > 0)
  {
    // Determine the value quality with respect to (alpha,beta)    
    if (valueBest <= vAlpha) quality = HashTableInt::lesser;
    else if (valueBest >= vBeta) quality = HashTableInt::greater;
    else quality = HashTableInt::exact;
    // Store board value in hash table
    m_hash.store(boardOri, m_nDepthLimit - nDepth, valueBest, quality);

    STAT(m_statNew.addCount(Statistics::branchFactor, i+1));
  }
  // Depth == 0
  else 
  {
    // Store the best move
    m_moveBest = m_moves[nDepth][nBest];

    STAT(m_statNew.addCount(Statistics::branchFactor, i+1));
  }

  // Return value
  return valueBest;
}





/* * * * * * * * * * * * * * * * * */
Move SimEmil
::calcMove(const Situation& sit)
{
  // ? 2 players
  if (sit.m_playerNum != 2) 
    throw SimulationE("This computer player supports only 2-player games");

  // Init "global" variables 
  m_bCancel = false;
  m_moveBest.m_count = 0;
  m_pSit = &sit;
  srand(Kernel::rand());
  STAT(Statistics statLast);
  STAT(m_statNew.clear());

  // Init. killer object (remove unnecessary information)
  m_killer.init();
    
  // Timeout
  int timeBegin = time(NULL);
  m_timeEnd = LONG_MAX;

  // Iterative minimaxing
  int value;
  try
  {
    for (m_nDepthLimit = 1;; m_nDepthLimit++)
    {
      // Statistics
      STAT(statLast = m_statNew);
      STAT(m_statNew.clear(Statistics::evalBoardFinal));
      STAT(m_statNew.clear(Statistics::lookaheadDepthQ));

      // Normalize killer and go for it
      m_killer.normalize();
      value = vEvalHash(HashBoardInt(m_hash, sit), 0, ValueMin, ValueMax);

      // Max depth attained (m_nDepthMax = 0 corresponds to infinity)
      if (m_nDepthLimit == m_nDepthMax) break;

      // Min search finished, activate timeout (m_nDepthMin > 0)
      if (m_nDepthLimit == m_nDepthMin)
      {
        if (m_nTimeOut != 0) m_timeEnd = timeBegin + m_nTimeOut;
      }
    }
  }
  catch(TimeOutE e)
  {
    STAT(m_statNew = statLast);
    m_nDepthLimit--;
  }

  // Statistics
  STAT(m_statNew.addCount(Statistics::lookaheadDepth, m_nDepthLimit));
  STAT(m_statNew.count(Statistics::evalBoardTotal));
  STAT(m_statNew.count(Statistics::evalBoardFinal));
  STAT(m_stat.addCount(m_statNew));

  // ? move found
  if (m_moveBest.m_count <= 0) throw NoMoveFoundE("No move found");

  // Ok
  return m_moveBest;
}


/* * * * * * * * * * * * * * * * * */
int SimEmil
::vEvalBoard(const HashBoardInt & board)
{
  int value = 0;

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
  int v;
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


/* * * * * * * * * * * * * * * * * */
void SimEmil
::cleanup()
{
  m_killer.clear();
  m_hash.clear();
}
