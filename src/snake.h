#ifndef MY_SNAKE
#define MY_SNAKE
#include <vector>
#include <mutex>

using std::vector;
using std::mutex;

enum Directions {kUp, kDown, kLeft, kRight, kNoChange};

struct Position {
	int x;
	int y;

//    Position& operator=(const Position &p) {
//      this->x = p.x;
//      this->y = p.y;
//      return *this;
//    }

    Position& operator+= (const Position &p) {
      this->x += p.x;
      this->y += p.y;
      return *this;
    }
    
    Position operator+ (const Position &p) {
      return {this->x + p.x, this->y + p.y};
    }
};

struct BodyPart {
  Position position;
  //TODO: add color & other feature.
  BodyPart& operator=(const BodyPart &bp) {
    this->position = bp.position;
    return *this;
  }
};


class Snake {
 public:
  
  Snake(Position position = {0, 0});
  ~Snake();

  void SetHead(Position position);

  Position head() const {
	return body_.front().position;
  }
  Position prev_tail() const {
	return prev_tail_;
  }

  // Returns whether the snake is alive.
  void Move();

  void setDirection(Directions direction);
 
 private:
  vector<BodyPart> body_;
  Directions direction_;
  Directions next_direction_; // store keyboard input.
  Position prev_tail_;
  mutex mutex_;
};


#endif
