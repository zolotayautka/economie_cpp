#ifndef MAINQT_H
#define MAINQT_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDate>
#include <QMessageBox>
#include <sys/stat.h>
#include "economie.h"

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
    chobo_exec* chobo;
    cb* a;
    std::vector<cb> all_list;
    std::vector<cb> sel_list;
    void all_load_day();

private slots:
    void load_sel_day();
    void ireru();
    void del_day();
};

#endif // MAINQT_H
