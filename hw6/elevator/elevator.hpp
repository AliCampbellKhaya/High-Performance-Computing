// multiple rider case attempt
/*
Passenger List: elevator maintains list of passengers (pairs of person_id and destination floor)
Batch Processing: elevator picks up as many passengers as possible from the queue if they are on the same floor, respecting the maximum occupancy
Route Management: elevator manages passengers' routes, moving to each passenger's destination floor and logging the necessary information
Complete Logging: detailed logs for each step including entering and exiting the elevator are added to provide comprehensive traceability
also
Occupancy Declaration: occupancy variable initialized to 0 at the start of the elevator function
Occupancy Management: increment occupancy each time a passenger enters the elevator and decrement each time a passenger exits.
*/

#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <random>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <tuple>
#include <unordered_map>

using namespace std;

const int NUM_FLOORS = 50;
const int NUM_ELEVATORS = 6;
const int MAX_OCCUPANCY = 5;
const int MAX_WAIT_TIME = 5000; // milliseconds

mutex cout_mtx;
mutex queue_mtx;
condition_variable cv;
queue<tuple<int, int, int>> global_queue; // person_id, start_floor, dest_floor
vector<int> elevator_positions(NUM_ELEVATORS, 0);
atomic<int> num_people_serviced(0);
vector<int> global_passengers_serviced(NUM_ELEVATORS, 0);
int npeople;

void elevator(int id) {

    int occupancy = 0; // Initialize occupancy for each elevator
    vector<pair<int, int>> passengers; // pairs of (person_id, dest_floor)

    while (true) {

        {
            unique_lock<mutex> lock(queue_mtx);
            cv.wait(lock, [] { 
                return !global_queue.empty() || num_people_serviced.load() >= npeople;
            });

            if (global_queue.empty() && num_people_serviced.load() >= npeople) {
                break;
            }

            tuple<int, int, int> queue_item = global_queue.front();
            global_queue.pop();

            int p_id = get<0>(queue_item);
            int p_curr_floor = get<1>(queue_item);
            int p_dest_floor = get<2>(queue_item);

            if (elevator_positions[id] != p_curr_floor) {
                {
                lock_guard<mutex> lock(cout_mtx);
                cout << "Elevator " << id << " moving from floor " << elevator_positions[id] << " to floor " << p_curr_floor << endl;
                }

                this_thread::sleep_for(chrono::milliseconds(abs(elevator_positions[id] - p_curr_floor) * 50));
                elevator_positions[id] = p_curr_floor;
            }

            passengers.emplace_back(make_pair(p_id, p_dest_floor));
            occupancy++;

            {
                lock_guard lock(cout_mtx);
                cout << "Person " << p_id << " entered elevator " << id << endl;
            }

            queue<tuple<int, int, int>> temp_queue;

            // Handle multiple riders
            while (!global_queue.empty() && occupancy < MAX_OCCUPANCY) {
                queue_item = global_queue.front();
                global_queue.pop();

                p_id = get<0>(queue_item);
                p_curr_floor = get<1>(queue_item);
                p_dest_floor = get<2>(queue_item);

                if (p_curr_floor == elevator_positions[id]) {
                    passengers.emplace_back(make_pair(p_id, p_dest_floor));
                    occupancy++;
                    num_people_serviced++;
                    global_passengers_serviced[id]++;

                    {
                        lock_guard lock(cout_mtx);
                        cout << "Person " << p_id << " entered elevator " << id << endl;
                    }
                }
                // moves all items from the global queue to the temp queue while maintaing ordering
                else {
                    temp_queue.push(queue_item);
                }
            }

            // swap global queue (which is now empty) with temp queue
            while (!global_queue.empty()) {
                temp_queue.push(global_queue.front());
                global_queue.pop();
            }
            global_queue.pop();

            cv.notify_all();
        }

        while (!passengers.empty()) {
            int closest_floor;
            int min_distance = abs(elevator_positions[id] - passengers[0].second);

            for (pair<int, int>& passenger : passengers) {
                int distance_to_floor = abs(elevator_positions[id] - passenger.second);
                if (distance_to_floor < min_distance) {
                    min_distance = distance_to_floor;
                    closest_floor = passenger.second;
                }
            }

            if (elevator_positions[id] != closest_floor) {
                {
                    lock_guard lock(cout_mtx);
                    cout << "Elevator " << id << " moving from floor" << elevator_positions[id] << " to floor " << closest_floor << endl;
                }
                this_thread::sleep_for(chrono::milliseconds(abs(elevator_positions[id] - closest_floor) * 50));
                elevator_positions[id] = closest_floor;
            }

            auto passenger_dropoff = passengers.begin();
            while (passenger_dropoff != passengers.end()) {
                if (passenger_dropoff->second == elevator_positions[id]) {
                    {
                        lock_guard lock(cout_mtx);
                        cout << "Person " << passenger_dropoff->first << "arrived at floor " << elevator_positions[id] << endl;
                    }
                    occupancy--;
                    num_people_serviced++;
                    global_passengers_serviced[id]++;
                    passenger_dropoff = passengers.erase(passenger_dropoff);
                } else {
                    passenger_dropoff++;
                }
            }
        }
    }

    {
        lock_guard<mutex> lock(cout_mtx);
        cout << "Elevator " << id << " has finished servicing all people." << endl;
        cout << "Elevator " << id << " serviced " << global_passengers_serviced[id] << " passengers." << endl;
    }
}

void person(int id) {
    int curr_floor = rand() % NUM_FLOORS;
    int dest_floor = rand() % NUM_FLOORS;
    while (dest_floor == curr_floor) {
        dest_floor = rand() % NUM_FLOORS;
    }

    {
        lock_guard<mutex> lock(cout_mtx);
        cout << "Person " << id << " wants to go from floor " << curr_floor << " to floor " << dest_floor << endl;
    }

    {
        lock_guard<mutex> lock(queue_mtx);
        global_queue.push(make_tuple(id, curr_floor, dest_floor));
    }
}

#endif // ELEVATOR_HPP

