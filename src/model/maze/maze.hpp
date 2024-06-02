#ifndef SRC_MODEL_SOURCE_MAZE_MAZE_HPP
#define SRC_MODEL_SOURCE_MAZE_MAZE_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "../../myenum.h"

namespace s21 {
class Maze {
 public:
  int Init(const std::string filename);
  int Parse(const std::string filename);
  void Clear();

  // Getters & Setters
  std::pair<int, int> GetColsRows();
  std::pair<int, int> GetExitCoords();
  std::pair<std::vector<bool>, std::vector<bool>> GetMatrix();

  std::vector<int> GetPath(int row1, int col1, int row2, int col2);

  void LearnQTable(int x2, int y2);
  std::vector<int> GetLearnedPath(int x1, int y1);

  bool GenerateMaze(int rows, int cols, std::string filename);

 private:
  std::vector<bool> r_walls_, b_walls_;
  int cols_, rows_;
  int exit_x = 0, exit_y = 0;  // coords
  std::vector<std::array<double, 4>> q_table_;

  // int cur_idx, fin_idx;
  // std::vector<int> path;
  // bool stop = 0;
  // bool Wave();

  bool IsWayFree(int row1, int col1, int row2, int col2);
};
}  // namespace s21

#endif  // SRC_MODEL_SOURCE_MAZE_MAZE_HPP
