//
// Created by ellabeeri on 10/11/2021.
//

#include "Trainer.h"
#include "Studio.h"
#include "Customer.h"
#include <iostream>

int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
    capacity--;

}

void Trainer::removeCustomer(int id) {
    std::vector<Customer *> tempCustomer;
    for (Customer *customermove: customersList) {
        if (customermove->getId() != id)
            tempCustomer.push_back(customermove);
        customersList.pop_back();
    }
    customersList.clear();
    customersList = tempCustomer;
    tempCustomer.clear();
    capacity++;
    std::vector<OrderPair> tempOrderList;
    if (customersList.empty())
        closeTrainer();
    for (OrderPair orderpairmove: orderList) {
        OrderPair tempOrderPair = orderList.at(orderList.size() - 1);
        if (tempOrderPair.first != id) {
            tempOrderList.push_back(tempOrderPair);
        } else {
            salary = salary - tempOrderPair.second.getPrice();
        }
        orderList.pop_back();
    }
    orderList.clear();
    for (OrderPair insertOrderList: tempOrderList) {
        orderList.push_back(insertOrderList);
        tempOrderList.pop_back();
    }
    tempOrderList.clear();
}



Customer *Trainer::getCustomer(int id) {
    for (unsigned i = 0; i < customersList.size(); ++i) {
        if (customersList[i]->getId() == id)
            return customersList[i];
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;

}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}

void
Trainer::order(const int customer_id, const std::vector<int> &workout_ids,
               const std::vector<Workout> &workout_options) {
    // bool found = false;

    for (unsigned i = 0; i < workout_ids.size(); i++) {
        for (unsigned j = 0; j < workout_options.size(); j++) {
            if (workout_options[j].getId() == workout_ids[i]) {
                OrderPair pairIdWorkout = std::make_pair(customer_id, workout_options[j]);
                salary = salary + pairIdWorkout.second.getPrice();
                orderList.push_back(pairIdWorkout);

            }


        }
    }
}




void Trainer::openTrainer() {
    open = true;
}

void Trainer::closeTrainer() {
    open = false;
    capacity = limitCapacity;
    while (!orderList.empty())
        orderList.pop_back();
    for (Customer *customer: customersList) {
        delete customer;
        customer = nullptr;
        this->getCustomers().clear();
    }
    customersList.clear();
    orderList.clear();
}

int Trainer::getSalary() {
    return salary;
}

bool Trainer::isOpen() {

    return open;
}

Trainer::Trainer(int t_capacity) :salary(0),limitCapacity(t_capacity), capacity(t_capacity), open(false), customersList(), orderList()
{


}

Trainer::Trainer(const Trainer &otherTrainer) :salary(otherTrainer.salary),limitCapacity(otherTrainer.limitCapacity), capacity(otherTrainer.capacity),
                                               open(otherTrainer.open),customersList(std::vector<Customer*>()),orderList(otherTrainer.orderList){
    for (unsigned i = 0; i < otherTrainer.customersList.size(); ++i) {
        customersList.push_back(otherTrainer.customersList.at(i)->clone());
        //orderList.clear();
        for (unsigned i = 0; i < otherTrainer.orderList.size(); i++)
            orderList.push_back(otherTrainer.orderList[i]);

    }
}


Trainer::Trainer(Trainer &&other) :salary(other.salary), limitCapacity(other.limitCapacity),capacity(other.capacity), open(other.open),
                                   customersList(other.customersList), orderList(other.orderList)
{
    // customersList.clear();
    // orderList.clear();
    //  limitCapacity = 0;
    //  capacity = 0;
    //  open = false;

};

void Trainer::clear() {
    for (Customer *customer: customersList) {
        delete customer;
        customer = nullptr;
    }
    capacity = 0;
    limitCapacity = 0;
    open = false;
    orderList.clear();
    customersList.clear();
    //  delete std::vector<OrderPair>orderList;

}

Trainer::~Trainer() {
    clear();

}

Trainer &Trainer::operator=(const Trainer &other) {
    if (&other != this) {
        salary = other.salary;
        limitCapacity = other.limitCapacity;
        capacity = other.capacity;
        open = other.open;
        orderList.clear();
        for (unsigned i = 0; i < other.orderList.size(); i++)
            orderList.push_back(other.orderList[i]);
        for (unsigned i = 0; i < customersList.size(); ++i) {
            delete customersList[i];
        }
        customersList.clear();
        for (unsigned i = 0; i < other.customersList.size(); ++i) {
            customersList.push_back(other.customersList[i]);
        }


    }
    return *this;
};

Trainer *Trainer::clone() {
    return new Trainer(*this);
}

Trainer &Trainer::operator=(Trainer &&other) {
    if (this != &other)  {
        salary = other.salary;
        limitCapacity = other.limitCapacity;
        capacity = other.capacity;
        open = other.open;
        orderList.clear();
        for (unsigned i = 0; i < other.orderList.size(); i++)
            orderList.push_back(other.orderList[i]);
        for (unsigned i = 0; i < customersList.size(); ++i) {
            delete customersList[i];
        }
        customersList.clear();
        for (unsigned i = 0; i < other.customersList.size(); ++i) {
            customersList.push_back(other.customersList[i]->clone());
        }
    }

    return *this;

};


int Trainer::getLimitCapacity() {
    return limitCapacity;

}
