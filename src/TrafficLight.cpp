#include <iostream>
#include <random>
#include <future>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */
template <typename T>
T MessageQueue<T>::receive() {
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function.
    
    // perform queue modification under the lock
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); }); // pass unique lock to condition variable

    // remove last vector element from queue
    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg; // will not be copied due to return value optimization (RVO) in C++ 
}

// copied from lecture example
template <typename T>
void MessageQueue<T>::send(T&& msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // perform vector modification under the lock
    std::lock_guard<std::mutex> uLock(_mutex);

    // Taken from: https://knowledge.udacity.com/questions/770770
    // all old messages are obsolete as soon as the light switches state
    _queue.clear();

    // add vector to queue
    // std::cout << "   Message " << msg << " has been sent to the queue" << std::endl;
    _queue.push_back(std::move(msg));

    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    _cond.notify_one(); // notify client
}


/* Implementation of class "TrafficLight" */

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while(true){
        if(_msgQueue->receive() == TrafficLightPhase::green){
            return;
        }
    }
}



void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method cycleThroughPhases should be started in a thread when the public method simulate is called.
    // To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
    _msgQueue = std::make_shared<MessageQueue<TrafficLightPhase>>();
}

TrafficLightPhase TrafficLight::getCurrentPhase() const
{
    return _currentPhase;
}

void TrafficLight::setCurrentPhase(TrafficLightPhase phase){
    _currentPhase = phase;
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    
    // initialize stopwatch
    auto lastUpdate = std::chrono::system_clock::now();

    // Random time init - The cycle duration should be a random value between 4 and 6 seconds. 
    std::random_device rd;
    std::mt19937 eng(rd());
    double lower_bound = 4000.0; // ms
    double upper_bound = 6000.0; // ms
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    double random_time_4_to_6s = unif(eng);
    
    // future
    std::future<void> future;

    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    for(;;) {
        // the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                            
        // measure time between two loops
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        
        if(timeSinceLastUpdate > random_time_4_to_6s){

            // toggles the current phase of the traffic light between red and green and sends an update method 
            // to the message queue using move semantics. 
            if(this->getCurrentPhase() == TrafficLightPhase::green){
                setCurrentPhase(TrafficLightPhase::red);
            }else{
                setCurrentPhase(TrafficLightPhase::green);
            }
            
            // FP4b ... and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
            // send in conjunction with move semantics.
            future = std::async(std::launch::async, &MessageQueue<TrafficLightPhase>::send, _msgQueue, std::move(_currentPhase));
            future.wait();

            // reset stopwatch time
            lastUpdate = std::chrono::system_clock::now();
            // new random time between 4...6 s
            random_time_4_to_6s = unif(eng);
        
        }
                    
    }
}

