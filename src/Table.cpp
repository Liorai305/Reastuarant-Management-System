//
//  Created by Libat Biton: 204329379 & Liora Ifraimov: 315389197
//
#include "../include/Table.h"
#include <iostream>
#include <vector>
using namespace std;

//This class represents a table in the restaurant.
//each table holds capacity, flag open, list of orders done in this table, num of customers and a list of customers.

Table::Table(int t_capacity): capacity(t_capacity),open(false),numOfCustomers(0) ,customersList(), orderList(){};

//copy constructor
Table::Table(const Table &table):capacity(table.capacity), open(table.open), numOfCustomers(table.numOfCustomers), customersList(),orderList(table.orderList){
    for(unsigned int i=0;i<table.customersList.size(); ++i){
        customersList.push_back(table.customersList[i]->clone());
    }
};
//move constructor
Table::Table(Table&& table):capacity(table.capacity), open(table.open), numOfCustomers(table.numOfCustomers),customersList(), orderList(table.orderList){
    for(unsigned int i=0;i<table.customersList.size(); ++i)
        customersList.push_back(table.customersList[i]->clone());
    for (unsigned int i = 0; i < table.customersList.size(); ++i)
        delete (table.customersList[i]);
    table.customersList.clear();
    table.orderList.clear();
};

//copy assignment operator
Table& Table::operator=(const Table &table) {
    if(this!=&table) {
        capacity = table.capacity;
        open = table.open;
        numOfCustomers = table.numOfCustomers;
        orderList.clear();
        for (unsigned int i = 0; i < table.orderList.size(); ++i)
            orderList.push_back(table.orderList[i]);
        for (unsigned int i = 0; i < customersList.size(); ++i)
            delete (customersList[i]);
        customersList.clear();
        for (unsigned int i = 0; i < table.customersList.size(); ++i)
            customersList.push_back(table.customersList[i]->clone());
    }
    return *this;
};

//move assignment operator
Table& Table::operator=(Table &&table) {
    if(this!=&table) {
        capacity = table.capacity;
        open = table.open;
        numOfCustomers = table.numOfCustomers;
        orderList.clear();
        for (unsigned int i = 0; i < table.orderList.size(); ++i)
            orderList.push_back(table.orderList[i]);
        for (unsigned int i = 0; i < customersList.size(); ++i)
            delete (customersList[i]);
        customersList.clear();
        for (unsigned int i = 0; i < table.customersList.size(); ++i)
            customersList.push_back(table.customersList[i]->clone());
        for (unsigned int i = 0; i < table.customersList.size(); ++i)
            delete (table.customersList[i]);
        table.customersList.clear();
        table.orderList.clear();
    }
    return *this;
};

//Destructor
Table::~Table() {
    if(open){
        for (unsigned int i =0; i<customersList.size(); ++i)
            delete(customersList[i]);
        customersList.clear();
        orderList.clear();
    }
};

//this function creates a copy of table
Table* Table::clone() {
    Table *table = new Table(*this);
    return table;
};

int Table:: getCapacity() const{
    return capacity;
};

// this function adds a customer to the customers list
void Table:: addCustomer(Customer* customer){
    if(numOfCustomers<capacity) {
        customersList.push_back(customer);
        numOfCustomers = numOfCustomers + 1;
    } else cout << "The table is full" << endl;
};

//this function removes a customer from the customers list
void Table::removeCustomer(int id){
    vector<Customer*> temp;
    for(unsigned int i=0; i<customersList.size(); ++i){
        if(customersList[i]->getId()!=id)
            temp.push_back(customersList[i]);
    }
    customersList.clear();
    for(unsigned int j=0; j<temp.size(); ++j)
            customersList.push_back(temp[j]);
    temp.clear();
    numOfCustomers=numOfCustomers-1;
};

//This function sets customers to table
void Table::setCustomers(vector<Customer*> other_customersList) {
    for(unsigned int i=0; i<other_customersList.size(); ++i)
        customersList.push_back(other_customersList[i]->clone());
    numOfCustomers=customersList.size();
};


Customer* Table::getCustomer(int id) {
for(unsigned int i=0; i<customersList.size(); ++i) {
    if (customersList[i]->getId() == id)
        return customersList[i];
}
    return nullptr;
};

vector<Customer*>& Table::getCustomers(){
    return customersList;
};

vector<OrderPair>& Table::getOrders(){
    return orderList;
};

bool Table::isCustomerExist(int id){
    bool flag=false;
    for(unsigned int i=0; (!flag)&(i<customersList.size()); ++i) {
        if (customersList[i]->getId() == id)
            flag = true;
    }
    return flag;
};
int Table::get_numOfCustomers() {
    return numOfCustomers;
};

//This function takes an order from each customer in the table.
void  Table::order(const vector<Dish> &menu) {
    for (unsigned int i = 0; i < customersList.size(); ++i) {
        if (customersList[i] != nullptr) {
            vector<int> order = customersList[i]->order(menu);
            for (unsigned int j = 0; j < order.size(); ++j) {
                orderList.push_back(OrderPair(customersList[i]->getId(),get_dish(order[j],menu)));
                cout << customersList[i]->getName() << " ordered " << get_dish(order[j],menu).getName() << endl;
            }
            order.clear();
        }

    }
};

// This function receives an id, and returns a dish according to the id.
    Dish Table::get_dish(int id, const vector<Dish> &menu){
            Dish dish= menu[0];
            for(unsigned int i=0; i<menu.size(); ++i){
                if(menu[i].getId()==id)
                    return menu[i];
            }
            return dish;
        };



void Table::openTable(){
    open=true;
};

//This function closes the table
void Table::closeTable(){
    numOfCustomers=0;
    open= false;
    for (unsigned int i =0; i<customersList.size(); ++i)
        delete(customersList[i]);
    customersList.clear();
    orderList.clear();

};

int Table::getBill(){
    int sum = 0;
    for (unsigned int i =0; i<orderList.size(); ++i)
        sum = sum + orderList[i].second.getPrice();
    return sum;
};

//This function sets orders to table
void Table::set_orders(std::vector<OrderPair> &temp){
    orderList.clear();
    for (unsigned int i =0; i<temp.size(); ++i)
        orderList.push_back(temp[i]);
}

bool Table::isOpen(){
    return open;
};

