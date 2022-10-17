#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;
//OrderPair *clone();

class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    int getLimitCapacity();
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int>& workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    int salary;
    Trainer(const Trainer &otherTrainer);
    virtual Trainer *clone();
    virtual ~Trainer();
    Trainer & operator=(const Trainer &other);
    Trainer(Trainer &&other);
    Trainer& operator=(Trainer &&other);

private:
    int limitCapacity;
    int capacity = limitCapacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    // Workout workoutFromID(int id);
    //Trainer *copy() const;
    void clear();

};


#endif
