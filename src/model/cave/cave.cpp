#include "cave.hpp"

namespace s21 {

/**
 * @brief Cave initialization
 * @param filename name of file with data
 * @return code of file's type 0-open file error, 1-maze, 2-cave, 3-MLmaze
 */
int Cave::Init(const std::string filename) {
  Clear();
  return Parse(filename);
}

/**
 * @brief Addition generation cave parameters
 * @param life life limit parameter
 * @param death death limit parameter
 */
void Cave::AddGenCaveParam(int life, int death) {
  life_ = life;
  death_ = death;
}

/**
 * @brief Cave data clearing
 */
void Cave::Clear() {
  cols_ = 0;
  rows_ = 0;
  matrix_.clear();
}

/**
 * @brief Parcing cave data from file and validation file types
 * @param filename name of file with data
 * @return code of file's type 0-open file error, 1-maze, 2-cave, 3-MLmaze
 */
int Cave::Parse(const std::string filename) {
  std::ifstream file(filename);
  int res = Simple_Cave;
  if (file.is_open()) {
    std::string line;
    if (std::getline(file, line)) {
      std::istringstream iss(line);
      iss >> cols_ >> rows_;
    }

    matrix_.reserve(cols_ * rows_);

    char c;
    int count = 0;
    for (; !file.eof();) {
      if (file.get(c) && c != '\n' && c != ' ') {
        matrix_.push_back(c - '0');
        ++count;
      }
    }
    if (cols_ * rows_ < count) {
      res = Simple_Maze;
    }
  } else {
    res = OpenFileError;
  }

  file.close();
  return res;  // returning code 0-open file error, 1-maze, 2-cave, 3-MLmaze,
}

/**
 * @brief Getter of cave's cols and rows data
 * @return pair with cols&rows data
 */
std::pair<int, int> Cave::GetColsRows() { return std::make_pair(cols_, rows_); }

/**
 * @brief Getter of cave's data
 * @return cave's data
 */
std::vector<bool> Cave::GetMatrix() { return matrix_; }

void Cave::print() {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      std::cout.width(3);
      std::cout << matrix_[i * cols_ + j];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

/**
 * @brief Step cave generation
 * @return cave stability information: 1 - cave is stability, 0 - cave is not
 * stability
 */
bool Cave::GenerateNext() {
  std::vector<bool> next_matrix(cols_ * rows_, false);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      int life = 0;
      for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++) {
          if (i + k < 0 || j + l < 0 || i + k >= rows_ || j + l >= cols_) {
            continue;
          }
          if (matrix_[(i + k) * cols_ + j + l]) {
            ++life;
          }
        }
      }
      if (matrix_[i * cols_ + j]) {
        next_matrix[i * cols_ + j] = life >= death_;
      } else {
        next_matrix[i * cols_ + j] = life > life_;
      }
    }
  }
  bool stop = IsMatricesEqual(next_matrix);
  matrix_ = next_matrix;
  return stop;
}

/**
 * @brief Cave file eneration
 * @param chance starting initializations chance
 * @param filename name of file with generated cave's data
 * @return generation status 1 - ok, 0 - generation failed
 */
bool Cave::GenerateCaveFile(int chance, const std::string filename) {}

/**
 * @brief Check all vector for value == 1 (for stop timer when cave is over
 * while generating)
 * @param matrix checking vector
 * @return checking result 1 - all vector values == 1, 0 - no
 */
bool Cave::IsMatricesEqual(const std::vector<bool> &next_matrix) {
  return std::equal(matrix_.begin(), matrix_.end(), next_matrix.begin());
}

}  // namespace s21