#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <QtWidgets/QMainWindow>
#include <QTimer>

#include "../controller/controller.hpp"
#include "../myenum.h"

namespace s21 {

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow {
  Q_OBJECT

 public:
  // Constructors and the destructor
  View(Controller* controller, QWidget* parent = nullptr);
  View(const View&) = delete;
  View& operator=(const View&) = delete;
  View(View&&) = delete;
  View& operator=(View&&) = delete;
  ~View();

 private slots:

  void on_openFileButton_clicked();

  void on_generateMazeButton_clicked();

  void on_pushButton_Resolve_clicked();

  void on_NextCaveButton_clicked();

  void StepGenCave();

  void on_StartCaveGen_pushButton_clicked();

  void on_StopCaveGen_pushButton_clicked();

  void on_generateCaveButton_clicked();

  private:
  Controller* controller_;
  Ui::View* ui;

  QTimer* gen_cave_timer = nullptr;
  int isMazeOrCave;  // 1 - Maze, 2 - Cave, 3 - MazeML
};
}  // namespace s21
#endif  // MAIN_VIEW_H
