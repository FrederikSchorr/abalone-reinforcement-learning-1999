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
float g_fPointsKicked[NUM_KICKS];
float g_fPointsLost[NUM_KICKS];

/* Players information */
int g_nPlayerBase, g_nPlayerNum;

/* Read-only */
int g_fRetTol;

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

int SimStoneMoveLine (PLAYFIELD *pf, MOVE *move, float fPoints[])
/* EV_OK, EV_KICKED, EV_ERROR */
 {
  int nPlayerAct;
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
  for (; nAllOver>0; nAllOver--)
    pf->cField[pLine[nAllOver]]= pf->cField[pLine[nAllOver-1]];
  pf->cField[pLine[0]]= ST_EMPTY;

  /* Give some points */
  fPoints[nPlayerAct] += (float)i * g_cp.fPoints[POI_PUSH];

  /* ? nobody kicked */
  if (pf->cField[0] == ST_BORDER) return EV_OK;

  /* Somebody kicked */
  pf->nLost[pf->cField[0]]++;
  pf->nKicked[nPlayerAct]++;
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
  int n;
  int nDir;
  FIELDPOS p;
  int nNeighFriend, nNeighEnemy, nNeighBorder;
  int nPlayer;

  /* Kicked and lost marbles */
  for (n = 0; n < g_nPlayerNum; n++)
   {
    fPoints[n] += g_fPointsKicked[pf->nKicked[n]];
    fPoints[n] += g_fPointsLost[pf->nLost[n]];
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
	fPoints[nPlayer] += g_cp.fPoints[POI_BORDER_ENEMY] *
	  (nNeighBorder + nNeighEnemy);

      /* Intruder */
      if (nNeighEnemy >= 5) fPoints[nPlayer] += g_cp.fPoints[POI_INTRUDER] *
	(float) g_cAbaDistBorder[p];

      /* Alone */
      if (nNeighFriend == 0)
       {
	fPoints[nPlayer] += g_cp.fPoints[POI_ALONE];
	if (g_cAbaDistBorder[p] <= 3 && nNeighEnemy <= 3)
	  fPoints[nPlayer] += g_cp.fPoints[POI_ALONE] * (float) nNeighEnemy;
       }



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
  int p, n;
  char pStore = -1;

  /* ? enough space in safe */
  if (safe->pFree >= NUM_SIM_MOVES)
   {
    GfxMsg ("Temporary move-safe full !", TRUE);
    return;
   }

  /*** Urgent move ***/
  if (bUrgent)
   {
    pStore = safe->pUpper;
    /* Update list */
    safe->cNext[pStore] = safe->pFree;
    safe->pUpper = safe->pFree;
   }

  /*** Regular move ***/
  /* ? more points */
  else if (fValue > safe->fValue[safe->pLower] && safe->nMovesReg > 0)
   {
    /* Insert at right postition */
    for (p = safe->pLower, n = 1; ; p = safe->cNext[p], n++)
     {
      if (n >= safe->nMovesReg) break;
      if (fValue <= safe->fValue[safe->cNext[p]]) break;
     }
    pStore = safe->pFree;
    safe->cNext[pStore] = safe->cNext[p];
    safe->cNext[p] = pStore;
    /* Update list information */
    if (safe->nMovesReg < g_cp.nMoves[nDepth])
      safe->nMovesReg++;
    else safe->pLower = safe->cNext[safe->pLower];
   }

  /* less points but still space */
  else if (safe->nMovesReg < g_cp.nMoves[nDepth])
   {
    pStore = safe->pFree;
    /* Insert at bottom */
    safe->cNext[pStore] = safe->pLower;
    safe->pLower = pStore;
    /* Update list information */
    safe->nMovesReg++;
   }

  /* ? store move */
  if (pStore >= 0)
   {
    /* Store field and points */
    safe->pf[pStore] = *pf;
    safe->fValue[pStore] = fValue;
    for (n = 0; n < g_nPlayerNum; n++)
      safe->fPoints[n][pStore] = fPoints[n];
    safe->pFree++;
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
    else f -= fPoints[n] *
      g_cp.fPlayerFactor[(n - nPlayer + g_nPlayerNum) % g_nPlayerNum];
   }
  return f;
 }


void SimShowValues (PLAYFIELD *pf, float fPoints[])
 {
  g_agDebug.pf = *pf;
  GfxFieldShow (&g_agDebug);
  sprintf(szBuffer, "Points(%.1f, %.1f)  Val(%.1f, %.1f)",
    fPoints[1], fPoints[2], fSimValue (1, fPoints), fSimValue (2, fPoints));
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

  SAFE safe = {0, 0, 1, 0, {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} };
  int p;

  float fPoints[NUM_PLAYERS], fPointsAttack[NUM_PLAYERS];
  float fValue, fValueBest = -1e35;

  char bDepthMax;

  int n;

  /*int nMove;
  float fValueDebug[NUM_SIM_MOVES + 1]; */

  /* Determine actual player */
  nPlayer = (g_nPlayerBase+ a_nDepth) % g_nPlayerNum;


  /*** Loop through all positions ***/
  bDepthMax = (a_nDepth + 1 >= g_cp.nDepth);
  pf = *a_pf;
  move.nCount= 1;

  for (move.p[0] = 1; move.p[0] <= NUM_STONES; move.p[0]++)
   {
    /* ? one of our marbles */
    if (pf.cField[move.p[0]] == nPlayer)
     {
      for (n = 0; n < g_nPlayerNum; n++) fPointsAttack[n] = 0.0;
      bAttack = bSimAttacked (&pf, move.p[0], fPointsAttack);

      /* Loop through all directions */
      for (move.nDir = 0; move.nDir < NUM_DIR; move.nDir++)
       {
	for (n = 0; n < g_nPlayerNum; n++) fPoints[n] = fPointsAttack[n];
	nEvent = SimStoneMoveLine (&pf, &move, fPoints);
	if (nEvent == EV_ERROR) continue;

	/* Possible move ! */
	SimPoints (fPoints, &pf);
	fValue = fSimValue (nPlayer, fPoints);

	/* ? last iteration */
	if (bDepthMax)
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
	  bUrgent = bAttack || (nEvent == EV_KICKED);
	  SimSafeAdd (&safe, &pf, fValue, fPoints, bUrgent, a_nDepth);
	  /* for (p = safe.pLower, nMove = 0; p != safe.pUpper;
	      p = safe.cNext[p], nMove++)
	    {
	     fValueDebug[nMove] = safe.fValue[p];
	    }
	  nMove=nMove;*/
	 }

	/* Restore field and points */
	pf= *a_pf;
       }
     }
   }

  if (bDepthMax)
   {
    for (n = 0; n < g_nPlayerNum; n++)
      a_fPoints[n] = a_fPoints[n] * g_cp.fDepthFactor[a_nDepth];
    return;
   }

  for (p = safe.pLower; p != safe.pUpper; p = safe.cNext[p])
   {
    /* Iterate through next move */
    Simulation (a_nDepth + 1, &safe.pf[p], fPoints);
    for (n = 0; n < g_nPlayerNum; n++)
      fPoints[n] += safe.fPoints[n][p] * g_cp.fDepthFactor[a_nDepth];
    fValue = fSimValue (nPlayer, fPoints);

    /* Level 0: pass back chosen move */
    if (a_nDepth == 0)
     {
      if ((random (2) && fValue + g_fRetTol >= fValueBest) ||
	fValue - g_fRetTol >= fValueBest)
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
  return;
 }



/* Initializes some global variables, all of them read-only.
 * This is ugly but faster.
 */
void SimInitGlobals (ABAGAME *ag)
 {
  int n;
  float factor;

  g_cp = ag->player[ag->nPlayerAct].cp;

  g_nPlayerBase= ag->nPlayerAct;
  g_nPlayerNum= ag->nPlayerNum;

  g_fRetTol= NUM_TOL * g_cp.nDepth;

  for (n = 0, factor = 1.0; n < NUM_KICKS; n++, factor *= 1.1)
   {
    if (n == ag->nKickedWon) factor *= 2.0;
    g_fPointsKicked[n] =  (float)n * g_cp.fPoints[POI_KICKED] * factor;
   }

  for (n = 0, factor = 1.0; n < NUM_KICKS; n++, factor *= 1.1)
   {
    g_fPointsLost[n] = (float)n * g_cp.fPoints[POI_LOST] * factor;
   }

  g_agDebug = *ag;
 }



/* This is the Simulation main method. It is called by the
 * framework.
 * It prepares some (read-only) global variables, then starts
 * the simulation recursion and passes back the result.
 */
int SimMove (ABAGAME *ag, char bWait)
 {
  int nRet = EV_OK;

  float fPoints[NUM_PLAYERS] = {0, 0, 0, 0};
  PLAYFIELD pf;

  FIELDPOS p, pOri, pDest;

  /* Preparation */
  SimInitGlobals (ag);

  /* Start recursion */
  pf = ag->pf;
  Simulation (0, &pf, fPoints);

  /* ? no move possible *
  if (!g_smPassBack.nCount)
    return (EV_NOMOVE);
  */

  /* Look for moved marbles */
  for (p = 1, pOri = 0, pDest = 0; p <= NUM_STONES; p++)
   {
    if (pf.cField[p] == ST_EMPTY && ag->pf.cField[p] != ST_EMPTY) pOri = p;
    if (pf.cField[p] != ST_EMPTY && ag->pf.cField[p] == ST_EMPTY) pDest = p;
   }
  if (pOri == 0) return (EV_ERROR);

  /* Copy result into structure */
  ag->pf = pf;

  /* Show move */
  GfxStoneMark (pOri);
  if (bWait)
   {
    sprintf (szBuffer, "%s (%d) has chosen its move",
      ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
    if (GfxMsg (szBuffer, TRUE) == 27) nRet = EV_ESC;
   }
  else delay (1000);
  if (pDest != 0)
   {
    GfxStoneMark (pDest);
    delay (500);
   }
  GfxFieldShow (ag);
  GfxStoneUnMark (pOri);
  if (pDest != 0)
   {
    delay (500);
    GfxStoneUnMark (pDest);
   }

  /* Look for Escape */
  while (kbhit ())
    if (getch () == 27) nRet= EV_ESC;

  /* Ok */
  return (nRet);
 }



/********** own functions end *********/

