#ifndef __SNAKE_GRAPH
#define __SNAKE_GRAPH

#include <vector>
#include <ncurses.h>
using std::vector;

enum Directions {kUp, kDown, kLeft, kRight, kNoChange};

struct Position {
	int x;
	int y;

    Position& operator+=(const Position &p) {
      this->x += p.x;
      this->y += p.y;
      return *this;
    }
    Position operator-(const Position &p) {
      Position res;
      res.x = this->x - p.x;
      res.y = this->y - p.y;
      return res;
    }
    Position operator+(const Position &p) { 
      return {this->x + p.x, this->y + p.y}; 
    }

    bool operator==(const Position &p) {
      return this->x == p.x && this->y == p.y;
    }
    bool operator!=(const Position &p) {
      return !(*this == p);
    }
};

struct Node {
  Node () {
    prev = nullptr;
    visited = false;
    seen = false;
  }
  Position position;
  bool block;
  Node* prev;
  bool visited;
  bool seen;
  vector<Node *> to;
};

class Graph {
 public:
  Graph(int X, int Y);

  void clear() {
    for (auto &es : grid_) for (auto &e : es) 
      e.block = e.seen = e.visited = false;
  }

  Node& operator[](Position p) { return grid_[p.x][p.y]; }

  vector<Directions> find(Position head, Position food);

  static Directions get_direction(Position to, Position from);

 private:
  vector<vector<Node> > grid_;
};

struct BodyPart {
  Position position;
  //TODO: add color & other feature.
  BodyPart& operator=(const BodyPart &bp) {
    this->position = bp.position;
    return *this;
  }
};

Graph::Graph(int X, int Y) : grid_(X, vector<Node> (Y)) {
  for (int x = 1; x < X; ++x) {
    for (int y = 1; y < Y; ++y) {
      grid_[x][y].position = {x, y};
//      mvwaddch(stdscr, y, x, '&');
//      wrefresh(stdscr);
      auto &to = grid_[x][y].to;
      if (x != 1) to.push_back(&grid_[x - 1][y]);
      if (y != 1) to.push_back(&grid_[x][y - 1]);
      if (x != X - 1) to.push_back(&grid_[x + 1][y]);
      if (y != Y - 1) to.push_back(&grid_[x][y + 1]);
    }
  }
}

vector<Directions> Graph::find(const Position head, Position food) {
  std::queue<Position> bfs_queue;
  bfs_queue.push(head);
  Position new_head;

  while (!bfs_queue.empty()) {
    new_head = bfs_queue.front();
    if (new_head == food) break;
    bfs_queue.pop();

//    mvwaddch(stdscr, new_head.y, new_head.x, '#');
//    wrefresh(stdscr);

    for (Node* node : (*this)[new_head].to) {
      if (node->block) continue;
      if (node->seen) continue;
      node->seen = true;
      node->prev = &(*this)[new_head];
      bfs_queue.push(node->position);
    }
  }
  if (new_head != food) {
    printw("dying > <");
  }

  vector<Directions> res;
  while ((*this)[new_head].position != head) {

    mvwaddch(stdscr, new_head.y, new_head.x, '@');
    wrefresh(stdscr);

    res.push_back(get_direction(new_head, 
          (*this)[new_head].prev->position));
    new_head = (*this)[new_head].prev->position;
  }
  return res;
}

Directions Graph::get_direction(Position to, Position from) {
  Position diff = to - from;
  Directions direction = kNoChange;
  if      (diff == Position{ 0, -1}) direction = kUp;
  else if (diff == Position{ 0,  1}) direction = kDown;
  else if (diff == Position{-1,  0}) direction = kLeft;
  else if (diff == Position{ 1,  0}) direction = kRight;
  else assert(0);
  return direction;
}

#endif

