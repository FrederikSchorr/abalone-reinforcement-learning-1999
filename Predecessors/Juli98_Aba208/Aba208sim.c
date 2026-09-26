/*

(C) LIPSOFT 1995



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
#include <ALLOC.H>

/********** includes end **************/




/********** defines *******************/

#define lSimPointsLost(ibPlayerBase)	(lSimPointsMax* \
 (ibPlayerBase ? (-1) : 1))

/*********** defines end **************/




/********** structures ****************/


/********** structures end ************/





/*********** global variables ********/

extern FIELDPOS pAbaNeigh[NUM_STONES+1][NUM_DIR];
extern char nAbaDirAttack[NUM_STONES+1][NUM_ATTACK_DIR];
extern unsigned char cAbaDistCenter[NUM_STONES+1];

SIMULATION sim;				/* player specific data */
SIMFIELD *sfSimPoss[NUM_LEVELS];	/* a lot of fields ... */
SIMFIELD *sfSimFieldAct;		/* act. field */
SIMFIELD *sfSimOri;			/* original field */
SIMFIELD *sfSimPassBack;		/* passes move back to SimMove */

int nSimPlayerBase;			/* base player */
int nSimPlayerAct;			/* act. player */
int nSimPlayerNum;			/* number of players */

FIELDPOS pSimPosAct;			/* act. fieldpos */
char nSimDirAct;			/* act. direction */

long lSimPointsKicked[NUM_STONES];	/* points for a kicked marble */
long lSimPointsMax;			/* max. possible points for a sit. */

int nSimKickedMax;			/* loosing with nSimKickedMax marb. */
int nSimRetTol;				/* tolerance for randommoving */

int nSimStoneMoveLineStatus;		/* 10, 20, 21, 30, 31, 32 */

/* debug var. */
char szBuffer[160];

/******** global variables end ********/




/************ C functions *************/

int sprintf (char *string,const char *format, ...);

/*********** C functions end **********/




/************ functions def ***********/

void GfxFieldShow (void);
void GfxMsg (char *sz, char bRet);
void GfxStoneMark (FIELDPOS p);
void GfxStoneUnMark (FIELDPOS p);

/******** functions def end ***********/




/********** own functions *************/

int iSimStoneMoveLineInfo (void)
/* EV_OK, EV_KICKED, EV_ERROR */
/* does change nSimStoneMoveLineStatus */
 {
  FIELDPOS pAct;
  int nOwn, nEnemy;

  sfSimFieldAct.cField[0]= ST_BORDER;
  pAct= pSimPosAct;
  nSimStoneMoveLineStatus= -1;

  for (nOwn=0; nOwn<NUM_MARK; nOwn++)
   {
    if (sfSimFieldAct.cField[pAct] != nSimPlayerAct) break;
    pAct= pAbaNeigh[pAct][nSimDirAct];
   }
  if (pAct == 0) return (EV_ERROR);
  for (nEnemy=0; nEnenmy<nOwn; nEnemy++)
   {
    if (sfSimFieldAct.cField[pAct] == nPlayerAct) return (EV_ERROR);
    if (sfSimFieldAct.cField[pAct] == ST_EMPTY || pAct == 0) break;
    pAct= pAbaNeigh[pAct][nSimDirAct];
   }
  if (nEnemy >= nOwn) return (EV_ERROR);
  if (pAct == 0) return (EV_KICKED);
  return (EV_OK);
  nSimStoneMoveLineStatus= 10*nOwn+ nEnemy;
 }

void SimStoneMoveLine (void)
/* changes sfSimFieldAct, needs copy in sfSimFieldOri */
 {
  FIELDPOS pAct, pOld;
  int i, nAll;
  char *bActive;

  pAct= pSimPosAct;
  sfSimFieldAct.cField[pAct]= ST_EMPTY;
  switch (nSimStoneMoveLineStatus)
   {
    case 10:
     {
      bActive= bSimActive[0];
      nAll= 1;
      break;
     }
    default:
      aba2flash (ERR_LOGIC, "Illegal case in SimStoneLine"):
   }


  for (i=0; i<nAll; i++)
   {
    pOld= pAct;
    pAct= pAbaNeigh[pAct][nSimDirAct];
    sfSimFieldAct.cField[pAct]= sfSimFieldOri[pOld];
   }


 }

long lSimPoints (void)
 {
/*  signed long l=0, lSignum;
  int nDir;
  FIELDPOS p;
  int nNeighFriend, nNeighEnemy, nNeighBorder;
signed long lHelp;

  for (p=1; p<=NUM_STONES; p++)
   {
    if (sfSimFieldAct.cField[p] != ST_EMPTY)
     {
      if (sfSimFieldAct.cField[p] == nSimPlayerBase) lSignum=-1;
      else lSignum=1;
      l-= lSignum * sim.lPoints[cAbaDistCenter[p]];
      nNeighFriend= nNeighEnemy= nNeighBorder= 0;
      for (nDir=0; nDir<NUM_DIR; nDir++)
       {
	switch (sfSimFieldAct.cField[pAbaNeigh[p][nDir]])
	 {
	  case ST_BORDER:
	    nNeighBorder++;
	    break;
	  case ST_EMPTY:
	    break;
	  default:
	   {
	    if (sfSimFieldAct.cField[pAbaNeigh[p][nDir]] == sfSimFieldAct.cField[p])
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
	  (sim.lPoints[POI_CENTER]+ 2));
	l-= lHelp;
       }
     }
   }
  if (sfSimFieldAct.cField[0] != ST_BORDER)
   {
    if (sfSimFieldAct.cField[0] == nSimPlayerBase) lSignum= -1;
    else lSignum= 1;
    l+= lSignum* lSimPointsKicked[sfSimFieldAct.nPlayerKicked[sfSimFieldAct.cField[0]]];
   }
*/  return (l);
 }



void SimMoveSort (int nLevelAct, int nCountPossReg, int *nIndexPoss,
  char bSmallLast)
 {
  int i,e;

/*  for (i=(-1)*NUM_SIM_MOVES; i<NUM_SIM_MOVES; i++) nIndexPoss[i]=i;
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
*/ }



long lSimCalc (int nLevelAct, int nPlayerAct)
/* erster aufruf: lSimCalc (0, nSimPlayerBase) */
/* veraendert folgende globale variable:
  sfSimPoss
  sfSimFieldAct
  sfSimOri
  sfSimPassBack

  nSimPlayerAct

  pSimPosAct
  nSimDirAct
*/
 {
  char iEvMove;
  char bPlayerBase, bLevelLast;
  int i;
  int nCountPossReg, nCountPossImp;
  int nIndexPoss[NUM_SIM_MOVES_REG+ NUM_SIM_MOVES_IMP];
  long lPointsDiffExt, lPoints;				/* extremum */

  nSimPlayerAct= ((nPlayerAct-1)% nSimPlayerNum)+ 1;
  bPlayerBase= (nSimPlayerBase == nSimPlayerAct);

  if (sfSimFieldAct->nKicked[nPlayerAct] >= nSimKickedMax)
    return (lSimPointsLost (bPlayerBase)* sim.fFactor[nLevelAct] + \
      ((nLevelAct>= sim.nLevels-1) ? 0 : lSimCalc (nLevelAct, nPlayerAct+ 1)));

/**/  bLevelLast= (nLevelAct >= sim.nLevels-1);

  sfSimOri= sfSimFieldAct;
  nCountPossReg= NUM_SIM_MOVES_IMP;
  nCountPossImp= nCountPossReg- 1;
  lPointsDiffExt= NUM_LONG* (bPlayerBase ? (-1) : 1);

  for (pSimPosAct=1; pSimPosAct<=NUM_STONES; pSimPosAct++)
   {
    if (sfSimFieldAct->cField[pSimPosAct] == nSimPlayerAct)
     {
      for (nSimDirAct=0; nSimDirAct<6; nSimDirAct++)
       {
	iEvMove= iSimStoneMoveLineInfo ();

	if (iEvMove == EV_OK && \
	  nCountPossReg < NUM_SIM_MOVES_REG+ NUM_SIM_MOVES_IMP)
	 {
	  sfSimFieldAct= &sfSimPoss[nLevelAct][nCountPossReg];
	  nCountPossReg++;
	 }
	else if (iEvMove == EV_KICKED && nCountPossImp >= 0)
	 {
	  sfSimFieldAct= &sfSimPoss[nLevelAct][nCountPossImp];
	  nCountPossImp--;
	 }
	else continue;

	*sfSimFieldAct= *sfSimOri;
	SimStoneMoveLine ();
	SimFieldCalc ();

/**/	if (bLevelLast)
/**/	 {
/**/      if ((bPlayerBase && sfSimFieldAct->lPointsDiff > lPointsDiffExt) || \
/**/        (!bPlayerBase && sfSimFieldAct->lPointsDiff < lPointsDiffExt))
/**/        lPointsDiffExt= sfSimFieldAct->lPointsDiff;
/**/     }
       }
     }
   }

/**/ if (bLevelLast)
/**/ return ((float)(lPointsDiffExt)* sim.fFactor[nLevelAct]);

  SimMoveSort (nLevelAct, nCountPossReg, nIndexPoss, bPlayerBase);
  nCountPossReg= min (nCountPossReg-1, \
    sim.nDepthCalc[nLevelAct]+ NUM_SIM_MOVES_IMP);

  for (i=nCountPossImp+1; i<nCountPossReg; i++)
   {
    sfSimFieldAct= &sfSimPoss[nLevelAct][nIndexPoss[i]];
    lPoints= (float)(sfSimFieldAct->lPointsDiff)* sim.fFactor[nLevelAct];
    lPoints+= lSimCalc (nLevelAct+ 1, nPlayerAct+ 1);
    /* attention: global variables lost !!! */

    if (!nLevelAct)
     {
      if (sfSimPassBack == NULL || \
	(random (3) && lPoints+nSimRetTol >= lPointsDiffExt) || \
	(lPoints-nSimRetTol >= lPointsDiffExt))
	sfSimPassBack= &sfSimPoss[nLevelAct][nIndexPoss[i]];
     }

    if ((bPlayerBase && lPoints > lPointsDiffExt) || \
      (!bPlayerBase && lPoints < lPointsDiffExt))
      lPointsDiffExt= lPoints;
   }

  if (nCountPossReg- nCountPossImp <= 1)	/* no possible move */
    lPointsDiffExt= lSimPointsLost (bPlayerBase);
  return (lPointsDiffExt);
 }



void SimInitGlobals (ABAGAME *ag)
 {
  int i;

  sim= ag->player[ag->nPlayerAct].sim;

  if ((sfSimFieldAct=farcalloc(1, sizeof(SIMFIELD))) == NULL)
      aba2flash (ERR_MEM, "Allocating memory on the farheap, 1");
  for (i=0; i<sim.nLevels; i++)
   {
    if ((sfSimPoss[i]=farcalloc(NUM_SIM_MOVES_REG+ NUM_SIM_MOVES_IMP, \
      sizeof(SIMFIELD))) == NULL)
      aba2flash (ERR_MEM, "Allocating memory on the farheap, 2");
   }

  strncpy (sfSimFieldAct->cField, ag->cField, NUM_STONES+1);
  for (i=1; i<=ag->nPlayerNum; i++)
    sfSimFieldAct->nPlayerKicked[i]= ag->player[i].nKicked;

  nSimPlayerBase= ag->nPlayerAct;
  nSimPlayerNum= ag->nPlayerNum;
  nSimKickedMax= ag->nKickedMax;
  nSimRetTol= NUM_TOL * sim.nLevels;

  /* lSimPoints */
  if (nSimKickedMax <= 2)
    for (i=1; i<nSimKickedMax; i++)
      lSimPointsKicked[i]= i * sim.lPoints[POI_KICK];
  else
   {
    lSimPointsKicked[0]= 0;
    for (i=1; i<nSimKickedMax; i++)
      lSimPointsKicked[i]= sim.lPoints[POI_KICK]* \
	(1.0+ (i-1.0) * (sim.lPoints[POI_KICK_FACTOR]/2.0 - 1.0) / \
	(nSimKickedMax-2.0));
    lSimPointsKicked[i]= sim.lPoints[POI_KICK_FACTOR]* sim.lPoints[POI_KICK];
   }
  lSimPointsMax= sim.lPoints[POI_CENTER] + (NUM_DIR* sim.lPoints[POI_NEIGH]);
  lSimPointsMax-= sim.lPoints[POI_ALONE] * \
    (sim.lPoints[cAbaDistCenter[1]]- (sim.lPoints[POI_CENTER]+ 2));
  lSimPointsMax*= NUM_STONES;
  lSimPointsMax+= lSimPointsKicked[i];
 }

 
int SimMove (ABAGAME *ag)
 {
  int i;
  int nRet= EV_OK;

  SimInitGlobals (ag);
  if (sim.nLevels < 2)
   {
    GfxMsg ("Warning: Calculation Level < 2!", TRUE);
    return (EV_ESC);
   }

  lSimCalc (0, nSimPlayerBase);

  if (sfSimPassBack == NULL)
    nRet= EV_NOMOVE;
  else
    strncpy (ag->cField, sfSimPassBack->cField, NUM_STONES+1);

  while (kbhit ())
    if (getch () == 27) nRet= EV_ESC;
  return (nRet);
 }



/********** own functions end *********/

