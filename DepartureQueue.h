#ifndef DEPARTURE_Q_H_
#define DEPARTURE_Q_H_

#include <queue>
#include "Plane.h"

class DepartureQueue
{
private:
	std::queue<Plane *> the_queue;  // the queue of planes in the departure queue
	int departure_time;  // the time a plane has to wait when it reaches the front of the queue before it can take-off
	int total_wait;   // total accumulated wait time in the departure queue
	int num_served;   // number of planes served through the departure queue
public:
	DepartureQueue() : total_wait(0), num_served(0) {}

	void set_departure_time(int departure_time) {
		this->departure_time = departure_time;
	}

	int get_num_served() {
		return num_served;
	}

	int get_total_wait() {
		return total_wait;
	}

	void update(int clock) 
	{
		// are there planes waiting in the departure queue?
		if (!the_queue.empty()) {
            //Identify the plane at the fornt of the queue
			Plane *plane = the_queue.front();
            //If ready_takeoff_time hasn't been set for this plane, set it to clock
			if (plane->ready_takeoff_time == -1) { // new plane has arrived at the front
				// update the ready_takeoff_time attribute of the plane
				plane->ready_takeoff_time = clock; 
			}
            //If ready_takeoff_time has been set for this plane execute this code
            else {
				//Compute the time the plane has been waiting at the front
                //If the plane has been waiting longer than the amount of time it takes for a plane to take off, execute the following code
				if (clock - plane->ready_takeoff_time > departure_time) {
					// plane has waited long enough
					// Remove plane from departure queue
                    the_queue.pop();

					//Calculate the wait time in the departure queue (current time - when plane was ready to take off)
                    int departure_wait = clock - plane->ready_takeoff_time;

					//Update total_wait and num_served in the departure queue
                    total_wait += departure_wait;
                    num_served++;

					// take off!   goodbye plane
					delete plane;
				}
			}
		}
	}

	friend class ServiceQueue;
	
};

#endif
