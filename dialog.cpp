#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent),
                                  ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Parameters");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);
    ui->label_7->setPalette(pa);
    connect(ui->m_row, SIGNAL(valueChanged(int)), this, SLOT(checkinform()));
    connect(ui->m_i1, SIGNAL(valueChanged(int)), this, SLOT(checkinform()));
    connect(ui->m_i2, SIGNAL(valueChanged(int)), this, SLOT(checkinform()));
    connect(ui->m_o1, SIGNAL(valueChanged(int)), this, SLOT(checkinform()));
    connect(ui->m_o2, SIGNAL(valueChanged(int)), this, SLOT(checkinform()));
    connect(ui->m_o3, SIGNAL(valueChanged(int)), this, SLOT(checkinform()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::checkinform()
{
    if (geti1() == geti2() || geto1() == geto2() || geto1() == geto3() || geto2() == geto3())
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->label_7->setHidden(false);
        return;
    }
    if (geti1() >= getrow() || geti2() >= getrow() || geto1() >= getrow() || geto2() >= getrow() || geto3() >= getrow())
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->label_7->setHidden(false);
        return;
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    ui->label_7->setHidden(true);
}

int Dialog::getrow()
{
    return ui->m_row->value();
}

int Dialog::geti1()
{
    return ui->m_i1->value();
}

int Dialog::geti2()
{
    return ui->m_i2->value();
}

int Dialog::geto1()
{
    return ui->m_o1->value();
}

int Dialog::geto2()
{
    return ui->m_o2->value();
}

int Dialog::geto3()
{
    return ui->m_o3->value();
}
