#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    virtual std::string getType() const = 0;
    virtual Customer* clone() const=0;
    virtual ~Customer();
    std::string getName() const;
    int getId() const;
	void change();
	bool if_order() const;
private:
    const std::string name;
    const int id;
    bool is_order;

};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string getType () const;
    Customer* clone() const;
    std::string toString() const;
    virtual ~VegetarianCustomer();
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string getType () const;
    std::string toString() const;
    Customer* clone() const;
    virtual ~CheapCustomer();
private:
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string getType ()const;
    std::string toString() const;
    Customer* clone() const;
    virtual ~SpicyCustomer();
private:
};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string getType () const;
    std::string toString() const;
    Customer* clone() const;
    virtual ~AlchoholicCustomer();

private:
	int last_price;
    std::vector<Dish> alc_dish;
};


#endif