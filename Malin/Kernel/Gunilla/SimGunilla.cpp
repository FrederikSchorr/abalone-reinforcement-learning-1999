/***********************************
/
/   SimGunilla.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 



/*** include ***********************/ 
#include "SimGunilla.h"
#include "../Utl/fstreamExt.h"
#include <float.h>

#include <afxwin.h>
#include <iomanip.h>


/*** Definitions *******************/ 

const float ValueMax = FLT_MAX * (float) 0.99;
const float ValueMin = -ValueMax;

/*** Static ************************/ 

int SimGunilla::m_nFeatureNum[featureLast] = {45, 56, 50, 50, 57, 60, 60};
int SimGunilla::m_group[POS_NUM+1] = { -1, 
    6,7,8,7,6,
   7,4,5,5,4,7,
  8,5,2,3,2,5,8,
 7,5,3,1,1,3,5,7,
6,4,2,1,0,1,2,4,6,
 7,5,3,1,1,3,5,7,
  8,5,2,3,2,5,8,
   7,4,5,5,4,7,
    6,7,8,7,6};

int SimGunilla::m_score[7][7] = {
  {-1, 0, 1, 2, 3, 4, 5},
  { 0,-1, 6, 7, 8, 9,10},
  { 1, 6 -1,11,12,13,14},
  { 2, 7,11,-1,15,16,17},
  { 3, 8,12,15,-1,18,19},
  { 4, 9,13,16,18,-1,20},
  { 5,10,14,17,19,20,-1}};

/*** SimGunilla *********************/ 


/* * * * * * * * * * * * * * * * * */
SimGunilla
::SimGunilla()
//:m_hash(21701, 4)
{
  m_type = Simulation::gunilla;
  m_nFeatureType = feature00;
    
  m_nDepthMax = 0;
  m_nDepthMin = 0;
  m_nTimeOut = 0;

  m_fTemperature = 0;

  m_nCounter = 0;

  m_bTraining = false;
}


/* * * * * * * * * * * * * * * * * */
void SimGunilla
::load(istream& is)
{
  is >> m_nFeatureType;
  is >> m_nDepthMin;
  is >> m_nDepthMax;
  is >> m_nTimeOut;
  is >> m_fTemperature;

  Optimistic_TDlambda::load(is);
}


/* * * * * * * * * * * * * * * * * */
void SimGunilla
::save(ostream& os) const
{
  os << m_nFeatureType  << endl;
  os << m_nDepthMin     << " "; 
  os << m_nDepthMax     << " "; 
  os << m_nTimeOut      << endl;
  os << m_fTemperature  << endl;
  
  Optimistic_TDlambda::save(os);
}



/* * * * * * * * * * * * * * * * * */
void SimGunilla
::modify(Modify &mod)
{
  mod.println("Gunilla computer player, alpha-beta brute, killer moves, based on a neural net");

  // Search depth
  mod.modify("Maximal search depth in plies (0 = inf)", m_nDepthMax, 0, 128);
  mod.modify("Maximal search time in sec (0 = inf)", m_nTimeOut, 0, 10000);
  mod.modify("Minimal search depth in plies (1-8)", m_nDepthMin, 1, 8);
  if (m_nDepthMax != 0 && m_nDepthMax < m_nDepthMin) 
    throw SimulationE("Maximal depth may not be below minimal depth");

  // Temperature
  mod.modify("Simulated annealing temperature (>0)", m_fTemperature, (float)1e-20, (float)1e20, 8);

  // Specify feature type - only if network unused
  if (lLearnStates() == 0)
  {
    // Feature type
    mod.modify("Feature type (0, ...)", m_nFeatureType, 0, featureLast - 1);
  
    // Hidden units
    int nHiddenNum = Optimistic_TDlambda::nHiddenNum();
    mod.modify("Number of hidden units (1-100)", nHiddenNum, 1, 1000);

    // Initialize network
    Optimistic_TDlambda::init(
        m_nFeatureNum[m_nFeatureType],  // input units
        nHiddenNum,                     // hidden units
        bSquashY(),            
        bOnline(),             
        fLambda(),
        fStepInput(),
        fStepHidden());

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
bool SimGunilla
::bIsHuman() const
{ 
  return false; 
}



/* * * * * * * * * * * * * * * * * */
void SimGunilla
::cancel()
{
  //TRACE("SimGunilla::cancel\n");
  m_bCancel = true; 
}



/* * * * * * * * * * * * * * * * * */
ostream & SimGunilla
::write(ostream& os) const 
{
  os << "Max search depth: "  << m_nDepthMax    << " plies\n";
  os << "Max search time: "   << m_nTimeOut     << " sec ";
  os << "(min search depth: " << m_nDepthMin    << " plies)\n";
  os << "Feature type: "      << m_nFeatureType << "\n";
  os << "Temperature: "       << m_fTemperature << endl;
  return Optimistic_TDlambda::write(os);
}



/* * * * * * * * * * * * * * * * * */
float SimGunilla
::vEval(const HashBoardF& boardOri, const char nDepth, float vAlpha, float vBeta)
{
  // Look in HashTable
  float value;
  HashTableF::Quality quality;
  if (nDepth > 0 && m_hash.bFind(boardOri, m_nDepthLimit - nDepth, value, quality)) 
  {
    switch(quality)
    {
    // Exact
    case HashTableF::exact:
      STAT(m_statNew.addCount(Statistics::hashFound, 1));        
      return value;
    // Greater
    case HashTableF::greater:
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
  else if (nDepth > 0) boardOri.allMoves(m_moves[nDepth]);
  // Depth == 0
  else m_pSit->allNewMoves(m_moves[nDepth]);

  // No more moves found
  if (m_moves[nDepth].size() <= 0)
  {
    value = vEvalBoard(boardOri);
    m_hash.store(boardOri, 0, value, HashTableF::exact);
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
  float valueBest = ValueMin;
  int nBest = -1;
  int nEqual = 1;

  for (int i = 0; i < m_moves[nDepth].size(); i++)
  {
    // Move it
    HashBoardF board(boardOri);
    board.moveRaw(m_moves[nDepth][i]);

    // ? game won
    if (board.m_kicked[1 - board.m_playerAct] >= board.m_ejectWon)
      value = ValueMax / (2 * board.m_kicked[board.m_playerAct] + nDepth + 1);
    // Continue recursion
    else
      value = -1 * vEval(board, nDepth + 1, -1 * vBeta, 
        -1 * __max(vAlpha, (nDepth == 0 ? valueBest-0.001 : valueBest)));

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

  STAT(m_statNew.addCount(Statistics::branchFactorQ, i+1));

  // Move has proved to be a killer
  m_killer.add(nDepth, m_moves[nDepth][nBest]);

  // Quiescence
  if (nDepth >= m_nDepthLimit)
  {
    // Store board value in hash table,
    // if value <= alpha no statement about the quality is possible
    if (valueBest > vAlpha)
      m_hash.store(boardOri, 0, valueBest, HashTableF::greater);
  }
  // Regular
  else if (nDepth > 0)
  {
    // Determine the value quality with respect to (alpha,beta)    
    if (valueBest <= vAlpha) quality = HashTableF::lesser;
    else if (valueBest >= vBeta) quality = HashTableF::greater;
    else quality = HashTableF::exact;
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
Move SimGunilla
::calcMove(const Situation& sit)
{
  // ? 2 players
  if (sit.m_playerNum != 2) 
    throw SimulationE("This computer player supports only 2-player games");

  // Init "global" variables 
  m_bCancel = false;
  m_moveBest.m_count = 0;
  srand(Kernel::rand());

  // Training
  if (m_bTraining)
  {
    //LOG3((int)sit.m_playerAct, vEvalBoard(sit), (FullBoard)sit);
    evalTraining(sit);
  }
  // Playing
  else
  {
    STAT(m_statNew.clear());
    STAT(Statistics statLast);
    
    // Save situation for calls of allNewMoves
    m_pSit = &sit;

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
        value = vEval(HashBoardF(m_hash, sit), 0, ValueMin, ValueMax);

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
  }

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
void SimGunilla
::learnBegin(const FullBoard & board)
{
  m_bTraining = true;

  FloatVector x;
  boardToInput(board, x);

  Optimistic_TDlambda::learnBegin(x);
}

/* * * * * * * * * * * * * * * * * */
void SimGunilla
::learnNext(const FullBoard & board)
{
  FloatVector x;
  boardToInput(board, x);

  Optimistic_TDlambda::learnNext(x);
}

/* * * * * * * * * * * * * * * * * */
void SimGunilla
::learnEnd(const FullBoard & board)
{
  float y;

  switch(m_nFeatureType)
  {
  case feature00:
  case feature01:
  case feature02:
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
  case feature05:
    {
      // 0 won
      if (board.m_kicked[0] >= board.m_ejectWon) y = 1;
      // 1 won
      else if (board.m_kicked[1] >= board.m_ejectWon) y = -1;
      // Nobody won
      else y = 0;
      break;
    }
  case feature06:
    {
      y = board.m_kicked[0] - board.m_kicked[1];
      break;
    }


  default:
    throw SimulationE("Illegal Gunilla feature type");
  }

  FloatVector x;
  boardToInput(board, x);

  Optimistic_TDlambda::learnEnd(x, y);

  m_bTraining = false;
}


/* * * * * * * * * * * * * * * * * */
float SimGunilla
::vEvalBoard(const FullBoard & board)
{
  FloatVector x;
  boardToInput(board, x);

  // Set input, calculate output
  float y;
  input(x);
  propagate();
  output(y);
  ASSERT(ValueMin < y && y < ValueMax);

  // The net always calculates the value for player 0
  if (board.m_playerAct != 0) y = -y;

  return y;
}



/* * * * * * * * * * * * * * * * * */
void SimGunilla
::boardToInput(const FullBoard & board, FloatVector & x)
{
  // Set nInputNum size
  x.assign(m_nFeatureNum[m_nFeatureType], 0.0);

  switch(m_nFeatureType)
  {
  // * * * * * * * * * * * * * * * * //
  case feature00:
    {
      int index = 0;
  
      // Nonlinear kicked - difference
      int diff = board.m_kicked[0] - board.m_kicked[1];
      if (diff > 0)
      {
        x[index++] = board.m_kicked[0];
        x[index++] = (diff + board.m_kicked[0]) / 10.0;
      }
      else if (diff < 0)
      {
        x[index++] = -board.m_kicked[1];
        x[index++] = (diff - board.m_kicked[1]) / 10.0;
      }
      else // diff == 0
      {
        x[index++] = 0;
        x[index++] = 0;
      }


      // Kicked difference
      for (int i = 1; i < 6+1; i++)
        x[index++] = (diff >= 0) ? (i <= diff) : -(i <= -diff);

      // Position
      Marble marble;
      Marble marbleNeigh;
      int neighOwn;
      int neighEnemy;
      float factor;
      int indexLoopBegin = index;
      for (Pos p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        
        if (marble > MARBLE_EMPTY)
        {
          // Neighbour count
          neighOwn = 0;
          neighEnemy = 0;
          for (Dir dir = 0; dir < DIR_NUM; dir++)
          {
            marbleNeigh = board.m_marble[Kernel::next[p][dir]];
            if (marbleNeigh == marble) neighOwn++;
            else if (marbleNeigh > MARBLE_EMPTY) neighEnemy++;
          }

          // Position
          factor = marble == 0 ? 0.1 : -0.1;
          index = indexLoopBegin + 4 * m_group[p];
          x[index++] += factor;
          // Neighbours
          x[index++] += neighOwn * factor;
          x[index++] += neighEnemy * factor;
          // Alone
          if (neighOwn + neighEnemy == 0) 
            x[index++] += factor;
        }
      }

      break;
    }

  // * * * * * * * * * * * * * * * * //
  case feature01:
    {
      int i;
      int index = 0;

      // Kicked
      for (i = 1; i < 6+1; i++)
        x[index++] = (i <= board.m_kicked[0]);
      for (i = 1; i < 6+1; i++)
        x[index++] = (i <= board.m_kicked[1]);
  
      // Nonlinear kicked - difference
      int diff = board.m_kicked[0] - board.m_kicked[1];
      if (diff > 0)
      {
        x[index++] = board.m_kicked[0];
        x[index++] = (diff + board.m_kicked[0]) / 10.0;
      }
      else if (diff < 0)
      {
        x[index++] = -board.m_kicked[1];
        x[index++] = (diff - board.m_kicked[1]) / 10.0;
      }
      else // diff == 0
      {
        x[index++] = 0;
        x[index++] = 0;
      }


      // Kicked difference
      for (i = 1; i < 6+1; i++)
        x[index++] = (diff >= 0) ? (i <= diff) : -(i <= -diff);

      // Position
      Marble marble;
      Marble marbleNeigh;
      int neighOwn;
      int neighEnemy;
      float factor;
      int indexLoopBegin = index;
      for (Pos p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        
        if (marble > MARBLE_EMPTY)
        {
          // Neighbour count
          neighOwn = 0;
          neighEnemy = 0;
          for (Dir dir = 0; dir < DIR_NUM; dir++)
          {
            marbleNeigh = board.m_marble[Kernel::next[p][dir]];
            if (marbleNeigh == marble) neighOwn++;
            else if (marbleNeigh > MARBLE_EMPTY) neighEnemy++;
          }

          // Position
          factor = marble == 0 ? 0.1 : -0.1;
          index = indexLoopBegin + 4 * m_group[p];
          x[index++] += factor;
          // Neighbours
          x[index++] += neighOwn * factor;
          x[index++] += neighEnemy * factor;
          // Alone
          if (neighOwn + neighEnemy == 0) 
            x[index++] += factor;
        }
      }

      break;
    }

  // * * * * * * * * * * * * * * * * //
  case feature02:
    {
      int i;
      int index = 0;

      // Kicked
      int diff = board.m_kicked[0] - board.m_kicked[1];
      for (i = 1; i < 6+1; i++)
      {
        if      (diff > 0) x[index++] =   (i <= board.m_kicked[0]);
        else if (diff < 0) x[index++] =  -(i <= board.m_kicked[1]);
        else               x[index++] =    0;
      }
  
      // Nonlinear kicked - difference
      if (diff > 0)
      {
        x[index++] = board.m_kicked[0];
        x[index++] = (diff + board.m_kicked[0]) / 10.0;
      }
      else if (diff < 0)
      {
        x[index++] = -board.m_kicked[1];
        x[index++] = (diff - board.m_kicked[1]) / 10.0;
      }
      else // diff == 0
      {
        x[index++] = 0;
        x[index++] = 0;
      }

      // Kicked difference
      for (i = 1; i < 6+1; i++)
        x[index++] = (diff >= 0) ? (i <= diff) : -(i <= -diff);

      // Position
      Marble marble;
      Marble marbleNeigh;
      int neighOwn;
      int neighEnemy;
      float factor;
      int indexLoopBegin = index;
      for (Pos p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        
        if (marble > MARBLE_EMPTY)
        {
          // Neighbour count
          neighOwn = 0;
          neighEnemy = 0;
          for (Dir dir = 0; dir < DIR_NUM; dir++)
          {
            marbleNeigh = board.m_marble[Kernel::next[p][dir]];
            if (marbleNeigh == marble) neighOwn++;
            else if (marbleNeigh > MARBLE_EMPTY) neighEnemy++;
          }

          // Position
          factor = marble == 0 ? 0.1 : -0.1;
          index = indexLoopBegin + 4 * m_group[p];
          x[index++] += factor;
          // Neighbours
          x[index++] += neighOwn * factor;
          x[index++] += neighEnemy * factor;
          // Alone
          if (neighOwn + neighEnemy == 0) 
            x[index++] += factor;
        }
      }

      break;
    }

    // * * * * * * * * * * * * * * * * //
    case feature03:
    {
      int i;
      int index = 0;

      // Kicked
      int diff = board.m_kicked[0] - board.m_kicked[1];
      for (i = 1; i < 6+1; i++)
      {
        if      (diff > 0) x[index++] =   (i == board.m_kicked[0]);
        else if (diff < 0) x[index++] =  -(i == board.m_kicked[1]);
        else               x[index++] =    0;
      }
  
      // Nonlinear kicked - difference
      if (diff > 0)
      {
        x[index++] = board.m_kicked[0];
        x[index++] = (diff + board.m_kicked[0]) / 10.0;
      }
      else if (diff < 0)
      {
        x[index++] = -board.m_kicked[1];
        x[index++] = (diff - board.m_kicked[1]) / 10.0;
      }
      else // diff == 0
      {
        x[index++] = 0;
        x[index++] = 0;
      }

      // Kicked difference
      for (i = 1; i < 6+1; i++)
        x[index++] = (diff >= 0) ? (i <= diff) : -(i <= -diff);

      // Position
      Marble marble;
      Marble marbleNeigh;
      int neighOwn;
      int neighEnemy;
      float factor;
      int indexLoopBegin = index;
      for (Pos p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        
        if (marble > MARBLE_EMPTY)
        {
          // Neighbour count
          neighOwn = 0;
          neighEnemy = 0;
          for (Dir dir = 0; dir < DIR_NUM; dir++)
          {
            marbleNeigh = board.m_marble[Kernel::next[p][dir]];
            if (marbleNeigh == marble) neighOwn++;
            else if (marbleNeigh > MARBLE_EMPTY) neighEnemy++;
          }

          // Position
          factor = marble == 0 ? 0.1 : -0.1;
          index = indexLoopBegin + 4 * m_group[p];
          x[index++] += factor;
          // Neighbours
          x[index++] += neighOwn * factor;
          x[index++] += neighEnemy * factor;
          // Alone
          if (neighOwn + neighEnemy == 0) 
            x[index++] += factor;
        }
      }

      break;
    }

    // * * * * * * * * * * * * * * * * //
    case feature04:
    {
      int index = 21;

      // Score
      int diff = board.m_kicked[0] - board.m_kicked[1];
      if (diff > 0)
        x[m_score[board.m_kicked[1]][board.m_kicked[0]]] = 1;
      else if (diff < 0)
        x[m_score[board.m_kicked[0]][board.m_kicked[1]]] = -1;

      // Position
      Marble marble;
      Marble marbleNeigh;
      int neighOwn;
      int neighEnemy;
      float factor;
      int indexLoopBegin = index;
      for (Pos p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        
        if (marble > MARBLE_EMPTY)
        {
          // Neighbour count
          neighOwn = 0;
          neighEnemy = 0;
          for (Dir dir = 0; dir < DIR_NUM; dir++)
          {
            marbleNeigh = board.m_marble[Kernel::next[p][dir]];
            if (marbleNeigh == marble) neighOwn++;
            else if (marbleNeigh > MARBLE_EMPTY) neighEnemy++;
          }

          // Position
          factor = marble == 0 ? 0.1 : -0.1;
          index = indexLoopBegin + 4 * m_group[p];
          x[index++] += factor;
          // Neighbours
          x[index++] += neighOwn * factor;
          x[index++] += neighEnemy * factor;
          // Alone
          if (neighOwn + neighEnemy == 0) 
            x[index++] += factor;
        }
      }
      break;
    }

    // * * * * * * * * * * * * * * * * //
    case feature05:
    case feature06:
    {
      int index = 0;
      // Score
      x[index++] = board.m_kicked[0];
      x[index++] = board.m_kicked[1];
      x[index++] = board.m_kicked[0] - board.m_kicked[1];

      // Score - classes
      int diff = board.m_kicked[0] - board.m_kicked[1];
      if (diff > 0)
        x[ index + m_score[board.m_kicked[1]][board.m_kicked[0]] ] = 1;
      else if (diff < 0)
        x[ index + m_score[board.m_kicked[0]][board.m_kicked[1]] ] = -1;
      index += 21;

      // Position
      Marble marble;
      Marble marbleNeigh;
      int neighOwn;
      int neighEnemy;
      float factor;
      int indexLoopBegin = index;
      for (Pos p = 1; p <= POS_NUM; p++)
      {
        marble = board.m_marble[p];
        
        if (marble > MARBLE_EMPTY)
        {
          // Neighbour count
          neighOwn = 0;
          neighEnemy = 0;
          for (Dir dir = 0; dir < DIR_NUM; dir++)
          {
            marbleNeigh = board.m_marble[Kernel::next[p][dir]];
            if (marbleNeigh == marble) neighOwn++;
            else if (marbleNeigh > MARBLE_EMPTY) neighEnemy++;
          }

          // Position
          factor = marble == 0 ? 0.1 : -0.1;
          index = indexLoopBegin + 4 * m_group[p];
          x[index++] += factor;
          // Neighbours
          x[index++] += neighOwn * factor;
          x[index++] += neighEnemy * factor;
          // Alone
          if (neighOwn + neighEnemy == 0) 
            x[index++] += factor;
        }
      }
      break;
    }

  default:
    throw SimulationE("Illegal Gunilla feature type");
  }
}


/* * * * * * * * * * * * * * * * * */
void SimGunilla
::evalTraining(const Situation & sit)
{
  MoveVector moves;
  sit.allNewMoves(moves);

  FullBoard board;
  float fValue;
  float fBias;
  int nSel = -1;
  double fProb;
  double fProbSum = 0;
  
  for (int i = 0; i < moves.size(); i++)
  {
    // Board after ith move
    board = sit;
    board.moveRaw(moves[i]);

    // Boltzmann
    fValue = -vEvalBoard(board);

    if (m_nFeatureType == feature06)
    {
      // Sharpen the difference between the values
      if (i == 0) fBias = fValue;
      fValue -= fBias;
    }

    fProb = exp(fValue / m_fTemperature);
    fProbSum += fProb;
    
    // Overflow
    if (!(DBL_MIN < fProb && fProb < DBL_MAX &&
      DBL_MIN < fProbSum && fProbSum < DBL_MAX)) 
    {
      //throw SimulationE("Overflow - temperature too low");
      g_log << "Gunilla temperature too low" << endl;
      m_fTemperature *= (float) 1.1;

      // Restart loop
      i = 0;
      fProbSum = 0;
      nSel = -1;
      continue;
    }
    
    if (Kernel::rand(0,1) <= (fProb/fProbSum)) nSel = i;
    
/*#ifdef _DEBUG
    char sz[256];
    sprintf(sz, "%-3d  sel %-3d  val %-7f  prob %-e  sum %-e  quo %-g\n",
      i, nSel, value, fProb, fProbSum, fProb/fProbSum);
    g_logDebug << sz;
#endif*/
  }
  if (nSel < 0) throw SimulationE("No move selected - temperature too low");

  // Return selected move
  m_moveBest = moves[nSel];
  //LOG2("Selected", nSel);
}