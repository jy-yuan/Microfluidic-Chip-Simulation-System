#ifndef TUBE_H
#define TUBE_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class tube : public QPushButton
{
  public:
    double v;
    double leng;
    int wid;
    int n1, n2;
    int rank;
    int max;
    explicit tube(QWidget *parent = 0)
    {
        setStyleSheet(
            "QPushButton{"
            "background-color:#FFFFFF;"
            "border:none"
            "}"
            "QPushButton:checked{"
            "background-color:#DC143C;"
            "border:none"
            "}");
        setCheckable(true);
    }
};

#endif // TUBE_H
