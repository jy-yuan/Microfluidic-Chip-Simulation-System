#include <iostream>
#include <QObject>
#include <stdlib.h>
#include <QSpinBox>
#include <QIcon>
#include <QFont>
#include <QPalette>
#include <QColor>
#include "dialog.h"
#include "tube.h"
#include "mainwindow.h"
#include "coreprogram.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    x_start = 30, y_start = 150;
    length = 80, width = 10;
    drawed = false;
    selecting = -1;
    QAction *startaction = new QAction(this);

    QIcon starticon(":new/prefix1/13 Play.png");
    startaction->setIcon(starticon);
    ui->mainToolBar->addAction(startaction);
    QObject::connect(startaction, SIGNAL(triggered(bool)), this, SLOT(get_inform()));

    ui->dockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    ui->dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    ui->dockWidget->setFixedWidth(250);
    ui->widthspinBox->setEnabled(false);
    ui->showspeeddistribution->setEnabled(false);
    ui->showconcentrationdistribution->setEnabled(false);
    ui->showdefault->setEnabled(false);
    QObject::connect(ui->showspeeddistribution, SIGNAL(clicked(bool)), this, SLOT(show_speed()));
    QObject::connect(ui->showconcentrationdistribution, SIGNAL(clicked(bool)), this, SLOT(show_concentration()));
    QObject::connect(ui->showdefault, SIGNAL(clicked(bool)), this, SLOT(show_default()));
    ui->label_3->setText("Speed in O1 = ");
    ui->label_4->setText("Speed in O2 = ");
    ui->label_5->setText("Speed in O3 = ");
    ui->label_6->setText("Concentration in O1 = ");
    ui->label_7->setText("Concentration in O2 = ");
    ui->label_8->setText("Concentration in O3 = ");
}

MainWindow::~MainWindow()
{
    delete ui;
    if (drawed)
    {
        for (auto i : tubes)
        {
            delete i;
        }
        tubes.clear();
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        tube *now = static_cast<tube *>(watched);
        selecting = now->rank;
        ui->widthspinBox->setValue(now->wid);
        ui->widthspinBox->setMaximum(now->max);
        ui->widthspinBox->setEnabled(true);
        return true;
    }
    else
        return false;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if (drawed)
    {
        QPainter p(this);
        QPen pen(Qt::NoPen);
        p.setBrush(QColor(255, 48, 48));
        p.setPen(pen);
        int n = row;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                p.drawRect(x_start + (length + width) * i, y_start + (length + width) * j, width, width);
            }
        }
    }
}

void MainWindow::get_inform()
{
    Dialog informdialog(this);
    if (informdialog.exec() == QDialog::Accepted)
    {
        if (drawed)
        {
            for (auto i : tubes)
            {
                delete i;
            }
            tubes.clear();
        }
        row = informdialog.getrow();
        i1 = informdialog.geti1();
        i2 = informdialog.geti2();
        o1 = informdialog.geto1();
        o2 = informdialog.geto2();
        o3 = informdialog.geto3();
        if (i1 > i2)
        {
            int tmp = i1;
            i1 = i2;
            i2 = tmp;
        }
        if (o1 > o2)
        {
            if (o2 > o3)
            {
                int tmp = o1;
                o1 = o3;
                o3 = tmp;
            }
            else if (o3 < o1)
            {
                int tmp = o1;
                o1 = o2;
                o2 = o3;
                o3 = tmp;
            }
            else
            {
                int tmp = o1;
                o1 = o2;
                o2 = tmp;
            }
        }
        else if (o3 < o1)
        {
            int tmp = o1;
            o1 = o3;
            o3 = o2;
            o2 = tmp;
        }
        else if (o3 < o2)
        {
            int tmp = o2;
            o2 = o3;
            o3 = tmp;
        }
        else
            ;
        print_field();
    }
}

void MainWindow::set_single_tube(int a, int b, int c, int d, int e, bool checked)
{
    tube *temp = new tube(this);
    temp->setParent(this);
    temp->max = 3000;
    temp->wid = 200;
    temp->rank = e;
    temp->setChecked(checked);
    temp->setGeometry(a, b, c, d);
    temp->show();
    temp->installEventFilter(this);
    tubes.push_back(temp);
}

void MainWindow::print_field()
{
    int n = row;
    int count = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            set_single_tube(x_start + (length + width) * i, y_start + (length + width) * j + width, width, length, count++, true);
        }
    }

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n; j++)
        {
            set_single_tube(x_start + (length + width) * i + width, y_start + (length + width) * j, length, width, count++, true);
        }
    }

    set_single_tube(x_start + (length + width) * i1, y_start - length, width, length, count++, true);
    set_single_tube(x_start + (length + width) * i2, y_start - length, width, length, count++, true);
    set_single_tube(x_start + (length + width) * o1, y_start + (length + width) * (n - 1) + width, width, length, count++, true);
    set_single_tube(x_start + (length + width) * o2, y_start + (length + width) * (n - 1) + width, width, length, count++, true);
    set_single_tube(x_start + (length + width) * o3, y_start + (length + width) * (n - 1) + width, width, length, count, true);

    QObject::connect(ui->startsimulation, SIGNAL(clicked(bool)), this, SLOT(calculate()));
    QObject::connect(ui->widthspinBox, SIGNAL(valueChanged(int)), this, SLOT(wid_changed(int)));

    update();

    selecting = -1;
    ui->widthspinBox->setEnabled(false);
    ui->widthspinBox->setValue(200);
    ui->label_3->setText("Speed in O1 = ");
    ui->label_4->setText("Speed in O2 = ");
    ui->label_5->setText("Speed in O3 = ");
    QFont font;
    font.setStrikeOut(false);
    ui->label_6->setFont(font);
    ui->label_7->setFont(font);
    ui->label_8->setFont(font);
    ui->label_6->setText("Concentration in O1 = ");
    ui->label_7->setText("Concentration in O2 = ");
    ui->label_8->setText("Concentration in O3 = ");
    ui->showspeeddistribution->setEnabled(false);
    ui->showconcentrationdistribution->setEnabled(false);
    ui->showdefault->setEnabled(false);

    drawed = true;
    widchanged = false;
}

void MainWindow::calculate()
{
    std::vector<double> leng(2 * row * row - 2 * row + 5, 0);
    for (int i = 0; i < 2 * row * row - 2 * row + 5; i++)
    {
        if (tubes[i]->isChecked())
        {
            double para = (double)(tubes[i]->wid) / 200;
            leng[i] = 1 / para;
        }
        else
        {
            leng[i] = 0;
        }
    }
    std::vector<double> ans = caluconspeedndense(row, leng, i1, i2, o1, o2, o3);
    ui->label_3->setText("Speed in O1 = " + QString::number(ans[0], 'f', 3));
    ui->label_4->setText("Speed in O2 = " + QString::number(ans[1], 'f', 3));
    ui->label_5->setText("Speed in O3 = " + QString::number(ans[2], 'f', 3));
    ui->showspeeddistribution->setEnabled(true);
    ui->label_3->repaint();
    ui->label_4->repaint();
    ui->label_5->repaint();
    QFont font;
    if (widchanged)
    {
        font.setStrikeOut(true);
        ui->label_6->setFont(font);
        ui->label_7->setFont(font);
        ui->label_8->setFont(font);
        ui->label_6->setText("Concentration in O1 = ");
        ui->label_7->setText("Concentration in O2 = ");
        ui->label_8->setText("Concentration in O3 = ");
        ui->showconcentrationdistribution->setEnabled(false);
    }
    else
    {
        font.setStrikeOut(false);
        ui->label_6->setFont(font);
        ui->label_7->setFont(font);
        ui->label_8->setFont(font);
        ui->label_6->setText("Concentration in O1 = " + QString::number(ans[3], 'f', 3));
        ui->label_7->setText("Concentration in O2 = " + QString::number(ans[4], 'f', 3));
        ui->label_8->setText("Concentration in O3 = " + QString::number(ans[5], 'f', 3));
        ui->showconcentrationdistribution->setEnabled(true);
    }
    ui->label_6->repaint();
    ui->label_7->repaint();
    ui->label_8->repaint();
    ui->showdefault->setEnabled(false);
    show_default();
}

void MainWindow::wid_changed(int a)
{
    widchanged = true;
    int newx, newy, neww, newh;
    int nearmax = 3200 - a;
    if (selecting == -1)
        return;
    if (selecting >= row * (row - 1) && selecting < 2 * row * (row - 1))
    { //横向边
        if (selecting != row * (row - 1))
        {
            tubes[selecting - 1]->max = nearmax;
        }
        if (selecting != 2 * row * row - 2 * row - 1)
        {
            tubes[selecting + 1]->max = nearmax;
        }
        auto now = tubes[selecting];
        newx = now->x();
        newy = now->y() + now->height() / 2 - width * a / 400;
        neww = now->width();
        newh = width * a / 200;
        tubes[selecting]->wid = a;
        tubes[selecting]->setGeometry(newx, newy, neww, newh);
    }
    else
    { //竖向边
        if (selecting < (row - 1) * (row - 1))
        {
            tubes[selecting + row - 1]->max = nearmax;
        }
        if (selecting >= row - 1)
        {
            tubes[selecting - row + 1]->max = nearmax;
        }
        auto now = tubes[selecting];
        newx = now->x() + now->width() / 2 - width * a / 400;
        newy = now->y();
        neww = width * a / 200;
        newh = now->height();
        tubes[selecting]->wid = a;
        tubes[selecting]->setGeometry(newx, newy, neww, newh);
    }
}

void MainWindow::show_speed()
{
    for (int i = 0; i < 2 * row * row - 2 * row + 5; i++)
    {
        QString col, middlepara;
        middlepara = QString::number(255 - (int)(edges[i].v * 255 / 400));
        col = "QPushButton{background-color:rgba(0,0,0,0);border:none}QPushButton:checked{background-color:rgb(255," + middlepara + "," + middlepara + ");border:none}";
        tubes[i]->setStyleSheet(col);
        tubes[i]->repaint();
    }
    ui->showdefault->setEnabled(true);
    ui->showdefault->repaint();
}

void MainWindow::show_concentration()
{
    for (int i = 0; i < 2 * row * row - 2 * row + 5; i++)
    {
        QString col, middlepara;
        middlepara = QString::number(255 - (int)(edges[i].dense * 255));
        col = "QPushButton{background-color:rgba(0,0,0,0);border:none}QPushButton:checked{background-color:rgb(" + middlepara + "," + middlepara + ",255);border:none}";
        tubes[i]->setStyleSheet(col);
        tubes[i]->repaint();
    }
    ui->showdefault->setEnabled(true);
    ui->showdefault->repaint();
}

void MainWindow::show_default()
{
    for (int i = 0; i < 2 * row * row - 2 * row + 5; i++)
    {
        tubes[i]->setStyleSheet(
            "QPushButton{"
            "background-color:#FFFFFF;"
            "border:none"
            "}"
            "QPushButton:checked{"
            "background-color:#DC143C;"
            "border:none"
            "}");
        ;
        tubes[i]->repaint();
    }
}
