#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"


class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    int cheapestWOIdByType(WorkoutType type, const std::vector<Workout> &vector);
    int expensiveWOIdByType(WorkoutType type, const std::vector<Workout> &vector);
    virtual Customer *clone()const=0;
    virtual  ~Customer() = default;

private:
    const std::string name;
    const int id;

};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    virtual std::vector<int> order(const std::vector<Workout> &workout_options);
    virtual std::string toString() const;
    virtual SweatyCustomer *clone() const{return new SweatyCustomer(*this);};
    ~SweatyCustomer()override = default;
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual CheapCustomer *clone() const{return new CheapCustomer(*this);};
    ~CheapCustomer() override = default;
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual HeavyMuscleCustomer *clone() const{return new HeavyMuscleCustomer(*this);};
    ~HeavyMuscleCustomer() override = default;
private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual FullBodyCustomer *clone() const{return new FullBodyCustomer(*this);};
    ~FullBodyCustomer() override = default;
private:

};


#endif
