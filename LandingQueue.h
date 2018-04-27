#ifndef LANDING_Q_H_
#define LANDING_Q_H_

#include <iostream>
#include <string>
#include <queue>
#include "Plane.h"
#include "Random.h"

using std:: queue;

extern Random my_random;  // Enables us to access the global variable declared in Simulator.h

class LandingQueue
{
private:
	double arrival_rate;            // plane arrival rate per minute
	std::queue<Plane *> the_queue;  // queue of planes in the landing queue
	int total_wait;  // total accumulated wait time in the landing queue
	int num_served;  // number of planes served through the landing queue

public:
	LandingQueue() : total_wait(0), num_served(0) {}

	void set_arrival_rate(double arrival_rate) {
		this->arrival_rate = arrival_rate;
	}

	int get_total_wait() {
		return total_wait;
	}

	int get_num_served() {
		return num_served;
	}

    void update(int clock)
	{
		//Add a new plane into the landing queue based on the arrival_rate
        //Generate a random double
        //If random double is less than or equal to the arrival rate of planes, create a new plane
        if (my_random.next_double() <= arrival_rate)
        {
            //Create a new Plane object with arrival_time set to clock and add it to the_queue
            the_queue.push(new Plane(clock));
        }

	}

	friend class ServiceQueue;
};

#endif
