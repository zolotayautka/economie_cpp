#include "economie.h"

std::vector<cb> chobo_exec::load_all(){
    const char sql[] = "SELECT day, nsf, naiyou, atai FROM chobo ORDER BY day ASC;";
    sqlite3_exec(db, sql, callback, &all_list, 0);
    return all_list;
}

std::vector<cb> chobo_exec::load_sel(std::string day){
    std::ostringstream sql;
    sql << "SELECT day, nsf, naiyou, atai FROM chobo WHERE day='" << day << "';";
    sqlite3_exec(db, sql.str().c_str(), callback, &sel_list, 0);
    return sel_list;
}
void chobo_exec::insert_day(cb in){
    #ifdef ja
    in.nsf = "収入";
    if (in.atai < 0){
        in.nsf = "支出";
    }
    #endif
    #ifdef ko
    in.nsf = "수입";
    if (in.atai < 0){
        in.nsf = "지출";
    }
    #endif
    std::ostringstream sql;
    sql << "INSERT INTO chobo VALUES ('" << in.day << "', '" << in.nsf << "', '" << in.naiyou << "', " << in.atai << ");";
    sqlite3_exec(db, sql.str().c_str(), 0, 0, 0);
}

void chobo_exec::del_day(std::string day){
    std::ostringstream sql;
    sql << "DELETE FROM chobo WHERE day='" << day << "';";
    sqlite3_exec(db, sql.str().c_str(), 0, 0, 0);
    const char sql_[] = "VACUUM;";
    sqlite3_exec(db, sql_, 0, 0, 0);
}

void create_book(){
    sqlite3* db;
    const char sql[] = "CREATE TABLE chobo (\
                day CHAR(20),\
                nsf CHAR(20),\
                naiyou CHAR(80),\
                atai INT\
            );";
    sqlite3_open("book", &db);
    sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_close(db);
}
