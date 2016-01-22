#include "snake.h"
#include <ncurses.h>
#include <iostream>
#include <cassert>

using std::vector;


Directions Brain::Next(const vector<BodyPart>& body, 
  Position head, Position food) {
  if (prev_food_ != food) {
    graph_.clear();
    for (auto &bp : body) { 
      graph_[bp.position].block = true; 
    }
    // New food. Got to recalculate the path to it!.
    next_ = graph_.find(head, food);
    prev_food_ = food;
  }
  auto re = next_.back();
  next_.pop_back();
  return re;
}

void Snake::SetHead(Position position) {
  body_ = vector<BodyPart>(3);
  body_[0].position = position;
  body_[1].position = position + Position{-1, 0};
  body_[2].position = position + Position{-2, 0};
  direction_ = kRight;
}

void Snake::MoveAndEat(Position food, bool *eaten, bool *suicided) {
  if (ai_) next_direction_ = brain_->Next(body_, head(), food);

  Position new_head = body_[0].position;
  mutex_.lock();
  direction_ = next_direction_;
  switch(direction_) {
    case kUp:    new_head.y -= 1; break;
    case kDown:  new_head.y += 1; break;
    case kLeft:  new_head.x -= 1; break;
    case kRight: new_head.x += 1; break;
    case kNoChange: assert(false);
  }
  mutex_.unlock();

  BodyPart tail = body_.back();
  prev_tail_ = body_.back().position;
  for (int i = body_.size() - 1; i > 0 ; --i) {
    body_[i] =  body_[i-1];
    if (body_[i].position == new_head) *suicided = true;
  }

  if (new_head == food) {
    body_.push_back(tail);
    *eaten = true;
  }
  body_[0].position = new_head;
}

void Snake::SetDirection(Directions direction) {
  mutex_.lock();
  // The opposite input direction is invalid.
  switch(direction_) {
    case kUp:    if (direction == kDown) direction = kNoChange;  break;
    case kDown:  if (direction == kUp) direction = kNoChange;    break;
    case kLeft:  if (direction == kRight) direction = kNoChange; break;
    case kRight: if (direction == kLeft) direction = kNoChange;  break;
    default:;
  }
  // Write directly to direction_ will cause error, e.g.
  // inverse direction_, if direction_ first set to a legal 
  // one then the inverse. next_direction_ serves like a cache. 
  if (direction != kNoChange) next_direction_ = direction;
  mutex_.unlock();
}

