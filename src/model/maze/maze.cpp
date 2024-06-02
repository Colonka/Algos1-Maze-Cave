#include "maze.hpp"

namespace s21 {
/**
 * @brief Maze initialization
 * @param filename name of file with data
 * @return code of file's type 0-open file error, 1-maze, 2-cave, 3-MLmaze
 */
int Maze::Init(const std::string filename) {
  int res = Parse(filename);
  if (res == ML_Maze) LearnQTable(exit_x, exit_y);
  return res;
}

/**
 * @brief Maze data clearing
 */
void Maze::Clear() {
  cols_ = 0;
  rows_ = 0;
  exit_x = 0;
  exit_y = 0;
  r_walls_.clear();
  b_walls_.clear();
}

/**
 * @brief Parcing maze data from file and validation file types
 * @param filename name of file with data
 * @return code of file's type 0-open file error, 1-maze, 2-cave, 3-MLmaze
 */
int Maze::Parse(const std::string filename) {
  Clear();
  int res = Simple_Maze;
  std::ifstream file(filename);     // reading main stream
  std::ifstream file_ML(filename);  // reading ML checking stream

  if (file.is_open()) {
    std::string line;
    if (std::getline(file, line)) {
      std::istringstream iss(line);
      iss >> cols_ >> rows_;
    }

    std::string line_1_ML;
    std::string line_2_ML;
    std::string line_3_ML;
    std::getline(file_ML, line_1_ML);
    std::getline(file_ML, line_2_ML);
    std::getline(file_ML, line_3_ML);

    if (line_3_ML.size() > line_2_ML.size()) {  // checking ML or simple maze
      std::string exit_cords;
      if (std::getline(file, exit_cords)) {
        std::istringstream exit(exit_cords);
        exit >> exit_x >> exit_y;  // getting output point indexes
      }
      res = ML_Maze;
    }

    file_ML.close();

    r_walls_.reserve(cols_ * rows_);
    b_walls_.reserve(cols_ * rows_);

    char c;
    int count = 0;
    int abs_count = 0;

    for (; count != cols_ * rows_;) {
      if (file.get(c) && c != '\n' && c != ' ') {
        r_walls_.push_back(c - '0');
        ++count;
        ++abs_count;
      }
    }
    count = 0;

    for (; count != cols_ * rows_ && !file.eof();) {
      if (file.get(c) && c != '\n' && c != ' ') {
        b_walls_.push_back(c - '0');
        ++count;
        ++abs_count;
      }
    }
    file.close();

    if (cols_ * rows_ == abs_count) {  // is it cave file check
      res = Simple_Cave;
    }
  } else {
    res = OpenFileError;
  }

  return res;  // returning code 0-open file error, 1-maze, 2-cave, 3-MLmaze
}

// Getters & Setters

/**
 * @brief Getter of Maze's cols and rows data
 * @return pair with cols&rows data
 */
std::pair<int, int> Maze::GetColsRows() { return std::make_pair(cols_, rows_); }

/**
 * @brief Getter of Maze's output point indexes data
 * @return pair with data
 */
std::pair<int, int> Maze::GetExitCoords() {
  return std::make_pair(exit_x, exit_y);
}

/**
 * @brief Getter of Maze's data
 * @return Maze's data
 */
std::pair<std::vector<bool>, std::vector<bool>> Maze::GetMatrix() {
  return std::make_pair(r_walls_, b_walls_);
}

/**
 * @brief Checking wall before step
 * @param row1 y1
 * @param col1 x1
 * @param row2 y2
 * @param col1 x2
 * @return 1 - no wall, 0 - there is wall
 */
bool Maze::IsWayFree(int row1, int col1, int row2, int col2) {
  if (row2 < 0 || row2 >= rows_ || col2 < 0 || col2 >= cols_) {
    return false;
  }
  if (row1 != row2) {
    int upper_row = (row1 < row2) ? row1 : row2;
    return b_walls_[upper_row * rows_ + col1] == 0;
  }
  if (col1 != col2) {
    int left_col = (col1 < col2) ? col1 : col2;
    return r_walls_[row1 * rows_ + left_col] == 0;
  }
  return false;
}

// std::vector<int> Maze::GetPath(int row1, int col1, int row2, int col2) {
//   std::vector<int> Path(rows_ * cols_, 0);
//   path = Path;
//   cur_idx = row1 * rows_ + col1;
//   fin_idx = row2 * rows_ + col2;
//   path[cur_idx] = 1;

//   // std::queue<std::pair<int, int>> q;
//   // int lenght = 0;
//   // q.push(std::make_pair(cur_idx, lenght));

//   return path;
// }

// bool Maze::Wave() {
//   if (cur_idx - 4 >= 0) {  // up
//     if (path[cur_idx - 4] == 0 && b_walls_[cur_idx - 4] == 0) {
//       path[cur_idx - 4] = path[cur_idx] + 1;
//       if (cur_idx - 4 == fin_idx) stop = 1;
//       ++step_indc;
//     }
//   }
//   if (path[cur_idx + 4] == 0 && b_walls_[cur_idx] == 0) {
//     path[cur_idx + 4] = path[cur_idx] + 1;
//     ++step_indc;
//   }
//   if (cur_idx - 1 >= 0) {
//     if (path[cur_idx - 1] == 0 && r_walls_[cur_idx - 1] == 0) {
//       path[cur_idx - 1] = path[cur_idx] + 1;
//       ++step_indc;
//     }
//   }
//   if (path[cur_idx + 1] == 0 && r_walls_[cur_idx] == 0) {
//     path[cur_idx + 1] = path[cur_idx] + 1;
//     ++step_indc;
//   }
// }
// }

/**
 * @brief Computing shorterst path in the maze
 * @param row1 y1
 * @param col1 x1
 * @param row2 y2
 * @param col1 x2
 * @return path's data
 */
std::vector<int> Maze::GetPath(int row1, int col1, int row2, int col2) {
  std::cout << row1 << ' ' << col1 << ' ' << row2 << ' ' << col2 << '\n';
  std::vector<int> path(rows_ * cols_, 0);
  path[row1 * rows_ + col1] = 1;
  std::queue<std::pair<int, int>> q;
  q.push(std::make_pair(row1, col1));
  while (q.size() != 0) {
    int y = q.front().first;
    int x = q.front().second;
    q.pop();
    for (int i = -1; i <= 1; ++i) {
      for (int j = -1; j <= 1; ++j) {
        if (abs(i) == abs(j)) continue;
        if (IsWayFree(y, x, y + i, x + j) && path[(y + i) * rows_ + x + j] == 0) {
          path[(y + i) * rows_ + x + j] = path[y * rows_ + x] + 1;
          q.push(std::make_pair(y + i, x + j));
        }
      }
    }
  }
  int y = row2;
  int x = col2;
  for (int current_value = path[row2 * rows_ + col2]; current_value != 0;
       --current_value) {
    for (int i = -1; i <= 1; ++i) {
      for (int j = -1; j <= 1; ++j) {
        if (abs(i) == abs(j)) continue;
        if (IsWayFree(y, x, y + i, x + j) && path[(y + i) * rows_ + x + j] == current_value - 1) {
          path[y * rows_ + x] = current_value + (cols_ * rows_);
          y += i;
          x += j;
          break;
        }
      }
    }
  }
  for (auto& i : path) {
    if (i != 1) {
      if (i < cols_ * rows_) {
        i = 0;
      } else {
        i -= cols_ * rows_;
      }
    }
  }
  return path;
}

/**
 * @brief Learning machine to search shortest path in the maze
 * @param x2 output point x index
 * @param y2 output point y index
 */
void Maze::LearnQTable(int x2, int y2) {
  q_table_.clear();
  q_table_ = std::vector<std::array<double, 4>>(
      cols_ * rows_, std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
  double alpha = 0.5;
  double gamma = 0.9;
  double epsilon = 1.0;
  for (size_t i = 0; i < 10000; ++i) {
    int x = rand() % cols_;
    int y = rand() % rows_;
    int action = 0;
    while (!(x == x2 && y == y2)) {
      if ((double)rand() / RAND_MAX < epsilon) {
        action = rand() % 4;
      } else {
        action = std::max_element(q_table_[x * rows_ + y].begin(),
                                  q_table_[x * rows_ + y].end()) -
                 q_table_[x * rows_ + y].begin();
      }
      int old_x = x;
      int old_y = y;
      switch (action) {
        case 0:
          if (x + 1 < rows_ && b_walls_[x * rows_ + y] == 0) {
            x += 1;
          }
          break;
        case 1:
          if (y + 1 < cols_ && r_walls_[x * rows_ + y] == 0) {
            y += 1;
          }
          break;
        case 2:
          if (x - 1 >= 0 && b_walls_[(x - 1) * rows_ + y] == 0) {
            x -= 1;
          }
          break;
        case 3:
          if (y - 1 >= 0 && r_walls_[x * rows_ + y - 1] == 0) {
            y -= 1;
          }
          break;
        default:
          break;
      }
      int reward = (x == x2 && y == y2) ? 100 : -1;
      int best_action = std::max_element(q_table_[x * rows_ + y].begin(),
                                         q_table_[x * rows_ + y].end()) -
                        q_table_[x * rows_ + y].begin();
      q_table_[old_x * rows_ + old_y][action] +=
          alpha * (reward + gamma * q_table_[x * rows_ + y][best_action] -
                   q_table_[old_x * rows_ + old_y][action]);
    }
  }
}

/**
 * @brief Computing shorterst path in the maze by machine learning
 * @param x1 input point x index
 * @param y1 input point y index
 * @return path's data
 */
std::vector<int> Maze::GetLearnedPath(int x1, int y1) {
  std::vector<int> path(cols_ * rows_, 0);
  int x = x1;
  int y = y1;
  int i = 1;
  path[x * rows_ + y] = i;
  while (q_table_[x * rows_ + y][0] != 0 || q_table_[x * rows_ + y][1] != 0 ||
         q_table_[x * rows_ + y][2] != 0 || q_table_[x * rows_ + y][3] != 0) {
    int best_action = std::max_element(q_table_[x * rows_ + y].begin(),
                                       q_table_[x * rows_ + y].end()) -
                      q_table_[x * rows_ + y].begin();
    switch (best_action) {
      case 0:
        x += 1;
        break;
      case 1:
        y += 1;
        break;
      case 2:
        x -= 1;
        break;
      case 3:
        y -= 1;
        break;
      default:
        break;
    }
    path[x * rows_ + y] = ++i;
  }
  return path;
}

/**
 * @brief Maze generating
 * @param rows rows number
 * @param cols cols number
 * @param filename name of file with maze's data
 * @return generation status 1 - ok, 0 - error
 */
bool Maze::GenerateMaze(int rows, int cols, std::string filename) {
  std::vector<int> group_id = std::vector<int>(cols * rows, 0);
  for (int i = 0; i < cols * rows; ++i) {
    group_id[i] = i;
  }
  std::vector<int> r_walls = std::vector<int>(cols * rows, 0);
  std::vector<int> b_walls = std::vector<int>(cols * rows, 0);
  srand(time(nullptr));
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int prev_group_id = (j == 0) ? -1 : group_id[i * cols + j - 1];

      if (j < cols - 1 && rand() % 2) {
        group_id[i * cols + j] = prev_group_id;
      } else {
        r_walls[i * cols + j] = true;
        if (i == rows - 1 && b_walls[(i - 1) * cols + j]) {
          r_walls[i * cols + j] = false;
        }
      }

      if (i < rows - 1) {
        bool way_down = group_id[i * cols + j] != prev_group_id || rand() % 2;
        b_walls[i * cols + j] = !way_down;
        if (way_down) {
          group_id[(i + 1) * cols + j] = group_id[i * cols + j];
        }
      } else {
        b_walls[i * cols + j] = true;
      }
    }
  }

  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  file << rows << " " << cols << std::endl;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      file << r_walls[i * cols + j] << " ";
    }
    file << std::endl;
  }
  file << std::endl;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      file << b_walls[i * cols + j] << " ";
    }
    file << std::endl;
  }

  file.close();
  return true;
}

}  // namespace s21
