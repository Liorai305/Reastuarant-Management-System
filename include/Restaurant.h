#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{		
public:
	Restaurant();
	Restaurant(const Restaurant& restaurant);
	Restaurant& operator=(const Restaurant &restaurant);
	Restaurant& operator=(Restaurant &&restaurant);
	Restaurant(Restaurant&& restaurant);
    Restaurant(const std::string &configFilePath);
    void start();
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    DishType convert_to_dishtype(std::string &type);
    std::vector<Table*> gettables ();
    virtual ~Restaurant();
    std::string convert_type_tostring(DishType &type);


private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    int num_of_tables;
    int cus_id;
};

#endif