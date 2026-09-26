/*	(C) LIPSOFT 1990 / 1991

	ABALONE.C

	(+ ABAUTL.C,ABADEF.C,ABALONE.PRJ)
	das Spiel Abalone
*/




/* prozeduren:
	setfelder
	setmoglkugel
	getzsp
	pfeile
	push
	test
	setaba
	showzug
	hiddenpush
	gegthink
	think
	compplay
	play
	twoplayer
	main
*/


#include "C:\PRO\TC\PROG\ABALONE\ABALONE.CD"


int ax,ay,but,ev,akt = 0;
int sp,prsp,gsp,gprsp;
char mca[69];
char mcpfeil[69];
char mcblitz[69];

#define  MAXMOGLZUG  	100


#define  setpartaba(source,ziel)\
	  {\
	   ifeld (source.feld,ziel.feld);\
	   ziel.outsp[SP1] = source.outsp[SP1];\
	   ziel.outsp[SP2] = source.outsp[SP2];\
	   ziel.pos = source.pos;\
	  }

#define	 setcompaba(source,ziel)\
	  {\
	   setpartaba (source,ziel);\
	   ziel.a = source.a;\
	   ziel.b = source.b;\
	   ziel.c = source.c;\
	   ziel.d = source.d;\
	   ziel.e = source.e;\
	   ziel.f = source.f;\
	   ziel.seite = source.seite;\
	   ziel.radius = source.radius;\
	   ziel.color[0] = source.color[0];\
	   ziel.color[1] = source.color[1];\
	   ziel.color[2] = source.color[2];\
	   ziel.color[3] = source.color[3];\
	   ziel.color[4] = source.color[4];\
	   ziel.buffer[0] = source.buffer[0];\
	   ziel.buffer[1] = source.buffer[1];\
	   ziel.buffer[2] = source.buffer[2];\
	   ziel.buffer[3] = source.buffer[3];\
	   ziel.buffer[4] = source.buffer[4];\
	  }



#define  setpunkte(maxpunktei,zugi,zugzahli,punktei,positioni,richti,anzi)\
	  {\
	   if (punktei == maxpunktei)\
	    {\
	     if (zugzahli >= (MAXMOGLZUG - 1))\
	      {\
	       help = random (MAXMOGLZUG);\
	       zugi[help].pos[0] = positioni[0];\
	       if (anzi >= 2) zugi[help].pos[1] = positioni[1];\
	       if (anzi == 3) zugi[help].pos[2] = positioni[2];\
	       zugi[help].anzahl = anzi;\
	       zugi[help].richt = richti;\
	      }\
	     else\
	      {\
	       zugi[zugzahli].pos[0] = positioni[0];\
	       if (anzi >= 2) zugi[zugzahli].pos[1] = positioni[1];\
	       if (anzi == 3) zugi[zugzahli].pos[2] = positioni[2];\
	       zugi[zugzahli].anzahl = anzi;\
	       zugi[zugzahli].richt = richti;\
	       zugzahli++;\
	      }\
	    }\
	   else if (punktei > maxpunktei)\
	    {\
	     zugzahli = 1;\
	     maxpunktei = punktei;\
	     zugi[0].pos[0] = positioni[0];\
	     if (anzi >= 2) zugi[0].pos[1] = positioni[1];\
	     if (anzi == 3) zugi[0].pos[2] = positioni[2];\
	     zugi[0].anzahl = anzi;\
	     zugi[0].richt = richti;\
	    }\
	  }





#define  ifeld(source,ziel)   \
	  {\
	   for (u = 1;u <= 9;u++)\
	     for (o = 1;o <= (9-betrag(5-u));o++) ziel[u][o] = source[u][o];\
	  }



#define  mcursor(ab,mc1,mc2,in)  \
	  {\
	   if (getzsp (ab,ax,ay) != SET)\
	    {\
	     if (in)\
	      {\
	       in = 0;\
	       mcakt (mc1);\
	      }\
	    }\
	   else if (testmpix (ax,ay,color))\
	    {\
	     if (!in)\
	      {\
	       in = 1;\
	       mcakt (mc2);\
	      }\
	    }\
	   else\
	    {\
	     if (in)\
	      {\
	       in = 0;\
	       mcakt (mc1);\
	      }\
	    }\
	  }



/*
void far closegraph (void);
int far getpixel (int x,int y);
void far circle (int x,int y,int radius);
void far line (int x1,int y1,int x2,int y2);
void far setwritemode (int mode);
void far putpixel (int x,int y,int col);
void far rectangle (int l,int o,int r,int u);
void far setcolor (int color);
void far setfillstyle (int pattern,int color);
void far bar (int l,int o,int r,int u);
void far outtextxy (int x,int y,char far *text);
void far floodfill (int x,int y,int border);
*/
int  gprintf (int *x,int *y,char *fmt, ... );
long int think (abalone *aba,byte spieler,byte rekurs);
void mouseon (void);
void mouseoff (void);


void setfelder (abalone *aba,int source,int inumzuw)
 {
  position pos;

  for (pos.z = 0;pos.z < 12;pos.z++)
    for (pos.sp = 0;pos.sp < 12;pos.sp++)
      if (aba->feld[pos.z][pos.sp] == source)
	aba->feld[pos.z][pos.sp] = inumzuw;
 }





void setmoglkugel (abalone *aba,position *press,byte spieler,byte anzahl)
 {
  byte richt[7];
  byte i;
  byte e;
  position pos;

  if (anzahl == 0)
   {
    for (aba->pos.z = 1;aba->pos.z < 10;aba->pos.z++)
      for (aba->pos.sp = 1;aba->pos.sp < 10;aba->pos.sp++)
	if (aba->feld[aba->pos.z][aba->pos.sp] == spieler && kugelkann (aba) == ABAYES)
	  aba->feld[aba->pos.z][aba->pos.sp] = SET;
   }
  else if (anzahl == 1)
   {
    aba->pos = press[0];
    nextinrichtung (&aba->pos,4);

    for (i = 0;i < 6;i++)
     {
      if (aba->feld[aba->pos.z][aba->pos.sp] == HG) richt[((i + 4) % 6)] = ABAYES;
      nextinrichtung (&aba->pos,i);
     }

    for (i = 0;i < 6;i++)
     {
      if (aba->feld[aba->pos.z][aba->pos.sp] == spieler)
       {
	for (e = 0;e < 6;e++)
	 {
	  if (richt[e] == ABAYES)
	   {
	    pos = aba->pos;
	    nextinrichtung (&pos,e);
	    if (aba->feld[pos.z][pos.sp] == HG)
	     {
	      aba->feld[aba->pos.z][aba->pos.sp] = SET;
	      break;
	     }
	   }
	 }
       }
      nextinrichtung (&aba->pos,i);
     }
    nextinrichtung (&aba->pos,1);
   }

  else if (anzahl ==  2)
   {
    aba->pos = press[0];
    nextinrichtung (&aba->pos,4);
    for (i = 0;i < 6;i++)
     {
      if (aba->feld[aba->pos.z][aba->pos.sp] == HG) richt[i] = ABAYES;
      else richt[i] = ABANO;
      nextinrichtung (&aba->pos,i);
     }
    aba->pos = press[1];
    nextinrichtung (&aba->pos,4);
    for (i = 0;i < 6;i++)
     {
      if (richt[i] == ABAYES && aba->feld[aba->pos.z][aba->pos.sp] != HG)
	richt[i] = ABANO;
      nextinrichtung (&aba->pos,i);
     }

    pos = press[0];
    nextinrichtung (&pos,4);
    for (i = 0;i < 6;i++)
     {
      if (aba->feld[pos.z][pos.sp] == (spieler + 2)) break;
      nextinrichtung (&pos,i);
     }
    i = (i + 4) % 6;
    pos = press[1];
    nextinrichtung (&pos,i);
    if (aba->feld[pos.z][pos.sp] == spieler)
     {
      aba->pos = pos;
      nextinrichtung (&pos,4);
      for (e = 0;e < 6;e++)
       {
	if (richt[e] == ABAYES && aba->feld[pos.z][pos.sp] == HG)
	 {
	  aba->feld[aba->pos.z][aba->pos.sp] = SET;
	  break;
	 }
	nextinrichtung (&pos,e);
       }
     }
    pos = press[0];
    i = (i + 3) % 6;
    nextinrichtung (&pos,i);
    if (aba->feld[pos.z][pos.sp] == spieler)
     {
      aba->pos = pos;
      nextinrichtung (&pos,4);
      for (e = 0;e < 6;e++)
       {
	if (richt[e] == ABAYES && aba->feld[pos.z][pos.sp] == HG)
	 {
	  aba->feld[aba->pos.z][aba->pos.sp] = SET;
	  break;
	 }
	nextinrichtung (&pos,e);
       }
     }
   }
 }


int getzsp (abalone *aba,int x,int y)
 {
  position pos;
  float help;


  if (y < aba->a.y || y > aba->d.y) return (OUT);
  help = y - aba->a.y - (aba->seite * 0.0866);
  help = help / aba->seite * 5 / 0.866;
  pos.z = (int)(help) + 1;

  help = x - aba->f.x - ((betrag (5 - pos.z) + 1) * aba->seite / 10);
  help = help / aba->seite * 5 + 1;
  pos.sp = (int)(help);

  if (pos.sp <= 0 || pos.sp > (9 - (betrag (5 - pos.z)))) return (OUT);
  aba->pos = pos;
  return (aba->feld[pos.z][pos.sp]);
 }


int pfeile (int color)
 {
  if (infeld (26,13,50,38,EGA_WHITE,color,mca,mcpfeil) == ABAYES) return (0);
  if (infeld (72,13,96,38,EGA_WHITE,color,mca,mcpfeil) == ABAYES) return (1);
  if (infeld (101,47,122,79,EGA_WHITE,color,mca,mcpfeil) == ABAYES) return (2);
  if (infeld (72,88,96,113,EGA_WHITE,color,mca,mcpfeil) == ABAYES) return (3);
  if (infeld (26,88,50,113,EGA_WHITE,color,mca,mcpfeil) == ABAYES) return (4);
  if (infeld (0,47,21,79,EGA_WHITE,color,mca,mcpfeil) == ABAYES) return (5);
  if (infeld (45,47,77,79,EGA_WHITE,color,mca,mcpfeil) == ABAYES) return (6);

  return (-1);
 }


byte push (abalone *aba,position *pos,byte anzahl,byte richt)
 {
  int i;
  int ain,neuin;

  for (i = 0;i < anzahl;i++)
   {
    aba->pos = pos[i];
    neuin = HG;
    for (;;)
     {
      ain = aba->feld[aba->pos.z][aba->pos.sp];
      setfeld (aba,neuin);
      if (ain == OUT || ain == HG) break;
      nextinrichtung (&aba->pos,richt);
      neuin = ain;
     }
   }
  return (NULL);
 }


int test (abalone *aba,byte spieler)
 {
  int punkte = 0;
  position pos;
  byte i;

  for (pos.z = 1;pos.z <= 9;pos.z++)
    for (pos.sp = 1;pos.sp <= 9;pos.sp++)
     {
      if (aba->feld[pos.z][pos.sp] == spieler)
       {
	nextinrichtung (&pos,4);
	for (i = 0;i < 6;i++)
	 {
	  if (aba->feld[pos.z][pos.sp] == spieler) punkte++;
	  nextinrichtung (&pos,i);
	 }
	nextinrichtung (&pos,1);
       }
     }
  return (punkte);
 }



void setaba (abalone *aba)
 {
  byte lastsp;
  position pos;
  byte inhalt;
  float x,y;

  pos = aba->pos;

  for (aba->pos.z = 1;aba->pos.z <= 9;aba->pos.z++)
   {
    lastsp = 9 - betrag (5 - aba->pos.z);
    for (aba->pos.sp = 1;aba->pos.sp <= lastsp;aba->pos.sp++)
     {
      inhalt = aba->feld[aba->pos.z][aba->pos.sp];
      x = (2 * aba->pos.sp + betrag (5 - aba->pos.z)) * (aba->seite / 10);
      x = x + aba->f.x;
      y = (aba->pos.z * aba->seite * 0.1732) + aba->a.y;
      if (!testmpix (x,y,aba->color[inhalt])) setfeld (aba,inhalt);
     }
   }

  aba->pos = pos;
 }




typedef struct
 {
  position pos[3];
  byte anzahl;
  byte richt;
 }moglzug;


void showzug (abalone *aba,position *pos,byte anzahl,byte richt,long int punkte)
 {
  byte i,u,o;
  abalone copyaba;

  setcompaba ((*aba),copyaba);

  push (&copyaba,pos,anzahl,richt);

  gotoxy (1,10);
  mouseoff ();
  for (i = 0;i < anzahl;i++)
   {
    printf ("%d/%d  ",(int)pos[i].z,(int)pos[i].sp);
   }
  printf ("%d, %ld            \n",(int)richt,punkte);

  setaba (aba);

  mouseon ();
 }


long int hiddenpush (abalone *aba,position *pos,byte anzahl,byte richt)
 {
  int i;
  int ain,neuin;

  for (i = 0;i < anzahl;i++)
   {
    aba->pos = pos[i];
    neuin = HG;
    for (;;)
     {
      ain = aba->feld[aba->pos.z][aba->pos.sp];
      if (ain == OUT) return (ABAKILLED);
      aba->feld[aba->pos.z][aba->pos.sp] = neuin;
      if (ain == HG) break;
      nextinrichtung (&aba->pos,richt);
      neuin = ain;
     }
   }
  return (ABAPUSHED);
 }




long int gegthink (abalone *aba,byte spieler,byte rekurs)
 {
  byte lastsp;
  abalone copyaba;
  position rund,rund1;
  byte richtung[6],richtung1[6];
  position posi[3];
  byte i,richt,u,o;
  long int punkte = 0;


  if (rekurs > 0)
   {
    rekurs--;

    for (aba->pos.z = 1;aba->pos.z <= 9;aba->pos.z++)
     {
      lastsp = 9 - betrag (5 - aba->pos.z);
      for (aba->pos.sp = 1;aba->pos.sp <= lastsp;aba->pos.sp++)
       {
	if (aba->feld[aba->pos.z][aba->pos.sp] == spieler)
	 {
	  rund = aba->pos;
	  nextinrichtung (&rund,4);
	  for (richt = 0;richt < 6;richt++)
	   {
	    if (aba->feld[rund.z][rund.sp] == HG) richtung[richt] = ABAYES;
	    else richtung[richt] = ABANO;
	    nextinrichtung (&rund,richt);

	    if (moglrichtung (aba,richt) == ABAYES)
	     {
	      setpartaba ((*aba),copyaba);
	      punkte-= hiddenpush (aba,&(aba->pos),1,richt);
	      punkte+= think (aba,3 - spieler,rekurs);
	      setpartaba (copyaba,(*aba));
	     }
	   }
	  posi[0] = aba->pos;
	  for (i = 0;i < 3;i++)
	   {
	    if (aba->feld[rund.z][rund.sp] == spieler)
	     {
	      rund1 = rund;
	      nextinrichtung (&rund1,4);
	      for (richt = 0;richt < 6;richt++)
	       {
		richtung1[richt] = ABANO;
		if (richtung[richt] == ABAYES)
		 {
		  if (aba->feld[rund1.z][rund1.sp] == HG)
		   {
                    posi[1] = rund;
		    setpartaba ((*aba),copyaba);
		    punkte-= hiddenpush (aba,posi,2,((richt + 4) % 6));
		    punkte+= think (aba,3 - spieler,rekurs);
		    setpartaba (copyaba,(*aba));
		    richtung1[richt] = ABAYES;
		   }
		 }
		nextinrichtung (&rund1,richt);
	       }
	      rund1 = rund;
	      nextinrichtung (&rund1,((i + 4) % 6));
	      if (aba->feld[rund1.z][rund1.sp] == spieler)
	       {
		posi[2] = rund1;
		nextinrichtung (&rund1,4);
		for (richt = 0;richt < 6;richt++)
		 {
		  if (richtung1[richt] == ABAYES)
		   {
		    if (aba->feld[rund1.z][rund1.sp] == HG)
		     {
                      setpartaba ((*aba),copyaba);
		      punkte-= hiddenpush (aba,posi,3,((richt + 4) % 6));
		      punkte+= think (aba,3 - spieler,rekurs);
		      setpartaba (copyaba,(*aba));
		     }
		   }
		  nextinrichtung (&rund1,richt);
		 }
	       }
	     }
	    nextinrichtung (&rund,i);
	   }
	 }
       }
     }
   }

  punkte-= test (aba,spieler);

  return (punkte);
 }



long int think (abalone *aba,byte spieler,byte rekurs)
 {
  byte lastsp;
  abalone copyaba;
  position rund,rund1;
  byte richtung[6],richtung1[6];
  position posi[3];
  byte i,richt,o,u;
  long int punkte = 0;


  if (rekurs > 0)
   {
    rekurs--;

    for (aba->pos.z = 1;aba->pos.z <= 9;aba->pos.z++)
     {
      lastsp = 9 - betrag (5 - aba->pos.z);
      for (aba->pos.sp = 1;aba->pos.sp <= lastsp;aba->pos.sp++)
       {
	if (aba->feld[aba->pos.z][aba->pos.sp] == spieler)
	 {
	  rund = aba->pos;
	  nextinrichtung (&rund,4);
	  for (richt = 0;richt < 6;richt++)
	   {
	    if (aba->feld[rund.z][rund.sp] == HG) richtung[richt] = ABAYES;
	    else richtung[richt] = ABANO;
	    nextinrichtung (&rund,richt);

	    if (moglrichtung (aba,richt) == ABAYES)
	     {
	      setpartaba ((*aba),copyaba);
	      punkte+= hiddenpush (aba,&(aba->pos),1,richt);
	      punkte+= gegthink (aba,3 - spieler,rekurs);
	      setpartaba (copyaba,(*aba));
	     }
	   }
	  posi[0] = aba->pos;
	  for (i = 0;i < 3;i++)
	   {
	    if (aba->feld[rund.z][rund.sp] == spieler)
	     {
	      rund1 = rund;
	      nextinrichtung (&rund1,4);
	      for (richt = 0;richt < 6;richt++)
	       {
		richtung1[richt] = ABANO;
		if (richtung[richt] == ABAYES)
		 {
		  if (aba->feld[rund1.z][rund1.sp] == HG)
		   {
                    posi[1] = rund;
		    setpartaba ((*aba),copyaba);
		    punkte+= hiddenpush (aba,posi,2,((richt + 4) % 6));
		    punkte+= gegthink (aba,3 - spieler,rekurs);
		    setpartaba (copyaba,(*aba));
		    richtung1[richt] = ABAYES;
		   }
		 }
		nextinrichtung (&rund1,richt);
	       }
	      rund1 = rund;
	      nextinrichtung (&rund1,((i + 4) % 6));
	      if (aba->feld[rund1.z][rund1.sp] == spieler)
	       {
		posi[2] = rund1;
		nextinrichtung (&rund1,4);
		for (richt = 0;richt < 6;richt++)
		 {
		  if (richtung1[richt] == ABAYES)
		   {
		    if (aba->feld[rund1.z][rund1.sp] == HG)
		     {
                      setpartaba ((*aba),copyaba);
		      punkte+= hiddenpush (aba,posi,3,((richt + 4) % 6));
		      punkte+= gegthink (aba,3 - spieler,rekurs);
		      setpartaba (copyaba,(*aba));
		     }
		   }
		  nextinrichtung (&rund1,richt);
		 }
	       }
	     }
	    nextinrichtung (&rund,i);
	   }
	 }
       }
     }
   }

  punkte+= test (aba,spieler);

  return (punkte);
 }


long int simulate (abalone *aba,byte spieler,byte rekurs)
 {
  long int punkte = 0;

  punkte = test (aba,spieler);
  punkte+= gegthink (aba,3 - spieler,rekurs);
  return (punkte);
 }




byte compplay (abalone *aba,byte spieler,byte rekurs)
 {
  moglzug zug[MAXMOGLZUG];
  byte zugzahl = 0;
  byte richtung[6],richtung1[6];
  position rund,rund1;
  position posi[3];
  byte anzahl;
  byte help;
  byte richt;
  byte i,u,o;
  long int maxpunkte,punkte = 12;
  abalone caba;

  maxpunkte = 0;
  for (aba->pos.z = 1;aba->pos.z <= 9;aba->pos.z++)
   {
    for (aba->pos.sp = 1;aba->pos.sp <= (9-betrag(5-aba->pos.z));aba->pos.sp++)
     {
      if (aba->feld[aba->pos.z][aba->pos.sp] == spieler)
       {
	rund = aba->pos;
	nextinrichtung (&rund,4);
	for (richt = 0;richt < 6;richt++)
	 {
	  if (aba->feld[rund.z][rund.sp] == HG) richtung[richt] = ABAYES;
	  else richtung[richt] = ABANO;
	  nextinrichtung (&rund,richt);

	  if (moglrichtung (aba,richt) == ABAYES)
	   {
	    setpartaba ((*aba),caba);
	    punkte = hiddenpush (aba,&(aba->pos),1,richt);
	    punkte+= simulate (aba,spieler,rekurs);
	    setpartaba (caba,(*aba));
	    anzahl = 1;
	    setpunkte (maxpunkte,zug,zugzahl,punkte,(&(aba->pos)),richt,anzahl);
	   }
	 }
	posi[0] = aba->pos;
	for (i = 0;i < 3;i++)
	 {
	  if (aba->feld[rund.z][rund.sp] == spieler)
	   {
	    rund1 = rund;
	    nextinrichtung (&rund1,4);
	    for (richt = 0;richt < 6;richt++)
	     {
              richtung1[richt] = ABANO;
	      if (richtung[richt] == ABAYES)
	       {
		if (aba->feld[rund1.z][rund1.sp] == HG)
		 {
		  posi[1] = rund;
		  anzahl = 2;
		  setpartaba ((*aba),caba);
		  punkte = hiddenpush (aba,posi,2,((richt + 4) % 6));
		  punkte+= simulate (aba,spieler,rekurs);
		  setpartaba (caba,(*aba));
		  setpunkte (maxpunkte,zug,zugzahl,punkte,posi,((richt + 4) % 6),anzahl);
		  richtung1[richt] = ABAYES;
		 }
	       }
	      nextinrichtung (&rund1,richt);
	     }
	    rund1 = rund;
	    nextinrichtung (&rund1,((i + 4) % 6));
	    if (aba->feld[rund1.z][rund1.sp] == spieler)
	     {
	      posi[2] = rund1;
	      nextinrichtung (&rund1,4);
	      for (richt = 0;richt < 6;richt++)
	       {
		if (richtung1[richt] == ABAYES)
		 {
		  if (aba->feld[rund1.z][rund1.sp] == HG)
		   {
		    anzahl = 3;

		    setpartaba ((*aba),caba);
		    punkte = hiddenpush (aba,posi,3,((richt + 4) % 6));
		    punkte+= simulate (aba,spieler,rekurs);
		    setpartaba (caba,(*aba));
		    setpunkte (maxpunkte,zug,zugzahl,punkte,posi,((richt + 4) % 6),anzahl);
		   }
		 }
		nextinrichtung (&rund1,richt);
	       }
	     }
	   }
	  nextinrichtung (&rund,i);
	 }
       }
     }
   }


  help = random (zugzahl);
  push (aba,zug[help].pos,zug[help].anzahl,zug[help].richt);

  return (ABAOKAY);
 }





byte play (abalone *aba,byte spieler)
 {
  byte in = 0;
  byte anzahl = 0;
  int richt;
  byte i;
  int color = aba->color[spieler];
  position press[4];
  byte nexter = 0;

  setmoglkugel (aba,press,spieler,0);

  for (akt = 1;!nexter;)
   {
    if (!akt) continue;
    akt = 0;

    if (pressedbox (505,405,545,415,"Quit",mca,mcblitz)) return (ABAEXIT);
    mcursor (aba,mca,mcpfeil,in);

    if (in && (but & 1))
     {
      press[anzahl] = aba->pos;
      setfelder (aba,SET,spieler);
      anzahl++;
      setpfeile (aba,press,anzahl);
      setfeld (aba,spieler + 2);
      setmoglkugel (aba,press,spieler,anzahl);
     }

    if (anzahl)
     {
      richt = pfeile (color);
      switch (richt)
       {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	  for (i = 0;i < anzahl;i++)
	   {
	    aba->pos = press[i];
	    setfeld (aba,spieler);
	   }
	  defaultpfeile ();
	  setfelder (aba,SET,spieler);
	  push (aba,press,anzahl,richt);
	  nexter = 1;
	  break;

	case 6:
	  setfelder (aba,SET,spieler);
	  anzahl--;
	  aba->pos = press[anzahl];
	  setfeld (aba,spieler);
	  setpfeile (aba,press,anzahl);
	  setmoglkugel (aba,press,spieler,anzahl);
	  break;
       }
     }
   }

  return (ABAOKAY);
 }





void twoplayer (abalone *aba)
 {

  while (play (aba,1) == ABAOKAY && play (aba,2) == ABAOKAY);
 }



void main (void)
 {
  abalone aba;

  atexit (abaende);
  zeichnen (&aba,EGA_LIGHTRED,EGA_RED,EGA_LIGHTBLUE,EGA_BLUE);

  twoplayer (&aba);

  exit (0);
 }








