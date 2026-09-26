/***********************************
/
/   SimHum.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef SIMHUM_H
#define SIMHUM_H

/*** SimHum ************************/ 
class SimHum : public Simulation
{
public:
  SimHum() { m_type = Simulation::human; }
  virtual ~SimHum() {}

public:
  void load(istream&) {}
  void save(ostream&) const {}
  
  ostream & write(ostream& o) const { o << "Human player"; return o; }

  void modify(Modify &mod)
  {
    mod.println("Human player");
  }


public:
  bool bIsHuman() const { return true; }
  Move calcMove(const Situation&) { throw SimulationE("Human player: Cannot calculate move"); } 
};

#endif