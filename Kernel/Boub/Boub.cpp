/*

  (C) LIPSOFT 1994
  
    
*/


/******** own functions list **********

  main
  
********* own functions list end ******/


/************* includes ***************/

#include "../Kernel.h"
#include "../Situation.h"
#include "BOUB.H" 

#include <STRING.H>
#include <STDLIB.H>
#include <STDIO.H>
#include <CONIO.H>
#include <TIME.H>


/********** includes end **************/




/********** defines *******************/


/*********** defines end **************/




/********** structures ****************/


/********** structures end ************/


namespace boub
{
  /*********** global variables ********/
  
  FIELDPOS g_pAbaNeigh[NUM_STONES+1][NUM_DIR];
  char g_nAbaDirAttack[NUM_STONES+1][NUM_ATTACK_DIR];
  unsigned char g_cAbaDistBorder[NUM_STONES+1];
  
  /* Cancel */
  volatile bool g_bCancel;
  
  /* Holds the computer players properties */
  COMPPLAYER g_cp;
  
  /* Calculation read-only's */
  float g_fPointsKicked[NUM_KICKS][NUM_KICKS];
  
  /* Players information */
  int g_nPlayerBase, g_nPlayerNum;
  
  /* Read-only */
  float g_fRetTol;
  int g_nKickedWon;
  
  /* History */
  PLAYFIELD g_pfHist[NUM_HIST];
  int g_nHist;
  
  /* Debug */
  ABAGAME g_agDebug;
  char szBuffer[160];
  
  /******** global variables end ********/
  
  
  
  
  /*********** C functions end **********/
  
  
  
  
  /************ functions def ***********
  
    void GfxFieldShow (ABAGAME *);
    char GfxMsg (char *sz, char bRet);
    void GfxStoneMark (FIELDPOS p);
    void GfxStoneUnMark (FIELDPOS p);
    
  /******** functions def end ***********/
  
  
  
  
  /********** own functions *************/
  
#ifdef _DEBUG
  void LOGBoard(PLAYFIELD *pf)
  {
    Situation sit;
    for (int n = 0; n <= NUM_STONES; n++)
      sit.m_marble[n] = pf->cField[n];
    
    for (n = 0; n < NUM_PLAYERS; n++)
    {
      sit.m_lost[n] = pf->nLost[n];
      sit.m_kicked[n] = pf->nKicked[n];
    }
    
    sit.m_playerNum = 2;
    sit.write(g_log);
  }
  
#else
#define   LOGBoard(_pf)
#endif  // _DEBUG
  
  
  char bSimAttacked (PLAYFIELD *, FIELDPOS, float[]);
  
  
  int SimStoneMoveLine (PLAYFIELD *pf, MOVE *move)
    /* EV_OK, EV_KICKED, EV_ERROR */
  {
    int nPlayerAct, nPlayerPushed;
    FIELDPOS pAct= move->p[0];
    FIELDPOS pLine[NUM_SIM_MOVES];
    int i, nStoneOwn, nAllOver;
    
    nPlayerAct = pf->cField[pAct];
    
    for (i=0, nAllOver=0; i<NUM_SEL; i++,nAllOver++)
    {
      if (pf->cField[pAct] != nPlayerAct) break;
      pLine[nAllOver]= pAct;
      pAct= g_pAbaNeigh[pAct][move->nDir];
    }
    if (pAct == 0) return (EV_ERROR);
    
    for (nStoneOwn=i,i=0; i<nStoneOwn; i++,nAllOver++)
    {
      if (pf->cField[pAct] == nPlayerAct) return (EV_ERROR);
      pLine[nAllOver]= pAct;
      if (pf->cField[pAct] == ST_EMPTY || pAct == 0) break;
      pAct= g_pAbaNeigh[pAct][move->nDir];
    }
    if (i >= nStoneOwn) return (EV_ERROR);
    
    /* Everything ok, move marbles */
    for (i = nAllOver; i > 0; i--)
    {
      nPlayerPushed = pf->cField[pLine[i-1]];
      pf->cField[pLine[i]]= nPlayerPushed;
    }
    pf->cField[pLine[0]]= ST_EMPTY;
    
    /* ? nobody kicked */
    if (pf->cField[0] == ST_BORDER) return EV_OK;
    
    /* Somebody kicked */
    if (pf->nWon == ST_EMPTY)
    {
      /* Do only update if game still is going on - nobody won */
      pf->nLost[pf->cField[0]]++;
      pf->nKicked[nPlayerAct]++;
    }
    if (pf->nKicked[nPlayerAct] >= g_nKickedWon)
    {
      /* This guy won */
      pf->nWon = nPlayerAct;
    }
    
    pf->cField[0] = ST_BORDER;
    return EV_KICKED;
  }
  
  
  /* Calculate points for given situation
  *
  * OUT fPoints[]
  * IN pf
  */
  
  void SimPoints (float fPoints[], PLAYFIELD *pf)
  {
    int nDir;
    FIELDPOS p;
    int nNeighFriend, nNeighEnemy, nNeighBorder;
    int nPlayer, nPlayer2;
    
    /* Kicked marbles */
    for (nPlayer = 0; nPlayer < g_nPlayerNum; nPlayer++)
    {
      for (nPlayer2 = 0; nPlayer2 < g_nPlayerNum; nPlayer2++)
      {
        if (nPlayer != nPlayer2)
          fPoints[nPlayer] +=
          g_fPointsKicked[pf->nKicked[nPlayer]][pf->nKicked[nPlayer2]];
      }
    }
    
    /* Loop through all marbles */
    for (p=1; p<=NUM_STONES; p++)
    {
      nPlayer = pf->cField[p];
      if (nPlayer != ST_EMPTY)
      {
        /* Distance to centerpoint */
        fPoints[nPlayer] += g_cp.fPoints[g_cAbaDistBorder[p]];
        
        /* Friends, loneliness, border */
        nNeighFriend= nNeighEnemy= nNeighBorder= 0;
        for (nDir=0; nDir<NUM_DIR; nDir++)
        {
          switch (pf->cField[g_pAbaNeigh[p][nDir]])
          {
          case ST_BORDER:
            nNeighBorder++;
            break;
          case ST_EMPTY:
            break;
          default:
            {
              if (pf->cField[g_pAbaNeigh[p][nDir]] == nPlayer) nNeighFriend++;
              else nNeighEnemy++;
            }
          }
        }
        /* Friends around me */
        fPoints[nPlayer] += g_cp.fPoints[POI_NEIGH] * (float) nNeighFriend;
        
        /* Enemy in front of me, border behind me */
        if (nNeighBorder != 0 && nNeighEnemy != 0)
          fPoints[nPlayer] += g_cp.fPoints[POI_BORDER_ENEMY] * nNeighEnemy;
        
        /* Alone */
        if (nNeighFriend == 0)
        {
          fPoints[nPlayer] += g_cp.fPoints[POI_ALONE];
          if (g_cAbaDistBorder[p] <= 2)
            fPoints[nPlayer] += g_cp.fPoints[POI_ALONE] * (float) nNeighEnemy;
        }
        
        /* Attacked */
        bSimAttacked (pf, p, fPoints);
      }
    }
  }
  
  
  char bSimAttacked (PLAYFIELD *pf, FIELDPOS pOri, float *fPoints)
  {
    char bAttacked = FALSE;
    int n, nDir;
    
    FIELDPOS pAct;
    int nPlayerAct, nPlayerAttack;
    int nOwn, nAttack;
    
    for (n = 0; n < NUM_ATTACK_DIR; n++)
    {
      nDir = g_nAbaDirAttack[pOri][n];
      if (nDir >= NUM_DIR) continue;
      
      /* nDir is a possible attack direction */
      nPlayerAct = pf->cField[pOri];
      nOwn= 1;
      pAct= g_pAbaNeigh[pOri][nDir];
      
      /* Count my own marbles */
      if (pf->cField[pAct] == nPlayerAct)
      {
        nOwn++;
        pAct= g_pAbaNeigh[pAct][nDir];
      }
      
      /* Are there enemy marbles ? */
      nPlayerAttack= pf->cField[pAct];
      if (nPlayerAttack==ST_EMPTY || nPlayerAttack==nPlayerAct) continue;
      
      /* Count enemy marbles */
      for (nAttack=1; nAttack<=NUM_SEL;)
      {
        pAct= g_pAbaNeigh[pAct][nDir];
        if (pf->cField[pAct] != nPlayerAttack) break;
        nAttack++;
      }
      
      /* ? are we stronger than enemy */
      if (nOwn >= nAttack) continue;
      
      /* We are attacked ! */
      bAttacked = TRUE;
      fPoints[nPlayerAct] -= g_cp.fPoints[POI_ATTACK] * nOwn;
      fPoints[nPlayerAttack] += g_cp.fPoints[POI_ATTACK] * nAttack;
    }
    return (bAttacked);
  }
  
  
  void SimSafeAdd (SAFE *safe, PLAYFIELD *pf, float fValue, float fPoints[],
    char bUrgent, int nDepth)
  {
    int p, pBefore;
    int n;
    int pStore = -1;
    
    /* ? enough space in safe */
    if (safe->nMoves >= NUM_SIM_MOVES)
    {
      printf("Temporary move-safe full !\n");
      return;
    }
    
    /*** Urgent move ***/
    if (bUrgent)
    {
      pStore = safe->pUpper;
      /* Update list */
      safe->pUpper = ++safe->nMoves;
      safe->nNext[pStore] = safe->pUpper;
    }
    
    /*** Regular move ***/
    else
    {
      /* Insert at right postition */
      for (p = safe->pLower, n = 0;
      n < safe->nMovesReg && fValue > safe->fValue[p];
      pBefore = p, p = safe->nNext[p], n++) ;
      
      /* Still enough space */
      if (safe->nMovesReg < g_cp.nMoves[nDepth])
      {
        safe->nMovesReg++;
        pStore = ++safe->nMoves;
        if (n == 0) safe->pLower = pStore;
        else safe->nNext[pBefore] = pStore;
        safe->nNext[pStore] = p;
      }
      /* Safe already full, replace pLower */
      else if (n == 1) pStore = safe->pLower;
      /* Safe full, insert */
      else if (n > 1)
      {
        pStore = safe->pLower;
        safe->pLower = safe->nNext[safe->pLower];
        safe->nNext[pBefore] = pStore;
        safe->nNext[pStore] = p;
      }
    }
    
    /* ? store move */
    if (pStore >= 0)
    {
      /* Store field and points */
      safe->pf[pStore] = *pf;
      safe->fValue[pStore] = fValue;
      for (n = 0; n < g_nPlayerNum; n++)
        safe->fPoints[n][pStore] = fPoints[n];
    }
  }
  
  /* Calculates a situations value for a given player and
  * a given points distribution.
  */
  float fSimValue (int nPlayer, float fPoints[])
  {
    int n;
    float f = 0;
    
    for (n = 0; n < g_nPlayerNum; n++)
    {
      if (n == nPlayer) f += fPoints[n] * g_cp.fPlayerFactor[0];
      else
      {
        f -= fPoints[n] *
          g_cp.fPlayerFactor[(n - nPlayer + g_nPlayerNum) % g_nPlayerNum];
      }
    }
    return f;
  }
  
  
  /* Loops as long as situation is not calm.
  */
  void SimQuiesence (int a_nDepth, PLAYFIELD *a_pf, float *a_fPoints)
  {
    int nPlayer;
    MOVE move;
    PLAYFIELD pf;
    
    float fPoints[NUM_PLAYERS];
    float fValue, fValueBest = (float)-1e35;
    
    int n;
    
    /* ? cancelled */
    if (g_bCancel == TRUE) return;
    
    /* Determine actual player */
    nPlayer = (g_nPlayerBase+ a_nDepth) % g_nPlayerNum;
    
    //LOG2("Quiesence depth ", a_nDepth);
    //LOGBoard(a_pf);
    
    /* Loop through all moves */
    move.nCount = 1;
    for (move.p[0] = 1; move.p[0] <= NUM_STONES; move.p[0]++)
    {
      pf = *a_pf;
      /* ? one of our marbles */
      if (pf.cField[move.p[0]] == nPlayer)
      {
        /* Loop through all directions */
        for (move.nDir = 0; move.nDir < NUM_DIR; move.nDir++)
        {
          /* Restore field */
          pf= *a_pf;
          if (SimStoneMoveLine (&pf, &move) != EV_KICKED) continue;
          //LOG("Quiesence kicked");
          
          /* Kicked a marble */
          SimQuiesence(a_nDepth + 1, &pf, fPoints);
          fValue = fSimValue (nPlayer, fPoints);
          
          if (fValue > fValueBest)
          {
            fValueBest = fValue;
            for (n = 0; n < g_nPlayerNum; n++)
              a_fPoints[n] = fPoints[n];
          }
        }
      }
    }
    
    /* ? Found no kick move */
    if (fValueBest <= (float) -1e35)
    {
      for (n = 0; n < g_nPlayerNum; n++) a_fPoints[n] = 0.0;
      SimPoints(a_fPoints, a_pf);
    }
  }
  
  
  
  
  void SimShowValues (PLAYFIELD *pf, float fPoints[])
  {
    g_agDebug.pf = *pf;
    //GfxFieldShow (&g_agDebug);
    printf(szBuffer, "Points(%.1f, %.1f)  Val(%.1f, %.1f)",
      fPoints[0], fPoints[1],
      fSimValue (0, fPoints), fSimValue (1, fPoints));
    //GfxMsg (szBuffer, TRUE);
  }
  
  
  /* The recursion. Move for move each situation is evaluated and
  * the best move selected.
  *
  * IN a_nDepth is the recursion level
  * IN OUT a_pf is only changed when a_nDepth == 0
  * OUT a_fPoints is always changed
  */
  void Simulation (int a_nDepth, PLAYFIELD *a_pf, float *a_fPoints)
  {
    int nPlayer;
    PLAYFIELD pf;
    MOVE move;
    char bUrgent, bAttack;
    int nEvent;
    
    SAFE safe = {0, 0, 0, 0, {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} };
    int p;
    
    float fPoints[NUM_PLAYERS];
    float fValue, fValueBest = (float)-1e35;
    
    char bDepthMax;
    
    int n;
    char bOk;
    
    /* ? cancelled */
    if (g_bCancel == TRUE) 
      return;
    
    //float fValueDebug[NUM_SIM_MOVES + 1];
    //SAFE safeDebug;
    
    /* Determine actual player */
    nPlayer = (g_nPlayerBase+ a_nDepth) % g_nPlayerNum;
    
    //LOG2("Simulation depth ", a_nDepth);
    //LOGBoard(a_pf);
    
    /*** Loop through all positions ***/
    bDepthMax = (a_nDepth + 1 >= g_cp.nDepth);
    move.nCount= 1;
    
    for (move.p[0] = 1; move.p[0] <= NUM_STONES; move.p[0]++)
    {
      pf = *a_pf;
      /* ? one of our marbles */
      if (pf.cField[move.p[0]] == nPlayer)
      {
        bAttack = bSimAttacked (&pf, move.p[0], fPoints);
        
        /* Loop through all directions */
        for (move.nDir = 0; move.nDir < NUM_DIR; move.nDir++)
        {
          pf = *a_pf;
          nEvent = SimStoneMoveLine (&pf, &move);
          if (nEvent == EV_ERROR) continue;
          
          /* Possible move ! */
          
          /* ? last iteration */
          if (bDepthMax)
          {
            SimQuiesence(a_nDepth + 1, &pf, fPoints);
            fValue = fSimValue (nPlayer, fPoints);
            
            if (fValue > fValueBest)
            {
              fValueBest = fValue;
              for (n = 0; n < g_nPlayerNum; n++)
                a_fPoints[n] = fPoints[n];
            }
          }
          /* ... still some moves to iterate */
          else
          {
            for (n = 0; n < g_nPlayerNum; n++) fPoints[n] = 0.0;
            SimPoints (fPoints, &pf);
            fValue = fSimValue (nPlayer, fPoints);
            
            bOk = TRUE;
            if (a_nDepth == 0 && g_nHist > 0)
            {
              for (n = 0; n < g_nHist; n++)
                if (memcmp(&pf, &g_pfHist[n], sizeof (PLAYFIELD)) == 0)
                {
                  bOk = FALSE;
                  break;
                }
            }   
            if (bOk)
            {
              bUrgent = bAttack || (nEvent == EV_KICKED);
              SimSafeAdd (&safe, &pf, fValue, fPoints, bUrgent, a_nDepth);
            }
          }
        }
      }
    }
    
    if (bDepthMax)
    {
      for (n = 0; n < g_nPlayerNum; n++)
        a_fPoints[n] = a_fPoints[n] * g_cp.fDepthFactor[a_nDepth];
      return;
    }
    
    for (p = safe.pLower; p != safe.pUpper; p = safe.nNext[p])
    {
      /* Iterate through next move */
      Simulation (a_nDepth + 1, &safe.pf[p], fPoints);
      for (n = 0; n < g_nPlayerNum; n++)
        fPoints[n] += safe.fPoints[n][p] * g_cp.fDepthFactor[a_nDepth];
      fValue = fSimValue (nPlayer, fPoints);
      
      /* Level 0: pass back chosen move */
      if (a_nDepth == 0)
      {
        //SimShowValues(&safe.pf[p], fPoints);
        if (fValue >= fValueBest * (1.0 + g_cp.fPoints[POI_TOLERANCE]) ||
          (fValue >= fValueBest * (1.0 - g_cp.fPoints[POI_TOLERANCE]) &&
          Kernel::rand(2)) )
          *a_pf = safe.pf[p];
      }
      
      /* Identify best value */
      if (fValue > fValueBest)
      {
        fValueBest = fValue;
        for (n = 0; n < g_nPlayerNum; n++)
          a_fPoints[n] = fPoints[n];
      }
    }
 }
 
 
 
 /* Initializes some global variables, all of them read-only.
 * This is ugly but faster.
 */
 void SimInitGlobals (ABAGAME *ag)
 {
   int n, m;
   float fTotal, fDiff;
   
   /* Some constants */
   g_nPlayerBase= ag->nPlayerAct;
   g_nPlayerNum= ag->nPlayerNum;
   
   g_fRetTol= (float)NUM_TOL * g_cp.nDepth;
   
   g_nKickedWon = ag->nKickedWon;
   
   /* Points for kicked marbles */
   fDiff = g_cp.fPoints[POI_KICKED] / (float)2.0;
   for (n = 0; n <= g_nKickedWon ; n++)
   {
     g_fPointsKicked[n][n] = 0.0;
     
     fTotal = (float)n * g_cp.fPoints[POI_KICKED];
     if (n == g_nKickedWon) fTotal *= 2.0;
     
     for (m = 0; m < n; m++, fTotal -= fDiff)
     {
       g_fPointsKicked[n][m] = fTotal;
       g_fPointsKicked[m][n] = (float)-1.0 * fTotal;
     }
   }
   
   /* Debug */
   g_agDebug = *ag;
   
   /* Prevent replaying history */
   g_nHist = 0;
   for (n = ag->nHistLower; n != ag->nHistUpper; n = (n + 1) % NUM_HIST)
   {
     if (ag->hist[n].nPlayerAct == ag->nPlayerAct &&
       memcmp(&ag->hist[n].pf, &ag->pf, sizeof (PLAYFIELD)) == 0)
     {
       g_pfHist[g_nHist] = ag->hist[(n + 1) % NUM_HIST].pf;
       g_nHist++;
     }
   }
 }
 
 
 
 /* This is the Simulation main method. It is called by the
 * framework.
 * It prepares some (read-only) global variables, then starts
 * the simulation recursion and passes back the result.
 */
 int SimMove (ABAGAME *ag, const COMPPLAYER *cp)
 {
   int nRet = EV_OK;
   
   float fPoints[NUM_PLAYERS] = {0, 0, 0, 0};
   PLAYFIELD pf;
   
   FIELDPOS p, pOri, pDest;
   
   /* Preparation */
   SimInitGlobals (ag);
   g_cp = *cp;
   
   /* Start recursion */
   pf = ag->pf;
   pf.nWon = ST_EMPTY;
   Simulation (0, &pf, fPoints);
   
   /* Look for moved marbles */
   for (p = 1, pOri = 0, pDest = 0; p <= NUM_STONES; p++)
   {
     if (pf.cField[p] == ST_EMPTY && ag->pf.cField[p] != ST_EMPTY) pOri = p;
     if (pf.cField[p] != ST_EMPTY && ag->pf.cField[p] == ST_EMPTY) pDest = p;
   }
   
   /* ? was a move made */
   if (pOri == 0) return (EV_NOMOVE);
   
   /* Copy result into structure */
   ag->pf = pf;
   
   /* Ok */
   return (nRet);
 }
 
 
 
 ostream & COMPPLAYER::write(ostream &os) const
 {
   /* nMoves */
   os << "Plies " << nDepth << ": ";
   for (int i = 0; i < NUM_LEVELS; i++)
     os << nMoves[i] << "  ";
   
   /* fDepthFactor */
   os << "\n  fDepthFactor: ";
   for (i = 0; i < NUM_LEVELS; i++)
     os << fDepthFactor[i] << "  ";
   
   /* fPlayerFactor */
   os << "\n  fPlayerFactor: ";
   for (i = 0; i < NUM_PLAYERS; i++)
     os << fPlayerFactor[i] << "  ";
   
   /* fPoints */
   os << "\n  fPoints: ";
   for (i = 0; i < POI_LAST; i++)
     os << fPoints[i] << "  ";
   
   os << endl;

   return os;
 }
 
 
 /********** own functions end *********/
 
}