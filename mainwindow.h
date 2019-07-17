#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QWidget>
#include <QDockWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPainter>
#include <QPaintEvent>
#include "dialog.h"
#include "tube.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void print_field();
  void set_single_tube(int a, int b, int c, int d, int e, bool checked);
  bool eventFilter(QObject *watched, QEvent *event);

public slots:
  void get_inform();
  void calculate();
  void wid_changed(int a);
  void show_speed();
  void show_concentration();
  void show_default();

protected:
  void paintEvent(QPaintEvent *event);

private:
  Ui::MainWindow *ui;
  std::vector<tube *> tubes;
  int row, i1, i2, o1, o2, o3;
  int x_start, y_start;
  int length, width;
  int selecting;
  bool drawed;
  bool widchanged;
};

#endif // MAINWINDOW_H
