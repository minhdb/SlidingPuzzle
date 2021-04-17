/*
* Implementation of breadth first search alogrithm.
*
*/

#ifndef SLIDING_PUZZLE_MODEL_SEARCH_UNIFORM_COST_H_
#define SLIDING_PUZZLE_MODEL_SEARCH_UNIFORM_COST_H_

#include <queue>
#include <set>
#include <chrono>

#include "state.h"
#include "utils.hpp"


class StateComparator
{
public:
	bool operator() (State* s1, State* s2)
	{
		return *s1 > *s2;
	}
};

class Ucs : public ISearch
{
public:
	Ucs() = default;

	void Execute(State* b)
	{
		using sec = std::chrono::seconds;
		auto start = std::chrono::high_resolution_clock::now();

		std::priority_queue<State*, std::vector<State*>, StateComparator> states_queue;
		std::set<std::string> visited;

		// Mark the current state as visited.
		State* init_state = new State(*b);
		visited.insert(init_state->GetStateId());
		states_queue.push(init_state);
		this->queue_size_ = 1;
		
		std::vector<State*>* cur_possible_states = nullptr;
		State* cur_state = nullptr;
		State* cur_visited_state = nullptr;

		while (!states_queue.empty()) {
			//printf("Queue size: %lu\n", states_queue.size());
			State* front_state = states_queue.top();

			states_queue.pop();

			if (front_state->IsGoalState()) {
				this->solution_path_length_ = front_state->TotalMoves().size();
				this->solution_cost_ = front_state->GetTotalCostToThisState();
				printf("Total moves: %lu\n", this->solution_path_length_);
				printf("Solution cost: %d\n", this->solution_cost_);
				printf("%s\n", front_state->CurrentStateToString().c_str());
				break;
			}
			else {
				cur_possible_states = front_state->GetPossibleStates();

				for (std::vector<State*>::iterator it1 = cur_possible_states->begin();
					it1 != cur_possible_states->end(); ++it1) {

					cur_state = *it1;

					// Check if it's already visited. If it is, don't add it to the queue.
					auto visited_it = visited.find(cur_state->GetStateId());
					if (visited_it != visited.end()) {
					}
					else {
						visited.insert(cur_state->GetStateId());
						states_queue.push(cur_state);
						if (states_queue.size() > this->queue_size_) this->queue_size_ = states_queue.size();
					}
				}
			}
		}

		auto finish = std::chrono::high_resolution_clock::now();
		this->time_ = std::chrono::duration_cast<sec>(finish - start).count();

		// Clean up.
		//DeleteObjectsVector(*cur_possible_states);
		//DeleteObjectsVector(visited);
	}

private:
};

#endif // SLIDING_PUZZLE_MODEL_SEARCH_UNIFORM_COST_H_