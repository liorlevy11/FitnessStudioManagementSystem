//
// Created by ellabeeri on 10/11/2021.
//
#include "Customer.h"
#include <vector>
#include <string>
#include <algorithm>

Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {

}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}


SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> output;
    for (unsigned i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == WorkoutType::CARDIO) {
            output.push_back(workout_options[i].getId());
        }
    }
    return output;


}

std::string SweatyCustomer::toString() const {
    std::string str = "swt";
    return str;
}



CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {

}


std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> output;
    int chepestWorkout = workout_options[0].getPrice();
    int idChepestWorkout = 0;
    for (unsigned i = 1; i < workout_options.size(); i++) {
        if (workout_options[i].getPrice() < chepestWorkout) {
            chepestWorkout = workout_options[i].getPrice();
            idChepestWorkout = i;
        }
    }
    output.push_back(idChepestWorkout);
    return output;
}

std::string CheapCustomer::toString() const {
    std::string str = "chp";
    return str;
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {
    std::vector<int> output;


}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> output;
    for (unsigned i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == WorkoutType::ANAEROBIC)
            output.push_back(workout_options[i].getId());
    }
    sort(output.begin(), output.end(), [&](const int &a, int &b) -> bool {
        return (workout_options[a].getPrice() > workout_options[b].getPrice());
    });
    return output;
}


std::string HeavyMuscleCustomer::toString() const {
    std::string str = "mcl";
    return str;
}



FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> output;
    int cheapestCardio = cheapestWOIdByType(WorkoutType::CARDIO, workout_options);
    if (cheapestCardio < std::numeric_limits<int>::max())
        output.push_back(cheapestCardio);
    int expensiveMix = expensiveWOIdByType(WorkoutType::MIXED, workout_options);
    if (expensiveMix > std::numeric_limits<int>::min())
        output.push_back(expensiveMix);
    int cheapestAnaerobic = cheapestWOIdByType(WorkoutType::ANAEROBIC, workout_options);
    if (cheapestAnaerobic < std::numeric_limits<int>::max())
        output.push_back(cheapestAnaerobic);


    return output;
}

std::string FullBodyCustomer::toString() const {
    std::string str = "fbd";
    return str;

}



int Customer::cheapestWOIdByType(WorkoutType type, const std::vector<Workout> &vector) {
    int cheapest = std::numeric_limits<int>::max();
    int cheapestID = std::numeric_limits<int>::max();
    for (unsigned i = 0; i < vector.size(); ++i) {
        if ((vector[i].getPrice() < cheapest) && (vector[i].getType() == type)) {
            cheapest = vector[i].getPrice();
            cheapestID = vector[i].getId();
        }
    }
    return cheapestID;
}

int Customer::expensiveWOIdByType(WorkoutType type, const std::vector<Workout> &vector) {
    int expensive = std::numeric_limits<int>::min();
    int expensiveID = std::numeric_limits<int>::min();
    for (unsigned i = 0; i < vector.size(); ++i) {
        if ((vector[i].getPrice() > expensive) && (vector[i].getType() == type)) {
            expensive = vector[i].getPrice();
            expensiveID = vector[i].getId();
        }
    }
    return expensiveID;

}

