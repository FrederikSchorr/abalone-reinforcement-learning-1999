/***********************************
/
/   Training.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 


/*** includes **********************/
#pragma warning (disable : 4786)

#include "Training.h"
#include "utl/fstreamExt.h"

#include "Tournament.h"

/*** defintions ********************/
typedef std::vector<float> FloatVector;



/*** Training **********************/

/* * * * * * * * * * * * * * * * * */
void Training
::init(const std::string & sFile)
{
  m_sFile = sFile;
}


/* * * * * * * * * * * * * * * * * */
void Training
::run()
{
  // Open script file
  ifstreamExt ifile(Kernel::sDirectoryRoot , m_sFile, EXTENSION_TRAINING);
  char sz[256];
  g_log << "\n\n*** Training session ***\nScript file: " << m_sFile << endl;

  // Loop through training sessions
  SmartPtrPlayer spPupil;
  while (true)
  {
    // Check for EOF
    ifile.eatwhite();
    if (ifile.eof()) break;

    // Open player to be trained
    char szPlayer[256];
    ifile >> szPlayer; 
    ifile.getline(sz, 255);
    spPupil = new Player(szPlayer);
    g_log << "*** Pupil: " << *spPupil;

    // # of players
    int playerNum;
    ifile >> playerNum;
    ifile.getline(sz, 255);
    if (playerNum != 2) throw TrainingE("Only 2 player mode supported");

    // # of contests
    int nContest;
    ifile >> nContest; ifile.getline(sz, 255);

    // # of contests between backups
    int nBackup;
    ifile >> nBackup; ifile.getline(sz, 255);

    // ejectWon
    int nEjectWon;
    ifile >> nEjectWon; ifile.getline(sz, 255);

    // P(randomize board)
    float fRandomize;
    ifile >> fRandomize; ifile.getline(sz, 255);

    // # of enemies
    int nEnemies;
    ifile >> nEnemies; ifile.getline(sz, 255);

    // ? still there
    if (ifile.eof()) break;
    g_log << "Contests " << nContest << ",  Backups " << nBackup <<
      ",  nEjectWon " << nEjectWon << ",  fRandomize " << fRandomize <<
      ",  nEnemies " << nEnemies << endl;

    // Read enemies
    PlayerMap enemyPlayers;
    FloatVector fEnemyProb(nEnemies);
    float fProbSum = 0.0;
    for (Marble player = 0; player < nEnemies; player++)
    {
      ifile >> szPlayer;
      ifile.getline(sz, 255);
      enemyPlayers[player] = new Player(szPlayer);

      ifile >> fEnemyProb[player];
      ifile.getline(sz, 255);
      fProbSum += fEnemyProb[player];
      g_log << "Enemy " << (int)player << ": " << szPlayer << ", " << 
        fEnemyProb[player] << endl;
    }
    if (fProbSum > 1.0) throw TrainingE("Sum of probabilites must be below 1.0");

    // Loop through contests
    PlayerMap players;
    int nNotFinished = 0;
    long lPlies = 0;
    for (int n = 0; ; n++)
    {
      // ? backup
      if (n % nBackup == 0 && n > 0)
      {
        spPupil->save();
        spPupil->save(spPupil->lLearnGames());
        sprintf(sz, "Learned games: %ld (%4.0f plies, %g%% not finished)",
          spPupil->lLearnGames(), (float)lPlies / nBackup,
          (100.0 * nNotFinished) / nBackup);
        g_log << sz << endl;
        nNotFinished = 0;
        lPlies = 0;
      }

      // ? enough
      if (n >= nContest) break;

      // Determine first player
      for (player = 0; player < nEnemies; player++)
        if (Kernel::rand(0.0, 1.0) <= fEnemyProb[player]) break;
      if (player < nEnemies) players[0] = enemyPlayers[player];
      else players[0] = spPupil;

      // Determine second player
      for (player = 0; player < nEnemies; player++)
        if (Kernel::rand(0.0, 1.0) <= fEnemyProb[player]) break;
      if (player < nEnemies) players[1] = enemyPlayers[player];
      else players[1] = spPupil;

      // Load game
      m_game.load(players);
      m_game.ejectWon(nEjectWon);
      if (Kernel::rand(0, 1) <= fRandomize) m_game.randomizeBoard();

      // Start game
      try
      {
        spPupil->learnBegin(m_game.sit());
        while (m_game.playerWon() == MARBLE_EMPTY)
        {
          m_game.move(m_game.calcMove());
          spPupil->learnNext(m_game.sit());

          if (m_game.nPlies() > 2000) break;
        }
        lPlies += m_game.nPlies();
        m_game.cleanup();
  
        // Game over - reward
        spPupil->learnEnd(m_game.sit());
        if (m_game.playerWon() == MARBLE_EMPTY) nNotFinished++;
      }
      catch(NoMoveFoundE e)
      {
        nNotFinished++;
        g_log << "No move found" << endl;
      }
      catch(exception e)
      {
        nNotFinished++;
        spPupil->save();
        spPupil->save(spPupil->lLearnGames());
        g_log << "Training cancelled, learned games: " << spPupil->lLearnGames() << endl;
        throw;
      }
    } // n = 0(1)contests-1
 
    // Save pupil player
    spPupil->save();
    spPupil->save(spPupil->lLearnGames());
    g_log << "Training finished, learned games: " << spPupil->lLearnGames() << endl;

  } // training sessions
}



/* * * * * * * * * * * * * * * * * */
void Training
::cancel()
{
  m_game.cancel();
}
