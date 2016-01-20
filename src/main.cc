
#include <iostream>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <signal.h>
#include <vector>
#include "snake.h"
//inline void display(char gsDomain[][22], int level, int moveSpeed)
//{
//  system("cls"); //清屏
//  cout << endl << endl;
//  for (int i = 0; i < 22; i++)
//      {
//    cout << "\t";
//    for (int j = 0; j < 22; j++)
//      cout << gsDomain[i][j] << " ";
//    if (i == 0)
//        {
//      cout << "\tLevel：" << level;
//        }
//    else if (i == 3)
//        {
//      cout << "\t自动前进时间";
//        }
//    else if (i == 5)
//        {
//      cout << "\t间隔：" << moveSpeed << " ms";
//        }
//
//    cout << endl;
//      }
//}

void resizeHandler (int sig) {
  int nh, nw;
  getmaxyx(stdscr, nh, nw);
}

class Snake {
};



int main() {

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  nodelay(stdscr, true);
  wrefresh(stdscr);
  signal(SIGWINCH, resizeHandler);

  wborder(stdscr, 0, 0, 0, 0, 0, 0, 0, 0);

  int ch;
  for (bool flag = true; flag ;) {
    if ((ch = getch()) == ERR) {
      // wait for a while
      // generate next state
      //generateNextState();
      // refresh
      wrefresh(stdscr);
    } else {
      switch(ch) {
        case KEY_UP: break;
        case KEY_DOWN: break;
        case KEY_LEFT: printf("%d", ch); flag = false; break;
        case KEY_RIGHT: printf("%d",ch); break;
        default: break;
      }
    }
  }

  

//  char gsDomain[22][22]; //贪吃蛇活动区域(包括墙壁)
//  //初始化贪吃蛇活动区域(不包括墙壁)
//  for (int i = 1; i <= 21; i++)
//      {
//    for (int j = 1; j <= 21; j++)
//      gsDomain[i][j] = ' ';
//      }
//  //初始化贪吃蛇活动区域的上下墙壁
//  for (int i = 0; i < 22; i++)
//    gsDomain[0][i] = gsDomain[21][i] = '-';
//  //初始化贪吃蛇活动区域的左右墙壁
//  for (int i = 1; i < 21; i++)
//    gsDomain[i][0] = gsDomain[i][21] = '|';
//  //初始化蛇身
//  for (int i = 1; i <= 3; i++)
//    gsDomain[1][i] = '*';
//  //初始化蛇头
//  gsDomain[1][4] = '#';
//
//  int snake[2][100]; //记录贪吃蛇每次出现的位置的坐标
//  for (int i = 0; i < 4; i++)
//      {
//    snake[0][i] = 1; //记录贪吃蛇所在位置的x坐标
//    snake[1][i] = i + 1; //记录贪吃蛇所在位置的y坐标
//      }
//  int head = 3, tail = 0, length = 4;
//
//  int beanX, beanY; //豆豆出现的位置
//  srand(time(0));
//  do
//      {
//    beanX = rand() % 20 + 1;
//    beanY = rand() % 20 + 1;
//      } while (gsDomain[beanX][beanY] != ' ');
//  gsDomain[beanX][beanY] = '*'; //豆豆
//
//  cout << "\n\n\t\t贪吃蛇游戏即将开始!\n";
//  long start;
//  int level = 1, moveSpeed = 1000;
//  for (int i = 3; i >= 0; i--)
//      {
//    start = clock();
//    while (clock() - start <= 1000){}
//    system("cls");
//    if (i)
//        {
//      cout << "\n\n\t\t进入游戏倒计时：" << i << endl;
//        }
//    else
//      display(gsDomain, level, moveSpeed);
//      }
//
//  char direction = 77; //贪吃蛇默认自动向右直线前进
//  while (true)
//      {
//    bool timeFlag = true;
//    int x, y;
//    start = clock();
//
//    //若时间超过自动前进时间或者键盘上有键按下则终止循环
//    while ((timeFlag = (clock() - start <= moveSpeed))){}
//
//    if (timeFlag)
//        {
//      //键盘上有键按下时读取键盘输入
//      getch();
//      direction = getch();
//        }
//
//    switch (direction)
//          {
//          //向上
//            case 72: x = snake[0][head] - 1, y = snake[1][head];
//          break;
//          //向下
//            case 80: x = snake[0][head] + 1, y = snake[1][head];
//          break;
//          //向左
//            case 75: x = snake[0][head], y = snake[1][head] - 1;
//          break;
//          //向右
//            case 77: x = snake[0][head], y = snake[1][head] + 1;
//          break;
//            default: cout << "\tGame Over!\n";
//          return 0;
//          }
//
//    if (x == 0 || x == 21 || y == 0 || y == 21)
//        {
//      //贪吃蛇触碰到墙壁
//      cout << "\tGame Over!\n";
//      return 0;
//        }
//
//    if (gsDomain[x][y] != ' ' && !(x == beanX && y == beanY))
//        {
//      //贪吃蛇的蛇头触碰到蛇身或者蛇尾
//      cout << "\tGame Over!\n";
//      return 0;
//        }
//
//    if (x == beanX && y == beanY)
//        {
//      //吃豆豆
//      length++; //长度加1
//      if (length >= 8)
//          {
//        //游戏升级处理
//        length -= 8;
//        level++;
//        if (moveSpeed > 100)
//          moveSpeed -= 100;
//          }
//      gsDomain[snake[0][head]][snake[1][head]] = '*';
//      gsDomain[x][y] = '#';
//      head = (head + 1) % 100;
//      snake[0][head] = x;
//      snake[1][head] = y;
//      do
//          {
//        beanX = rand() % 20 + 1;
//        beanY = rand() % 20 + 1;
//          } while (gsDomain[beanX][beanY] != ' ');
//      gsDomain[beanX][beanY] = '*';
//      
//      display(gsDomain, level, moveSpeed); //屏幕上显示
//        }
//    else
//        {
//      //不吃豆豆
//      gsDomain[snake[0][tail]][snake[1][tail]] = ' '; //蛇尾前移一格
//      tail = (tail + 1) % 100;
//      gsDomain[snake[0][head]][snake[1][head]] = '*';
//      head = (head + 1) % 100;
//      snake[0][head] = x;
//      snake[1][head] = y;
//      gsDomain[x][y] = '#'; //蛇头前移一格
//      display(gsDomain, level, moveSpeed); //屏幕上显示
//        }
//      }
  endwin();
  return 0;
}
