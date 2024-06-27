#ifndef ECONOMIE_H
#define ECONOMIE_H

#include <string>
#include <vector>
#include <sstream>
#include "sqlite3.h"


struct cb{
    std::string day;
    std::string nsf;
    std::string naiyou;
    int atai;
};

class chobo_exec{
private:
    sqlite3* db;
    std::vector<cb> all_list;
    std::vector<cb> sel_list;
    static int callback(void *data, int argc, char **argv, char **azColName) {
        std::vector<cb>* list = static_cast<std::vector<cb>*>(data);
        list->push_back({argv[0], argv[1], argv[2], std::stoi(argv[3])});
        return 0;
    }
public:
    chobo_exec(){
        sqlite3_open("book", &db);
    }
    ~chobo_exec(){
        sqlite3_close(db);
    }
    std::vector<cb> load_all();
    std::vector<cb> load_sel(std::string day);
    void insert_day(cb in);
    void del_day(std::string day);
};

void create_book();

#endif // ECONOMIE_H
