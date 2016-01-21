#include "snake.h"
#include <cassert>

Snake::Snake(Position position) {
  body_ = vector<BodyPart>(1);
  body_[0].position = position;
  //body_[1].position.x = 19;
  //body_[1].position.y = 20;
  direction_ = kRight;
  prev_tail_.x = 0;
  prev_tail_.y = 0;
  //body_[0] = {{20, 20}};
}

Snake::~Snake() {
}

void Snake::SetHead(Position position) {
  body_ = vector<BodyPart>(3);
  body_[0].position = position;
  body_[1].position = position + Position{-1, 0};
  body_[2].position = position + Position{-2, 0};

  direction_ = kRight;
}

void Snake::Move() {
  prev_tail_ = body_.back().position;
  for (int i = body_.size() - 1; i > 0 ; --i) {
    body_[i] =  body_[i-1];
  }
  Position new_head = body_[0].position;
  mutex_.lock();
  direction_ = next_direction_;
  switch(direction_) {
    case kUp:
      new_head.y -= 1;
      break;
    case kDown:
      new_head.y += 1;
      break;
    case kLeft:
      new_head.x -= 1;
      break;
    case kRight:
      new_head.x += 1;
      break;
    case kNoChange:
      assert(false);
  }
  mutex_.unlock();
  body_[0].position = new_head;
}

void Snake::setDirection(Directions direction) {
  mutex_.lock();
  // The opposite input direction is invalid.
  switch(direction_) {
    case kUp:
      if (direction == kDown) direction = kNoChange;
      break;
    case kDown:
      if (direction == kUp) direction = kNoChange;
      break;
    case kLeft:
      if (direction == kRight) direction = kNoChange;
      break;
    case kRight:
      if (direction == kLeft) direction = kNoChange;
      break;
    default:;
  }
  // Write directly to direction_ will cause error, e.g.
  // inverse direction_, if direction_ first set to a legal 
  // one then the inverse. next_direction_ serves like a cache. 
  if (direction != kNoChange) next_direction_ = direction;
  mutex_.unlock();
}
