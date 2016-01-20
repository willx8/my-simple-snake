#ifndef MY_SNAKE
#define MY_SNAKE
#include <vector>
using std::vector;

enum Directions {kUp, kDown, kLeft, kRight};

struct BodyPart{
  int x;
  int y;
  //TODO: add color & other feature.
}

class Snake {
 public:
  
  Snake();

  ~Snake();

  Move
 
 private:
  vector<BodyPart> body_;

};


#endif
