#ifndef SERVICE_Q_H_
#define SERVICE_Q_H_

#include <queue>
#include "Plane.h"
#include "LandingQueue.h"
#include "DepartureQueue.h"
#include "Random.h"
#include "Simulator.h"

extern Random my_random; // Enables us to access the global variable declared in Simulator.h

/** The service queue takes a plane from the landing queue and adds it to its queue.
    When a plane in service queue has finished being serviced, it will be placed in the departure queue.
*/
class ServiceQueue
{
private:
	int min_service_time, max_service_time;  // range of service times
    int num_gates;
	LandingQueue *landing_queue;             // pointer to the landing queue
	DepartureQueue *departure_queue;         // pointer to the departure queue
	std::queue<Plane *> the_queue;           // queue of planes (just ONE) in the service queue
public:
	ServiceQueue() {}

    //Set the min and max service times
	void set_service_times(int min_service_time, int max_service_time) {
		this->min_service_time = min_service_time;
		this->max_service_time = max_service_time;
	}

    //Set the landing queue
	void set_landing_queue(LandingQueue *landing_queue) {
		this->landing_queue = landing_queue;
	}

    //Set the departure queue
	void set_departure_queue(DepartureQueue *departure_queue) {
		this->departure_queue = departure_queue;
	}
    
    //Set the number of gates
    void set_num_gates(int num_gates){
        this->num_gates = num_gates;
    }

    //This loop moves a plane to the departure queue if it has finished being service
    //Then, it takes a plane from the landing queue and adds it to the ServiceQueue queue
	void update(int clock) 
	{
		// Executes if there is a plane at the gate
		if (!the_queue.empty()) {
            
            //Identifies plane object at front of queue
			Plane *plane = the_queue.front();

			//Checks if a plane is ready to move from the service queue to the departure queue
			if ((clock - plane->start_service_time) > plane->service_time) {  
				//Removes plane from the service queue
                the_queue.pop();

				//Updates the enter_departure_time attribute for the plane based on clock
                int time = clock;
                plane->enter_departure_time = time;

                //Adds the plane to the departure queue
				departure_queue->the_queue.push(plane);
			}
		}

		//Executes if the gate is empty - ready to serve!
		if (the_queue.empty()) {

			// move a plane from the landing queue to the service queue
			if (!landing_queue->the_queue.empty()) {

                //Create a new plane object pointing to the first plane in the landing queue
				Plane *plane = landing_queue->the_queue.front();
                //Remove that plane object from the landing queue
				landing_queue->the_queue.pop();

				//Calculate the wait time of the plane in the landing queue
                //Current time
                int time = clock;
                //Time plane arrived in landing queue
                int arr_time = plane->arrival_time;
                //Time plane waited in the landing queue
                int wait_time_landing = time - arr_time;
				
				//Update total_wait and num_served for the landing queue
                //(this class is a friend class to LandingQueue)
                (landing_queue->total_wait)+= wait_time_landing;
                (landing_queue->num_served)++;

				//Update the start_service_time attribute for the plane
                int s_time = clock;
                plane->start_service_time = s_time;

				// Compute a random service time for the plane between min_service_time and max_service_time
        
                //Calculate the range of time between min and max service times
                int n = max_service_time - min_service_time;
                //my_random.next_int(n) generates a random value between 0 and n
                //This number is not within the correct range
                int serv_time = my_random.next_int(n);
                //Add the randomly generated int to the min service time to get a value in the correct range of values
                int service_time = serv_time + min_service_time;
                //Set the planes service time to the calculated service time
                plane->service_time = service_time;

				//Add the plane to the service queue
                the_queue.push(plane);
				
			}
        }
	}

};

#endif
