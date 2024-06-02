#ifndef SRC_VIEW_SCENE_H
#define SRC_VIEW_SCENE_H

#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>
#include <QWidget>

#include "../controller/controller.hpp"

constexpr int FIELD_SIZE = 500;

namespace s21 {
class Scene : public QGraphicsView {
  Q_OBJECT

 public:
  Scene(QWidget *parent = nullptr);
  Scene(const Scene &) = delete;
  Scene &operator=(const Scene &) = delete;
  ~Scene();

  void SetController(Controller *controller);

  void DrawMaze(int isMl);
  void DrawCave();
  void DrawPathMaze();

 private:
  QGraphicsScene *scene;
  Controller *controller_;

  int cols = 0, rows = 0;
  double width = 0.0, height = 0.0;
  int ML = 0;

  std::pair<int, int> start;   // input point indexes
  std::pair<int, int> finish;  // output point indexes

  // scene's items
  QGraphicsItemGroup *field = nullptr;
  QGraphicsItemGroup *path = nullptr;
  QGraphicsItemGroup *start_pos = nullptr;
  QGraphicsItemGroup *finish_pos = nullptr;

  QColor wall_color_ = QColor(Qt::white), empty_color_ = QColor(Qt::black),
         path_color_ = QColor(Qt::red), start_color_ = QColor(Qt::green),
         finish_color_ = QColor(Qt::blue);

  std::pair<int, int> getCurrentPointOfStructure(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event) override;

  void GroupsDelete();

  void PathPainting(QGraphicsItemGroup *path, std::vector<int> &matrix,
                    int start_index, int finish_index, double x0, double y0,
                    double x1, double y1);
  void IndexToCord(double &x, double &y, int col, int row);
  void VerDownCord(double &y, int row, int height);
  void VerUpCord(double &y, int row, int height);
  void HorRightCord(double &x, int col, int width);
  void HorLeftCord(double &x, int col, int width);
  void PaintStart();
  void PaintFinish();
};
}  // namespace s21

#endif  // SRC_VIEW_SCENE_H
