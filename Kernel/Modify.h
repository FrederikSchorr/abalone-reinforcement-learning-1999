/***********************************
/
/   Modify.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 

#ifndef MODIFY_H
#define MODIFY_H

#include <iostream.h>
#include <string>

/*** Modify ************************/ 

class Modify
{
public:
  virtual void modify(const std::string &sOutput, float &f)
  {
    cout << sOutput.c_str();
    cin >> f;
  }

  virtual void modify(const std::string &sOutput, std::string &sInput)
  {
    cout << sOutput.c_str();
    char buffer[512];
    cin >> buffer;
    sInput = buffer;
  }

  virtual void modify(const std::string &sOutput, float &fInput, 
    const float &fLower, const float &fUpper, const int nDecimals)
  {
    cout << sOutput.c_str();
    cin >> fInput;
  }

  virtual void modify(const std::string &sOutput, int &nInput, 
    const int nLower, const int nUpper)
  {
    float f = nInput;
    modify(sOutput, f, nLower, nUpper, 0);
    nInput = (int) f;
  }

  virtual void println(const std::string &sOutput)
  {
    cout << sOutput.c_str() << endl;
  }
};


#endif