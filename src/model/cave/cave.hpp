#ifndef SRC_MODEL_SOURCE_CAVE_CAVE_HPP
#define SRC_MODEL_SOURCE_CAVE_CAVE_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../../myenum.h"

namespace s21 {
class Cave {
 public:
  int Init(const std::string filename);
  void AddGenCaveParam(int life, int death);
  int Parse(const std::string filename);
  void Clear();

  // Getters & Setters
  std::pair<int, int> GetColsRows();
  std::vector<bool> GetMatrix();
  bool GenerateNext();
  bool GenerateCaveFile(int chance, const std::string filename);

  void print();

 private:
  std::vector<bool> matrix_;
  int cols_, rows_;
  int life_, death_;

  bool IsMatricesEqual(const std::vector<bool> &next_matrix);
};
}  // namespace s21

#endif  // SRC_MODEL_SOURCE_CAVE_CAVE_HPP