#include "mainqt.h"
#include "ui_mainqt.h"

bool fileExists(const std::string& filePath) {
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}

mainQT::mainQT(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainQT)
{
    ui->setupUi(this);
    setFixedSize(QSize(961, 632));
    ui->all_table->setColumnWidth(2, 500);
    if (!(fileExists("book"))){
        create_book();
    }
    ui->new_day->setDate(QDate::currentDate());
    all_load_day();
    load_sel_day();
    connect(ui->karenda, &QCalendarWidget::clicked, this, &mainQT::load_sel_day);
    connect(ui->karenda, &QCalendarWidget::selectionChanged, this, &mainQT::load_sel_day);
    connect(ui->add_btn, &QPushButton::clicked, this, &mainQT::ireru);
    connect(ui->del_btn, &QPushButton::clicked, this, &mainQT::del_day);
}

mainQT::~mainQT()
{
    delete ui;
}

void mainQT::all_load_day(){
    ui->all_table->setRowCount(0);
    chobo = new chobo_exec();
    all_list = chobo->load_all();
    delete chobo;
    int l = all_list.size();
    ui->all_table->setRowCount(l);
    int nokori = 0;
    for(int i = 0; i < l; i++){
        nokori += all_list[i].atai;
        QTableWidgetItem* day = new QTableWidgetItem(QString::fromStdString(all_list[i].day));
        ui->all_table->setItem(l-1-i, 0, day);
        QTableWidgetItem* nsf = new QTableWidgetItem(QString::fromStdString(all_list[i].nsf));
        ui->all_table->setItem(l-1-i, 1, nsf);
        QTableWidgetItem* naiyou = new QTableWidgetItem(QString::fromStdString(all_list[i].naiyou));
        ui->all_table->setItem(l-1-i, 2, naiyou);
        #ifdef ja
        QTableWidgetItem* atai = new QTableWidgetItem(QString::number(all_list[i].atai) + "¥");
        #endif
        #ifdef ko
        QTableWidgetItem* atai = new QTableWidgetItem(QString::number(all_list[i].atai) + "₩");
        #endif
        ui->all_table->setItem(l-1-i, 3, atai);
        #ifdef ja
        QTableWidgetItem* nokor = new QTableWidgetItem(QString::number(nokori) + "¥");
        #endif
        #ifdef ko
        QTableWidgetItem* nokor = new QTableWidgetItem(QString::number(nokori) + "₩");
        #endif
        ui->all_table->setItem(l-1-i, 4, nokor);
    }
}

void mainQT::load_sel_day(){
    ui->sel_table->setRowCount(0);
    ui->sel_view->setText("");
    QDate tday = ui->karenda->selectedDate();
    QString qday = tday.toString("yyyy-MM-dd");
    std::string day = qday.toStdString();
    chobo = new chobo_exec();
    sel_list = chobo->load_sel(day);
    delete chobo;
    int l = sel_list.size();
    ui->sel_table->setRowCount(l);
    for(int i = 0; i < l; i++){
        QTableWidgetItem* day = new QTableWidgetItem(QString::fromStdString(sel_list[i].day));
        ui->sel_table->setItem(l-1-i, 0, day);
        QTableWidgetItem* nsf = new QTableWidgetItem(QString::fromStdString(sel_list[i].nsf));
        ui->sel_table->setItem(l-1-i, 1, nsf);
        #ifdef ja
        QTableWidgetItem* atai = new QTableWidgetItem(QString::number(sel_list[i].atai) + "¥");
        #endif
        #ifdef ko
        QTableWidgetItem* atai = new QTableWidgetItem(QString::number(sel_list[i].atai) + "₩");
        #endif
        ui->sel_table->setItem(l-1-i, 2, atai);
    }
    for(int i = 0; i < l; i++)
        ui->sel_view->append(QString::fromStdString(sel_list[l-1-i].naiyou));
}

void mainQT::ireru(){
    a = new cb;
    QDate tday = ui->new_day->date();
    QString qday = tday.toString("yyyy-MM-dd");
    a->day = qday.toStdString();
    QString tn = ui->new_naiyou->text();
    a->naiyou = tn.toStdString();
    a->atai = ui->new_atai->value();
    chobo = new chobo_exec();
    chobo->insert_day(*a);
    delete chobo;
    delete a;
    all_load_day();
    load_sel_day();
}

void mainQT::del_day(){
    int t = ui->sel_table->rowCount();
    if (t == 0){
        return;
    }
    #ifdef ja
    QMessageBox::StandardButton f = QMessageBox::question(this, "警告", "本当に消してもいいですか？",
                                                          QMessageBox::Yes | QMessageBox::No);
    #endif
    #ifdef ko
    QMessageBox::StandardButton f = QMessageBox::question(this, "경고", "정말로 삭제 하겠습니까?",
                                                          QMessageBox::Yes | QMessageBox::No);
    #endif
    if (f == QMessageBox::No){
        return;
    }
    QDate tday = ui->karenda->selectedDate();
    QString qday = tday.toString("yyyy-MM-dd");
    std::string day = qday.toStdString();
    chobo = new chobo_exec();
    chobo->del_day(day);
    delete chobo;
    all_load_day();
    load_sel_day();
}

