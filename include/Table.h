#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    Table(const Table& table);
    Table(Table&& table);
    Table& operator=(const Table &table);
    Table& operator=(Table &&table);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    void setCustomers(std::vector<Customer*> customersList);
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    bool isCustomerExist(int id);
    int get_numOfCustomers();
    Dish get_dish(int id, const std::vector<Dish> &menu);
    void set_orders (std::vector<OrderPair> &temp);
    virtual ~Table();
    Table* clone();

private:
    int capacity;
    bool open;
    int numOfCustomers; // add to the constructor
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif