/***********************************
/
/   SmartPtr.h
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 


#ifndef SMARTPTR_H
#define SMARTPTR_H

#include <exception>

/*** exception *********************/ 
class SmartE : public exception
{
public: SmartE(const std::string& s) : 
  exception(s.c_str()) { };
};


/*** SmartPtr **********************/ 
class SmartObject 
{
  int crefs;
  
public:    
  SmartObject(void) 
  {
    //TRACE("SmartObject::Constructor %p\n", this);
    crefs = 0;
  }
  
  virtual ~SmartObject() 
  {
    //TRACE("SmartObject::Deconstructor %p\n", this);
  }
  
  void upcount(void) 
  {
    ++crefs; 
    //TRACE("up to %d\n", crefs);
  }
  
  void downcount(void)
  {     
    if (--crefs == 0)      
    {
      delete this;
    }
    else
    {
      //TRACE("downto %d\n", crefs);     
    }
  }
};

/*** SmartPtr **********************/ 
template <class T> class SmartPtr 
{   
  T* p;
  
public:
  SmartPtr()
  {
    p = NULL;
  }

  SmartPtr(T* p_) : p(p_) 
  {
    if (p != NULL) p->upcount(); 
  }

  SmartPtr(SmartPtr<T> &p_)
  {
    p = p_;
    if (p != NULL) p->upcount(); 
  }

  ~SmartPtr(void) 
  {
    if (p != NULL) p->downcount(); 
  }
  
  operator T*(void) const { return p; }    
  T& operator*(void) const
  { 
    if (p == NULL) throw SmartE("Pointer == NULL");
    return *p; 
  }
  T* operator->(void) const { return p; }    
  SmartPtr& operator=(SmartPtr<T> &p_)
  {
    return operator=((T *) p_); 
  }
  SmartPtr& operator=(T* p_) 
  {
    if (p != NULL) p->downcount(); 
    p = p_; 
    if (p != NULL) p->upcount(); 
    return *this;    
  }
};


#endif