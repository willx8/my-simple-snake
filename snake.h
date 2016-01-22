#ifndef __MY_SNAKE
#include <vector>
#include <mutex>
#include <cassert>
#include <queue>
#include <memory>
#include <thread>
#include <chrono>
#include "snake_graph.h"

using std::vector;
using std::mutex;

class Brain {
 public:
  Brain(int columns, int lines) : graph_(columns, lines) { 
    prev_food_ = {-1, -1}; 
  }

  Directions Next(const vector<BodyPart> &body, 
      Position head, Position food);

  void clear() {
    graph_.clear();
    prev_food_ = {-1, -1}; 
    next_.clear();
  }

 private:
  vector<Directions> next_;
  Graph graph_;
  Position prev_food_;
};

class Snake {
 public:
  Snake() { direction_ = kRight; }

  void SetBorder(int columns, int lines) {
    columns_ = columns;
    lines_ = lines;
  }

  void SetHead(Position position);

  Position head() const { return body_.front().position; }

  Position prev_tail() const { return prev_tail_; }

  bool Contains(Position p) {
    for (auto &bp : body_) 
      if (bp.position == p) return true;
    return false;
  }
  void clear() {
    body_.clear();
    if (brain_) brain_->clear();
  }

  void MoveAndEat(Position food_, bool *eaten, bool *suicided);

  void SetDirection(Directions direction);

  void Ai (bool ai) {
    if ((ai_ = ai) == true) 
      brain_ = std::make_unique<Brain>(columns_, lines_);
  }
 
 private:
  vector<BodyPart> body_;
  Directions direction_;
  Directions next_direction_; // store keyboard input.
  Position prev_tail_;
  bool ai_;
  std::unique_ptr<Brain> brain_;
  mutex mutex_;
  int columns_;
  int lines_;
};

#endif

