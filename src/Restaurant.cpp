//
// Created by Libat Biton: 204329379 & Liora Ifraimov: 315389197
//
#include "../include/Restaurant.h"
#include "../include/Table.h"
#include "../include/Dish.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
//This class holds a list of tables, list of customers, the menu of the restaurant
// and other information that is relevant to the restaurant.

//CONSTRUCTOR
Restaurant::Restaurant():open(false), tables(),menu(), actionsLog(), num_of_tables(), cus_id(0){};

//DESTRUCTOR
Restaurant::~Restaurant() {
    for(unsigned int i=0;i<tables.size(); ++i)
        delete(tables[i]);
    tables.clear();
    for(unsigned int j=0; j<actionsLog.size(); ++j)
        delete(actionsLog[j]);
    actionsLog.clear();
    menu.clear();
};

//COPY CONSTRUCTOR
Restaurant::Restaurant(const Restaurant &restaurant):open(restaurant.open),tables(),menu(),actionsLog(), num_of_tables(restaurant.num_of_tables),cus_id(restaurant.cus_id) {
    for (unsigned int i = 0; i < restaurant.tables.size(); ++i)
        tables.push_back(restaurant.tables[i]->clone());
    for (unsigned int i = 0; i < restaurant.actionsLog.size(); ++i)
        actionsLog.push_back(restaurant.actionsLog[i]->clone());
    for (unsigned int i = 0; i<restaurant.menu.size(); ++i)
        menu.push_back(restaurant.menu[i]);
};

//MOVE CONSTRUCTOR
Restaurant::Restaurant(Restaurant&& restaurant):open(restaurant.open),tables(),menu(),actionsLog(), num_of_tables(restaurant.num_of_tables),cus_id(restaurant.cus_id) {
    for (unsigned int i = 0; i < restaurant.tables.size(); ++i)
        tables.push_back(restaurant.tables[i]->clone());
    for (unsigned int i = 0; i < restaurant.actionsLog.size(); ++i)
        actionsLog.push_back(restaurant.actionsLog[i]->clone());
    for (unsigned int i = 0; i<restaurant.menu.size(); ++i)
        menu.push_back(restaurant.menu[i]);
    for (unsigned int i = 0; i < restaurant.tables.size(); ++i)
        delete (restaurant.tables[i]);
    for (unsigned int j = 0; j < restaurant.actionsLog.size(); ++j)
        delete (restaurant.actionsLog[j]);
    restaurant.menu.clear();
    restaurant.actionsLog.clear();
    restaurant.tables.clear();
};

//COPY ASSIGNMENT OPERATOR
Restaurant& Restaurant::operator=(const Restaurant &restaurant) {
    if(this!=&restaurant) {
        open = restaurant.open;
        cus_id = restaurant.cus_id;
        num_of_tables = restaurant.num_of_tables;
        for (unsigned int i = 0; i < tables.size(); ++i)
            delete (tables[i]);
        tables.clear();
        for (unsigned int i = 0; i < actionsLog.size(); ++i)
            delete (actionsLog[i]);
        actionsLog.clear();
        menu.clear();
        for (unsigned int i = 0; i < restaurant.menu.size(); ++i)
            menu.push_back(restaurant.menu[i]);
        for (unsigned int i = 0; i < restaurant.tables.size(); ++i) {
            Table *table = new Table(*restaurant.tables[i]);
            tables.push_back(table);
        }
        for (unsigned int i = 0; i < restaurant.actionsLog.size(); ++i)
            actionsLog.push_back(restaurant.actionsLog[i]->clone());
    }
    return *this;
};

//MOVE ASSIGNMENT OPERATOR
Restaurant& Restaurant::operator=(Restaurant &&restaurant) {
    if(this!=&restaurant) {
        open = restaurant.open;
        cus_id = restaurant.cus_id;
        num_of_tables = restaurant.num_of_tables;
        menu.clear();
        for (unsigned int i = 0; i < restaurant.menu.size(); ++i)
            menu.push_back(restaurant.menu[i]);
        for (unsigned int i = 0; i < tables.size(); ++i)
            delete (tables[i]);
        tables.clear();
        for (unsigned int j = 0; j < actionsLog.size(); ++j)
            delete (actionsLog[j]);
        actionsLog.clear();
        for (unsigned int i = 0; i < restaurant.tables.size(); ++i) {
            Table *table = new Table(*restaurant.tables[i]);
            tables.push_back(table);
        }
        for (unsigned int i = 0; i < restaurant.actionsLog.size(); ++i)
            actionsLog.push_back(restaurant.actionsLog[i]->clone());
        for (unsigned int i = 0; i < restaurant.tables.size(); ++i)
            delete (restaurant.tables[i]);
        restaurant.tables.clear();
        for (unsigned int j = 0; j < restaurant.actionsLog.size(); ++j)
            delete (restaurant.actionsLog[j]);
        restaurant.tables.clear();
        restaurant.actionsLog.clear();
        restaurant.menu.clear();
    }
    return *this;
};

//gets and sets the information about the restaurant from the config file.
Restaurant::Restaurant(const string &configFilePath):open(false), tables(),menu(), actionsLog(), num_of_tables(), cus_id(0) {
    ifstream inFile;
    inFile.open(configFilePath);
    if (!inFile) {
        cerr << "Unable to open file datafile.txt";
        exit(1);   // call system to stop
    }
    string line;
    vector<string> lines;
    while (getline(inFile, line)) {
        string s = line; //reading from file line by line
        if (s[0] != '#' && !s.empty())
            lines.push_back(s);
    }
    num_of_tables = stoi(lines[0]);
    istringstream iss(lines[1]);
    vector<string> capacity;
    string temp;
    while (getline(iss, temp, ',')) {
        capacity.push_back(temp);
    }
    for (unsigned int i = 0; i <capacity.size(); ++i) {
        int cap = stoi(capacity[i]);
        Table *table = new Table(cap);
        tables.push_back(table);
    }
    if(lines.size()>2) {
        for (unsigned int j = 2; j < lines.size(); ++j) {
            istringstream iss(lines[j]);
            vector<string> dish;
            string temp;
            while (getline(iss, temp, ',')) {
                dish.push_back(temp);
            }
            DishType type = convert_to_dishtype(dish[1]);
            menu.push_back(Dish(j - 1, dish[0], stoi(dish[2]), type));
        }
    }
    inFile.close();
};

//This function converts string to dish type
DishType Restaurant::convert_to_dishtype(string &type){
    if(type=="ALC")
        return ALC;
    if(type=="SPC")
        return SPC;
    if(type=="VEG")
        return VEG;
    else
        return BVG;


};

//This function converts dish type to string
string Restaurant::convert_type_tostring(DishType &type){
    if(type==ALC)
        return "ALC";
    if(type==SPC)
        return "SPC";
    if(type==VEG)
        return "VEG";
    else
        return "BVG";


};

// This function opens the restaurant, waits for commands, and execute them.

void Restaurant:: start() {
    cout << "Restaurant is now open!" << endl;
    open=true;
    string str;
    getline(cin,str);
    while (str != "closeall") {
        //parsing the request by space
        vector<string> tokens; // Create vector to hold our words
        int index = str.find(" ");
        string temp ;
        while (index != -1) {
            temp = str.substr(0, index);
            str = str.substr(index + 1);
            tokens.push_back(temp);
            index = str.find(" ");
        }
        tokens.push_back(str);
        if (tokens[0] == "open") {
            int num = stoi(tokens[1]);
            vector<Customer *> customersList;
            for (unsigned int i = 2; i < tokens.size(); ++i) {
                int pos = tokens[i].find(",");
                string name = tokens[i].substr(0, pos);
                string type = tokens[i].substr(pos+1);
                if (type == "veg") {
                    VegetarianCustomer* customer = new VegetarianCustomer(name, cus_id);
                    customersList.push_back(customer);
                    cus_id = cus_id+1;
                }
                if (type == "chp") {
                    CheapCustomer* customer = new CheapCustomer(name,cus_id);
                    customersList.push_back(customer);
                    cus_id = cus_id+1;
                }
                if (type == "spc") {
                    SpicyCustomer* customer = new SpicyCustomer(name, cus_id);
                    customersList.push_back(customer);
                    cus_id = cus_id+1;
                }
                if (type == "alc") {
                    AlchoholicCustomer* customer = new AlchoholicCustomer(name, cus_id);
                    customersList.push_back(customer);
                    cus_id = cus_id+1;
                }
            }
            OpenTable *open_table = new OpenTable(num, customersList);
            open_table->act(*this);
             customersList.clear();
            actionsLog.push_back(open_table);
        }
        if (tokens[0] == "move") {
            int from_table = stoi(tokens[1]);
            int to_table = stoi(tokens[2]);
            int customer_id = stoi(tokens[3]);
            MoveCustomer *move_costumer = new MoveCustomer(from_table, to_table, customer_id);
            move_costumer->act(*this);
            actionsLog.push_back(move_costumer);
        }
        if (tokens[0] == "order") {
            int num = stoi(tokens[1]);
            Order *order = new Order(num);
            order->act(*this);
            actionsLog.push_back(order);
        }
        if (tokens[0] == "close") {
            int num = stoi(tokens[1]);
            Close *close = new Close(num);
            close->act(*this);
            actionsLog.push_back(close);

        }
        if (tokens[0] == "menu") {
            PrintMenu *print_menu = new PrintMenu();
            print_menu->act(*this);
            actionsLog.push_back(print_menu);

        }
        if (tokens[0] == "status") {
            int num = stoi(tokens[1]);
            PrintTableStatus* print_Table_Status=new PrintTableStatus(num);
            print_Table_Status->act(*this);
            actionsLog.push_back(print_Table_Status);

        }
        if (tokens[0] == "log") {
            PrintActionsLog *print_Action_Log = new PrintActionsLog();
            print_Action_Log->act(*this);
            actionsLog.push_back(print_Action_Log);
        }
        if (tokens[0] == "backup") {
            BackupRestaurant *backup_Restaurant = new BackupRestaurant();
            backup_Restaurant->act(*this);
            actionsLog.push_back(backup_Restaurant);
        }
        if (tokens[0] == "restore") {
            RestoreResturant *restore_Restaurant = new RestoreResturant();
            restore_Restaurant->act(*this);
            actionsLog.push_back(restore_Restaurant);
        }
        tokens.clear();
        getline(cin,str);
    }
    CloseAll* close_All=new CloseAll();
    close_All->act(*this);
    delete(close_All);
    open= false;
    };

vector<Table*> Restaurant::gettables () {
    return tables;
};

// Return a reference to the history of actions
const vector<BaseAction*>& Restaurant::getActionsLog() const{
    return actionsLog;
};

vector<Dish>& Restaurant:: getMenu() {
    return menu;
};





