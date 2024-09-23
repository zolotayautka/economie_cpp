#ifndef ECONOMIE_H
#define ECONOMIE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "sqlite3.h"

struct cb{
    std::string day;
    std::string nsf;
    std::string naiyou;
    int atai;
};

std::vector<cb> sagasu(int kara, int made);
std::vector<cb> load_sel(std::string day);
void insert_day(cb in);
void del_day(std::string day);

void out(std::string filename);
void import(std::string filename);

void create_book();

#endif // ECONOMIE_H
