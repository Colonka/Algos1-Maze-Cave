#include "scene.hpp"

namespace s21 {

Scene::Scene(QWidget *parent)
    : QGraphicsView(parent),
      scene(new QGraphicsScene(QRect(0, 0, FIELD_SIZE, FIELD_SIZE))) {
  setScene(scene);
  setAlignment(Qt::AlignTop | Qt::AlignLeft);
  setBackgroundBrush(empty_color_);
}

Scene::~Scene() {
  GroupsDelete();
  delete scene;
}

/**
 * @brief Controller getting method
 * @param controller pointer to controller
 */
void Scene::SetController(Controller *controller) { controller_ = controller; }

/**
 * @brief Scene's items deletion method
 */
void Scene::GroupsDelete() {
  if (field) delete field;
  field = nullptr;
  if (path) delete path;
  path = nullptr;
  if (start_pos) delete start_pos;
  start_pos = nullptr;
  if (finish_pos) delete finish_pos;
  finish_pos = nullptr;
}

/**
 * @brief Maze drawing method
 * @param isML maze type indicator
 */
void Scene::DrawMaze(int isML) {
  GroupsDelete();
  ML = isML;
  field = new QGraphicsItemGroup();
  scene->addItem(field);

  cols = controller_->GettingMazeColsRows().first;
  rows = controller_->GettingMazeColsRows().second;

  std::vector<bool> right_w = controller_->GettingMazeMatrix().first;
  std::vector<bool> bottom_w = controller_->GettingMazeMatrix().second;

  width = FIELD_SIZE / cols, height = FIELD_SIZE / rows;

  setBackgroundBrush(empty_color_);
  QPen pen(wall_color_, 2);
  field->addToGroup(
      scene->addLine(0, 0, FIELD_SIZE, 0, pen));  // отрисовка левой стенки
  field->addToGroup(
      scene->addLine(0, 0, 0, FIELD_SIZE, pen));  // отрисовка верхней стенки
  int i = 0;
  for (int row = 0; row != rows; ++row) {
    for (int col = 0; col != cols; ++col) {
      int x0 = width * col, y0 = height * row;
      int x1 = width * (col + 1), y1 = height * (row + 1);
      if (right_w[i]) field->addToGroup(scene->addLine(x1, y0, x1, y1, pen));
      if (bottom_w[i]) field->addToGroup(scene->addLine(x0, y1, x1, y1, pen));
      ++i;
    }
  }
  if (ML == ML_Maze) {  // if ML_maze paint finish point
    finish.first = controller_->GettingMazeExitCoords().first;
    finish.second = controller_->GettingMazeExitCoords().second;
    PaintFinish();
  }
}

/**
 * @brief Cave drawing method
 */
void Scene::DrawCave() {
  GroupsDelete();

  field = new QGraphicsItemGroup();
  scene->addItem(field);

  cols = controller_->GettingCaveColsRows().first;
  rows = controller_->GettingCaveColsRows().second;

  std::vector<bool> matrix = controller_->GettingCaveMatrix();

  width = FIELD_SIZE / cols, height = FIELD_SIZE / rows;

  double step_row = 0, step_col = 0;

  int i = 0;
  setBackgroundBrush(empty_color_);
  for (int row = 0; row != rows; ++row) {
    step_row = 0;
    for (int col = 0; col != cols; ++col) {
      if (matrix[i] == 0) {
        field->addToGroup(scene->addRect(step_row, step_col, width, height,
                                         QPen(wall_color_),
                                         QBrush(wall_color_)));
      }
      step_row += width;
      ++i;
    }
    step_col += height;
  }
}

/**
 * @brief Maze path drawing method
 */
void Scene::DrawPathMaze() {
  if (path) delete path;
  path = new QGraphicsItemGroup();
  scene->addItem(path);

  int start_index =
      start.second * rows + start.first;  // input point index in vector
  int finish_index =
      finish.second * rows + finish.first;  // output point index in vector

  double x0 = 0, y0 = 0, x1 = 0, y1 = 0;  // input/output scene's coords
  IndexToCord(x0, y0, start.first, start.second);    // get start coords
  IndexToCord(x1, y1, finish.first, finish.second);  // get finish coords

  if (x0 > FIELD_SIZE || y0 > FIELD_SIZE || x1 > FIELD_SIZE ||
      y1 > FIELD_SIZE) {  // out of field bound checking
    QMessageBox::critical(this, "Fail", "In/Out point out of bound!");
  } else {
    std::vector<int> matrix = {0};
    if (ML == Simple_Maze) {
      matrix = controller_->GetPathMaze(start.second, start.first,
                                        finish.second, finish.first);
    } else if (ML == ML_Maze) {
      matrix = controller_->GetPathMaze_ML(start.second, start.first);
    }
    PathPainting(path, matrix, start_index, finish_index, x0, y0, x1, y1);
  }
}

/**
 * @brief Path painting loop
 * @param path path scene's items
 * @param matrix path data
 * @param start_index input point index in vector
 * @param finish_index output point index in vector
 * @param x0 scene's x0 coord
 * @param y0 scene's y0 coord
 * @param x1 scene's x1 coord
 * @param y1 scene's y1 coord
 */
void Scene::PathPainting(QGraphicsItemGroup *path, std::vector<int> &matrix,
                         int start_index, int finish_index, double x0,
                         double y0, double x1, double y1) {
  int step_number = 1;       // number of step in maze
  bool next_step_found = 0;  // number of next step in maze is found
  QPen pen(path_color_, 2);

  while (start_index != finish_index) {  // path painting loop
    int i = 0;                           // current index
    next_step_found = 0;
    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        if ((matrix[i] - step_number) != 1) {
          ++i;
        } else {
          ++step_number;
          if (i - start_index == cols) {  // down step
            VerDownCord(y1, row, height);
            x1 = x0;
            path->addToGroup(scene->addLine(x0, y0, x1, y1, pen));
            y0 = y1;
            start_index = i;
          } else if (i - start_index == -cols) {  // up step
            VerUpCord(y1, row, height);
            x1 = x0;
            path->addToGroup(scene->addLine(x0, y0, x1, y1, pen));
            y0 = y1;
            start_index = i;
          } else if (i - start_index == 1) {  // right step
            HorRightCord(x1, col, width);
            y1 = y0;
            path->addToGroup(scene->addLine(x0, y0, x1, y1, pen));
            x0 = x1;
            start_index = i;
          } else if (i - start_index == -1) {  // left step
            HorLeftCord(x1, col, width);
            y1 = y0;
            path->addToGroup(scene->addLine(x0, y0, x1, y1, pen));
            x0 = x1;
            start_index = i;
          }
          next_step_found = 1;
          break;
        }
      }
      if (next_step_found) break;
    }
  }
}

/**
 * @brief Translating from index to coords method
 */
void Scene::IndexToCord(double &x, double &y, int x_idx, int y_idx) {
  if (x_idx) {
    x = x_idx * width + width / 2;
  } else {
    x = width / 2;
  }
  if (y_idx) {
    y = y_idx * height + height / 2;
  } else {
    y = height / 2;
  }
}

/**
 * @brief Getting coords for down step
 */
void Scene::VerDownCord(double &y, int row, int height) {
  if (row) {
    y = row * height + (height / 2);
  } else {
    y = height / 2;
  }
}

/**
 * @brief Getting coords for up step
 */
void Scene::VerUpCord(double &y, int row, int height) {
  if (row) {
    y = (row + 1) * height - (height / 2);
  } else {
    y = height / 2;
  }
}

/**
 * @brief Getting coords for right step
 */
void Scene::HorRightCord(double &x, int col, int width) {
  if (col) {
    x = col * width + (width / 2);
  } else {
    x = width / 2;
  }
}

/**
 * @brief Getting coords for left step
 */
void Scene::HorLeftCord(double &x, int col, int width) {
  if (col) {
    x = (col + 1) * width - (width / 2);
  } else {
    x = width / 2;
  }
}

/**
 * @brief Choosing input/output point
 * @param event pointer to QMouseEvent object
 */
void Scene::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    start = getCurrentPointOfStructure(event);
    PaintStart();
  } else if (event->button() == Qt::RightButton && ML != ML_Maze) {
    finish = getCurrentPointOfStructure(event);
    PaintFinish();
  }
}

/**
 * @brief Painting input point
 */
void Scene::PaintStart() {
  if (start_pos) delete start_pos;
  if (path) delete path;
  path = nullptr;

  start_pos = new QGraphicsItemGroup();
  scene->addItem(start_pos);

  double x = start.first * width;
  double y = start.second * height;

  start_pos->addToGroup(scene->addRect(x, y, width, height, QPen(start_color_),
                                       QBrush(start_color_)));
}

/**
 * @brief Painting output point
 */
void Scene::PaintFinish() {
  if (finish_pos) delete finish_pos;
  if (path) delete path;
  path = nullptr;

  finish_pos = new QGraphicsItemGroup();
  scene->addItem(finish_pos);

  double x = finish.first * width;
  double y = finish.second * height;

  finish_pos->addToGroup(scene->addRect(
      x, y, width, height, QPen(finish_color_), QBrush(finish_color_)));
}

/**
 * @brief Getting current index of input/output point
 * @param event pointer to QMouseEvent object
 */
std::pair<int, int> Scene::getCurrentPointOfStructure(QMouseEvent *event) {
  int x = static_cast<int>(event->pos().x()) / height;
  int y = static_cast<int>(event->pos().y()) / width;

  return std::make_pair(x, y);
}
}  // namespace s21
