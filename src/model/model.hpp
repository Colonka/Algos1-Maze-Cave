#ifndef SRC_MODEL_MODEL_HPP
#define SRC_MODEL_MODEL_HPP

#include "cave/cave.hpp"
#include "maze/maze.hpp"

namespace s21 {
class Model {  // Class developed by Fasade pattern
 public:
  int Init_Maze(const std::string filename);
  int Init_Cave(const std::string filename);
  void AddGenCaveParam(int life, int death);
  bool GenCave();
  bool GenerateCaveFile(int chance, const std::string filename);

  // Getters & Setters
  std::pair<int, int> GetMazeColsRows();
  std::pair<int, int> GetMazeExitCoords();
  std::pair<std::vector<bool>, std::vector<bool>> GetMazeMatrix();

  std::vector<int> GetPathMaze(int x1, int y1, int x2, int y2);
  std::vector<int> GetPathMaze_ML(int x1, int y1);

  std::pair<int, int> GetCaveColsRows();
  std::vector<bool> GetCaveMatrix();

  bool generateMazeMatrix(const int cols, const int rows, std::string filename);

 private:
  Maze maze_;
  Cave cave_;
};
}  // namespace s21

#endif  // SRC_MODEL_MODEL_HPP
