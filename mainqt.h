#ifndef MAINQT_H
#define MAINQT_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDate>
#include <QMessageBox>
#include <sys/stat.h>
#include "economie.h"
#include <QFileDialog>
#include <cstdio>

namespace Ui {
class mainQT;
}

class mainQT : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainQT(QWidget *parent = nullptr);
    ~mainQT();

private:
    Ui::mainQT *ui;

private slots:
    void load_sel_day();
    void ireru();
    void del_day_();
    void search();
    void import_();
    void out_();
    void modify();
};

#endif // MAINQT_H
