//
// Created by liorlevy on 17/11/2021.
//

#ifndef TEST_STUDIO_H
#define TEST_STUDIO_H

#endif //TEST_STUDIO_H
#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{
public:
    Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    Studio(const Studio &otherStudio);
    virtual ~Studio();
    Studio & operator=(const Studio&other);
    Studio(Studio &&other);
    Studio& operator=(Studio &&other);

private:
    bool open;
    unsigned numberOfCustomers;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    void clear();
    void buildTrainers(std::string line, int numberOfTrainer);
    void BuildWorkout(std::string line);
    WorkoutType foundWorkoutType(std::string workoutType);
    std::string extractWord(std::string input, unsigned  i);
    Customer* createCustomer(std::string name, std::string workoutType);
    unsigned  extactInt(std::string input, unsigned  i);

};

#endif
