/*

(C) LIPSOFT 1994



void beispiel (void)
 {

 }

*/


/******** own functions list **********

 main

********* own functions list end ******/




/************* includes ***************/

#include "ABA2.H"

#include <STRING.H>
#include <STDLIB.H>
#include <CONIO.H>
#include <TIME.H>

/********** includes end **************/




/********** defines *******************/


/*********** defines end **************/




/********** structures ****************/


/********** structures end ************/





/*********** global variables ********/

extern FIELDPOS g_pAbaNeigh[NUM_STONES+1][NUM_DIR];
extern char g_nAbaDirAttack[NUM_STONES+1][NUM_ATTACK_DIR];
extern unsigned char g_cAbaDistBorder[NUM_STONES+1];

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




/************ C functions *************/

int sprintf (char *string,const char *format, ...);
void delay (unsigned ms);

/*********** C functions end **********/




/************ functions def ***********/

void GfxFieldShow (ABAGAME *);
char GfxMsg (char *sz, char bRet);
void GfxStoneMark (FIELDPOS p);
void GfxStoneUnMark (FIELDPOS p);

/******** functions def end ***********/




/********** own functions *************/

char bSimAttacked (PLAYFIELD *, FIELDPOS, float[]);


int SimStoneMoveLine (PLAYFIELD *pf, MOVE *move, float fPoints[])
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
    if (nPlayerPushed != nPlayerAct)
      fPoints[nPlayerPushed] += g_cp.fPoints[POI_PUSHED];
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

      /* Intruder */
      if (nNeighEnemy >= 5) fPoints[nPlayer] += g_cp.fPoints[POI_INTRUDER] *
	(float) g_cAbaDistBorder[p];

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
  char bUrgent, int nMovesRegMax)
 {
  int p, pBefore;
  int n;
  int pStore = -1;

  /* ? enough space in safe */
  if (safe->nMoves >= NUM_SIM_MOVES)
   {
    GfxMsg ("Temporary move-safe full !", FALSE);
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
    if (safe->nMovesReg < nMovesRegMax)
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


void SimShowValues (PLAYFIELD *pf, float fPoints[])
 {
  g_agDebug.pf = *pf;
  GfxFieldShow (&g_agDebug);
  sprintf(szBuffer, "Points(%.1f, %.1f)  Val(%.1f, %.1f)",
    fPoints[0], fPoints[1],
    fSimValue (0, fPoints), fSimValue (1, fPoints));
  GfxMsg (szBuffer, TRUE);
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
  
  SAFE *safe;
  int p;
  
  float fPoints[NUM_PLAYERS];
  float fValue, fValueBest = -1e35;
  
  char bDepthMax, bChain;
  
  int nMovesRegMax;
  float fDepthFactor;
  
  int n, nMove;
  char bOk;
  
  //float fValueDebug[NUM_SIM_MOVES + 1];
  //SAFE safeDebug;
  
  /* Determine actual player */
  nPlayer = (g_nPlayerBase+ a_nDepth) % g_nPlayerNum;
  
  /* Determine some constants */
  if (a_nDepth < g_cp.nDepth - 1)
  {
    nMovesRegMax = g_cp.nMoves[a_nDepth];
    fDepthFactor = g_cp.fDepthFactor[a_nDepth];
    bDepthMax = FALSE;
  }
  else
  {
    nMovesRegMax = g_cp.nMoves[g_cp.nDepth - 1];
    fDepthFactor = g_cp.fDepthFactor[g_cp.nDepth - 1];
    bDepthMax = TRUE;
  }
  
  if ((safe = malloc(sizeof(SAFE))) == NULL)
    aba2flash (ERR_MEM, "Creating safe");
  safe->pLower = safe->pUpper = safe->nMoves = safe->nMovesReg = 0;
  
  /*** Loop through all positions ***/
  pf = *a_pf;
  move.nCount= 1;
  bChain = FALSE;
  
  for (move.p[0] = 1; move.p[0] <= NUM_STONES; move.p[0]++)
  {
    /* ? one of our marbles */
    if (pf.cField[move.p[0]] == nPlayer)
    {
      bAttack = bSimAttacked (&pf, move.p[0], fPoints);
      
      /* Loop through all directions */
      for (move.nDir = 0; move.nDir < NUM_DIR; move.nDir++)
      {
        for (n = 0; n < g_nPlayerNum; n++) fPoints[n] = 0.0;
        nEvent = SimStoneMoveLine (&pf, &move, fPoints);
        if (nEvent == EV_ERROR) continue;
        
        /* Possible move ! */
        SimPoints (fPoints, &pf);
        fValue = fSimValue (nPlayer, fPoints);
        bUrgent = bAttack || (nEvent == EV_KICKED);
        
        /* ? last iteration && no urengcy */
        if (bDepthMax && !bUrgent)
        {
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
          bOk = TRUE;
          if (a_nDepth == 0 && g_nHist > 0)
            for (n = 0; n < g_nHist; n++)
              if (memcmp(&pf, &g_pfHist[n], sizeof (PLAYFIELD)) == 0)
              {
                bOk = FALSE;
                break;
              }
              
              if (bOk)
              {
                SimSafeAdd (safe, &pf, fValue, fPoints, bUrgent, nMovesRegMax);
                if (bUrgent)
                  bChain = TRUE;
              }
        }
        /* Restore field and points */
        pf= *a_pf;
      }
    }
  }
  
  if (bDepthMax)
  {
    if (bChain && a_nDepth < 6)
      fDepthFactor = 0.0;
    else
    {
      for (n = 0; n < g_nPlayerNum; n++)
        a_fPoints[n] = a_fPoints[n] * fDepthFactor;
      free(safe);
      return;
    }
  }
  
  for (nMove = 0, p = safe->pLower; nMove < safe->nMoves;
  p = safe->nNext[p], nMove++)
  {
    /* Iterate through next move */
    Simulation (a_nDepth + 1, &safe->pf[p], fPoints);
    for (n = 0; n < g_nPlayerNum; n++)
      fPoints[n] += safe->fPoints[n][p] * fDepthFactor;
    fValue = fSimValue (nPlayer, fPoints);
    
    /* Level 0: pass back chosen move */
    if (a_nDepth == 0)
    {
      //SimShowValues(&safe.pf[p], fPoints);
      if (fValue >= fValueBest * (1.0 + g_cp.fPoints[POI_TOLERANCE]) ||
        (fValue >= fValueBest * (1.0 - g_cp.fPoints[POI_TOLERANCE]) &&
        random (2)) )
        *a_pf = safe->pf[p];
    }
    
    /* Identify best value */
    if (fValue > fValueBest)
    {
      fValueBest = fValue;
      for (n = 0; n < g_nPlayerNum; n++)
        a_fPoints[n] = fPoints[n];
    }
  }
  free(safe);
 }



/* Initializes some global variables, all of them read-only.
 * This is ugly but faster.
 */
void SimInitGlobals (ABAGAME *ag)
 {
  int n, m;
  float fTotal, fDiff;

  /* Some constants */
  g_cp = ag->player[ag->nPlayerAct].cp;

  g_nPlayerBase= ag->nPlayerAct;
  g_nPlayerNum= ag->nPlayerNum;

  g_fRetTol= NUM_TOL * g_cp.nDepth;

  g_nKickedWon = ag->nKickedWon;

  /* Points for kicked marbles */
  fDiff = g_cp.fPoints[POI_KICKED] / 2.0;
  for (n = 0; n <= g_nKickedWon ; n++)
   {
    g_fPointsKicked[n][n] = 0.0;

    fTotal = (float)n * g_cp.fPoints[POI_KICKED];
    if (n == g_nKickedWon) fTotal *= 2.0;

    for (m = 0; m < n; m++, fTotal -= fDiff)
     {
      g_fPointsKicked[n][m] = fTotal;
      g_fPointsKicked[m][n] = -1.0 * fTotal;
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
int SimMove (ABAGAME *ag, int nWait)
 {
  int nRet = EV_OK;

  float fPoints[NUM_PLAYERS] = {0, 0, 0, 0};
  PLAYFIELD pf;

  FIELDPOS p, pOri, pDest;

  /* Preparation */
  SimInitGlobals (ag);

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

  /* Show move */
  GfxStoneMark (pOri);
  if (nWait == WAIT_USER)
   {
    sprintf (szBuffer, "%s (%d) has chosen its move",
      ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
    if (GfxMsg (szBuffer, TRUE) == 27) nRet = EV_ESC;
   }
  else if (nWait != WAIT_FAST) delay (1000);
  if (pDest != 0)
   {
    GfxStoneMark (pDest);
    if (nWait != WAIT_FAST) delay (500);
   }
  GfxFieldShow (ag);
  GfxStoneUnMark (pOri);
  if (pDest != 0)
   {
    if (nWait != WAIT_FAST) delay (500);
    GfxStoneUnMark (pDest);
   }

  /* Look for Escape */
  while (kbhit ())
    if (getch () == 27) nRet= EV_ESC;

  /* Ok */
  return (nRet);
 }



/********** own functions end *********/



