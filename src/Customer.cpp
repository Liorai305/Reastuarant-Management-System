//
//  Created by Libat Biton: 204329379 & Liora Ifraimov: 315389197
//

#include <vector>
#include <string>
#include "../include/Dish.h"
#include "../include/Table.h"

#include <iostream>

using namespace std;

//This is an abstract class for the different customers classes.
// All customers have an id, name, and bool flag that shows whether the customer ordered.

//CONSTRUCTOR
    Customer::Customer(std::string c_name, int c_id): name(c_name),id(c_id),is_order(false){};

//DESTRUCTOR
    Customer::~Customer(){};

    string Customer::getName() const{
        return name;
    };

    int Customer:: getId() const{
        return id;
    };

    void Customer::change(){
        is_order=true;
    };

    bool Customer::if_order() const {
        return is_order;
    };


//This customer orders the vegetarian dish with the smallest id and the most expensive beverage (non alcoholic).

    VegetarianCustomer::VegetarianCustomer(string name, int id): Customer(name,id){};

    VegetarianCustomer::~VegetarianCustomer(){};

    vector<int> VegetarianCustomer::order(const vector<Dish> &menu){
        int temp_id=-1;
        int temp_price=-1;
        int id_B=-1;
        bool flag=false;
        for(unsigned int i=0; i< menu.size(); ++i){
            if((menu[i].getType()==VEG)&&(!flag)){
                    temp_id=menu[i].getId();
                    flag= true;
                }
            if(menu[i].getType()==BVG && menu[i].getPrice()>temp_price) {
                temp_price = menu[i].getPrice();
                id_B=menu[i].getId();
            }
        }
        vector <int> order;
        if((temp_id!=-1)&&(id_B!=-1)) {
            order.push_back(temp_id);
            order.push_back(id_B);
            change();
        }
        return order;
    };
    string VegetarianCustomer::getType() const{
        return "veg";
    };
    string VegetarianCustomer::toString() const{
        int a=getId();
        string str = to_string(a);
        return "name: "+getName()+", id: "+str+ ", type: veg";
    };

    //This function creates a copy of the customer
    Customer* VegetarianCustomer::clone() const {
        VegetarianCustomer *customer = new VegetarianCustomer(getName(), getId());
        return customer;
    };

    //This customer always orders the cheapest dish in the menu, this customer orders only once.
    CheapCustomer::CheapCustomer(string name, int id):Customer(name,id){};

    CheapCustomer::~CheapCustomer(){};

    vector<int> CheapCustomer::order(const vector<Dish> &menu){
        vector<int> order;
        if(!if_order()) {
            int temp_id = menu[0].getId();
            int temp_price = menu[0].getPrice();
            for (unsigned int i = 1; i < menu.size(); ++i) {
                if (menu[i].getPrice() < temp_price) {
                    temp_id = menu[i].getId();
                    temp_price = menu[i].getPrice();
                }
            }
            order.push_back(temp_id);
            change();  //
        }
        return order;
        };

    string CheapCustomer::getType() const{
        return "chp";
    };

    string CheapCustomer::toString() const{
        int a=getId();
        string str = to_string(a);
        return "name: "+getName()+", id: "+str+ ", type: chp";
    };

//This function creates a copy of the customer
Customer* CheapCustomer::clone() const{
    CheapCustomer* customer=new CheapCustomer(getName(), getId());
    if(if_order())
        customer->change();
    return customer;
};


//This customer orders the most expensive spicy dish in the menu.
// For further orders he picks ths cheapest non alcoholic beverage in the menu.
SpicyCustomer::SpicyCustomer(string name, int id): Customer(name, id){};

SpicyCustomer::~SpicyCustomer(){};

    vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
        vector<int> order;
        if (!if_order()) {
            int temp_id = -1;
            int temp_price = -1;

            for (unsigned int i = 0; i < menu.size(); ++i) {
                if (menu[i].getType() == SPC && menu[i].getPrice() > temp_price) {
                    temp_id = menu[i].getId();
                    temp_price = menu[i].getPrice();
                }
            }
            if(temp_id!=-1) {
                order.push_back(temp_id);
                change();
            }
        }
        else {  //cheapest non alcoholic beverage
            int temp_id = menu[0].getId();
            int temp_price = menu[0].getPrice();
            for (unsigned int i = 1; i < menu.size(); ++i) {
                if (menu[i].getType() == BVG && menu[i].getPrice() < temp_price) {
                    temp_id = menu[i].getId();
                    temp_price = menu[i].getPrice();
                }
            }
            order.push_back(temp_id);
        }
        return order;
    };

    string SpicyCustomer::getType() const{
        return "spc";
    };
    string  SpicyCustomer::toString() const {
        int a = getId();
        string str = to_string(a);
        return "name: " + getName() + ", id: " + str + ", type: spc";
    };

//This function creates a copy of the customer
Customer* SpicyCustomer::clone() const{
    SpicyCustomer* customer=new SpicyCustomer(getName(), getId());
    if(if_order())
        customer->change();
    return customer;
};

//This customer only orders alcoholic beverages.
//He starts with ordering the cheapest one and in each further order he picks the
//next expensive alcoholic beverage.
//This customer holds a vector of all alcoholic beverages from the menu, and the price of his last order.
//when ordering, he finds the cheapest dish in the vector, orders it and removes the dish from the vector.

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id):Customer(name,id), last_price(-1),alc_dish(){};

AlchoholicCustomer::~AlchoholicCustomer(){
    alc_dish.clear();
};

    vector<int> AlchoholicCustomer:: order(const vector<Dish> &menu) {
        if (last_price == -1) {
            for (unsigned int i = 0; i < menu.size(); ++i)
                if (menu[i].getType() == ALC)
                    alc_dish.push_back(menu[i]);
        }
        vector<int> order;
        if (alc_dish.size() > 0) {
            int temp_price = alc_dish[0].getPrice();
            int temp_id = alc_dish[0].getId();
            for (unsigned int i = 1; i < alc_dish.size(); ++i) {
                int temp = alc_dish[i].getPrice();
                if (temp_price > temp) {
                    temp_price = alc_dish[i].getPrice();
                    temp_id = alc_dish[i].getId();
                }
            }
            last_price = temp_price;
            order.push_back(temp_id);
        vector<Dish> help;
        for (unsigned int i = 0; i < alc_dish.size(); ++i) {
            int id1 = alc_dish[i].getId();
            if (temp_id != id1)
                help.push_back(alc_dish[i]);
        }
        alc_dish.clear();
        for (unsigned int i = 0; i < help.size(); ++i)
            alc_dish.push_back(help[i]);
        help.clear();

        }
        return order;
    };


    string AlchoholicCustomer::getType() const {
        return "alc";
    };
    string AlchoholicCustomer::toString() const{
        int a = getId();
        string str = to_string(a);
        return "name: " + getName() + ", id: " + str + ", type: alc";
    };

//This function creates a copy of the customer
Customer* AlchoholicCustomer::clone() const{
    AlchoholicCustomer* customer=new AlchoholicCustomer(getName(), getId());
    customer->last_price=last_price;
    for (unsigned int i = 0; i < alc_dish.size(); ++i)
        customer->alc_dish.push_back(alc_dish[i]);
    return customer;
};

;


