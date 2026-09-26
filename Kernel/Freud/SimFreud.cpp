/***********************************
/
/   SimFreud.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 



/*** include ***********************/ 
#include "SimFreud.h"
#include "../Utl/fstreamExt.h"
#include <float.h>
#include <afxwin.h>
#include <iomanip.h>


/*** Definitions *******************/ 

const float ValueMax = FLT_MAX * (float) 0.99;
const float ValueMin = -ValueMax;

/*** Static ************************/ 

int SimFreud::m_nFeatureNum[featureLast] = {127, 127, 128, 151, 142};


/*** SimFreud *********************/ 


/* * * * * * * * * * * * * * * * * */
SimFreud
::SimFreud()
{
  m_type = Simulation::freud;
  m_nFeatureType = feature00;
    
  m_nDepthMax = 0;
  m_nDepthMin = 0;
  m_nTimeOut = 0;

  m_nCounter = 0;
}


/* * * * * * * * * * * * * * * * * */
void SimFreud
::load(istream& is)
{
  is >> m_nFeatureType;
  is >> m_nDepthMin;
  is >> m_nDepthMax;
  is >> m_nTimeOut;

  Optimistic_TDlambda::load(is);
}


/* * * * * * * * * * * * * * * * * */
void SimFreud
::save(ostream& os) const
{
  os << m_nFeatureType << " " << m_nDepthMin << " " << m_nDepthMax << " " 
    << m_nTimeOut << endl;
  
  Optimistic_TDlambda::save(os);
}



/* * * * * * * * * * * * * * * * * */
void SimFreud
::modify(Modify &mod)
{
  mod.println("Freud computer player, alpha-beta brute, killer moves, based on a neural net");

  // Search depth
  mod.modify("Maximal search depth in plies (0 = inf)", m_nDepthMax, 0, 128);
  mod.modify("Maximal search time in sec (0 = inf)", m_nTimeOut, 0, 10000);
  mod.modify("Minimal search depth in plies (1-8)", m_nDepthMin, 1, 8);

  if (m_nDepthMax != 0 && m_nDepthMax < m_nDepthMin) 
    throw SimulationE("Maximal depth may not be below minimal depth");

  // Specify feature type - only if network unused
  if (lLearnStates() == 0)
  {
    // Feature type
    mod.modify("Feature type (0, ...)", m_nFeatureType, 0, featureLast - 1);
  
    // Hidden units
    int nHiddenNum = Optimistic_TDlambda::nHiddenNum();
    mod.modify("Number of hidden units (1-100)", nHiddenNum, 1, 1000);

    // Initialize network
    Optimistic_TDlambda::clear();
    Optimistic_TDlambda::init(
        m_nFeatureNum[m_nFeatureType],  // input units
        nHiddenNum,                     // hidden units
        false,                          // squash y
        true,                           // online algo.
        (float)0.3);                    // lambda

    // Randomize the network
    Optimistic_TDlambda::randomize();
    mod.println("A new network was created and randomized");
  }
  // Net already configured
  else 
  {
    mod.println("Neural net already configured");
    char buffer[256];
    sprintf(buffer, "Network feature type: %d", m_nFeatureType);
    mod.println(buffer);
    sprintf(buffer, "Number of hidden units: %d", nHiddenNum());
    mod.println(buffer);
  }

  // Adjust learning parameters
  // Squash y
  int n = bSquashY();
  mod.modify("Apply sigmoidal function to output unit ? (0=no, 1=yes)", n, 0, 1);
  squashY(n > 0);

  // Squash y
  n = bOnline();
  mod.modify("Use offline or online algorithm ? (0=off, 1=on)", n, 0, 1);
  online(n > 0);

  // Lambda
  float f = fLambda();
  mod.modify("Lamda (decay) factor (0.0 - 1.0)", f, 0.0, (float)1.0, 2);
  lambda(f);
  
  // Input units stepsize
  f = fStepInput();
  mod.modify("Step size for input layer (0.0 - 100.0)", f, 0.0, (float)100.0, 3);
  stepInput(f);
  
  // Hidden units stepsize
  f = fStepHidden();
  mod.modify("Step size for hidden layer (0.0 - 100.0)", f, 0.0, (float)100.0, 3);
  stepHidden(f);
}



/* * * * * * * * * * * * * * * * * */
bool SimFreud
::bIsHuman() const
{ 
  return false; 
}



/* * * * * * * * * * * * * * * * * */
void SimFreud
::cancel()
{
  //TRACE("SimFreud::cancel\n");
  m_bCancel = true; 
}



/* * * * * * * * * * * * * * * * * */
ostream & SimFreud
::write(ostream& os) const 
{
  os << "Max search depth: "  << m_nDepthMax    << " plies\n";
  os << "Max search time: "   << m_nTimeOut     << " sec ";
  os << "(min search depth: " << m_nDepthMin    << " plies)\n";
  os << "Feature type: "      << m_nFeatureType << endl;
  return Optimistic_TDlambda::write(os);
}



/* * * * * * * * * * * * * * * * * */
float SimFreud
::vEval(const FullBoard& boardOri, const char nDepth, float vAlpha, float vBeta)
{
  float value;
  // Reserve memory
  if (m_moves.size() <= nDepth) 
    m_moves.push_back(MoveVector(FullBoard::movesMax));

  // How deep are we ?
  // Quiesence search
  if (nDepth >= m_nDepthLimit) boardOri.ejectMoves(m_moves[nDepth]);
  // Regular search
  else if (nDepth > 0) boardOri.allMoves(m_moves[nDepth]);
  // Depth == 0
  else m_pSit->allNewMoves(m_moves[nDepth]);

  // No more moves found
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
  //m_killer.initDepth(nDepth);
  m_killer.sort(nDepth, m_moves[nDepth]);

  // Loop through moves
  FullBoard board;
  float valueBest = ValueMin;
  int nBest = 0;
  int nEqual = 1;

  for (int i = 0; i < m_moves[nDepth].size(); i++)
  {
    // Move it
    board = boardOri;
    board.moveRaw(m_moves[nDepth][i]);

    // ? game won
    if (board.m_kicked[1 - board.m_playerAct] >= board.m_ejectWon)
      value = ValueMax / (2 * board.m_kicked[board.m_playerAct] + nDepth + 1);
    // Continue recursion
    else
      value = -1 * vEval(board, nDepth + 1, -1 * vBeta, 
        -1 * __max(vAlpha, (nDepth == 0 ? valueBest-0.1 : valueBest)));

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
  ASSERT(valueBest >= ValueMin);

  // Statistics
  STAT(m_statNew.addCount(Statistics::branchFactorQ, i+1));
  STAT(if (nDepth < m_nDepthMax) \
    m_statNew.addCount(Statistics::branchFactor, i+1));

  // Move has prooved to be a killer
  m_killer.add(nDepth, m_moves[nDepth][nBest]);

  if (nDepth == 0)
  {
    // Store the best move
    m_moveBest = m_moves[nDepth][nBest];
  }

  // Return value
  return valueBest;
}





/* * * * * * * * * * * * * * * * * */
Move SimFreud
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


  // Init. killer object
  m_killer.init();
  
  // Timeout
  int timeBegin = time(NULL);
  m_timeEnd = LONG_MAX;

  // Iterative minimaxing
  float value;
  try
  {
    for (m_nDepthLimit = 1;; m_nDepthLimit++)
    {
      // Statistics
      STAT(statLast = m_statNew);
      STAT(m_statNew.clear(Statistics::evalBoardFinal));
      STAT(m_statNew.clear(Statistics::lookaheadDepthQ));

      // Start recursion
      m_killer.normalize();
      value = vEval(sit, 0, ValueMin, ValueMax);

      // Max depth attained (m_nDepthMax = 0 corresponds to infinity)
      if (m_nDepthLimit == m_nDepthMax) break;

      // Min search finished, activate timeout
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

  // ? Cancel
  if (m_bCancel) throw CancelE("Move computation cancelled");

  // ? move found
  if (m_moveBest.m_count <= 0) throw NoMoveFoundE("No move found");

  /*FloatVector x;
  boardToInput(sit, x);
  input(x);
  propagate();
  float y;
  output(y);
  long flags = g_log.setf(ios::left | ios::showpos);
  g_log << setw(6) << sit.m_liHist.size() << " " <<
    (int)sit.m_kicked[0] << ":" << (int)sit.m_kicked[1] << " " <<
    setw(15) << y << " " << m_moveBest << 
    (m_moveBest.m_bEject ? " Kick" : "") << endl;
  g_log.flags(flags);*/

  // Ok
  return m_moveBest;
}



/* * * * * * * * * * * * * * * * * */
void SimFreud
::learnBegin(const FullBoard & board)
{
  FloatVector x;
  boardToInput(board, x);

  Optimistic_TDlambda::learnBegin(x);
}

/* * * * * * * * * * * * * * * * * */
void SimFreud
::learnNext(const FullBoard & board)
{
  FloatVector x;
  boardToInput(board, x);

  Optimistic_TDlambda::learnNext(x);
}

/* * * * * * * * * * * * * * * * * */
void SimFreud
::learnEnd(const FullBoard & board)
{
  float y;

  switch(m_nFeatureType)
  {
  case feature00:
    {
      // 0 won
      if (board.m_kicked[0] > board.m_kicked[1]) 
        y = 0.5 + (board.m_kicked[0] - board.m_kicked[1]) / 100.0;
      // 1 won
      else if (board.m_kicked[0] < board.m_kicked[1]) 
        y = -0.5 - (board.m_kicked[1] - board.m_kicked[0]) / 100.0;
      // Oops
      else y = 0.0;
      break;
    }
  case feature01:
    {
      // 0 won
      if (board.m_kicked[0] > board.m_kicked[1]) 
        y = (10 + board.m_kicked[0] - board.m_kicked[1]) / 100.0;
      // 1 won
      else if (board.m_kicked[0] < board.m_kicked[1]) 
        y = -(10 + board.m_kicked[1] - board.m_kicked[0]) / 100.0;
      // Oops
      else y = 0.0;
      break;
    }
  case feature02:
    {
      // 0 won
      if (board.m_kicked[0] > board.m_kicked[1]) y = (float)0.1;
      // 1 won
      else if (board.m_kicked[0] < board.m_kicked[1]) y = (float)-0.1;
      // Oops
      else y = 0.0;
      break;
    }
  case feature03:
  case feature04:
    {
      // 0 won
      if (board.m_kicked[0] > board.m_kicked[1]) y = 1;
      // 1 won
      else if (board.m_kicked[0] < board.m_kicked[1]) y = -1;
      // Oops
      else y = 0;
      break;
    }

  default:
    throw SimulationE("Illegal Freud feature type");
  }

  FloatVector x;
  boardToInput(board, x);

  Optimistic_TDlambda::learnEnd(x, y);
}


/* * * * * * * * * * * * * * * * * */
float SimFreud
::vEvalBoard(const FullBoard & board)
{
  FloatVector x;
  boardToInput(board, x);

  // Set input, calculate output
  float y;
  input(x);
  propagate();
  output(y);
  
  // The net always calculates the value for player 0
  return (board.m_playerAct == 0 ? y : -y);
}



/* * * * * * * * * * * * * * * * * */
void SimFreud
::boardToInput(const FullBoard & board, FloatVector & x)
{
  // Set nInputNum size
  x.assign(m_nFeatureNum[m_nFeatureType], 0.0);

  switch(m_nFeatureType)
  {
  case feature00:
    {
      // Encode board to x
      Pos p;
      Dir dir;

      // Game over
      if (board.m_kicked[0] == board.m_ejectWon) x[0] = 1;
      else x[0] = 0;
      if (board.m_kicked[1] == board.m_ejectWon) x[1] = -1;
      else x[1] = 0;

      // Pushed marbles
      x[2] = board.m_kicked[0];
      x[3] = -board.m_kicked[1];

      // Whose turn is it
      x[4] = board.m_playerAct;

      // Neighbours, singles
      int neigh;
      Marble marble;
      for (p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        neigh = 0;

        // For every marble
        if (marble != MARBLE_EMPTY)
        {
          for (dir = 0; dir < DIR_NUM; dir++)
          {
            if (board.m_marble[Kernel::next[p][dir]] == marble) neigh++;
          }

          // 0 marble
          if (marble == 0)
          {
            x[4 + p] = neigh + 1;
            x[4 + 61 + p] = (neigh == 0);
          }
          // 1 marble
          else
          {
            x[4 + p] = -(neigh + 1);
            x[4 + 61 + p] = -(neigh == 0);
          }
        }
      }

      // That's it
      break;
    }
  case feature01:
    {
      // Encode board to x
      Pos p;
      Dir dir;

      // Game over
      x[0] = (board.m_kicked[0] == board.m_ejectWon) ? 1 : 0;
      x[1] = (board.m_kicked[1] == board.m_ejectWon) ? 1 : 0;
      
      // Pushed marbles
      x[2] = board.m_kicked[0] - board.m_kicked[1];

      if (board.m_kicked[0] > board.m_kicked[1]) x[3] = x[2] + board.m_kicked[0];
      else if (board.m_kicked[0] < board.m_kicked[1]) x[3] = x[2] - board.m_kicked[1];
      else x[3] = 0;

      // Whose turn is it
      x[4] = board.m_playerAct;

      // Position, Neighbours
      int neigh;
      Marble marble;
      for (p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        neigh = 0;

        // For every marble
        if (marble != MARBLE_EMPTY)
        {
          for (dir = 0; dir < DIR_NUM; dir++)
          {
            if (board.m_marble[Kernel::next[p][dir]] == marble) neigh++;
          }

          // 0 marble
          if (marble == 0)
          {
            x[3 + 2*p] = 1;
            x[4 + 2*p] = neigh;
          }
          // 1 marble
          else
          {
            x[3 + 2*p] = -1;
            x[4 + 2*p] = -neigh;
          }
        }
      }

      // That's it
      break;
    }
  case feature02:
    {
      // Encode board to x
      Pos p;
      Dir dir;

      // Game over
      x[0] = (board.m_kicked[0] == board.m_ejectWon) ? 1 : 0;
      x[1] = (board.m_kicked[1] == board.m_ejectWon) ? 1 : 0;
      
      // Pushed marbles
      x[2] = board.m_kicked[0];
      x[3] = board.m_kicked[1];

      if (board.m_kicked[0] > board.m_kicked[1]) 
        x[4] = 2 * board.m_kicked[0] - board.m_kicked[1];
      else if (board.m_kicked[0] < board.m_kicked[1]) 
        x[4] = board.m_kicked[0] - 2 * board.m_kicked[1];
      else x[4] = 0;

      // Whose turn is it
      x[5] = board.m_playerAct;

      // Position, Neighbours
      int neigh;
      Marble marble;
      for (p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        neigh = 0;

        // For every marble
        if (marble != MARBLE_EMPTY)
        {
          for (dir = 0; dir < DIR_NUM; dir++)
          {
            if (board.m_marble[Kernel::next[p][dir]] == marble) neigh++;
          }

          // 0 marble
          if (marble == 0)
          {
            x[4 + 2*p] = 1;
            x[5 + 2*p] = neigh;
          }
          // 1 marble
          else
          {
            x[4 + 2*p] = -1;
            x[5 + 2*p] = -neigh;
          }
        }
      }
      // That's it
      break;
    }
  case feature03:
    {
      // Encode board to x
      int i;
      int index = 0;
      Pos p;
      Dir dir;

      ASSERT(board.m_ejectWon <= 6);
      // Kicked marbles
      for (i = 0; i < 6+1; i++)
      {
        x[index++] = (board.m_kicked[0] == i);
        x[index++] = (board.m_kicked[1] == i);  
      }

      // Kicked difference
      int diff = board.m_kicked[0] - board.m_kicked[1];
      for (i = -6; i <= 6; i++)
        x[index++] = (i == diff);

      // Nonlinear kicked - difference
      if (board.m_kicked[0] > board.m_kicked[1]) 
        x[index++] = 2 * board.m_kicked[0] - board.m_kicked[1];
      else if (board.m_kicked[0] < board.m_kicked[1]) 
        x[index++] = board.m_kicked[0] - 2 * board.m_kicked[1];
      else x[index++] = 0;

      // Whose turn is it
      x[index++] = board.m_playerAct;

      // Position, Neighbours
      int neigh;
      Marble marble;
      for (p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        neigh = 0;

        // For every marble
        if (marble != MARBLE_EMPTY)
        {
          for (dir = 0; dir < DIR_NUM; dir++)
          {
            if (board.m_marble[Kernel::next[p][dir]] == marble) neigh++;
          }

          // 0 marble
          if (marble == 0)
          {
            x[index + 2*p - 2] = 1;
            x[index + 2*p - 1] = neigh;
          }
          // 1 marble
          else
          {
            x[index + 2*p - 2] = -1;
            x[index + 2*p - 1] = -neigh;
          }
        }
      }
      // That's it
      break;
    }

  case feature04:
    {
      // Encode board to x
      int i;
      int index = 0;
      Pos p;
      Dir dir;

      ASSERT(board.m_ejectWon <= 6);
      // Kicked marbles
      for (i = 1; i < 6+1; i++)
      {
        x[index++] = (i <= board.m_kicked[0]);
        x[index++] = (i <= board.m_kicked[1]);  
      }

      // Kicked difference
      int diff = board.m_kicked[0] - board.m_kicked[1];
      for (i = 1; i < 6+1; i++)
        x[index++] = (diff >= 0) ? (i <= diff) : -(i <= -diff);

      // Nonlinear kicked - difference
      if (board.m_kicked[0] > board.m_kicked[1]) 
        x[index++] = 2 * board.m_kicked[0] - board.m_kicked[1];
      else if (board.m_kicked[0] < board.m_kicked[1]) 
        x[index++] = board.m_kicked[0] - 2 * board.m_kicked[1];
      else x[index++] = 0;

      // Whose turn is it
      x[index++] = board.m_playerAct;

      // Position, Neighbours
      int neigh;
      Marble marble;
      for (p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        neigh = 0;

        // For every marble
        if (marble != MARBLE_EMPTY)
        {
          for (dir = 0; dir < DIR_NUM; dir++)
          {
            if (board.m_marble[Kernel::next[p][dir]] == marble) neigh++;
          }

          // 0 marble
          if (marble == 0)
          {
            x[index + 2*p - 2] = 1;
            x[index + 2*p - 1] = neigh;
          }
          // 1 marble
          else
          {
            x[index + 2*p - 2] = -1;
            x[index + 2*p - 1] = -neigh;
          }
        }
      }
      // That's it
      break;
    }


  default:
    throw SimulationE("Illegal Freud feature type");
  }
}


/* * * * * * * * * * * * * * * * * */
void SimFreud
::cleanup()
{
  m_killer.clear();
}


