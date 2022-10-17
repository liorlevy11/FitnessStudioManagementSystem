//
// Created by ellabeeri on 10/11/2021.
//
#include <string>
#include <iostream>
#include "Action.h"
#include "Studio.h"

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;

}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
    std::cout << "ERROR:" << errorMsg << std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

BaseAction::BaseAction():errorMsg(""),status(ERROR) {

}



OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers() {
    for (unsigned i = 0; i < customersList.size(); ++i) {
        customers.push_back(customersList[i]);

    }


}

void OpenTrainer::act(Studio &studio) {
    if ((studio.getTrainer(trainerId) == nullptr) || (studio.getTrainer(trainerId)->isOpen())) {
        error("Workout session does not exist or is already open");
    } else {
        Trainer *trainer = studio.getTrainer(trainerId);
        trainer->openTrainer();
        unsigned counter = 0;
        while (counter < customers.size() && trainer->getCapacity() > 0) {
            trainer->addCustomer(customers[counter]);
            counter++;
        }
        complete();
    }


}

std::string OpenTrainer::toString() const {
    std::string str = "open ";
    str = str + std::to_string(trainerId);
    for (unsigned i = 0; i < customers.size(); i++) {
        str = str + " " + customers[i]->getName() + "," + customers[i]->toString();
    }
    if (getStatus() == COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error:" + getErrorMsg();
    return str;
}



void Order::act(Studio &studio) {
    if (studio.getTrainer(trainerId) == nullptr || !studio.getTrainer(trainerId)->isOpen()) {
        error("Trainer does not exist or is not open");
    } else {
        Trainer *trainer = studio.getTrainer(trainerId);
        std::vector<int> workoutForCustomer;
        for (Customer *customer: studio.getTrainer(trainerId)->getCustomers()) {
            workoutForCustomer = customer->order(studio.getWorkoutOptions());
            trainer->order(customer->getId(), workoutForCustomer, studio.getWorkoutOptions());
        }

        for (unsigned i = 0; i < trainer->getOrders().size(); ++i) {
            std::string name = trainer->getCustomer(trainer->getOrders()[i].first)->getName();
            std::cout << name
                      << " Is Doing " << trainer->getOrders()[i].second.getName() << std::endl;
        }
        complete();
    }


}


std::string Order::toString() const {
    std::string str = "order ";
    str = str + std::to_string(trainerId);
    if (getStatus() == COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error:" + getErrorMsg();
    return str;
}

Order::Order(int id) : trainerId(id) {
}



MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {

}

void MoveCustomer::act(Studio &studio) {
    Trainer *src = studio.getTrainer(srcTrainer);
    Trainer *dst = studio.getTrainer(dstTrainer);
    bool found = false;
    std::vector<int>moveWorkourId;
    if (!src->isOpen() || !dst->isOpen() || src == nullptr || dst->getCapacity() <= 0||dst == nullptr)
        error("Cannot move customer");
    else {

        for (unsigned i = 0; i < src->getOrders().size(); ++i) {
            if (src->getOrders()[i].first == id){
                found = true;
                //dst->salary = dst->salary + src->getOrders()[i].second.getPrice();
                moveWorkourId.push_back(src->getOrders()[i].second.getId());}
        }
        if (!found) { error("Cannot move customer"); }
        else {
            dst->addCustomer(src->getCustomer(id));
            dst->order(id,moveWorkourId,studio.getWorkoutOptions());
            studio.getTrainer(srcTrainer)->removeCustomer(id);
            if (src->getCustomers().empty())
                src->closeTrainer();
            complete();
        }
    }
}
//void MoveCustomer::act(Studio &studio) {
//  Trainer *src = studio.getTrainer(srcTrainer);
//Trainer *dst = studio.getTrainer(dstTrainer);
//if (!src->isOpen() | !dst->isOpen() | src == nullptr | dst == nullptr|dst->getCapacity()<=0)
//   error("cannot move customer");
//else{
//  dst->addCustomer(src->getCustomer(id));
//for (int i = 0; i < src->getOrders().size(); ++i) {
//  if (src->getOrders()[i].first == id)
//    dst->salary = dst->salary + src->getOrders()[i].second.getPrice();
//}
//studio.getTrainer(srcTrainer)->removeCustomer(id);
//if (src->getCustomers().empty())
//  src->closeTrainer();
//   /  /complete();
//}
//}

std::string MoveCustomer::toString() const {
    std::string str =
            "move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id);
    if (getStatus() == COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error:" + getErrorMsg();
    return str;
}



Close::Close(int id) : trainerId(id) {

}

void Close::act(Studio &studio) {

    if (studio.getTrainer(trainerId) == nullptr || !studio.getTrainer(trainerId)->isOpen()) {
        error("Trainer doses not exist or is not open");
    } else {
        studio.getTrainer(trainerId)->closeTrainer();
        std::cout << "Trainer " << trainerId << " closed. salary " << studio.getTrainer(trainerId)->getSalary() << "NIS"
                  << std::endl;

        complete();
    }
}


std::string Close::toString() const {
    std::string str = "close";
    str = str + std::to_string(trainerId);
    if (getStatus() == COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error:" + getErrorMsg();
    return str;
}



CloseAll::CloseAll() {
}

void CloseAll::act(Studio &studio) {

    std::vector<Trainer *> trainerList;
    for (int i = 0; i < studio.getNumOfTrainers(); ++i) {
        if (studio.getTrainer(i)->isOpen()){
            Close(i).act(studio);}


    }

    //studio.~Studio();

    complete();
}

std::string CloseAll::toString() const {
    std::string str = "closeall ";
    if (getStatus() == COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error:" + getErrorMsg();
    return str;
}



PrintWorkoutOptions::PrintWorkoutOptions() {

}

void PrintWorkoutOptions::act(Studio &studio) {
    for (unsigned i = 0; i < studio.getWorkoutOptions().size(); ++i) {
        std::cout << studio.getWorkoutOptions()[i].getName() << ", " <<
                  workoutTypeToString(studio.getWorkoutOptions()[i].getType()) << ", "
                  << studio.getWorkoutOptions()[i].getPrice() << std::endl;
    }
    complete();
}


std::string PrintWorkoutOptions::toString() const {
    std::string str = "workout_options ";
    if (getStatus() == COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error:" + getErrorMsg();
    return str;
}



std::string PrintWorkoutOptions::workoutTypeToString(WorkoutType id) {
    if (id == ANAEROBIC)
        return "Anaerobic";
    else if (id == MIXED)
        return "Mixed";
    else return "Cardio";
}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    std::cout << "Trainer " << trainerId << " status: " << isOpenTostring(trainer->isOpen()) << std::endl;
    if (trainer->isOpen()) {
        std::cout << "Customers:" << std::endl;
        for (unsigned i = 0; i < trainer->getCustomers().size(); ++i) {
            std::cout << trainer->getCustomers()[i]->getId() << " " << trainer->getCustomers()[i]->getName()
                      << std::endl;
        }
        for (unsigned i = 0; i < trainer->getOrders().size(); ++i) {
            std::cout << trainer->getOrders()[i].second.getName() << " "
                      << trainer->getOrders()[i].second.getPrice() << "NIS "
                      << studio.getTrainer(trainerId)->getCustomer(trainer->getOrders()[i].first)->getId()
                      << std::endl;
        }
        std::cout << "Current Trainer's Salary: " << trainer->salary << " NIS" << std::endl;
    }
    complete();

}


std::string PrintTrainerStatus::toString() const {
    std::string str = "status ";
    str = str + std::to_string(trainerId);
    if (getStatus() == COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error:" + getErrorMsg();
    return str;
}

PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {

}


std::string PrintTrainerStatus::isOpenTostring(int open) {
    if (open == 1)
        return "open";
    else return "closed";
}

PrintActionsLog::PrintActionsLog() {

}

void PrintActionsLog::act(Studio &studio) {
    studio.getActionsLog();
    for (unsigned i = 0; i < studio.getActionsLog().size(); ++i) {
        std::cout << studio.getActionsLog()[i]->toString() << std::endl;
    }
    complete();
}


std::string PrintActionsLog::toString() const {
    std::string str = "log ";
    if (getStatus() == COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error:" + getErrorMsg();
    return str;
}




BackupStudio::BackupStudio():BaseAction() {
    //Studio(Studio);


}



void BackupStudio::act(Studio &studio) {
    if (backup!= nullptr)
    {
        delete backup;
    }

    backup = new Studio(studio);
    complete();

}

std::string BackupStudio::toString() const {
    std::string str = "backup ";
    if (getStatus() == COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error:" + getErrorMsg();
    return str;
}

RestoreStudio::RestoreStudio() {


}


OpenTrainer::OpenTrainer(const OpenTrainer& other):BaseAction(),trainerId(other.trainerId),customers(){
    for(Customer* c:other.customers)
        customers.push_back(c->clone());}



void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr) {
        error("No backup available");
    } else {
        studio = *backup;
        complete();
    }
}



std::string RestoreStudio::toString() const {
    std::string str = "restore ";
    if (getStatus() == COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error:" + getErrorMsg();
    return str;
}
