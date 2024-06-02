#include "model.hpp"

namespace s21 {

// Maze
/**
 * @brief Maze initialization trigger
 * @param filename name of file with data
 * @return code of file's type 0-open file error, 1-maze, 2-cave, 3-MLmaze
 */
int Model::Init_Maze(const std::string filename) {
  return maze_.Init(filename);
}

/**
 * @brief Getter of Maze's cols and rows data
 * @return pair with cols&rows data
 */
std::pair<int, int> Model::GetMazeColsRows() { return maze_.GetColsRows(); }

/**
 * @brief Getter of Maze's output point indexes data
 * @return pair with data
 */
std::pair<int, int> Model::GetMazeExitCoords() { return maze_.GetExitCoords(); }

/**
 * @brief Getter of Maze's data
 * @return Maze's data
 */
std::pair<std::vector<bool>, std::vector<bool>> Model::GetMazeMatrix() {
  return maze_.GetMatrix();
}

/**
 * @brief Getter path in Maze
 * @return path's data
 */
std::vector<int> Model::GetPathMaze(int x1, int y1, int x2, int y2) {
  return maze_.GetPath(x1, y1, x2, y2);
}

/**
 * @brief Getter maze's path calculating by machine
 * @return path's data
 */
std::vector<int> Model::GetPathMaze_ML(int x1, int y1) {
  return maze_.GetLearnedPath(x1, y1);
}

/**
 * @brief Maze generation
 * @return errors's code 0 - file is not opened, 1 - ok
 */
bool Model::generateMazeMatrix(const int cols, const int rows,
                               std::string filename) {
  return maze_.GenerateMaze(cols, rows, filename);
}

// Cave

/**
 * @brief Cave initialization trigger
 * @param filename name of file with data
 * @return code of file's type 0-open file error, 1-maze, 2-cave, 3-MLmaze
 */
int Model::Init_Cave(const std::string filename) {
  return cave_.Init(filename);
}

/**
 * @brief Addition generation cave parameters
 * @param life life limit parameter
 * @param death death limit parameter
 */
void Model::AddGenCaveParam(int life, int death) {
  cave_.AddGenCaveParam(life, death);
}

/**
 * @brief Cave generation
 * @return cave stability information: 1 - cave is stability, 0 - cave is not
 * stability
 */
bool Model::GenCave() { return cave_.GenerateNext(); }

/**
 * @brief Cave file eneration
 * @param chance starting initializations chance
 * @param filename name of file with generated cave's data
 * @return generation status 1 - ok, 0 - generation failed
 */
bool Model::GenerateCaveFile(int chance, const std::string filename) {
  return cave_.GenerateCaveFile(chance, filename);
}

/**
 * @brief Getter of Cave's cols and rows data
 * @return pair with cols&rows data
 */
std::pair<int, int> Model::GetCaveColsRows() { return cave_.GetColsRows(); }

/**
 * @brief Getter of Cave's data
 * @return Cave's data
 */
std::vector<bool> Model::GetCaveMatrix() { return cave_.GetMatrix(); }

}  // namespace s21
