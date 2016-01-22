#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <string>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <signal.h>
#include "snake.h"

using std::chrono::milliseconds;

class Playground {
 public:
  Playground(int speed_mode, bool human) : kRefreshSpeed(speed_mode) {
    srand(time(NULL));
    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);
    init_pair(4, COLOR_MAGENTA, COLOR_YELLOW);
    init_pair(5, COLOR_CYAN, COLOR_BLACK); 

    kLeftBound = 0;
    kUpBound = 0;
    int h,w;
    getmaxyx(stdscr, h, w);
    kDownBound = h - 1;
    kRightBound = w - 1;
    snake_.SetBorder(kRightBound, kDownBound);
    snake_.Ai(!human);
    
    window_ = stdscr;
    keypad(window_, TRUE);
    wrefresh(window_);
    wmove(window_, 1, 1);
  }
 
//  Playground(int left_bound, int up_bound, int nlines, int ncols) {
//    initscr();
//    kLeftBound = left_bound;
//    kUpBound = up_bound;
//    kRightBound = left_bound + ncols;
//    kDownBound = up_bound + nlines;
//
//    wborder(window_, 0, 0, 0, 0, 0, 0, 0, 0);
//    cbreak();
//    noecho();
//    keypad(window_, true);
//    nodelay(window_, true);
//    wrefresh(window_);
////    signal(SIGWINCH, resizeHandler);
//  }

  void Welcome() {
    print_get_mutex_.lock();
    wmove(window_, center().y - 1, center().x - 15);
    printw("Welcome to play My Simple Snake!\n");
    wmove(window_, center().y, center().x - 15);
    printw("Press any key to start. Enjoy yourself!\n");
    wmove(window_, center().y+1, center().x - 15);
    printw("You are in speed mode:x%d", 500 / kRefreshSpeed);    
    getch();
    wmove(window_, 1, 1);
    print_get_mutex_.unlock();
  }

  bool DrawSnake(const Snake& snake) {
    // This mutex avoid conflict when deals with window.
    // Comment mutexs you may see the strange bug.
    {
      std::unique_lock<std::mutex> lck(print_get_mutex_);
      mvwaddch(window_, snake.head().y, 
          snake.head().x, '*' | COLOR_PAIR(4));
      mvwaddch(window_, snake.prev_tail().y, snake.prev_tail().x, ' ');
      wmove(window_, kDownBound, 0);
      wrefresh(window_);
    }
   // printw(" I ain't locked!\n");
   // wrefresh(window_);
    bool game_over = snake.head().x == kLeftBound || 
      snake.head().x == kRightBound || snake.head().y == kUpBound || 
      snake.head().y == kDownBound;
    return game_over;
  }

  bool GameOver() {
    print_get_mutex_.lock();
    wmove(window_, center().y, center().x - 15);
    printw("Game over. Final score: %d", score_);
    wmove(window_, center().y + 1, center().x - 15);
    printw("%s. Play again? [y/N]?", 
        (user_quit_ ? "You quit" : "You die"));
    nodelay(window_, FALSE);
    nocbreak();
    echo();
    int ch = getch();
    print_get_mutex_.unlock();
    return ch == 'y'; 
  }

  void Clear() {
    snake_.clear();
    noecho();
    cbreak();
    wclear(window_);
    wborder(window_, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(window_);
    score_ = -1;
    game_over_ = user_quit_ = false;
  }

  void RunSingleSnake() {
    nodelay(window_, TRUE);
    snake_.SetHead(center());
    wmove(window_, kDownBound, center().x - 20);
    printw("You may press <q> to stop game at anytime.");
    UpdateScoreAndNewFood();

    auto kb_listen = [this] () {
      int ch;
      while (!game_over_ && !user_quit_) {
        std::this_thread::sleep_for(milliseconds(5));
        // This mutex avoid conflict when deals with window.
        // Comment mutexs you may see the strange bug.
        {
          std::unique_lock<std::mutex> lck(print_get_mutex_);
          ch = getch();
        }
        if (ch != ERR) {
          Directions direction = kNoChange;
          switch (ch) {
            case KEY_UP: direction = kUp; break;
            case KEY_DOWN: direction = kDown; break;
            case KEY_LEFT: direction = kLeft; break;
            case KEY_RIGHT: direction = kRight; break;
            case 'q': user_quit_ = true; break;
            default: ;
          }
          snake_.SetDirection(direction);
        }
      }
    };
    std::thread keyboard_listen;
    if (!ai_) keyboard_listen = std::thread(kb_listen);

    while (!game_over_) {
      std::this_thread::sleep_for(milliseconds(kRefreshSpeed));
      if (user_quit_) break;

      bool eaten = false, suicided = false;
  	  snake_.MoveAndEat(food_, &eaten, &suicided);
      if (suicided) game_over_ = true;
      else game_over_ = DrawSnake(snake_); // killed by wall.
      if (eaten) UpdateScoreAndNewFood();
    }
    if (!ai_) keyboard_listen.join();
  }

  void UpdateScoreAndNewFood() {
    score_ += 1;
    for (bool cell_ok = false; !cell_ok; ) {
      food_ = {rand()%(kRightBound - 1) + 1, rand()%(kDownBound - 1) + 1};
      cell_ok = !snake_.Contains(food_);
    }
    std::unique_lock<std::mutex> lck(print_get_mutex_);
	mvwaddch(window_, food_.y, food_.x, '@' | COLOR_PAIR(2));
    print_score();
  }
  

 private:
  void print_score() {
    wmove(window_, kUpBound, 2);
    printw("Score: %d", score_);
    wmove(window_, kDownBound, 0);
  }

  Position center() {
    return {(kLeftBound + kRightBound) / 2, (kUpBound + kDownBound) / 2};
  }

  Snake snake_;
  const int kRefreshSpeed;
  int kUpBound;
  int kDownBound;
  int kLeftBound;
  int kRightBound;
  bool game_over_;
  bool user_quit_;
  bool ai_;
  int score_;
  WINDOW* window_;
  Position food_;
  std::mutex print_get_mutex_; // make printw and getch mutex.
};

void Help() {
  printf("-m[ode]: easy / hard.\n");
}
  
int main(int argc, char *argv[]) {
  bool is_human = true;

  int speed = 200;
  if (argc > 1) {
    std::string s = argv[1];
    if (s == "-a") {
      speed = 5;
      is_human = false;
    } else if (s == "-m") {
      if (argc == 2) {
        printf("too few arguments!\n");
        return 1;
      }
      std::string mode = argv[2];
      if (mode == "easy") {
        speed = 300;
      } else if (mode == "medium") {
        speed = 200;
      } else if (mode == "hard") {
        speed = 100;
      } else if (mode == "expert") {
        speed = 50;
      } else {
        printf("-m: Please enter easy, medium, hard or expert!\n");
        return 1;
      }
    } else {
      Help();
      return 1;
    }
  }
  Playground playground(speed, is_human);
  playground.Welcome();

  for (bool play_again = true; play_again; ) {
    playground.Clear();
    playground.RunSingleSnake();
    play_again = playground.GameOver();
  }

  endwin();
  return 0;
}

