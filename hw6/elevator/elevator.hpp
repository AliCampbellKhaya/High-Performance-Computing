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
        vector<tuple<int, int, int>> passenger_batch;

        {
            unique_lock<mutex> lock(queue_mtx);

            cv.wait(lock, [] { 
                return !global_queue.empty() || num_people_serviced.load() >= npeople;
            });

            if (global_queue.empty() && num_people_serviced.load() >= npeople) {
                break;
            }

            // if (num_people_serviced.load() >= npeople) {
            //     break;
            // }

            if (global_queue.empty()) {
                continue;
            }

            tuple<int, int, int> queue_item = global_queue.front();
            global_queue.pop();

            int p_id = get<0>(queue_item);
            int p_curr_floor = get<1>(queue_item);
            int p_dest_floor = get<2>(queue_item);

            passenger_batch.push_back(make_tuple(p_id, p_curr_floor, p_dest_floor));

            queue<tuple<int, int, int>> skipped_passengers;
            while (passenger_batch.size() < MAX_OCCUPANCY && !global_queue.empty()) {
                tuple<int, int, int> next_queue_item = global_queue.front();
                int next_p_id = get<0>(next_queue_item);
                int next_p_curr_floor = get<1>(next_queue_item);
                int next_p_dest_floor = get<2>(next_queue_item);

                if(next_p_curr_floor == p_curr_floor) {
                    global_queue.pop();
                    passenger_batch.push_back(make_tuple(next_p_id, next_p_curr_floor, next_p_dest_floor));
                }
                else {
                    global_queue.pop();
                    skipped_passengers.push(make_tuple(next_p_id, next_p_curr_floor, next_p_dest_floor));
                }
            }

            while (!skipped_passengers.empty()) {
                global_queue.push(skipped_passengers.front());
                skipped_passengers.pop();
            }

        }

        int start_floor = get<1>(passenger_batch[0]);

        if (elevator_positions[id] != start_floor) {
            {
                lock_guard<mutex> lock(cout_mtx);
                cout << "Elevator " << id << " moving from floor " << elevator_positions[id] << " to floor " << start_floor << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(abs(elevator_positions[id] - start_floor) * 50));
            elevator_positions[id] = start_floor;
            // {
            //     lock_guard<mutex> lock(queue_mtx);
            //     elevator_positions[id] = start_floor;
            // }
        }

        for (tuple<int, int, int>& passenger : passenger_batch) {
            {
                lock_guard<mutex> lock(cout_mtx);
                cout << "Person " << get<0>(passenger) << " entered elevator " << id << endl;
            }
            passengers.push_back(make_pair(get<0>(passenger), get<2>(passenger)));
            occupancy++;
            global_passengers_serviced[id]++;
        }

        vector<int> destinations;
        for (pair<int, int> passenger : passengers) {
            bool already_seen = false;
            for (int dest : destinations) {
                if (dest == passenger.second) {
                    already_seen = true; 
                    break;
                }
            }
            if (!already_seen) {
                destinations.push_back(passenger.second);
            }
        }

        for (int dest : destinations) {
            {
                lock_guard<mutex> locak(cout_mtx);
                cout << "Elevator " << id << " moving from floor " << elevator_positions[id] << " to floor " << dest << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(abs(elevator_positions[id] - dest) * 50));
            elevator_positions[id] = dest;
            // {
            //     lock_guard<mutex> lock(queue_mtx);
            //     elevator_positions[id] = dest;
            // }
            

            vector<pair<int, int>> remaining_passengers;
            for (pair<int, int> passenger : passengers) {
                if (passenger.second == dest) {
                    {
                        lock_guard<mutex> lock(cout_mtx);
                        cout << "Person " << passenger.first << " arrived at floor " << dest << endl;
                    }
                    occupancy--;
                    num_people_serviced++;
                    cv.notify_all();
                }
                else {
                    remaining_passengers.push_back(passenger);
                }
            }
            passengers = remaining_passengers;
        }

    }

    {
        lock_guard<mutex> lock(cout_mtx);
        cout << "Elevator " << id << " has finished servicing all people." << endl;
        cout << "Elevator " << id << " serviced " << global_passengers_serviced[id] << " passengers." << endl;
    }
}

void person(int id) {
    /* since I had a bug of getting curr_floor = 41
    and dest_floor = 17 every time */
    srand(time(nullptr) ^ (id * 2654435761u));

    int curr_floor = rand() % NUM_FLOORS;
    int dest_floor = rand() % NUM_FLOORS;
    while (dest_floor == curr_floor) {
        dest_floor = rand() % NUM_FLOORS;
    }

    // DEBUGGING
    // int r1 = rand();
    // int r2 = rand();
    // int curr_floor = r1 % NUM_FLOORS;
    // int dest_floor = r2 % NUM_FLOORS;

    // {
    //     lock_guard<mutex> lock(cout_mtx);
    //     cout << "DEBUG RANDOM " << id << " r1: " << r1 << " r2: " << r2 << " curr floor " << curr_floor << " dest floor " << dest_floor << endl;
    // }

    {
        lock_guard<mutex> lock(cout_mtx);
        cout << "Person " << id << " wants to go from floor " << curr_floor << " to floor " << dest_floor << endl;
    }

    {
        lock_guard<mutex> lock(queue_mtx);
        global_queue.push(make_tuple(id, curr_floor, dest_floor));
    }

    //cv.notify_all();
    cv.notify_one();
}

#endif // ELEVATOR_HPP

