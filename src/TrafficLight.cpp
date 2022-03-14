#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

/* 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}
*/

/* Implementation of class "TrafficLight" */

/* 


void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

*/

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method cycleThroughPhases should be started in a thread when the public method simulate is called.
    // To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
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
            
            // TODO: send message to queue (not implemented at the moment of FP.2a)

            // reset stopwatch time
            lastUpdate = std::chrono::system_clock::now();
            // new random time between 4...6 s
            random_time_4_to_6s = unif(eng);
        
        }
                    
    }
}

