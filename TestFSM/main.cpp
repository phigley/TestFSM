//
//  main.cpp
//  TestFSM
//
//  Created by Peter Higley on 8/3/17.
//  Copyright © 2017 Peter Higley. All rights reserved.
//

#include "simple_elevator.hpp"
#include "passenger.hpp"

#include <iostream>
#include <vector>

// TODO :
//   Have people who want floors
//   load/unload people
//   Have "Fire" state

// Use BT instead of FSM?


int main(int , const char * []) {
    
	Simple::Elevator simple_elevator = {};
    
    Passenger frank{ "Frank" };
    simple_elevator.addPassenger( frank );

    Passenger julie{ "Julie" };
    simple_elevator.addPassenger( julie );
    
    Passenger phil{ "Phil" };
    simple_elevator.addPassenger( phil );

    frank.requestFloor(3);
    julie.requestFloor(-1);
    phil.requestFloor(5);
    
    std::cout << simple_elevator;
    
    for( int i = 0; i < 10; ++i ) {
        simple_elevator.step();
        std::cout << simple_elevator;
    }

    Passenger  heather{ "Heather" };
    simple_elevator.addPassenger( heather );
    
    heather.requestFloor(6);
    frank.requestFloor(-1);
    
    while( !simple_elevator.allPassengersAtDesiredFloor() ) {
        simple_elevator.step();
        std::cout << simple_elevator;
    }

    frank.requestFloor(0);
    julie.requestFloor(0);
    phil.requestFloor(0);
    heather.requestFloor(0);
    
    while( !simple_elevator.allPassengersAtDesiredFloor() ) {
        simple_elevator.step();
        std::cout << simple_elevator;
    }
    
    return 0;
}
