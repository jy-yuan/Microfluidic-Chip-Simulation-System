#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui
{
class Dialog;
}

class Dialog : public QDialog
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = 0);
  ~Dialog();
  int getrow();
  int geti1();
  int geti2();
  int geto1();
  int geto2();
  int geto3();

public slots:
  void checkinform();

private:
  Ui::Dialog *ui;
};

#endif // DIALOG_H
