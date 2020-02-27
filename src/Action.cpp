//
//  Created by Libat Biton: 204329379 & Liora Ifraimov: 315389197
//
#include <string>
#include <vector>
#include <iostream>
#include "../include/Customer.h"
#include "../include/Restaurant.h"
extern Restaurant* backup;
using namespace std;

// this is an abstract class for the different action classes.
// Each action has status and an error massege

    //CONSTRUCTOR
    BaseAction::BaseAction():errorMsg(), status(PENDING){};

    //DESTRUCTOR
    BaseAction::~BaseAction(){};

    ActionStatus BaseAction::getStatus() const {
        return status;
    };


    void BaseAction::setStatus(ActionStatus status_temp) {
        status = status_temp;
    };

    void BaseAction::complete() {
        status = COMPLETED;
    };

    void BaseAction::error(string errorMsg) {
        this->errorMsg=errorMsg;
        status = ERROR;
    };

    string BaseAction::getErrorMsg() const {
        return errorMsg;
    };

    // OpenTable: opens a given table and assigns a list of customers to it.

    //CONSTRUCTOR
    OpenTable::OpenTable(int id, vector<Customer *> &customersList): tableId(id), customers(){
        for (unsigned int i = 0; i < customersList.size(); ++i)
            customers.push_back(customersList[i]);
    };

    //DESTRUCTOR
    OpenTable::~OpenTable() {
        for (unsigned int i = 0; i < customers.size(); ++i)
            delete (customers[i]);
        customers.clear();
    };

    //COPY CONSTRUCTOR
    OpenTable::OpenTable(const OpenTable& action): tableId(action.tableId), customers() {
        for (unsigned int i = 0; i < action.customers.size(); ++i)
            customers.push_back(action.customers[i]->clone());
    };

    //MOVE CONSTRUCTOR
    OpenTable::OpenTable(OpenTable&& action):tableId(action.tableId), customers() {
        for (unsigned int i = 0; i < action.customers.size(); ++i)
            customers.push_back(action.customers[i]->clone());
        for (unsigned int i = 0; i < action.customers.size(); ++i)
            delete (action.customers[i]);
        action.customers.clear();
    };

    // Creates a copy of the action OpenTable.
    OpenTable* OpenTable::clone() const {
        vector <Customer*> customers_list;
        for(unsigned int i=0;i<customers.size(); ++i) {
            customers_list.push_back(customers[i]->clone());
        }
        OpenTable *action = new OpenTable(tableId,customers_list);
        action->setStatus(this->getStatus());
        if(this->getStatus()==ERROR)
            action->error(this->getErrorMsg());
        return action;
    };

    // This action opens a table in the restaurant
    void OpenTable::act(Restaurant &restaurant) {
        int temp=restaurant.gettables().size();
        if ( temp< tableId || restaurant.gettables()[tableId]->isOpen()) {
            error("Error: Table does not exist or is already open");
            cout << "Error: Table does not exist or is already open" << endl;
        }
        else {
            int temp2=restaurant.gettables()[tableId]->getCapacity();
            int temp3=customers.size();
            if (temp2 < temp3) {
                error("Error: too much people for this table");
                cout << "Error: too much people for this table" << endl;
            }
            else {
                restaurant.gettables()[tableId]->setCustomers(customers);
                restaurant.gettables()[tableId]->openTable();
                complete();
            }
        }
    };

    string OpenTable::toString() const {
        string status;
        if (getStatus() == PENDING)
            status = " Pending";
        if (getStatus() == COMPLETED)
            status = " Completed";
        if (getStatus() == ERROR)
            status =" "+ getErrorMsg();
        string str = "open " + to_string(tableId);
        for (unsigned int i = 0; i < customers.size(); ++i) {
            str = str + " " + customers[i]->getName() + "," + customers[i]->getType();
        }
        return str + status;
    };

    // Order: takes an order from a given table

    //CONSTRUCTOR
    Order :: Order(int id): tableId(id){};

    // Creates a copy of the action Order
    Order* Order::clone() const {
        Order *action = new Order(tableId);
        action->setStatus(this->getStatus());
        if(this->getStatus()==ERROR)
            action->error(this->getErrorMsg());
        return action;
    };

    // takes the order from the given table
    void Order::act(Restaurant &restaurant){
        int temp=restaurant.gettables().size();
        if (temp< tableId || !restaurant.gettables()[tableId]->isOpen()) {
            error("Error: Table does not exist or is not open");
            cout << "Error: Table does not exist or is not open" << endl;
        }
        else {
            if(restaurant.getMenu().size()>0)
                restaurant.gettables()[tableId]->order(restaurant.getMenu());
            complete();
        }
    };

    string Order::toString() const {
        string status;
        if (getStatus() == PENDING)
            status = " Pending";
        if (getStatus() == COMPLETED)
            status = " Completed";
        if (getStatus() == ERROR)
            status = " " + getErrorMsg();
        return "order " + to_string(tableId)+status;
    };




// MoveCustomer: moves a customer from one table to another

    //CONSTRUCTOR
    MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTable(src), dstTable(dst), id(customerId){};

    // Creates a copy of MoveCustomer
    MoveCustomer* MoveCustomer::clone() const {
        MoveCustomer *action = new MoveCustomer(srcTable, dstTable, id);
        action->setStatus(this->getStatus());
        if(this->getStatus()==ERROR)
            action->error(this->getErrorMsg());
        return action;
    };

    //moves the customer
    void MoveCustomer::act(Restaurant &restaurant) {

        Table *src_Table = restaurant.gettables()[srcTable];
        Table *dst_Table = restaurant.gettables()[dstTable];
        int temp1=restaurant.gettables().size();
        if ( temp1< srcTable || !src_Table->isOpen()) {
            error("Error: Cannot move customer");
            cout << "Error: Cannot move customer" << endl;
        }
        else {
            if (temp1 < dstTable || !dst_Table->isOpen()) {
                error("Error: Cannot move customer");
                cout << "Error: Cannot move customer" << endl;
            }
            else {
                int temp2=dst_Table->getCapacity();
                int num=dst_Table->get_numOfCustomers();
                if ( temp2<  num + 1) {
                    error("Error: Cannot move customer");
                    cout << "Error: Cannot move customer" << endl;
                }
                else {
                    if (!src_Table->isCustomerExist(id)) {
                        error("Error: Cannot move customer");
                        cout << "Error: Cannot move customer" << endl;
                    }
                    else {
                        Customer *customer = src_Table->getCustomer(id);
                        src_Table->removeCustomer(id);
                        dst_Table->addCustomer(customer);
                        //moves customers orders from origin table to destination table
                        vector<OrderPair> src_orders;
                        for (unsigned int i=0; i<src_Table->getOrders().size(); ++i)
                            src_orders.push_back(src_Table->getOrders()[i]);
                        vector<OrderPair> dst_orders;
                        for (unsigned int i=0; i<dst_Table->getOrders().size(); ++i)
                            dst_orders.push_back(dst_Table->getOrders()[i]);
                        vector<OrderPair> temp;
                        for (unsigned int i = 0; i < src_orders.size(); ++i) {
                            if (src_orders[i].first == id) {
                                dst_orders.push_back(src_orders[i]);
                            } else
                                temp.push_back(src_orders[i]);
                        }
                        dst_Table->set_orders(dst_orders);
                        src_Table->set_orders(temp);
                        temp.clear();
                        src_orders.clear();
                        dst_orders.clear();
                        //checks if origin table is empty after the transfer
                        if (src_Table->get_numOfCustomers() == 0)
                            src_Table->closeTable();
                        complete();
                    }

                }
            }
        }
    };

    string MoveCustomer::toString() const{
        string status;
        if (getStatus() == PENDING)
            status = " Pending";
        if (getStatus() == COMPLETED)
            status = " Completed";
        if (getStatus() == ERROR)
            status = " "+getErrorMsg();
        return "move "+to_string(srcTable)+ " " + to_string(dstTable) + " " + to_string(id)+status;

    };

    // Close: closes a given table and prints the bill of the table to the screen

    //CONSTRUCTOR
    Close::Close(int id):tableId(id){};

    // Creates a copy of Close action
    Close* Close::clone() const {
        Close *action = new Close(tableId);
        action->setStatus(this->getStatus());
        if(this->getStatus()==ERROR)
            action->error(this->getErrorMsg());
        return action;
    };


    void Close::act(Restaurant &restaurant) {
        int temp = restaurant.gettables().size();
        if (temp < tableId || !restaurant.gettables()[tableId]->isOpen()) {
            error("Error: Table does not exist or is not open");
            cout << "Error: Table does not exist or is not open" << endl;
        }
       else {
            int bill = restaurant.gettables()[tableId]->getBill();
            restaurant.gettables()[tableId]->closeTable();
            cout << "Table " << tableId << " was closed. Bill " << bill << "NIS" << endl;
            complete();
        }
    };
        

   string Close:: toString() const {
       string status;
       if (getStatus() == PENDING)
           status = " Pending";
       if (getStatus() == COMPLETED)
           status = " Completed";
       if (getStatus() == ERROR)
           status = " "+getErrorMsg();
       return  "close " + to_string(tableId)+status;
   };



    // CloseAll: closes all tables in the restaurant, and then closes the restaurant and exits

    //CONSTRUCTOR
    CloseAll::CloseAll(){};

    // Creates a copy of CloseAll action
    CloseAll* CloseAll::clone() const {
        CloseAll *action = new CloseAll();
        action->setStatus(this->getStatus());
        return action;
    };


    void CloseAll::act(Restaurant &restaurant) {
        for (unsigned int i = 0; i < restaurant.gettables().size(); ++i) {
            if (restaurant.gettables()[i]->isOpen()) {
                int bill = restaurant.gettables()[i]->getBill();
                restaurant.gettables()[i]->closeTable();
                cout << "Table " << i << " was closed. Bill " << bill << "NIS" << endl;
            }
        }
        complete();
    };


    string CloseAll::toString() const{
        return "closeall";
    };

    //PrintMenu: prints the menu of the restaurant

    //CONSTRUCTOR
    PrintMenu::PrintMenu(){};

    // Creates a copy of PrintMenu action
    PrintMenu*  PrintMenu::clone() const {
        PrintMenu *action = new PrintMenu();
        action->setStatus(this->getStatus());
        return action;
    };

    void PrintMenu::act(Restaurant &restaurant) {
        for (unsigned int i = 0; i < restaurant.getMenu().size(); ++i) {
            DishType type = restaurant.getMenu()[i].getType();
            cout << restaurant.getMenu()[i].getName() << " " << restaurant.convert_type_tostring(type) << " "
                 << restaurant.getMenu()[i].getPrice() << "NIS" << endl;
        }
        complete();
    };

    string  PrintMenu::toString() const{
        string status;
        if (getStatus() == PENDING)
            status = " Pending";
        if (getStatus() == COMPLETED)
            status = " Completed";
        if (getStatus() == ERROR)
            status = " "+getErrorMsg();
        return "menu"+status;
    };

    //PrintTableStatus: prints a status report of a given table

    //CONSTRUCTOR
    PrintTableStatus::PrintTableStatus(int id):tableId(id){};

    //Creates a copy of PrintTableStatus action
    PrintTableStatus*  PrintTableStatus::clone() const {
        PrintTableStatus *action = new PrintTableStatus(tableId);
        action->setStatus(this->getStatus());
        return action;
    };

    void PrintTableStatus::act(Restaurant &restaurant) {
        Table *table = restaurant.gettables()[tableId];
        if (!table->isOpen())
            cout << "Table " << tableId << " status: closed" << endl;
        else {
            cout << "Table " << tableId << " status: open" << endl;
            cout << "Customers:" << endl;
            for (unsigned int i = 0; i < table->getCustomers().size(); ++i) {
                if (table->getCustomers()[i] != nullptr)
                    cout << table->getCustomers()[i]->getId() << " " << table->getCustomers()[i]->getName() << endl;
            }
            cout << "Orders:" << endl;
            vector<OrderPair> orders = table->getOrders();
            for (unsigned int j = 0; j < orders.size(); ++j)
                cout << orders[j].second.getName() << " " << orders[j].second.getPrice() << "NIS " << orders[j].first
                     << endl;
            int bill = table->getBill();
            cout << "Current Bill: " << bill << "NIS" << endl;
        }
        complete();
    };

    string PrintTableStatus::toString() const{
        string status;
        if (getStatus() == PENDING)
            status = " Pending";
        if (getStatus() == COMPLETED)
            status = " Completed";
        if (getStatus() == ERROR)
            status =" "+getErrorMsg();
        return "status "+ to_string(tableId)+status;

    };

    //PrintActionsLog: prints all the actions that were performed by the user

    //CONSTRUCTOR
    PrintActionsLog::PrintActionsLog(){};

    //Creates a copy of PrintActionsLog action
    PrintActionsLog*  PrintActionsLog::clone() const {
        PrintActionsLog *action = new PrintActionsLog();
        action->setStatus(this->getStatus());
        return action;
    };


    void PrintActionsLog::act(Restaurant &restaurant) {

        vector<BaseAction *> actions_log;
        for (unsigned int i = 0; i < restaurant.getActionsLog().size(); ++i)
            actions_log.push_back(restaurant.getActionsLog()[i]->clone());
        for (unsigned int i = 0; i < actions_log.size(); ++i)
            cout << actions_log[i]->toString() << endl;
        for (unsigned int i = 0; i < actions_log.size(); ++i)
            delete(actions_log[i]);
        actions_log.clear();
        complete();
    }
    string PrintActionsLog::toString() const {
        string status;
        if (getStatus() == PENDING)
            status = " Pending";
        if (getStatus() == COMPLETED)
            status = " Completed";
        if (getStatus() == ERROR)
            status =" "+ getErrorMsg();
        return "log"+status;
    };


    //BackupRestaurant: saves all restaurants information

    //CONSTRUCTOR
    BackupRestaurant::BackupRestaurant(){};

    //Creates a copy of BackupRestaurant action
    BackupRestaurant*  BackupRestaurant::clone() const {
        BackupRestaurant *action = new BackupRestaurant();
        action->setStatus(this->getStatus());
        return action;
    };

    void BackupRestaurant:: act(Restaurant &restaurant){
        if(backup== nullptr)
            backup=new Restaurant(restaurant);
        else
            *backup=restaurant;
        complete();
    };

    string BackupRestaurant:: toString() const {
        string status;
        if (getStatus() == PENDING)
            status = " Pending";
        if (getStatus() == COMPLETED)
            status = " Completed";
        if (getStatus() == ERROR)
            status = " "+getErrorMsg();
        return "backup"+status;
    };


    //RestoreResturant: restore the backed up restaurant status and overwrite the current restaurant status

    //CONSTRUCTOR
    RestoreResturant::RestoreResturant(){};

    //Creates a copy of BackupRestaurant action
    RestoreResturant* RestoreResturant::clone() const {
        RestoreResturant *action = new RestoreResturant();
        action->setStatus(this->getStatus());
        if(this->getStatus()==ERROR)
            action->error(this->getErrorMsg());
        return action;
    };


    void RestoreResturant::act(Restaurant &restaurant) {
        bool is_backup=false;
        for (unsigned int i = 0; i<restaurant.getActionsLog().size(); ++i){
            if(restaurant.getActionsLog()[i]->toString()=="backup Completed")
                is_backup=true;
        }
        if (!is_backup) {
            error("Error: No backup available");
            cout << "Error: No backup available" << endl;
        }
        else {
            restaurant = *backup;
            complete();
        }
    };
    string RestoreResturant:: toString() const {
        string status;
        if (getStatus() == PENDING)
            status = " Pending";
        if (getStatus() == COMPLETED)
            status = " Completed";
        if (getStatus() == ERROR)
            status = " " + getErrorMsg();
        return "restore"+status;
    };



