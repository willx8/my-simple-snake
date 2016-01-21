#include <iostream>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <signal.h>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include "snake.h"

void resizeHandler (int sig) {
  int nh, nw;
  getmaxyx(stdscr, nh, nw);
}

class Playground {
 public:
  Playground() {
    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_WHITE);
    left_bound_ = 0;
    up_bound_ = 0;
    
    int h,w;
    getmaxyx(stdscr, h, w);
    down_bound_ = h - 1;
    right_bound_ = w - 1;
    game_over_ = true;
    
    window_ = stdscr;

    cbreak();
    //halfdelay(1);

    keypad(window_, TRUE);
    wrefresh(window_);
    wmove(window_, 1, 1);
  }
 
//  Playground(int left_bound, int up_bound, int nlines, int ncols) {
//    initscr();
//    left_bound_ = left_bound;
//    up_bound_ = up_bound;
//    right_bound_ = left_bound + ncols;
//    down_bound_ = up_bound + nlines;
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
    wmove(window_, center().y - 1, center().x - 15);
    printw("Welcome to play My Simple Snake!\n");
    wmove(window_, center().y, center().x - 15);
    printw("Press any key to start. Enjoy yourself!\n");
    wmove(window_, center().y+1, center().x - 15);
    printw("COLORS:%d, COLOR_PAIRS:%d", COLORS, COLOR_PAIRS);    
    wgetch(window_);
    wmove(window_, 1, 1);
  }

  void Clear() {
    noecho();
    wclear(window_);
    wrefresh(window_);
    wborder(window_, 0, 0, 0, 0, 0, 0, 0, 0);
  }

  bool DrawSnake(const Snake& snake) {
    // This mutex avoid conflict between addch and getch.
    // Comment mutexs below and in thread keyboard_listen,
    // you may see the strange bug.
    print_get_mutex_.lock();
	mvwaddch(window_, snake.head().y, snake.head().x, ' ' | COLOR_PAIR(1));
    mvwaddch(window_, snake.prev_tail().y, snake.prev_tail().x, ' ');
    print_get_mutex_.unlock();

    wmove(window_, down_bound_, 0);
    wrefresh(window_);
    if (snake.head().x == left_bound_ || snake.head().x == right_bound_
        || snake.head().y == up_bound_ || snake.head().y == down_bound_) {
      return true; // Yes, game over.
    } else {
      return false;
    }
  }

  bool GameOver() {
    wmove(window_, 5, center().x - 10);
    printw("Game Over :) Play again? [y/N]?");
    nodelay(window_, FALSE);
    int ch = wgetch(window_);
    return ch == 'y'; 
  }
  
  Position center() {
    return {(left_bound_ + right_bound_) / 2,
            (up_bound_ + down_bound_) / 2};
  }

  void RunSingleSnake() {
    nodelay(window_, TRUE);
    snake_.SetHead(center());
    game_over_ = false;
    wmove(window_, down_bound_, 0);
    printw("  You may press <q> to stop game at anytime.");

    std::thread keyboard_listen([this] () {
      int ch;
      while (!game_over_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        // This mutex avoid conflict between addch and getch.
        // Comment mutexs below and in DrawSnake, you may see 
        // the strange bug.
        print_get_mutex_.lock();
        ch = getch();
        print_get_mutex_.unlock();

        if (ch != ERR) {
  	      Directions direction = kNoChange;
          switch(ch) {
            case KEY_UP: direction = kUp; break;
            case KEY_DOWN: direction = kDown; break;
            case KEY_LEFT: direction = kLeft; break;
            case KEY_RIGHT: direction = kRight; break;
            case 'q': game_over_ = true; break;
            default:
              mvwaddch(window_, up_bound_, left_bound_, ch);
          }
  	      snake_.setDirection(direction);
        }
      }
    });
    
    while (!game_over_) {
      // wait for a while
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if (game_over_) break;
      // generate next state.
  	  snake_.Move();
  	  game_over_ = DrawSnake(snake_);
    }
    keyboard_listen.join();
  }
  

 private:
  Snake snake_;
  int up_bound_;
  int down_bound_;
  int left_bound_;
  int right_bound_;
  bool game_over_;
  WINDOW* window_;
  std::mutex print_get_mutex_; // make printw and getch mutex.
};

  
int main() {
  Playground playground;


  playground.Welcome();

  for (bool play_again = true; play_again; ) {
    playground.Clear();
    playground.RunSingleSnake();
    play_again = playground.GameOver();
  }

  endwin();
  return 0;
}
