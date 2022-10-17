//
// Created by liorlevy on 17/11/2021.
//

#include "Studio.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Trainer.h"
#include <bits/stdc++.h>

using namespace std;

Studio::Studio() : open(false),numberOfCustomers(0), trainers(), workout_options(), actionsLog() {
}

void Studio::start() {
    open = true;
    std::cout << "Studio is now open!" << std::endl;
    string input;
    while (open) {
        unsigned  i = 0;
        getline(cin, input);
        string action = extractWord(input, i);
        i = input.find(action) + action.size();
        if (action == "open") {
            vector<Customer *> customerList;
            string id = extractWord(input, i);
            i = extactInt(input,i);
            int trainerId = stoi(id);
            if ( trainerId<getNumOfTrainers() ) {
                unsigned  maxCustomer=numberOfCustomers+getTrainer(trainerId)->getCapacity();
                while (i < input.size()&& numberOfCustomers<maxCustomer&&(!getTrainer(trainerId)->isOpen())) {
                    string name = extractWord(input, i);
                    i = extactInt(input, i);
                    string workoutType = extractWord(input, i);
                    i = extactInt(input, i);
                    customerList.push_back(createCustomer(name, workoutType));
                }
            }
            auto *openNow = new OpenTrainer(trainerId, customerList);
            openNow->act(*this);
            actionsLog.push_back(openNow);

        }else if (action == "order") {
            string id = extractWord(input, i);
            int trainerId = stoi(id);
            auto *OrderTrainer = new Order(trainerId);
            OrderTrainer->act(*this);
            actionsLog.push_back(OrderTrainer);
        } else if (action == "move") {
            string srcTrainerId = extractWord(input, i);
            i = extactInt(input,i);
            int src= stoi(srcTrainerId);
            string destTrainerId = extractWord(input, i);
            i = extactInt(input,i);
            int dst=stoi(destTrainerId);
            string customerId = extractWord(input, i);
            int cus=stoi(customerId);
            auto *move = new MoveCustomer(src, dst, cus);
            move->act(*this);
            actionsLog.push_back(move);

        } else if (action == "close") {
            string id = extractWord(input, i);
            int trainerId = stoi(id);
            auto *closeTrainer = new Close(trainerId);
            closeTrainer->act(*this);
            bool allClosed = true;
            for (unsigned  i = 0; i < (getNumOfTrainers() & allClosed); ++i) {
                if (getTrainer(i)->isOpen())
                    allClosed = false;
            }
            if (allClosed)
                open = false;
            actionsLog.push_back(closeTrainer);
        } else if (action == "closeall") {
            auto *closeall = new CloseAll();
            closeall->act(*this);
            this->clear();
            delete closeall;
            open = false;
        } else if (action == "log") {
            auto *log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);
        } else if (action == "status") {
            string id = extractWord(input, i);
            int trainerId = stoi(id);
            auto *statusTrainer = new PrintTrainerStatus(trainerId);
            statusTrainer->act(*this);
            actionsLog.push_back(statusTrainer);
        } else if (action == "workout_options") {
            auto *workoutOption = new PrintWorkoutOptions();
            workoutOption->act(*this);
            actionsLog.push_back(workoutOption);
        } else if (action == "backup") {
            auto *backupAction = new BackupStudio();
            backupAction->act(*this);
            actionsLog.push_back(backupAction);
        } else if (action == "restore") {
            auto *restoreAction = new RestoreStudio();
            restoreAction->act(*this);
            actionsLog.push_back(restoreAction);
        } else {
            std::cout << "command is undefined."<<endl;
        }

    }
}

int extactInt(std::string input, unsigned  i) {
    string output = "";
    while ((input[i] == '#') || (input[i] == ' ') || (input[i] == ',') || i >= input.size())
        i++;
    while ((input[i] != '#') && (input[i] != ' ') && (input[i] != ',') && i < input.size()) {
        output = output + input[i];
        i++;
    }
    return i;
}


int Studio::getNumOfTrainers() const {

    return trainers.size();
}


Trainer *Studio::getTrainer(int tid) {
    int diff = trainers.size()-tid;
    if ((diff > 0) && tid >= 0) {
        return trainers[tid];
    } else
        return nullptr;
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}

Studio::Studio(const Studio &otherStudio):open(false),numberOfCustomers(0),trainers(std::vector<Trainer*>()),workout_options(std::vector<Workout>()),actionsLog(std::vector<BaseAction*>()) {
    this->clear();
    open=otherStudio.open;
    numberOfCustomers = otherStudio.numberOfCustomers;
    for (unsigned i = 0; i < otherStudio.trainers.size(); ++i) {
        trainers.push_back(otherStudio.trainers.at(i)->clone());
    }
    for (unsigned i = 0; i < otherStudio.workout_options.size(); i++){
        workout_options.push_back((otherStudio.workout_options[i]));}

    for (unsigned i = 0; i < otherStudio.getActionsLog().size(); ++i) {
        Studio::actionsLog.push_back(otherStudio.getActionsLog().at(i)->clone());
    }

}

Studio::~Studio() {
    clear();
}

void Studio::clear() {
    for(Trainer *trainer: trainers) {
        delete trainer;
        trainer = nullptr;
    }
    for(BaseAction *action: this->actionsLog) {
        delete action;
        action = nullptr;
    }
    numberOfCustomers=0;
    open= false;
    actionsLog.clear();
    trainers.clear();
    workout_options.clear();

}
Studio &Studio::operator=(const Studio &other) {
    if (&other != this) {
        open = other.open;
        numberOfCustomers = other.numberOfCustomers;
        workout_options.clear();
        for (unsigned i = 0; i < other.workout_options.size(); i++)
            workout_options.push_back((other.workout_options[i]));
        for (unsigned i = 0; i < trainers.size(); ++i) {
            delete trainers.at(i);
        }
        trainers.clear();
        for (unsigned i = 0; i < other.trainers.size(); ++i) {
            trainers.push_back(other.trainers.at(i)->clone());
        }
        for (unsigned i = 0; i < actionsLog.size(); ++i) {
            delete actionsLog.at(i);
        }
        actionsLog.clear();
        for (unsigned i = 0; i < other.actionsLog.size(); ++i) {
            actionsLog.push_back(other.actionsLog.at(i)->clone());
        }

    }
    return *this;
}

Studio::Studio(Studio &&other) : open(other.open), numberOfCustomers(other.numberOfCustomers), trainers(other.trainers), workout_options(other.workout_options),
                                 actionsLog(other.actionsLog) {
    //open = false;
    //trainers.clear();
    //workout_options.clear();
    //actionsLog.clear();

}

Studio &Studio::operator=(Studio &&other) {
    if (this != &other) {
        clear();
        open = other.open;
        trainers = other.trainers;
        workout_options.clear();
        for (unsigned i = 0; i < other.workout_options.size(); i++)
            workout_options.push_back((other.workout_options[i]));
        actionsLog = other.actionsLog;
        other.trainers.clear();
        other.workout_options.clear();
        other.actionsLog.clear();
        other.open = false;
        numberOfCustomers = other.numberOfCustomers;
    }
    return *this;
}

void Studio::buildTrainers(string line, int numberOfTrainer) {
    unsigned  line_size = sizeof(line);
    int counter = 0;
    bool finish = false;
    for (unsigned i = 0; i < line_size && !finish; i++) {
        string s = "";
        while ((line[i] == '#') || (line[i] == '\0') || line[i] == ',' || finish || line[i] == ' ' || line[i] == '"')
            i++;
        while ((line[i] != '#') && (line[i] != '\0') && line[i] != ',' && !finish) {
            s = s + line[i];
            i++;
        }
        counter++;
        int CapsityOfTrainer = std::stoi(s);;
        Trainer *trainer = new Trainer(CapsityOfTrainer);
        trainers.push_back(trainer);
        if (counter == numberOfTrainer)
            finish = true;
    }
}


void Studio::BuildWorkout(string line) {
    string workoutName = "";
    string workoutTypeString = "";
    string priceString = "";
    unsigned  i = 0;
    while ((line[i] != '#') && (line[i] != '\0') && (line[i] != ',')) {
        workoutName = workoutName + line[i];
        i++;
    }
    while ((line[i] == '#') || (line[i] == '\0') || (line[i] == ','))
        i++;
    while ((line[i] != '\0') && line[i] != ',') {
        workoutTypeString = workoutTypeString + line[i];
        i++;
    }
    while ((line[i] == '#') || (line[i] == '\0') || (line[i] == ','))
        i++;
    while ((line[i] != '\0') && line[i] != ',') {
        priceString = priceString + line[i];
        i++;
    }

    int id = workout_options.size();
    int price = stoi(priceString);
    WorkoutType workoutType = foundWorkoutType(workoutTypeString);
    Workout *workout = new Workout(id, workoutName, price, workoutType);
    workout_options.push_back(*workout);
    delete workout;
}

WorkoutType Studio::foundWorkoutType(std::string workoutType) {
    string firstChar = workoutType.substr(1, 1);
    if ((firstChar == "A") | (firstChar == "a")) {
        return ANAEROBIC;
    } else if ((firstChar == "m") | (firstChar == "M")) { return MIXED; }
    else return CARDIO;
}


std::string Studio::extractWord(std::string input, unsigned  i) {
    string output = "";
    while ((input[i] == '#') || (input[i] == ' ') || input[i] == '\0' || (input[i] == ','))
        i++;
    while ((input[i] != '#') && (input[i] != ' ') && input[i] != '\0' && (input[i] != ',') && i < input.size()) {
        output = output + input[i];
        i++;
    }
    return output;
}

Customer *Studio::createCustomer(std::string name, std::string workoutType) {
    Customer *customer;
    if (workoutType == "swt") {
        customer = new SweatyCustomer(name, numberOfCustomers);

    } else if (workoutType == "chp") {
        customer = new CheapCustomer(name, numberOfCustomers);

    } else if (workoutType == "mcl") {
        customer = new HeavyMuscleCustomer(name, numberOfCustomers);
    } else
        customer = new FullBodyCustomer(name, numberOfCustomers);


    numberOfCustomers++;
    return customer;
}

Studio::Studio(const std::string &configFilePath):open(false),numberOfCustomers(0),trainers(std::vector<Trainer*>()),workout_options(std::vector<Workout>()),actionsLog(std::vector<BaseAction*>()) {

    std::ifstream file(configFilePath);
    string line = "";
    std::getline(file, line);
    while ((line[0] == '#') | (line[0] == '\0'))
        std::getline(file, line);
    int numberOfTrainer = std::stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    while ((line[0] == '#') | (line[0] == '\0'))
        std::getline(file, line);
    buildTrainers(line, numberOfTrainer);
    while (file) {
        std::getline(file, line);
        if (line[0] == '#')
            continue;
        if (line[0] == '\0')
            continue;
        else
            BuildWorkout(line);
    };

    file.close();
}


unsigned  Studio::extactInt(std::string input, unsigned i) {
    string output = "";
    while ((input[i] == '#') || (input[i] == ' ') || (input[i] == ',') || i >= input.size())
        i++;
    while ((input[i] != '#') && (input[i] != ' ') && (input[i] != ',') && i < input.size()) {
        output = output + input[i];
        i++;
    }
    return i;
}

//check//
