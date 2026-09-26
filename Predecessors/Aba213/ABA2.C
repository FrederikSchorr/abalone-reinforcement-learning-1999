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

#include <FILE\FILEFUNK.H>

/********** includes end **************/




/********** defines *******************/

#define     FILE_RES "aba2_1.ar"

/*********** defines end **************/



/*********** global variables ********/

/*
 * g_pAbaNeigh[pPos][nDir]: neighbour to pPos in direction nDir
 *   0: border
 *   1: first stone
 *   NUM_STONES: last stone
 * g_pAbaCursor: actual position of graphic cursor in the field
 * g_nAbaDirAttack[pPos][nDir]: directions from where pPos could be killed
 * g_cAbaDistCenter[NUM_STONES+1]: contains index for SIMULATION.lpoints
 *   (distance to center)
 */
FIELDPOS g_pAbaNeigh[NUM_STONES+1][NUM_DIR];   /* p... position in cField */
FIELDPOS g_pAbaCursor=0;
char g_nAbaDirAttack[NUM_STONES+1][NUM_ATTACK_DIR];
unsigned char g_cAbaDistBorder[NUM_STONES+1];

char buffer[80];
/******** global variables end ********/




/************ C functions *************/

int getch (void);
int putch (int ch);
int kbhit (void);
int toupper (int);
void delay (unsigned);

/*********** C functions end **********/




/************ functions def ***********/

/* routines from ABA2GFX.C */
void GfxInit (void);
void GfxEnd (void);
void GfxOn (ABAGAME *);
void GfxOff (void);
char GfxMsg (char *, char);
void GfxCursorMove (FIELDPOS);
void GfxStoneMark (FIELDPOS);
void GfxStoneUnMark (FIELDPOS);
void GfxFieldShow (ABAGAME *);
void GfxKickedShow (ABAGAME *);

/* routines from ABA2MENU.C */
void MenuABA2 (void);
void MenuGameInit3 (ABAGAME *ag, int, int, int);

/* routines from ABA2SIM.C */
int SimMove (ABAGAME *ag, int nWait);

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
      g_pAbaNeigh[i][e]= atoi (buffer);
     }

    if (fileread (fh, 3, buffer)!=3)
      aba2flash (ERR_FILE, "Reading Distance Data");
    g_cAbaDistBorder[i]= atoi (buffer);

    for (e=0; e<NUM_ATTACK_DIR; e++)
     {
      if (fileread (fh, 3, buffer)!=3)
   aba2flash (ERR_FILE, "Reading Attack Data");
      g_nAbaDirAttack[i][e]= atoi (buffer);
     }
   }
  if (fileclose (fh)== LIPKEINZUGRIFF)
    aba2flash (ERR_FILE, "Closing Neigh File");
 }



/*int PlayerKicked (ABAGAME *ag)
* EV_OK / EV_KICKED / EV_WON *
 {
  if (ag->pf.cField[0] != ST_BORDER)
   {
    ag->pf.nLost[ag->pf.cField[0]]++;
    GfxKickedShow (ag);
    if (ag->pf.nLost[ag->pf.cField[0]] == ag->nKickedWon)
     {
      sprintf (buffer, "%s (%d) looses the game !!!",
	ag->player[ag->pf.cField[0]].szName, ag->pf.cField[0]);
      GfxMsg (buffer, TRUE);
      return (EV_WON);
     }
    else return (EV_KICKED);
   }
  return (EV_OK);
 }
*/

void CursorMoveRel (int nDir)
 {
  if (g_pAbaNeigh[g_pAbaCursor][nDir] == 0) return;
  GfxCursorMove (g_pAbaNeigh[g_pAbaCursor][nDir]);
 }

void StoneMarkAll (ABAGAME *ag, char bMark)
 {
  int i;

  for (i=0; i<ag->sel.nCount; i++)
   {
    if (bMark) GfxStoneMark (ag->sel.p[i]);
    else GfxStoneUnMark (ag->sel.p[i]);
   }
  if (!bMark) ag->sel.nCount= 0;
 }


int WaitEvent (ABAGAME *ag)
 {
  /* nDir, EV_ESC, EV_BACKSPACE */

  signed char buffer[80];
  int i;

  GfxCursorMove (ag->player[ag->nPlayerAct].pCursor);
  while (TRUE)
   {
    ag->player[ag->nPlayerAct].pCursor= g_pAbaCursor;
    switch (toupper(getch ()))
     {
      case 27:
       {
	return (EV_ESC);
       }
      case '\r':
      case ' ':
       {
	for (i=0; i<ag->sel.nCount; i++)
	 {
	  if (g_pAbaCursor == ag->sel.p[i])
	   {
	    GfxStoneUnMark (g_pAbaCursor);
	    ag->sel.p[i]= ag->sel.p[ag->sel.nCount-1];
	    ag->sel.nCount--;
	    i= NUM_STONES;
	    break;
	   }
	 }
	if (i == ag->sel.nCount)
	 {
	  if (ag->nPlayerAct != ag->pf.cField[g_pAbaCursor])
	   {
	    sprintf (buffer, "You can't mark this stone, %s (%d) !", \
	      ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
	    GfxMsg (buffer, TRUE);
	   }
	  else if (ag->sel.nCount >= NUM_SEL)
	   {
	    sprintf (buffer, "Already marked %d stones !", NUM_SEL);
	    GfxMsg (buffer, TRUE);
	   }
	  else
	   {
	    GfxStoneMark (g_pAbaCursor);
	    ag->sel.p[ag->sel.nCount]= g_pAbaCursor;
	    ag->sel.nCount++;
	   }
	 }
	break;
       }
      case 8: /* BS */
	return EV_BACKSPACE;

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

  p= ag->sel.p;

  for (i=0; i<ag->sel.nCount; i++)
    if (ag->pf.cField[g_pAbaNeigh[p[i]][nDir]] != ST_EMPTY) break;
  if (i == ag->sel.nCount)
   {
    for (i=0; i<ag->sel.nCount; i++)
      {
       ag->pf.cField[p[i]]= ST_EMPTY;
       ag->pf.cField[g_pAbaNeigh[p[i]][nDir]]= ag->nPlayerAct;
      }
    return (TRUE);
   }

  if (ag->sel.nCount == 2)
   {
    if (g_pAbaNeigh[p[0]][nDir]==p[1]) pAct= p[0];
    else if (g_pAbaNeigh[p[1]][nDir]==p[0]) pAct= p[1];
    else return (FALSE);
   }
  else if (ag->sel.nCount == 3)
   {
    if (g_pAbaNeigh[p[0]][nDir]==p[1]) pAct= p[0];
    else if (g_pAbaNeigh[p[2]][nDir]==p[1]) pAct= p[2];
    else return (FALSE);
   }
  else pAct= p[0];

  for (i=0,n=0; i<NUM_SEL; i++,n++)
   {
    if (ag->pf.cField[pAct] != ag->nPlayerAct) break;
    pAct= g_pAbaNeigh[pAct][nDir];
   }
  if (pAct == 0) return (FALSE);
  nDirRev= (nDir+NUM_DIR/2) % NUM_DIR;
  for (nStoneOwn=i,i=0; i<nStoneOwn; i++,n++)
   {
    if (ag->pf.cField[pAct] == ag->nPlayerAct) return (FALSE);
    if (ag->pf.cField[pAct] == ST_EMPTY || pAct == 0) break;
    g_pAbaNeigh[g_pAbaNeigh[pAct][nDir]][nDirRev]= pAct;
    pAct= g_pAbaNeigh[pAct][nDir];
   }
  if (i >= nStoneOwn) return (FALSE);

  for (; n>0; n--)
   {
    ag->pf.cField[pAct]= ag->pf.cField[g_pAbaNeigh[pAct][nDirRev]];
    pAct= g_pAbaNeigh[pAct][nDirRev];
   }
  ag->pf.cField[pAct]= ST_EMPTY;

  if (ag->pf.cField[0] != ST_BORDER)
   {
    ag->pf.nLost[ag->pf.cField[0]]++;
    ag->pf.nKicked[ag->nPlayerAct]++;
    ag->pf.cField[0] = ST_BORDER;
   }
  return (TRUE);
 }

int StoneMarkLegal (ABAGAME *ag)
 {
  int i,n;
  FIELDPOS *p, pHelp;

  p= ag->sel.p;

  if (ag->sel.nCount == 1) return (TRUE);
  else if (ag->sel.nCount == 2)
   {
    for (i=0; i<NUM_DIR; i++)
      if (g_pAbaNeigh[p[0]][i] == p[1])
   return (TRUE);
    return (FALSE);
   }
  else if (ag->sel.nCount == 3)
   {
    for (n=0; n<NUM_SEL-1; n++)
     {
      for (i=0; i<NUM_SEL-1; i++)
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
      if (g_pAbaNeigh[p[0]][i] == p[1] && g_pAbaNeigh[p[1]][i] == p[2])
   return (TRUE);
    return (FALSE);
   }
  return (FALSE);
 }


int GamePlay (ABAGAME *ag, int nWait)
/* EV_ESC */
 {
  int nEvent;
  char buffer[80];

  int n;

  HISTORY hist;
  int nHist;

  ag->nHistLower = ag->nHistUpper = 0;

  /* ? human player */
  for (n = 0; n < ag->nPlayerNum; n++)
    if (ag->player[n].nType == TYPE_HUMAN) nWait = WAIT_USER;

  /* Loop through moves */
  randomize ();
  GfxOn (ag);

  for (; ; ag->nPlayerAct = ag->nPlayerAct % ag->nPlayerNum)
   {
    hist.nPlayerAct = ag->nPlayerAct;
    hist.pf = ag->pf;

    if (ag->player[ag->nPlayerAct].nType == TYPE_HUMAN)
     {
      sprintf (buffer, "It's %s's (%d) move.",
	ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
      GfxMsg (buffer, FALSE);

      /* Wait for user to move */
      nEvent= WaitEvent (ag);

      /* ? Problem */
      if (nEvent == EV_ESC || nEvent == EV_ERROR) break;

      /* ? Backspace */
      if (nEvent == EV_BACKSPACE)
       {
	/* Look for last human player */
	nHist = -1;
	for (n = ag->nHistUpper; n != ag->nHistLower; n = (n - 1 + NUM_HIST) % NUM_HIST)
	 {
	  if (ag->player[ag->hist[n].nPlayerAct].nType == TYPE_HUMAN)
	   {
	    nHist = n;
	    break;
	   }
	 }

	/* No history found */
	if (nHist < 0)
	 {
	  GfxMsg ("You can't take back another move", TRUE);
	  continue;
	 }

	/* Take back move */
	StoneMarkAll (ag, FALSE);
	ag->pf = ag->hist[nHist].pf;
	ag->nPlayerAct = ag->hist[nHist].nPlayerAct;
	ag->nHistUpper = (nHist - 1 + NUM_HIST) % NUM_HIST;

	/* Display */
	GfxFieldShow (ag);
	GfxKickedShow (ag);
	continue;
       }


      /*** A move was chosen ***/
      /* ? legal marble combination */
      if (!StoneMarkLegal (ag))
       {
	GfxMsg ("Sorry, illegal stone combination", TRUE);
	continue;
       }

      /* ? legal move */
      if (!StoneMarkMove (ag, nEvent))
       {
	GfxMsg ("Sorry, illegal move", TRUE);
	continue;
       }

      StoneMarkAll (ag, FALSE);
      GfxFieldShow (ag);

      /* Ok */

     }
    else /* computer player */
     {
      sprintf (buffer, "Computing %s's (%d) move ...",
	ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
      GfxMsg (buffer, FALSE);

      /* Let's go ! */
      nEvent = SimMove (ag, nWait);

      /* ? Problem */
      if (nEvent == EV_NOMOVE)
       {
	sprintf (buffer, "%s (%d) can't move anymore !!!",
	  ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
	GfxMsg (buffer, TRUE);
	continue;
       }
      else if (nEvent == EV_ESC) break;
     }

    /* Ok */
    GfxKickedShow (ag);

    /* ? won the game */
    if (ag->pf.nKicked[ag->nPlayerAct] >= ag->nKickedWon)
     {
      sprintf (buffer, "%s (%d) won the game !!!",
	ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);

      if (nWait == WAIT_USER) GfxMsg (buffer, TRUE);
      else
       {
	GfxMsg (buffer, FALSE);
	if (nWait != WAIT_FAST) delay (10000);
       }
      ag->nPlayerAct++;
      nEvent = EV_OK;
      break;
     }
    /* Continue the game */
    else ag->nPlayerAct++;

    /* Save to history */
    ag->nHistUpper = (ag->nHistUpper + 1) % NUM_HIST;
    ag->hist[ag->nHistUpper] = hist;
    if (ag->nHistUpper == ag->nHistLower)
      ag->nHistLower = (ag->nHistLower + 1) % NUM_HIST;
   }

   /* End */
   GfxOff ();
   return (nEvent);
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

#endif
