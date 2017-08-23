//
//  elevator.cpp
//  TestFSM
//
//  Created by Peter Higley on 8/5/17.
//  Copyright © 2017 Peter Higley. All rights reserved.
//

#include "elevator.hpp"
#include "passenger.hpp"

#include <iostream>
#include <cassert>

namespace {
    
    Elevator::Direction calcDirection(int currentFloor, int desiredFloor) {
        if( currentFloor < desiredFloor ) {
            return Elevator::Direction::Up;
        } else if( currentFloor > desiredFloor ) {
            return Elevator::Direction::Down;
        }
        
        return Elevator::Direction::None;
    }

    bool calcShouldLoad( const Elevator::PassengerList& passengers, int currentFloor, int desiredFloor ) {
        
        const Elevator::Direction newDirection = calcDirection(currentFloor, desiredFloor);
        

        for( const auto* passengerPtr : passengers ) {
            
            if( passengerPtr->getOnElevator() ) {
                if( passengerPtr->getDesiredFloor() == currentFloor ) {
                    return true;
                }
            } else { // passneger not on elevator
                
                if( passengerPtr->getCurrentFloor() == currentFloor ) {
                    
                    const Elevator::Direction passengerDirection = calcDirection(currentFloor, passengerPtr->getDesiredFloor());
                    
                    if( newDirection ==  Elevator::Direction::None || newDirection == passengerDirection ) {
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
}

void Elevator::addPassenger(Passenger& passenger) {
    passengers.push_back( &passenger );
}

void Elevator::step() {
    
    switch (currentState ) {
        case State::Up :
        {
            ++currentFloor;
            
            const bool shouldLoad = calcShouldLoad(passengers, currentFloor, desiredFloor);
            
            if( shouldLoad ) {
                currentState = State::Loading;
            }
        }
        break;
            
        case State::Down :
        {
            --currentFloor;

            const bool shouldLoad = calcShouldLoad(passengers, currentFloor, desiredFloor);
            
            if( shouldLoad ) {
                currentState = State::Loading;
            }
        }
        break;
            
        case State::Loading :
        {
            Direction headingDirection = calcDirection(currentFloor, desiredFloor);

            for( auto* passengerPtr : passengers ) {
                if( passengerPtr->getOnElevator() ) {
                    if( passengerPtr->getDesiredFloor() == currentFloor ) {
                        passengerPtr->leaveElevator(currentFloor);
                    }
                }
            }
            
            if( headingDirection != Direction::None ) {
                // Load any passengers heading in our direction.
                
                for( auto* passengerPtr : passengers ) {
                    if( !passengerPtr->getOnElevator() && passengerPtr->getCurrentFloor() == currentFloor ) {
                        
                        const int passengerDesiredFloor = passengerPtr->getDesiredFloor();
                        const Direction passengerDirection = calcDirection( currentFloor, passengerDesiredFloor );
                        
                        if( passengerDirection == headingDirection ) {
                            passengerPtr->enterElevator();
                            
                            // Extend the desired floor the new passenger wants to go further in our direction.
                            if( headingDirection == Direction::Up ) {
                                if( passengerDesiredFloor > desiredFloor ) {
                                    desiredFloor = passengerDesiredFloor;
                                }
                            } else {
                                assert( headingDirection == Direction::Down );
                                if( passengerDesiredFloor < desiredFloor ) {
                                    desiredFloor = passengerDesiredFloor;
                                }
                            }
                            
                        }
                    }
                }
                
                if( headingDirection == Direction::Up ) {
                    currentState = State::Up;
                } else {
                    assert( headingDirection == Direction::Down );
                    currentState = State::Down;
                }
                
            } else {
                
                int upCount = 0;
                int bestUpDesired = currentFloor;
                
                int downCount = 0;
                int bestDownDesired = currentFloor;
                
                // Look for any passengers on the current floor that wish to go to a new direction.
                for( auto* passengerPtr : passengers ) {
                    
                    // There should be no one on the elevator at this point.
                    assert(!passengerPtr->getOnElevator());
                    
                    if( passengerPtr->getCurrentFloor() == currentFloor ) {

                        const int passengerDesiredFloor = passengerPtr->getDesiredFloor();
                        
                        if( passengerDesiredFloor > currentFloor ) {
                            ++upCount;
                            if( passengerDesiredFloor > bestUpDesired ) {
                                bestUpDesired = passengerDesiredFloor;
                            }
                        } else if( passengerDesiredFloor < currentFloor ) {
                            ++downCount;
                            if( passengerDesiredFloor < bestDownDesired ) {
                                bestDownDesired = passengerDesiredFloor;
                            }
                        }
                    }
                }
                
                // If we found passengers, set our desired floor and stay in the loading state.
                // Next update, we will be loading all passengers headed in that direction.
                if( upCount > downCount ) {
                    assert( bestUpDesired != currentFloor );
                    desiredFloor = bestUpDesired;
                } else if( upCount < downCount ) {
                    assert( bestDownDesired != currentFloor );
                    desiredFloor = bestDownDesired;
                } else {
                    
                    // We did not find any passengers to load, just idle on our current floor.
                    currentState = State::Idle;
                }
                
            }
        }
        break;
            
        case State::Idle :
        {
            int maxFloorDelta = 0;
            int bestFloor = currentFloor;
            
            bool foundPassengerOnCurrentFloor = false;
            
            for( const auto* passengerPtr : passengers ) {
                
                // There should be no passengers on the elevator while we are idle.
                assert(passengerPtr->getOnElevator() == false );

                const int passengerCurrentFloor = passengerPtr->getCurrentFloor();
                const int passengerDesiredFloor = passengerPtr->getDesiredFloor();
                if( passengerCurrentFloor != passengerDesiredFloor ) {
                
                    const int passengerFloorDelta = abs(passengerCurrentFloor - currentFloor);
                    
                    if( passengerFloorDelta == 0 ) {
                        // We found a request on our current floor, always load them immediately.
                        foundPassengerOnCurrentFloor = true;
                        break;
                    } else if( passengerFloorDelta > maxFloorDelta ) {
                        
                        // We always go to the furthest passenger so that they will not be ignored
                        // by constant smaller requests.
                        maxFloorDelta = passengerFloorDelta;
                        bestFloor = passengerCurrentFloor;
                    }
                }
            }
            
            if( foundPassengerOnCurrentFloor ) {
                currentState = State::Loading;
            } else if( maxFloorDelta > 0 ) {
                desiredFloor = bestFloor;
                
                if( desiredFloor > currentFloor ) {
                    currentState = State::Up;
                } else {
                    assert(desiredFloor < currentFloor);
                    currentState = State::Down;
                }
            }
        }
        break;
            
        default:
            assert(false);
            break;
    }
}

bool Elevator::allPassengersAtDesiredFloor() const {
    
    return std::all_of(passengers.begin(), passengers.end(), [](const Passenger* passengerPtr) {
        return passengerPtr->isAtDesiredFloor();
    });
    
}

const char* Elevator::describeState(State state) {
    
    const char* g_szState[] = {
        "Up",
        "Down",
        "Idle",
        "Loading"
    };
    
    static_assert( sizeof(g_szState) == std::size_t(State::MaxStates)*sizeof(g_szState[0]) , "Missing state description");
    
    return g_szState[ std::size_t( state ) ];
}

std::ostream& operator<<(std::ostream& out, const Elevator& elevator) {
    
    out << "Floor = "
    << elevator.currentFloor
    << " State = "
    << Elevator::describeState(elevator.currentState)
    << " desired = "
    << elevator.desiredFloor
    << " : " << std::endl;
    
    for( auto passengerPtr : elevator.passengers )
    {
        std::cout << "\t" << *passengerPtr << std::endl;
    }
    std::cout << "-----------------" << std::endl;
    
    return out;
}


