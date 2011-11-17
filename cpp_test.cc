#include <iostream>

using namespace std;

class humanoid{
public:
  humanoid(unsigned int h =0, unsigned int w =0 )  {cout<<"humanoid"<<endl; }

  void set(unsigned h, unsigned w){ this->height = h; this->weight = w;}
  virtual void display() = 0;
  //  virtual void display(){ cout << "humanoid height:"<<this->height<<", weight:"<<this->weight<<endl; }

protected:
  unsigned int height, weight;
};

class agile_hero : virtual public humanoid{
public:
  agile_hero(unsigned int h, unsigned int w): humanoid(h, w), agile_val(h/2) { cout<<"agile hero"<<endl; }
  void display(){ cout << "agile hero height:"<<this->height<<", weight:"<<this->weight<<", agile value:"<<this->agile_val<<endl; }

protected:
  unsigned int agile_val;
};

class strength_hero : virtual public humanoid{
public:
  strength_hero(unsigned int h, unsigned int w) : humanoid(h, w), strength_val(w/2) { cout<<"strength hero"<< endl; }
  void display(){ cout << "strength hero height:"<<this->height<<", weight:"<<this->weight<<", strength value:"<<this->strength_val<<endl; }

protected:
  unsigned int strength_val;
};
#if 1
class undead : public agile_hero, public strength_hero{
public:
  undead(unsigned int h, unsigned int w) : agile_hero(h, w), strength_hero(h, w) { cout<<"undead"<<endl;}
  void display(){ cout << "undead hero height:"<<this->height<<", weight:"<<this->weight<<", strength value:"<<this->strength_val<<", agile value:"<<this->agile_val<<endl; }
};
#endif
int main()
{
  
  undead arth(10,10);
  arth.set(10,10);
  arth.display();
  return 0;
}
