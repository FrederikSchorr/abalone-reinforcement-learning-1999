/***********************************
/
/   Player.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 




/*** includes **********************/
#include "Player.h"
#include "SimHum.h"

#include "Aba212/Sim212.h"
#include "Boub/SimBoub.h"
#include "Caesar/SimCaesar.h"
#include "Caesar/SimCharles.h"
#include "Caesar/SimRandom.h"
#include "David/SimDavid.h"
#include "Emil/SimEmil.h"
#include "Freud/SimFreud.h"
#include "Gunilla/SimGunilla.h"

#include "Utl/fstreamExt.h"
#include <direct.h>


/* * * * * * * * * * * * * * * * * */
Player
::Player(Simulation::Type type)
{
  switch (type)
  {
  case Simulation::human:
    m_spSim = new SimHum();
    m_sName = "Human";
    m_sDesc = "Human player";
    break;

  case Simulation::aba212:
    m_spSim = new Sim212(4);
    m_sName = "Aba";
    m_sDesc = "? plies, heuristic search";
    break;

  case Simulation::boub:
    m_spSim = new SimBoub(4);
    m_sName = "Boub";
    m_sDesc = "? plies, heuristic, quiescence";
    break;

  case Simulation::caesar:
    m_spSim = new SimCaesar();
    m_sName = "Caesar";
    m_sDesc = "? plies, brute force";
    break;

  case Simulation::charles:
    m_spSim = new SimCharles();
    m_sName = "Charles";
    m_sDesc = "? sec, brute, alpha-beta";
    break;

  case Simulation::david:
    m_spSim = new SimDavid();
    m_sName = "David";
    m_sDesc = "? sec, alpha-beta, killer moves";
    break;

  case Simulation::emil:
    m_spSim = new SimEmil();
    m_sName = "Emil";
    m_sDesc = "? sec, alpha-beta, hash table";
    break;

  case Simulation::random:
    m_spSim = new SimRandom();
    m_sName = "Random";
    m_sDesc = "Random moves";
    break;

  case Simulation::freud:
    m_spSim = new SimFreud();
    m_sName = "Freud";
    m_sDesc = "? lambda, no squash, online";
    break;

  case Simulation::gunilla:
    m_spSim = new SimGunilla();
    m_sName = "Gunilla";
    m_sDesc = "? lambda, ? temp, no squash, online";
    break;

  default:
    throw PlayerE("Unknown player type");
  }
}



/* * * * * * * * * * * * * * * * * */
Player
::Player(const SmartPtrSim& spSim, const std::string& sName, const std::string& sDesc)
{
  m_spSim = spSim;
  m_sName = sName;
  m_sDesc = sDesc;
}

/* * * * * * * * * * * * * * * * * */
Player
::Player(const std::string& sFile)
{
  load(sFile);
}



/* * * * * * * * * * * * * * * * * */
bool Player
::bIsHuman() const
{ 
  return m_spSim->bIsHuman(); 
}


/* * * * * * * * * * * * * * * * * */
Move Player
::calcMove(const Situation& sit)
{ 
  return m_spSim->calcMove(sit); 
}

/* * * * * * * * * * * * * * * * * */
void Player
::load(const std::string& sFile)
{
  /* Open file */
  ifstreamExt ifile(Kernel::sDirectoryPlayer, sFile, EXTENSION_PLAYER);

  /* Get file name without path & ext*/
  m_sName = ifile.m_sFileNoExt;

  /* Read from file */
  char szBuffer[256];
  ifile.getline(szBuffer, 255);
  m_sDesc = szBuffer;
  
  int nType;
  ifile >> nType;
  switch(nType)
  {
  case Simulation::human:
    m_spSim = new SimHum();
    break;
  
  case Simulation::aba212:
    m_spSim = new Sim212();
    break;

  case Simulation::boub:
    m_spSim = new SimBoub();
    break;
    
  case Simulation::caesar:
    m_spSim = new SimCaesar();
    break;

  case Simulation::charles:
    m_spSim = new SimCharles();
    break;

  case Simulation::david:
    m_spSim = new SimDavid();
    break;

  case Simulation::emil:
    m_spSim = new SimEmil();
    break;

  case Simulation::random:
    m_spSim = new SimRandom();
    break;

  case Simulation::freud:
    m_spSim = new SimFreud();
    break;

  case Simulation::gunilla:
    m_spSim = new SimGunilla();
    break;

  default:
    throw PlayerE("No such player type defined");
  }

  m_spSim->load(ifile);
  ifile.checkEnd();
}


/* * * * * * * * * * * * * * * * * */
void Player
::save(const std::string& sFile) const
{
  /* Open file */
  ofstreamExt ofile(Kernel::sDirectoryPlayer, sFile, EXTENSION_PLAYER);

  /* Write to file */
  ofile << m_sDesc.c_str() << endl;
  ofile << (int)m_spSim->type() << endl;
  m_spSim->save(ofile);
  ofile.writeEnd();
} 

/* * * * * * * * * * * * * * * * * */
void Player
::save() const
{
  save(m_sName);
}

/* * * * * * * * * * * * * * * * * */
void Player
::save(int nIndex) const
{
  // Ensure that directory is existing
  char szDir[512];
  sprintf(szDir, "%s%s", Kernel::sDirectoryPlayer.c_str(), m_sName.c_str());
  mkdir(szDir);

  // Save file
  char sz[256];
  sprintf(sz, "%s/%s%7.7d.%s", szDir, m_sName.c_str(), nIndex, EXTENSION_PLAYER);
  save(sz);
}


/* * * * * * * * * * * * * * * * * */
void Player
::modify(Modify &mod)
{
  m_spSim->modify(mod);
  mod.modify("Description", m_sDesc);
}



/* * * * * * * * * * * * * * * * * */
const char *Player
::sSimDesc(Simulation::Type type)
{
  switch(type)
  {
  case Simulation::human:
    return "Human Player (Human)";

  case Simulation::aba212:
    return "Heuristic (Aba)";

  case Simulation::boub:
    return " + quiescence (Boub)";

  case Simulation::caesar:
    return "Brute force (Caesar)";

  case Simulation::charles:
    return " + alpha-beta (Charles)";

  case Simulation::david:
    return " + killer moves (David)";

  case Simulation::emil:
    return " + hash table (Emil)";

  case Simulation::random:
    return "Random moves (Random)";

  case Simulation::freud:
    return "Neural network (Freud)";

  case Simulation::gunilla:
    return "Neural network, simulated annealing (Gunilla)";

  default:
    return "Unknown player type";
  }
}


/* * * * * * * * * * * * * * * * * */
void Player
::init()
{
  /* Init simulations */
  Sim212::init();
  SimBoub::init();
}


/* * * * * * * * * * * * * * * * * */
ostream & Player
::write(ostream & os) const
{
  os << "Player <" << m_sName << ">: " << m_sDesc << endl;
  return m_spSim->write(os);
}
