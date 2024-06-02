#include "controller.hpp"

namespace s21 {

// Maze

int s21::Controller::InitializeMaze(const std::string filename) {
  return model_->Init_Maze(filename);
}

std::pair<int, int> Controller::GettingMazeColsRows() {
  return model_->GetMazeColsRows();
}

std::pair<int, int> Controller::GettingMazeExitCoords() {
  return model_->GetMazeExitCoords();
}

std::pair<std::vector<bool>, std::vector<bool>>
Controller::GettingMazeMatrix() {
  return model_->GetMazeMatrix();
}

std::vector<int> Controller::GetPathMaze(int x1, int y1, int x2, int y2) {
  return model_->GetPathMaze(x1, y1, x2, y2);
}

std::vector<int> Controller::GetPathMaze_ML(int x1, int y1) {
  return model_->GetPathMaze_ML(x1, y1);
}

// Cave

int s21::Controller::InitializeCave(const std::string filename) {
  return model_->Init_Cave(filename);
}

void Controller::AddGenCaveParam(int life, int death) {
  model_->AddGenCaveParam(life, death);
}

bool Controller::GenCave() { return model_->GenCave(); }

bool Controller::GenerateCaveFile(int chance, const std::string filename) {
  return model_->GenerateCaveFile(chance, filename);
}

std::pair<int, int> Controller::GettingCaveColsRows() {
  return model_->GetCaveColsRows();
}
std::vector<bool> Controller::GettingCaveMatrix() {
  return model_->GetCaveMatrix();
}

bool s21::Controller::generateMazeMatrix(const int cols, const int rows,
                                         std::string filename) {
  return model_->generateMazeMatrix(cols, rows, filename);
}

}  // namespace s21
