#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;


// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 

template <class T>
class MessageQueue {
public:
    MessageQueue();
    T receive() ;
    void send(T&& msg);

private:
    // Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
    std::deque<T> _queue;

    // Also, there should be an std::condition_variable as well as an std::mutex as private members. 
    std::condition_variable _cond;
    std::mutex _mutex;
    
};


// where TrafficLightPhase is an enum that can be either „red“ or „green“.
enum class TrafficLightPhase {red, green};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. 
class TrafficLight : public TrafficObject {
  public:
    // constructor / desctructor
    TrafficLight();

    // getters / setters
    // as well as „TrafficLightPhase getCurrentPhase()“
    TrafficLightPhase getCurrentPhase(void) const;
    void setCurrentPhase(TrafficLightPhase phase);

    // typical behaviour methods
    // The class shall have the public methods „void waitForGreen()“ and „void simulate()“ 
    
    void waitForGreen();
    void simulate();

  private:
    // typical behaviour methods
    // Also, add the private method „void cycleThroughPhases()“  
    void cycleThroughPhases();

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    std::shared_ptr<MessageQueue<TrafficLightPhase>> _msgQueue; // use shared pointer on the heap for move semantics
    std::condition_variable _condition;
    std::mutex _mutex;
    // Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 
    TrafficLightPhase _currentPhase;

};

#endif