#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    COMPLETED, ERROR
};


//Forward declaration
class Studio;
extern Studio* backup;
class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone() const=0;
    virtual ~BaseAction() = default;
    // Studio backup;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;


private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    OpenTrainer(const OpenTrainer& other);
    void act(Studio &studio);
    std::string toString() const;
    virtual OpenTrainer* clone() const {return new OpenTrainer(*this);};
   // ~OpenTrainer() ;
private:
    const int trainerId;
    std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio &studio);
    std::string toString() const;
    virtual Order *clone()const{    return new Order(*this);};
    ~Order() {};
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio &studio);
    std::string toString() const;
    virtual MoveCustomer *clone()const{return new MoveCustomer(*this);};
    ~MoveCustomer() {};

private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Studio &studio);
    std::string toString() const;
    virtual Close *clone() const { return new Close(*this);};
    ~Close() {};
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string toString() const;
    virtual CloseAll *clone() const { return new CloseAll(*this);};
    ~CloseAll() {};
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
    virtual PrintWorkoutOptions *clone() const {    return new PrintWorkoutOptions(*this);};
    ~PrintWorkoutOptions() {};
private:
    std::string workoutTypeToString(WorkoutType id);
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio &studio);
    std::string toString() const;
    virtual PrintTrainerStatus *clone() const {    return new PrintTrainerStatus(*this);};
    ~PrintTrainerStatus() {};
private:
    const int trainerId;
    std::string isOpenTostring(int open);
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
    virtual PrintActionsLog *clone() const {  return new PrintActionsLog(*this);};
    ~PrintActionsLog() {};
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
    virtual BackupStudio *clone() const {    return new BackupStudio(*this);};
    ~BackupStudio() {};
private:

};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
    virtual RestoreStudio *clone() const {    return new RestoreStudio(*this);};
    ~RestoreStudio() {};

};


#endif
