//
//  simple_elevator.hpp
//  TestFSM
//
//  Created by Peter Higley on 8/5/17.
//  Copyright © 2017 Peter Higley. All rights reserved.
//

#ifndef simple_elevator_hpp
#define simple_elevator_hpp

#include <vector>

class Passenger;

namespace Simple
{
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

}


#endif /* simple_elevator_hpp */
