#include "main_view.h"

#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QMessageBox>
#include <fstream>
#include <iostream>

#include "ui_main_view.h"

namespace s21 {

View::View(Controller* controller, QWidget* parent)
    : QMainWindow(parent), controller_(controller), ui(new Ui::View) {
  ui->setupUi(this);
  isMazeOrCave = 0;

  ui->graphicsView->SetController(
      controller_);  // forwarding controller to scene

  ui->Maze_radioButton->setChecked(true);

  gen_cave_timer = new QTimer(this);
  connect(gen_cave_timer, &QTimer::timeout, this, &View::StepGenCave);
}

View::~View() {
  delete ui;
  if (gen_cave_timer != nullptr) {
    delete gen_cave_timer;
  }
}

/**
 * @brief Opening file method
 */
void View::on_openFileButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Choose maze", "../../../src/tests/matrix", "");
  int res = 0;
  if (ui->Maze_radioButton->isChecked()) {
    res = controller_->InitializeMaze(filename.toStdString());
    if (res == Simple_Maze) {
      isMazeOrCave = Simple_Maze;
      ui->graphicsView->DrawMaze(isMazeOrCave);
    } else if (res == ML_Maze) {
      QMessageBox::critical(this, "Fail",
                            "This is not maze file. This is mazeML file!");
    } else if (res == Simple_Cave) {
      QMessageBox::critical(this, "Fail",
                            "This is not maze file. This is cave file!");
    } else if (res == OpenFileError) {
      QMessageBox::critical(this, "Fail", "File is not open");
    }
  } else if (ui->Cave_radioButton->isChecked()) {
    res = controller_->InitializeCave(filename.toStdString());
    if (res == Simple_Cave) {
      ui->graphicsView->DrawCave();
      isMazeOrCave = Simple_Cave;
    } else if (res == Simple_Maze) {
      QMessageBox::critical(this, "Fail",
                            "This is not cave file. This is maze file!");
    } else if (res == OpenFileError) {
      QMessageBox::critical(this, "Fail", "File is not open");
    }
  } else if (ui->ML_radioButton->isChecked()) {
    res = controller_->InitializeMaze(filename.toStdString());
    if (res == ML_Maze) {
      isMazeOrCave = ML_Maze;
      ui->graphicsView->DrawMaze(isMazeOrCave);
    } else if (res == Simple_Maze) {
      QMessageBox::critical(this, "Fail",
                            "This is not mazeML file. This is maze file!");
    } else if (res == Simple_Cave) {
      QMessageBox::critical(this, "Fail",
                            "This is not mazeML file. This is cave file!");
    } else if (res == OpenFileError) {
      QMessageBox::critical(this, "Fail", "File is not open");
    }
  }
}

/**
 * @brief Maze generation method
 */
void View::on_generateMazeButton_clicked() {
  QString saving_dir =
      QFileDialog::getSaveFileName(this, "Save maze", "../", "TXT (*.txt)");

  int cols_ = ui->colsSpinBox->value();
  int rows_ = ui->colsSpinBox->value();

  if (controller_->generateMazeMatrix(cols_, rows_, saving_dir.toStdString())) {
    QMessageBox::information(this, "Success", "Success. File was created");
    isMazeOrCave = Simple_Maze;
  }
}

/**
 * @brief Maze resolving method
 */
void View::on_pushButton_Resolve_clicked() {
  if (isMazeOrCave != Simple_Cave) {
    ui->graphicsView->DrawPathMaze();
  }
}

/**
 * @brief Step of cave generation
 */
void View::on_NextCaveButton_clicked() {
  if (isMazeOrCave == Simple_Cave) {
    controller_->AddGenCaveParam(ui->LifeLimit_spinBox->value(),
                                 ui->DeathLimit_spinBox->value());
    controller_->GenCave();
    ui->graphicsView->DrawCave();
  }
}

/**
 * @brief Step of uninterruption cave generation
 */
void View::StepGenCave() {
  if (controller_->GenCave() == 1)
    gen_cave_timer->stop();  // if cave is over - stop timer
  ui->graphicsView->DrawCave();
}

/**
 * @brief Uninterruption cave generation
 */
void View::on_StartCaveGen_pushButton_clicked() {
  if (isMazeOrCave == Simple_Cave) {
    int interval = ui->TimeInterval_spinBox->value();
    controller_->AddGenCaveParam(ui->LifeLimit_spinBox->value(),
                                 ui->DeathLimit_spinBox->value());
    gen_cave_timer->start(interval);
  }
}

/**
 * @brief Stop of uninterruption cave generation
 */
void View::on_StopCaveGen_pushButton_clicked() { gen_cave_timer->stop(); }

/**
 * @brief Generating cave's file
 */
void View::on_generateCaveButton_clicked() {
  QString saving_dir =
      QFileDialog::getSaveFileName(this, "Save cave", "../", "TXT (*.txt)");
  if (!controller_->GenerateCaveFile(ui->Chance_spinBox->value(),
                                     saving_dir.toStdString())) {
    QMessageBox::critical(this, "Fail", "Generation is failed");
  }
}

}  // namespace s21
