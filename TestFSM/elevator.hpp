//
//  elevator.hpp
//  TestFSM
//
//  Created by Peter Higley on 8/5/17.
//  Copyright © 2017 Peter Higley. All rights reserved.
//

#ifndef elevator_hpp
#define elevator_hpp

#include <vector>

class Passenger;

class Elevator {
    
public:
    
    enum class Direction {
        Up = 0,
        Down,
        None
    };
    
    enum class State : std::size_t {
        Up = 0,
        Down,
        Idle,
        Loading,
        
        MaxStates
    };
    
    static const char* describeState(State state);
    
    using PassengerList = std::vector< Passenger* >;
    
public:
    
    void step();
    
    void addPassenger(Passenger& passenger);
    
    State getState() const { return currentState; }
    
    bool allPassengersAtDesiredFloor() const;
    
    friend std::ostream& operator<<(std::ostream& out, const Elevator& elevator);
    
private:
    
    int   currentFloor = 0;
    int   desiredFloor = 0;
    State currentState = State::Idle;
    
    PassengerList passengers;
    
};

#endif /* elevator_hpp */
