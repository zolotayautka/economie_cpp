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
    setFixedSize(QSize(961, 681));
    ui->all_table->setColumnWidth(2, 500);
    if (!(fileExists(".book"))){
        create_book();
    }
    ui->new_day->setDate(QDate::currentDate());
    ui->made->setDate(QDate::currentDate());
    search();
    load_sel_day();
    connect(ui->karenda, &QCalendarWidget::clicked, this, &mainQT::load_sel_day);
    connect(ui->karenda, &QCalendarWidget::selectionChanged, this, &mainQT::load_sel_day);
    connect(ui->add_btn, &QPushButton::clicked, this, &mainQT::ireru);
    connect(ui->modify_btn, &QPushButton::clicked, this, &mainQT::modify);
    connect(ui->del_btn, &QPushButton::clicked, this, &mainQT::del_day_);
    connect(ui->csv_import_btn, &QPushButton::clicked, this, &mainQT::import_);
    connect(ui->csv_out_btn, &QPushButton::clicked, this, &mainQT::out_);
    connect(ui->kara, &QDateEdit::dateChanged, this, &mainQT::search);
    connect(ui->made, &QDateEdit::dateChanged, this, &mainQT::search);
}

mainQT::~mainQT()
{
    delete ui;
}

void mainQT::search(){
    ui->made->setMinimumDate(ui->kara->date());
    ui->kara->setMaximumDate(ui->made->date());
    std::vector<cb> slist = sagasu(ui->kara->date().toString("yyyyMMdd").toInt(), ui->made->date().toString("yyyyMMdd").toInt());
    std::vector<cb> t = sagasu(0, ui->kara->date().toString("yyyyMMdd").toInt() - 1);
    int nokori = 0;
    for (int i = 0; i < (int)t.size(); i++)
        nokori += t[i].atai;
    int l = slist.size();
    ui->all_table->setRowCount(l);
    for(int i = 0; i < l; i++){  // QTableWidgetItem客体はQTableWidget客体によって管理されるから別にdeleteを使う必要はありません。
        nokori += slist[i].atai;
        QTableWidgetItem* day = new QTableWidgetItem(QString::fromStdString(slist[i].day));
        ui->all_table->setItem(l-1-i, 0, day);
        QTableWidgetItem* nsf = new QTableWidgetItem(QString::fromStdString(slist[i].nsf));
        ui->all_table->setItem(l-1-i, 1, nsf);
        QTableWidgetItem* naiyou = new QTableWidgetItem(QString::fromStdString(slist[i].naiyou));
        ui->all_table->setItem(l-1-i, 2, naiyou);
        #ifdef ja
        QTableWidgetItem* atai = new QTableWidgetItem(QString::number(slist[i].atai) + "¥");
        #endif
        #ifdef ko
        QTableWidgetItem* atai = new QTableWidgetItem(QString::number(slist[i].atai) + "₩");
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

void mainQT::load_sel_day(){  // QTableWidgetItem客体はQTableWidget客体によって管理されるから別にdeleteを使う必要はありません。
    ui->sel_table->setRowCount(0);
    ui->sel_view->setText("");
    std::string day = ui->karenda->selectedDate().toString("yyyyMMdd").toStdString();
    std::vector<cb> sel_list = load_sel(day);
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
    cb a;
    QDate tday = ui->new_day->date();
    a.day = tday.toString("yyyyMMdd").toStdString();
    QString tn = ui->new_naiyou->text();
    a.naiyou = tn.toStdString();
    if (!a.naiyou.compare(""))
        a.naiyou = "空欄";
    a.atai = ui->new_atai->value();
    insert_day(a);
    search();
    load_sel_day();
}

void mainQT::del_day_(){
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
    QString qday = tday.toString("yyyyMMdd");
    std::string day = qday.toStdString();
    del_day(day);
    search();
    load_sel_day();
}

void mainQT::modify(){
    int l = ui->sel_table->rowCount();
    if (l < 1)
        return;
    std::string day = ui->karenda->selectedDate().toString("yyyyMMdd").toStdString();
    std::string t = ui->sel_view->toPlainText().toStdString();
    std::vector<cb> in;
    std::stringstream line(t);
    for (int i = 0; i < l; i++){
        cb t_;
        t_.day = day;
        std::string s = ui->sel_table->item(i, 2)->text().toStdString();
        if (s.size()>2){
            #ifdef ja
            if (!(ui->sel_table->item(i, 2)->text().back() == QChar(L'¥'))){
            #endif
            #ifdef ko
            if (!(ui->sel_table->item(i, 2)->text().back() == QChar(L'₩'))){
            #endif
                load_sel_day();
                return;
            }
            s.pop_back();
            s.pop_back();
        } else {
            load_sel_day();
            return;
        }
        t_.atai = std::stoi(s);
        std::getline(line, t_.naiyou, '\n');
        if (!t_.naiyou.compare(""))
            t_.naiyou = "空欄";
        in.push_back(t_);
    }
    del_day(day);
    for (int i = 0; i < l; i++)
        insert_day(in[l-1-i]);
    search();
    load_sel_day();
}

void mainQT::import_(){
    #ifdef ja
    QMessageBox::StandardButton f = QMessageBox::question(this, "お知らせ", "既存のデータを維持しますか？",
                                                          QMessageBox::Yes | QMessageBox::No);
    #endif
    #ifdef ko
    QMessageBox::StandardButton f = QMessageBox::question(this, "알림", "기존 데이터를 유지하겠습니까?",
                                                          QMessageBox::Yes | QMessageBox::No);
    #endif
    QString s = QCoreApplication::applicationDirPath();
    std::string path = QFileDialog::getOpenFileName(this, "Select MP3 File", s, "CSV files (*.csv)").toStdString();
    if (f == QMessageBox::No){
        remove(".book");
        create_book();
    }
    import(path);
    search();
    load_sel_day();
}

void mainQT::out_(){
    QString s = QCoreApplication::applicationDirPath();
    std::string path = QFileDialog::getSaveFileName(this, "Select MP3 File", s, "CSV files (*.csv)").toStdString();
    out(path);
}
