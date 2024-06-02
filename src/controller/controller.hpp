#ifndef SRC_CONTROLLER_CONTROLLER_H
#define SRC_CONTROLLER_CONTROLLER_H

#include <string>
#include <vector>

#include "../model/model.hpp"

namespace s21 {

class Controller {
 public:
  // Constructors and the destructor
  Controller(Model* model) : model_(model) {}
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;
  Controller(Controller&&) = delete;
  Controller& operator=(Controller&&) = delete;
  ~Controller() = default;

  int InitializeMaze(const std::string filename);

  int InitializeCave(const std::string filename);
  void AddGenCaveParam(int life, int death);
  bool GenCave();
  bool GenerateCaveFile(int chance, const std::string filename);

  bool generateMazeMatrix(const int cols, const int rows, std::string filename);

  // Getters & Setters
  std::pair<int, int> GettingMazeColsRows();
  std::pair<int, int> GettingMazeExitCoords();
  std::pair<std::vector<bool>, std::vector<bool>> GettingMazeMatrix();
  std::vector<int> GetPathMaze(int x1, int y1, int x2, int y2);
  std::vector<int> GetPathMaze_ML(int x1, int y1);

  std::pair<int, int> GettingCaveColsRows();
  std::vector<bool> GettingCaveMatrix();

 private:
  Model* model_;
};

}  // namespace s21

#endif  // SRC_CONTROLLER_CONTROLLER_H
