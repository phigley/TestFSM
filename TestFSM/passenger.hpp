//
//  passenger.h
//  TestFSM
//
//  Created by Peter Higley on 8/5/17.
//  Copyright © 2017 Peter Higley. All rights reserved.
//

#ifndef passenger_h
#define passenger_h

#include <string>
#include <iosfwd>

class Passenger {
public:
    
    Passenger(const std::string& _name)
        : name(_name)
    {
    }
    
    void requestFloor(int floor) { desiredFloor = floor; }
    void enterElevator() { onElevator = true; }
    void leaveElevator(int floor) { onElevator = false; currentFloor = floor; }
    
    int     getCurrentFloor() const { return currentFloor; }
    int     getDesiredFloor() const { return desiredFloor; }
    bool    getOnElevator() const   { return onElevator; }
    
    bool    isAtDesiredFloor() const { return onElevator == false && currentFloor == desiredFloor; }
    
    friend std::ostream& operator<<(std::ostream& out, const Passenger& passenger);
    
private:
    
    std::string name;
    
    bool    onElevator = false;
    int     currentFloor = 0;
    int     desiredFloor = 0;
};


#endif /* passenger_h */
