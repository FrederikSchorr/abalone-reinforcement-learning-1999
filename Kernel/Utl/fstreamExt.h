/***********************************
/
/   fstreamExt.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef FSTREAMEXT_H
#define FSTREAMEXT_H

#include <string>
#include <fstream.h>

/* * * * * * * * * * * * * * * * * */
class ifstreamExt : public ifstream
{
public:
  // Construct full path (if necessary) and open input text file
  ifstreamExt(const std::string& sDir, const std::string& sFile, 
    const std::string& sExt);

public:
  // Check for Malin EOF sequence
  void checkEnd();

public:
  std::string m_sPath;
  std::string m_sFileNoExt;
};


/* * * * * * * * * * * * * * * * * */
class ofstreamExt : public ofstream
{
public:
  // Construct full path (if necessary) and open output text file
  ofstreamExt(const std::string& sDir, const std::string& sFile, const std::string& sExt);
  ofstreamExt();

  // Construct full path (if necessary) and open output text file
  open(const std::string& sDir, const std::string& sFile, const std::string& sExt);

public:
  // Write Malin EOF sequence to file
  void writeEnd();

private:
  std::string m_sPath;

};

#endif



