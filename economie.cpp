#include "economie.h"

static int callback(void *data, int argc, char **argv, char **azColName) {
    std::vector<cb>* list = static_cast<std::vector<cb>*>(data);
    list->push_back({argv[0], argv[1], argv[2], std::stoi(argv[3])});
    return 0;
}

std::vector<cb> sagasu(int kara, int made){
    std::vector<cb> slist;
    std::ostringstream sql;
    sql << "SELECT day, nsf, naiyou, atai FROM chobo WHERE day >= " << kara << " AND day <= " << made <<" ORDER BY day ASC;";
    sqlite3* db;
    sqlite3_open(".book", &db);
    sqlite3_exec(db, sql.str().c_str(), callback, &slist, 0);
    sqlite3_close(db);
    return slist;
}

std::vector<cb> load_sel(std::string day){
    std::vector<cb> sel_list;
    std::ostringstream sql;
    sql << "SELECT day, nsf, naiyou, atai FROM chobo WHERE day=" << std::stoi(day) << ";";
    sqlite3* db;
    sqlite3_open(".book", &db);
    sqlite3_exec(db, sql.str().c_str(), callback, &sel_list, 0);
    sqlite3_close(db);
    return sel_list;
}

void insert_day(cb in){
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
    sql << "INSERT INTO chobo VALUES (" << in.day << ", '" << in.nsf << "', '" << in.naiyou << "', " << in.atai << ");";
    sqlite3* db;
    sqlite3_open(".book", &db);
    sqlite3_exec(db, sql.str().c_str(), 0, 0, 0);
    sqlite3_close(db);
}

void del_day(std::string day){
    std::ostringstream sql;
    sql << "DELETE FROM chobo WHERE day=" << std::stoi(day) << ";";
    sqlite3* db;
    sqlite3_open(".book", &db);
    sqlite3_exec(db, sql.str().c_str(), 0, 0, 0);
    const char sql_[] = "VACUUM;";
    sqlite3_exec(db, sql_, 0, 0, 0);
    sqlite3_close(db);
}

void out(std::string filename){
    std::vector<std::string> csv_file;
    std::vector<cb> tmp = sagasu(0, 99999999);
    int size = tmp.size();
    int zangaku = 0;
    for (int i = 0; i < size; i++){
        zangaku += tmp[i].atai;
        csv_file.push_back(tmp[i].day + ';' + tmp[i].naiyou + ';' + std::to_string(tmp[i].atai) + ';' + std::to_string(zangaku) + '\n');
    }
    std::ofstream ofile(filename, std::ios_base::out);
    for(int i = 0; i < size; i++){
        ofile << csv_file[i];
    }
    ofile.close();
}

void import(std::string filename){
    std::vector<std::string> csv_file;
    std::string temp;
    std::ifstream rfile(filename);
    while (std::getline(rfile, temp)){
        csv_file.push_back(temp);
    }
    rfile.close();
    int size = csv_file.size();
    for(int i = 0; i < size; i++){
        std::stringstream line(csv_file[i]);
        std::string t;
        cb t_;
        std::getline(line, t, ';');
        t_.day = t;
        std::getline(line, t, ';');
        t_.naiyou = t;
        std::getline(line, t, ';');
        t_.atai = std::stoi(t);
        insert_day(t_);
    }
}

void create_book(){
    sqlite3* db;
    const char sql[] = "CREATE TABLE chobo (\
                day INT,\
                nsf CHAR(20),\
                naiyou CHAR(80),\
                atai INT\
            );";
    sqlite3_open(".book", &db);
    sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_close(db);
}
