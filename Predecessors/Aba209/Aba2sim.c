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

extern FIELDPOS pAbaNeigh[NUM_STONES+1][NUM_DIR];
extern char nAbaDirAttack[NUM_STONES+1][NUM_ATTACK_DIR];
extern unsigned char cAbaDistCenter[NUM_STONES+1];

SIMMOVE smPassBack;
SIMMOVE smMove;
SIMFIELD sfField;
SIMULATION sim;
long lSimPointsKicked[NUM_STONES];
long lSimPointsMax;
int nSimPlayerBase, nSimPlayerNum;
int nSimKickedMax, nSimRetTol;
long lSimPointsMax;

ABAGAME *agTest;
clock_t clStart, clEnd, clSum;
int nCount;
char szBuffer[160];


/******** global variables end ********/




/************ C functions *************/

int sprintf (char *string,const char *format, ...);
void delay (unsigned ms);

/*********** C functions end **********/




/************ functions def ***********/

void GfxFieldShow (void);
void GfxMsg (char *sz, char bRet);
void GfxStoneMark (FIELDPOS p);
void GfxStoneUnMark (FIELDPOS p);

/******** functions def end ***********/




/********** own functions *************/

int SimStoneMoveLine (int nPlayerAct)
/* EV_OK, EV_ERROR */
/* operating on sfField, smMark */
 {
  FIELDPOS pAct= *smMove.pMark;
  FIELDPOS pLine[NUM_SIM_MOVES];
  int i, nStoneOwn, nAllOver;

  sfField.cField[0]= ST_BORDER;
  for (i=0, nAllOver=0; i<NUM_MARK; i++,nAllOver++)
   {
    if (sfField.cField[pAct] != nPlayerAct) break;
    pLine[nAllOver]= pAct;
    pAct= pAbaNeigh[pAct][smMove.nDir];
   }
  if (pAct == 0) return (EV_ERROR);

  for (nStoneOwn=i,i=0; i<nStoneOwn; i++,nAllOver++)
   {
    if (sfField.cField[pAct] == nPlayerAct) return (EV_ERROR);
    pLine[nAllOver]= pAct;
    if (sfField.cField[pAct] == ST_EMPTY || pAct == 0) break;
    pAct= pAbaNeigh[pAct][smMove.nDir];
   }
  if (i >= nStoneOwn) return (EV_ERROR);

  for (; nAllOver>0; nAllOver--)
    sfField.cField[pLine[nAllOver]]= sfField.cField[pLine[nAllOver-1]];
  sfField.cField[pLine[0]]= ST_EMPTY;
  if (sfField.cField[0] != ST_BORDER)
    sfField.nPlayerKicked[sfField.cField[0]]++;
  return (EV_OK);
 }



long lSimPointsLost (int nPlayerAct)
 {
  signed lSignum;

  if (nSimPlayerBase == nPlayerAct) lSignum= -1;
  else lSignum= 1;
  return (lSignum* lSimPointsMax);
 }

long lSimPoints (void)
 {
  signed long l=0, lSignum;
  int nDir;
  FIELDPOS p;
  int nNeighFriend, nNeighEnemy, nNeighBorder;
signed long lHelp;

  for (p=1; p<=NUM_STONES; p++)
   {
    if (sfField.cField[p] != ST_EMPTY)
     {
      if (sfField.cField[p] == nSimPlayerBase) lSignum=-1;
      else lSignum=1;
      l-= lSignum * sim.lPoints[cAbaDistCenter[p]];
      nNeighFriend= nNeighEnemy= nNeighBorder= 0;
      for (nDir=0; nDir<NUM_DIR; nDir++)
       {
	switch (sfField.cField[pAbaNeigh[p][nDir]])
	 {
	  case ST_BORDER:
	    nNeighBorder++;
	    break;
	  case ST_EMPTY:
	    break;
	  default:
	   {
	    if (sfField.cField[pAbaNeigh[p][nDir]] == sfField.cField[p])
	     {
	      l-= lSignum * sim.lPoints[POI_NEIGH];
	      nNeighFriend++;
	     }
	    else
	      nNeighEnemy++;
	   }
	 }
       }
      if ((nNeighBorder != 0 && nNeighEnemy != 0) || \
	(nNeighEnemy >= 2 && nNeighFriend == 0))
       {
	lHelp= lSignum * sim.lPoints[POI_ALONE] * (sim.lPoints[cAbaDistCenter[p]]- \
	  (sim.lPoints[POI_KICK-1]+ 2));
	l-= lHelp;
/*strncpy (agTest->cField, sfField.cField, NUM_STONES+1);
GfxFieldShow ();
sprintf (szBuffer, "Player %d: Alone Points for %d: %ld", nSimPlayerBase, p, -1 * lHelp);
GfxMsg (szBuffer, TRUE);
*/
       }
     }
   }
  if (sfField.cField[0] != ST_BORDER)
   {
    if (sfField.cField[0] == nSimPlayerBase) lSignum= -1;
    else lSignum= 1;
    l+= lSignum* lSimPointsKicked[sfField.nPlayerKicked[sfField.cField[0]]];
   }
  return (l);
 }



void SimMoveSort (SIMMOVE *smPoss, int *nPossIndex, int nPossReg,
  char bSmallLast)
 {
  int i,e;

  for (i=(-1)*NUM_SIM_MOVES; i<NUM_SIM_MOVES; i++) nPossIndex[i]=i;
  for (i=0; i<nPossReg; i++)
   {
    for (e=i; e>0; e--)
     {
      if ((bSmallLast && smPoss[nPossIndex[e]].lPoints > \
	smPoss[nPossIndex[e-1]].lPoints) || \
	(!bSmallLast && smPoss[nPossIndex[e]].lPoints < \
	smPoss[nPossIndex[e-1]].lPoints))
       {
	nPossIndex[e]= nPossIndex[e-1];
	nPossIndex[e-1]= i;
       }
     }
   }
 }



char bSimAttackedDir (FIELDPOS pBorder, char nDir)
 {
  FIELDPOS pAct;
  int nPlayerAct, nPlayerAttack;
  int nOwn, nAttack;

  nPlayerAct= sfField.cField[pBorder];
  nOwn= 1;
  pAct= pAbaNeigh[pBorder][nDir];

  if (sfField.cField[pAct] == nPlayerAct)
   {
    nOwn++;
    pAct= pAbaNeigh[pAct][nDir];
   }

  nPlayerAttack= sfField.cField[pAct];
  if (nPlayerAttack==ST_EMPTY || nPlayerAttack==nPlayerAct) return (FALSE);

  for (nAttack=1; nAttack<=NUM_MARK;)
   {
    pAct= pAbaNeigh[pAct][nDir];
    if (sfField.cField[pAct] != nPlayerAttack) break;
    nAttack++;
   }
  if (nAttack > nOwn) return (TRUE);
  else return (FALSE);
 }

char bSimAttacked (FIELDPOS p)
 {
  char bAttacked;
  int e;

  bAttacked= FALSE;
  for (e=0; e<NUM_ATTACK_DIR && nAbaDirAttack[p][e]<NUM_DIR; e++)
    if (bSimAttackedDir (p, nAbaDirAttack[p][e])) bAttacked= TRUE;
  return (bAttacked);
 }



long lSimCalc (int nLevelAct)
 {
  FIELDPOS p;
  SIMFIELD sfOri;
  SIMMOVE smPossXXX[2*NUM_SIM_MOVES], *smPoss;
  int nPossIndexXXX[2*NUM_SIM_MOVES], *nPossIndex;
  int nPossReg, nPossUrgent;
  long lReturn, lHelp;
  int nPlayerAct;
  int i;
  char bPlayerBase, bAttacked;

  if (nLevelAct >= sim.nLevels)
    return (0L);

  nPlayerAct= (nSimPlayerBase+ nLevelAct- 1)% nSimPlayerNum+ 1;
  if (sfField.nPlayerKicked[nPlayerAct] >= nSimKickedMax)
    return (lSimPointsLost (nPlayerAct) * sim.fLevelFactor[nLevelAct] + \
      lSimCalc (nLevelAct+1));

  bPlayerBase= (nPlayerAct == nSimPlayerBase);
  nPossIndex= &nPossIndexXXX[NUM_SIM_MOVES];
  smPoss= &smPossXXX[NUM_SIM_MOVES];
  sfOri= sfField;
  nPossReg= 0, nPossUrgent= -1;

  smMove.nMarkNum= 1;
  for (p=1; p<=NUM_STONES; p++)
   {
    if (sfField.cField[p] == nPlayerAct)
     {
      bAttacked= bSimAttacked (p);
/*if (bAttacked && nLevelAct == 0)
 {
  GfxStoneMark (p);
  GfxMsg ("This Stone is attacked", TRUE);
  GfxStoneUnMark (p);
 }
*/
      *smMove.pMark= p;

      for (smMove.nDir=0; smMove.nDir<NUM_DIR; smMove.nDir++)
       {
	if (SimStoneMoveLine (nPlayerAct) == EV_OK)
	 {
	  smMove.lPoints= (long) ((float) lSimPoints () *
	    sim.fLevelFactor[nLevelAct]);
	  if (sfField.cField[0] != ST_BORDER || bAttacked)
	   {
	    if (nPossUrgent >= (-1)*NUM_SIM_MOVES)
	     {
	      smPoss[nPossUrgent]= smMove;
	      nPossUrgent--;
	     }
	   }
	  else
	   {
	    if (nPossReg < NUM_SIM_MOVES)
	     {
	      smPoss[nPossReg]= smMove;
	      nPossReg++;
	     }
	   }
	  sfField= sfOri;
	 }
       }
     }
   }

  SimMoveSort (smPoss, nPossIndex, nPossReg, bPlayerBase);
  nPossReg= min (nPossReg, sim.nLevelCalcNum[nLevelAct]);

  if (bPlayerBase) lReturn= NUM_LONG * (-1);
  else lReturn= NUM_LONG;

  for (i= nPossUrgent+1; i<nPossReg; i++)
   {
    lHelp= smPoss[nPossIndex[i]].lPoints;

    /* Iterate through next level */
    if (nLevelAct+1 < sim.nLevels)
     {
      smMove= smPoss[nPossIndex[i]];
      SimStoneMoveLine (nPlayerAct);
      lHelp+= lSimCalc (nLevelAct+1);
      sfField= sfOri;
     }

    /* Level 0: pass back chosen move */
    if (nLevelAct == 0)
     {
      if (smPassBack.nMarkNum == 0 ||
	(random (2) && lHelp+nSimRetTol >= lReturn) ||
	lHelp-nSimRetTol >= lReturn)
	smPassBack= smPoss[nPossIndex[i]];
     }

    /* Identify best SimCalc value */
    if ((bPlayerBase && lHelp>lReturn) ||
      (!bPlayerBase && lHelp<lReturn))
      lReturn= lHelp;
   }
  return (lReturn);
 }


int SimMove (ABAGAME *ag)
 {
  int i;
  int nRet= EV_OK;

  strncpy (sfField.cField, ag->cField, NUM_STONES+1);
  for (i=1; i<=ag->nPlayerNum; i++)
    sfField.nPlayerKicked[i]= ag->player[i].nKicked;

  sim= ag->player[ag->nPlayerAct].sim;
  smPassBack.nMarkNum= 0;
  nSimPlayerBase= ag->nPlayerAct;
  nSimPlayerNum= ag->nPlayerNum;
  nSimKickedMax= ag->nKickedMax;
  nSimRetTol= NUM_TOL * sim.nLevels;
  if (nSimKickedMax <= 2)
    for (i=1; i<nSimKickedMax; i++)
      lSimPointsKicked[i]= i * sim.lPoints[POI_KICK];
  else
   {
    lSimPointsKicked[0]= 0;
    for (i=1; i<nSimKickedMax; i++)
      lSimPointsKicked[i]= sim.lPoints[POI_KICK]* \
	(1+ (i-1.0) * (sim.lPoints[POI_KICK_FACTOR]/2.0 - 1.0) / \
	(nSimKickedMax-2.0));
    lSimPointsKicked[i]= sim.lPoints[POI_KICK_FACTOR]* sim.lPoints[POI_KICK];
   }
  lSimPointsMax= sim.lPoints[POI_KICK- 1] + (NUM_DIR* sim.lPoints[POI_NEIGH]);
  lSimPointsMax-= sim.lPoints[POI_ALONE] * \
    (sim.lPoints[cAbaDistCenter[1]]- (sim.lPoints[POI_KICK-1]+ 2));
  lSimPointsMax*= NUM_STONES;
  lSimPointsMax+= lSimPointsKicked[i];


agTest= ag;
nCount= 0;
clSum= 0;

  lSimCalc (0);

  if (!smPassBack.nMarkNum)
    return (EV_NOMOVE);

  smMove= smPassBack;
  for (i=0; i<smMove.nMarkNum; i++)
    GfxStoneMark (smMove.pMark[i]);
  delay (MS_COMP_MOVE);
  for (i=0; i<smMove.nMarkNum; i++)
    GfxStoneUnMark (smMove.pMark[i]);
  SimStoneMoveLine (ag->nPlayerAct);
  strncpy (ag->cField, sfField.cField, NUM_STONES+1);

  while (kbhit ())
    if (getch () == 27) nRet= EV_ESC;

  return (nRet);
 }



/********** own functions end *********/

