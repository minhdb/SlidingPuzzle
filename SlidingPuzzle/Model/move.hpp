/*
* Move class
*	Abstraction that captures how a cell in the puzzle can move.
*/

#ifndef SLIDING_PUZZLE_MODEL_MOVE_HPP_
#define SLIDING_PUZZLE_MODEL_MOVE_HPP_

#include <string>

#include "point.hpp"

using std::string;

enum class Direction
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Move
{
public:
	Move() = delete;
	Move(int x1, int y1, int x2, int y2, unsigned long cost, string d)
		: start_(Point(x1, y1))
		, end_(Point(x2, y2))
		, cost_(cost) 
		, direction_(d)
	{}
	
	Move(const Move& m)
		: start_(m.start_)
		, end_(m.end_)
		, cost_(m.cost_)
		, direction_(m.direction_)
	{}

	Move& operator=(const Move& m)
	{
		start_ = m.start_;
		end_ = m.end_;
		cost_ = m.cost_;
		direction_ = m.direction_;
		return *this;
	}

	~Move() = default;

	Point GetStartPoint() const { return this->start_; }
	Point GetEndPoint() const { return this->end_;  }
	unsigned long GetCost() const { return this->cost_; }
	void SetCost(unsigned long c) { this->cost_ = c; }
	string Direction() { return direction_; }

	std::string ToString()
	{
		// This is so bad. Temporary solution.
		string result("");
		result.append("(");
		result.append(std::to_string(start_.x_));
		result.append(", ");
		result.append(std::to_string(start_.y_));
		result.append(") to ");
		
		result.append("(");
		result.append(std::to_string(end_.x_));
		result.append(", ");
		result.append(std::to_string(end_.y_));
		result.append(")\n");

		return result;
	}

private:
	Point start_;
	Point end_;
	unsigned long long cost_;
	string direction_;
};

#endif // SLIDING_PUZZLE_MODEL_MOVE_HPP_