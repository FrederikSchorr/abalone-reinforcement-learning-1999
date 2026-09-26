#ifndef _LIP_
#define _LIP_

/*

(C) LIPSOFT 1994

ABA2.C

void beispiel (void)
 {

 }

*/


/******** own functions list **********

 main

********* own functions list end ******/




/************* includes ***************/

#include "ABA2.H"
#include <STDLIB.H>
#include <STDIO.H>
#include <STRING.H>
#include <TIME.H>

#include <FILEFUNK.H>

/********** includes end **************/




/********** defines *******************/

#define		FILE_RES	"aba2_1.ar"

/*********** defines end **************/



/*********** global variables ********/

/*
 * pAbaNeigh[pPos][nDir]: neighbour to pPos in direction nDir
 *   0: border
 *   1: first stone
 *   NUM_STONES: last stone
 * pAbaCursor: actual position of graphic cursor in the field
 * nAbaDirAttack[pPos][nDir]: directions from where pPos could be killed
 * cAbaDistCenter[NUM_STONES+1]: contains index for SIMULATION.lpoints
 *   (distance to center)
 */
FIELDPOS pAbaNeigh[NUM_STONES+1][NUM_DIR];	/* p... position in cField */
FIELDPOS pAbaCursor=0;
char nAbaDirAttack[NUM_STONES+1][NUM_ATTACK_DIR];
unsigned char cAbaDistCenter[NUM_STONES+1];

/* old stuff */
ABAGAME agOld;
int nOldPlayerKicked[NUM_PLAYERS];
char bOld;

char buffer[80];
/******** global variables end ********/




/************ C functions *************/

int getch (void);
int putch (int ch);
int kbhit (void);
int toupper (int);

/*********** C functions end **********/




/************ functions def ***********/

/* routines from ABA2GFX.C */
void GfxInit (void);
void GfxEnd (void);
void GfxOn (ABAGAME *);
void GfxOff (void);
void GfxMsg (char *, char);
void GfxCursorMove (FIELDPOS);
void GfxStoneMark (FIELDPOS);
void GfxStoneUnMark (FIELDPOS);
void GfxFieldShow (void);
void GfxKickedShow (void);

/* routines from ABA2MENU.C */
void MenuABA2 (void);
void MenuGameInit3 (ABAGAME *ag, int, int, int);

/* routines from ABA2SIM.C */
int SimMove (ABAGAME *ag);

/******** functions def end ***********/




/********** own functions *************/

void aba2flash (int nError, char *sz)
 {

  fprintf (stderr, " ");
  fprintf (stderr, "\n\nAba2");
  if (nError) fprintf (stderr, " Error");
  fprintf (stderr, ": %s", sz);
  if (nError == ERR_FILE)
   {
    fprintf (stderr, "\nFollowing files required:");
    fprintf (stderr, "\n%c %s", 254, FILE_RES);
   }
  fprintf (stderr, "\n");
  exit (nError);
 }

void aba2Init (char *szFileNeigh)
 {
  int i,e,fh;
  char buffer[80];

  if ((fh=fileopen (szFileNeigh,LIPFREAD))==LIPKEINZUGRIFF)
    aba2flash (ERR_FILE, "Opening the Neigh File");
  if (fileread (fh, strlen (CODE_NEIGH), buffer)==LIPKEINZUGRIFF)
    aba2flash (ERR_FILE, "Reading Neigh Code");
  buffer[strlen (CODE_NEIGH)]= 0;
  if (strcmp (CODE_NEIGH,buffer)!=0)
    aba2flash (ERR_FILE, "File identification");
  buffer[3]=0;
  for (i=1; i<=NUM_STONES; i++)
   {
    if (fileread (fh, 2, buffer)!=2)
      aba2flash (ERR_FILE, "Reading CR/LF");

    for (e=0; e<NUM_DIR; e++)
     {
      if (fileread (fh, 3, buffer)!=3)
	aba2flash (ERR_FILE, "Reading Neigh Data");
      pAbaNeigh[i][e]= atoi (buffer);
     }

    if (fileread (fh, 3, buffer)!=3)
      aba2flash (ERR_FILE, "Reading Distance Data");
    cAbaDistCenter[i]= atoi (buffer);

    for (e=0; e<NUM_ATTACK_DIR; e++)
     {
      if (fileread (fh, 3, buffer)!=3)
	aba2flash (ERR_FILE, "Reading Attack Data");
      nAbaDirAttack[i][e]= atoi (buffer);
     }
   }
  if (fileclose (fh)== LIPKEINZUGRIFF)
    aba2flash (ERR_FILE, "Closing Neigh File");
 }



int PlayerKicked (ABAGAME *ag)
/* EV_OK / EV_KICKED / EV_WON */
 {
  if (ag->cField[0] != ST_BORDER)
   {
    ag->player[ag->cField[0]].nKicked++;
    GfxKickedShow ();
    if (ag->player[ag->cField[0]].nKicked == ag->nKickedMax)
     {
      sprintf (buffer, "%s (%d) looses the game !!!", \
	ag->player[ag->cField[0]].szName, ag->cField[0]);
      GfxMsg (buffer, TRUE);
      return (EV_WON);
     }
    else return (EV_KICKED);
   }
  return (EV_OK);
 }


void CursorMoveRel (int nDir)
 {
  if (pAbaNeigh[pAbaCursor][nDir] == 0) return;
  GfxCursorMove (pAbaNeigh[pAbaCursor][nDir]);
 }

void StoneMarkAll (ABAGAME *ag, char bMark)
 {
  int i;

  for (i=0; i<ag->nMarkNum; i++)
   {
    if (bMark) GfxStoneMark (ag->pMark[i]);
    else GfxStoneUnMark (ag->pMark[i]);
   }
  if (!bMark) ag->nMarkNum= 0;
 }


int WaitEvent (ABAGAME *ag)
 {
  signed char buffer[80];
  int i;

  GfxCursorMove (ag->player[ag->nPlayerAct].pCursor);
  while (TRUE)
   {
    ag->player[ag->nPlayerAct].pCursor= pAbaCursor;
    switch (toupper(getch ()))
     {
      case 27:
       {
	return (EV_ESC);
       }
      case '\r':
      case ' ':
       {
	for (i=0; i<ag->nMarkNum; i++)
	 {
	  if (pAbaCursor == ag->pMark[i])
	   {
	    GfxStoneUnMark (pAbaCursor);
	    ag->pMark[i]= ag->pMark[ag->nMarkNum-1];
	    ag->nMarkNum--; i= NUM_STONES;
	    break;
	   }
	 }
	if (i == ag->nMarkNum)
	 {
	  if (ag->nPlayerAct != ag->cField[pAbaCursor])
	   {
	    sprintf (buffer, "You can't mark this stone, %s (%d) !", \
	      ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
	    GfxMsg (buffer, TRUE);
	   }
	  else if (ag->nMarkNum >= NUM_MARK)
	   {
	    sprintf (buffer, "Already marked %d stones !", NUM_MARK);
	    GfxMsg (buffer, TRUE);
	   }
	  else
	   {
	    GfxStoneMark (pAbaCursor);
	    ag->pMark[ag->nMarkNum]= pAbaCursor;
	    ag->nMarkNum++;
	   }
	 }
	break;
       }
      case 8: /* BS */
       {
	if (!bOld)
	  GfxMsg ("You can't take back another move", TRUE);
	else
	 {
	  StoneMarkAll (ag, FALSE);
	  *ag= agOld;
	  for (i= 1; i<=ag->nPlayerNum; i++)
	    ag->player[i].nKicked= nOldPlayerKicked[i];
	  bOld= FALSE;
	  GfxFieldShow ();
	  GfxKickedShow ();
	  StoneMarkAll (ag, TRUE);
	  return (EV_UNDO);
	 }
	break;
       }

      case '6':
      case 'D':
	CursorMoveRel (0);
	break;
      case '9':
      case '8':
      case 'E':
      case 'W':
	CursorMoveRel (1);
	break;
      case '7':
      case 'Q':
	CursorMoveRel (2);
	break;
      case '4':
      case 'A':
	CursorMoveRel (3);
	break;
      case '1':
      case '2':
      case 'X':
      case 'Y':
      case 'Z':
	CursorMoveRel (4);
	break;
      case '3':
      case 'C':
	CursorMoveRel (5);
	break;
      case 4:
	return (0);
      case 5:
      case 23:
	return (1);
      case 17:
	return (2);
      case 1:
	return (3);
      case 24:
      case 25:
      case 26:
	return (4);
      case 3:
	return (5);
      case 0:
       {
	switch (getch ())
	 {
	  case 77:
	    CursorMoveRel (0);
	    break;
	  case 73:
	  case 72:
	    CursorMoveRel (1);
	    break;
	  case 71:
	    CursorMoveRel (2);
	    break;
	  case 75:
	    CursorMoveRel (3);
	    break;
	  case 79:
	  case 80:
	    CursorMoveRel (4);
	    break;
	  case 81:
	    CursorMoveRel (5);
	    break;
	  case 116:
	    return (0);
	  case 132:
	  case 141:
	    return (1);
	  case 119:
	    return (2);
	  case 115:
	    return (3);
	  case 117:
	  case 145:
	    return (4);
	  case 118:
	    return (5);
	 }
	break;
       }
     }
   }
  return (EV_ERROR);
 }


void PlayTest (ABAGAME *ag)
 {
  GfxOn (ag);
  GfxMsg ("Just fool around ...",FALSE);
  while (WaitEvent (ag) != EV_ESC);
 }


int StoneMarkMove (ABAGAME *ag, int nDir)
/* TRUE / FALSE */
 {
  int i, n, nStoneOwn, nDirRev;
  FIELDPOS *p, pAct;

  p= ag->pMark;
  ag->cField[0]= ST_BORDER;

  for (i=0; i<ag->nMarkNum; i++)
    if (ag->cField[pAbaNeigh[p[i]][nDir]] != ST_EMPTY) break;
  if (i == ag->nMarkNum)
   {
    for (i=0; i<ag->nMarkNum; i++)
      {
       ag->cField[p[i]]= ST_EMPTY;
       ag->cField[pAbaNeigh[p[i]][nDir]]= ag->nPlayerAct;
      }
    return (TRUE);
   }

  if (ag->nMarkNum == 2)
   {
    if (pAbaNeigh[p[0]][nDir]==p[1]) pAct= p[0];
    else if (pAbaNeigh[p[1]][nDir]==p[0]) pAct= p[1];
    else return (FALSE);
   }
  else if (ag->nMarkNum == 3)
   {
    if (pAbaNeigh[p[0]][nDir]==p[1]) pAct= p[0];
    else if (pAbaNeigh[p[2]][nDir]==p[1]) pAct= p[2];
    else return (FALSE);
   }
  else pAct= p[0];

  for (i=0,n=0; i<NUM_MARK; i++,n++)
   {
    if (ag->cField[pAct] != ag->nPlayerAct) break;
    pAct= pAbaNeigh[pAct][nDir];
   }
  if (pAct == 0) return (FALSE);
  nDirRev= (nDir+NUM_DIR/2) % NUM_DIR;
  for (nStoneOwn=i,i=0; i<nStoneOwn; i++,n++)
   {
    if (ag->cField[pAct] == ag->nPlayerAct) return (FALSE);
    if (ag->cField[pAct] == ST_EMPTY || pAct == 0) break;
    pAbaNeigh[pAbaNeigh[pAct][nDir]][nDirRev]= pAct;
    pAct= pAbaNeigh[pAct][nDir];
   }
  if (i >= nStoneOwn) return (FALSE);

  for (; n>0; n--)
   {
    ag->cField[pAct]= ag->cField[pAbaNeigh[pAct][nDirRev]];
    pAct= pAbaNeigh[pAct][nDirRev];
   }
  ag->cField[pAct]= ST_EMPTY;
  return (TRUE);
 }

int StoneMarkLegal (ABAGAME *ag)
 {
  int i,n;
  FIELDPOS *p, pHelp;

  p= ag->pMark;

  if (ag->nMarkNum == 1) return (TRUE);
  else if (ag->nMarkNum == 2)
   {
    for (i=0; i<NUM_DIR; i++)
      if (pAbaNeigh[p[0]][i] == p[1])
	return (TRUE);
    return (FALSE);
   }
  else if (ag->nMarkNum == 3)
   {
    for (n=0; n<NUM_MARK-1; n++)
     {
      for (i=0; i<NUM_MARK-1; i++)
       {
	if (p[i]<p[i+1])
	 {
	  pHelp= p[i];
	  p[i]=p[i+1];
	  p[i+1]=pHelp;
	 }
       }
     }

    for (i=0; i<NUM_DIR; i++)
      if (pAbaNeigh[p[0]][i] == p[1] && pAbaNeigh[p[1]][i] == p[2])
	return (TRUE);
    return (FALSE);
   }
  return (FALSE);
 }


int GamePlay (ABAGAME *ag)
/* EV_ESC / EV_WON*/
 {
  int nEvent;
  char buffer[80], bEsc= FALSE;
  int nPlayerLast=0;
  ABAGAME agHelp;
  int i;

  randomize ();
  GfxOn (ag);
  bOld= FALSE;

  while (TRUE)
   {
    if (ag->player[ag->nPlayerAct].nKicked >= ag->nKickedMax)
     {
      ag->nPlayerAct= (ag->nPlayerAct % ag->nPlayerNum) + 1;
      continue;
     }
    if (nPlayerLast == ag->nPlayerAct)
     {
      sprintf (buffer, "Hey lonely desperado %s (%d), you won !!!", \
	ag->player[nPlayerLast].szName, nPlayerLast);
      GfxMsg (buffer, TRUE);
      GfxOff ();
      return (EV_WON);
     }

    if (ag->player[ag->nPlayerAct].nType == TYPE_HUMAN)
     {
      sprintf (buffer, "It's %s's (%d) move.", \
	ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
      GfxMsg (buffer, FALSE);

      nEvent= WaitEvent (ag);
      switch (nEvent)
       {
	case EV_ESC:
	case EV_ERROR:
	 {
	  GfxOff ();
	  return (nEvent);
	 }
	case EV_UNDO:
	 {
	  nPlayerLast= ST_EMPTY;
	  continue;
	 }
       }

      agHelp= *ag;
      if (!StoneMarkLegal (ag))
	GfxMsg ("Sorry, illegal stone combination", TRUE);
      else if (!StoneMarkMove (ag, nEvent))
	GfxMsg ("Sorry, illegal move", TRUE);
      else
       {
	agOld= agHelp;
	for (i=1; i<=ag->nPlayerNum; i++)
	  nOldPlayerKicked[i]= ag->player[i].nKicked;
	bOld= TRUE;

	StoneMarkAll (ag, FALSE);
	GfxFieldShow ();

	PlayerKicked (ag);

        nPlayerLast= ag->nPlayerAct;
        ag->nPlayerAct= (ag->nPlayerAct % ag->nPlayerNum) + 1;
       }
     }
    else /* computer player */
     {
      sprintf (buffer, "Computing %s's (%d) move ...", \
	ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
      GfxMsg (buffer, FALSE);
      nPlayerLast= ag->nPlayerAct;

      switch (SimMove (ag))
       {
	case EV_NOMOVE:
	 {
	  sprintf (buffer, "%s (%d) can't move anymore !!!", \
	    ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
	  GfxMsg (buffer, TRUE);
	  ag->nPlayerAct= (ag->nPlayerAct % ag->nPlayerNum) + 1;
	  continue;
	 }
	case EV_ESC: bEsc= TRUE;
       }

      GfxFieldShow ();
      PlayerKicked (ag);

      ag->nPlayerAct= (ag->nPlayerAct % ag->nPlayerNum) + 1;

      if (bEsc)
       {
	GfxOff ();
	return (EV_ESC);
       }
     }
   }
  return (EV_ERROR);
 }


void main (void)
 {
  aba2Init (FILE_RES);
  GfxInit ();
  MenuABA2 ();
  GfxEnd ();

  aba2flash (0, "Terminated succesfully");
 }


/********** own functions end *********/

#endif