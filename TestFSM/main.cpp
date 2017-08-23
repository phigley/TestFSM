//
//  main.cpp
//  TestFSM
//
//  Created by Peter Higley on 8/3/17.
//  Copyright © 2017 Peter Higley. All rights reserved.
//

#include "elevator.hpp"
#include "passenger.hpp"

#include <iostream>
#include <vector>

// TODO :
//   Have people who want floors
//   load/unload people
//   Have "Fire" state

// Use BT instead of FSM?


int main(int argc, const char * argv[]) {
    
    Elevator elevator = {};
    
    Passenger frank{ "Frank" };
    elevator.addPassenger( frank );

    Passenger julie{ "Julie" };
    elevator.addPassenger( julie );
    
    Passenger phil{ "Phil" };
    elevator.addPassenger( phil );

    frank.requestFloor(3);
    julie.requestFloor(-1);
    phil.requestFloor(5);
    
    std::cout << elevator;
    
    for( int i = 0; i < 10; ++i ) {
        elevator.step();
        std::cout << elevator;
    }

    Passenger  heather{ "Heather" };
    elevator.addPassenger( heather );
    
    heather.requestFloor(6);
    frank.requestFloor(-1);
    
    while( !elevator.allPassengersAtDesiredFloor() ) {
        elevator.step();
        std::cout << elevator;
    }

    frank.requestFloor(0);
    julie.requestFloor(0);
    phil.requestFloor(0);
    heather.requestFloor(0);
    
    while( !elevator.allPassengersAtDesiredFloor() ) {
        elevator.step();
        std::cout << elevator;
    }
    
    return 0;
}
