//
//  Created by Libat Biton: 204329379 & Liora Ifraimov: 315389197
//

#include "../include/Dish.h"
#include <iostream>
//This class represents a dish in the menu. It has an id, name, price and a type.

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type): id(d_id), name(d_name), price(d_price), type (d_type){
}
int Dish::getId() const{
    return id;
}
std::string Dish::getName() const{
    return name;
}
int Dish::getPrice() const {
    return price;
}
DishType Dish::getType() const{
    return type;
}
;