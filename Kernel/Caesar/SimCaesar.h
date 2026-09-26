/***********************************
/
/   SimCaesar.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef SIMCAESAR_H
#define SIMCAESAR_H


/*** include ***********************/ 
#include "../Simulation.h"


/*** Caesar ************************/ 
namespace caesar
{

  /* * * * * * * * * * * * * * * * * */
  typedef int Value;

  /* * * * * * * * * * * * * * * * * */
  class Data
  {
  public:
    Data();

  public:
    ostream & write(ostream& os) const;

  public:
    enum Points { Border = 0, Center = 5, Neighbour, Kicked, PointsNum};

    int m_nDepth;
    int m_nTimeOut;
    Value m_vPoints[PointsNum];
  };
}

/*** SimCaesar *********************/ 
class SimCaesar : public Simulation, public caesar::Data
{
public:
  SimCaesar();
  virtual ~SimCaesar() {}
  
public:
  void load(istream&);
  void save(ostream&) const;

  ostream & write(ostream& os) const ;

  void modify(Modify &mod);

  void cancel();
  
public:
  bool bIsHuman() const;
  Move calcMove(const Situation&); 

protected:
  volatile bool m_bCancel;

private:
  caesar::Value vEval(const FullBoard& fullBoard, char nDepth);

protected:
  caesar::Value vEvalBoard(const FullBoard& fullBoard);


protected:
  FullMove m_moveBest;
  char m_nDepthMax;
  const Situation* m_pSit;
  time_t m_timeEnd;
  int m_nCounter;

  // Temporary (avoid memory reallocation)
  std::vector<MoveVector> m_moves;

  STAT(Statistics m_statNew);
};

#endif