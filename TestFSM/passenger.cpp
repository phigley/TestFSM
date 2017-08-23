//
//  passenger.cpp
//  TestFSM
//
//  Created by Peter Higley on 8/5/17.
//  Copyright © 2017 Peter Higley. All rights reserved.
//

#include "passenger.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& out, const Passenger& passenger)
{
    out << passenger.name;
    
    if( passenger.onElevator ) {
        out << " - on elevator";
    } else {
        out << " - on floor " << passenger.currentFloor;
    }
    
    if( !passenger.isAtDesiredFloor() ) {
        out << " wants floor " << passenger.desiredFloor;
    }
    
    return out;
}

