/***********************************
/
/   fStreamExt.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 




/*** includes **********************/
#include "fstreamExt.h"
#include "../Kernel.h"


/* * * * * * * * * * * * * * * * * */
ifstreamExt
::ifstreamExt(const std::string& _sDir, const std::string& _sFile, const std::string& _sExt)
{
  std::string sFile, sExt;

  /* Dot in extension */
  if (_sExt[0] == '.') sExt = _sExt;
  else sExt = "." + _sExt;

  sFile = _sFile;

  /* Look for extension in file name */
  if (sFile.find(sExt) == std::string::npos)
  { 
    /* Correct ext not found */
    /* ? another extension */
    if (sFile.find('.') != std::string::npos)
      throw FileE("Illegal file extension: <" + sFile + ">");
    
    /* File without ext */
    sFile = sFile + sExt;
  }

  /* ? absolute path */
  if (sFile[1] == ':' || sFile[0] == '\\' || sFile[0] == '/') m_sPath = sFile;
  else m_sPath = _sDir + sFile;
  
  for (int n = 0; n < m_sPath.size(); n++) 
    if (m_sPath[n] == '\\') m_sPath[n] = '/';

  open(m_sPath.c_str(), ios::in | ios::nocreate);
  if (fail()) throw FileE("Could not open <" + m_sPath + ">");

  int nDot = m_sPath.find_last_of('.');
  int nSlash = m_sPath.find_last_of('/');
  m_sFileNoExt = m_sPath.substr(nSlash + 1, nDot - nSlash - 1);
}



/* * * * * * * * * * * * * * * * * */
void ifstreamExt
::checkEnd()
{
  /* Check for eof string ("Malin EOF") */
  setmode(filebuf::text);
  eatwhite();
  char buffer[256];
  getline(buffer, 255);
  if (strcmp(buffer, FILE_END_CODE) != 0)
    throw FileE("Incorrect file signature: <" + m_sPath + ">");
}



/* * * * * * * * * * * * * * * * * */
ofstreamExt
::ofstreamExt()
{
}


/* * * * * * * * * * * * * * * * * */
ofstreamExt
::ofstreamExt(const std::string& _sDir, const std::string& _sFile, const std::string& _sExt)
{
  open(_sDir, _sFile, _sExt);
}



/* * * * * * * * * * * * * * * * * */
ofstreamExt
::open(const std::string& _sDir, const std::string& _sFile, const std::string& _sExt)
{
  std::string sFile, sExt;

  /* Dot in extension */
  if (_sExt[0] == '.') sExt = _sExt;
  else sExt = "." + _sExt;

  sFile = _sFile;

  /* Look for extension in file name */
  if (sFile.find(sExt) == std::string::npos)
  { 
    /* Correct ext not found */
    /* ? another extension */
    if (sFile.find('.') != std::string::npos)
      throw FileE("Illegal file extension: <" + sFile + ">");
    
    /* File without ext */
    sFile = sFile + sExt;
  }

  /* ? absolute path */
  if (sFile[1] == ':' || sFile[0] == '\\' || sFile[0] == '/') m_sPath = sFile;
  else m_sPath = _sDir + sFile;

  ofstream::open(m_sPath.c_str(), ios::out);
  if (fail()) throw FileE("Could not create <" + m_sPath + ">");
}



/* * * * * * * * * * * * * * * * * */
void ofstreamExt
::writeEnd()
{
  setmode(filebuf::text);
  *this << "\n" << FILE_END_CODE;
}