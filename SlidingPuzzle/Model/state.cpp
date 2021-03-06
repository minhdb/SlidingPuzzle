#include "utils.hpp"
#include "state.h"

State::State(int n, bool solved)
	: n_(n)
	, recent_move_cost_(0)
	, total_move_cost_(0)
	, board_(n, vector<int>(n, 0))
	, solution_(n, vector<int>(n, 0))
{
	string state_id_str("");

	// Temporary solution: Generate a random permutation of an array of size n_ * n_
	// 	containing numbers from 0 to (n_ * n_ - 1) and copy them to the board_.
	// 	   A better solution would be extending a random permutation of an 2d array of
	// 	   dimension n_ x n_. But this should do for now.
	// Masked off with 64 - bits to avoid overflow.
	GenerateSolutionState();
	if (solved) {
		for (int i = 0; i < n_; i++) {
			for (int j = 0; j < n_; j++) {
				board_[i][j] = solution_[i][j];
				if (board_[i][j] == 0) blank_ = Point(i, j);
			}
		}
	}
	else {
		int* random_array = RandomPermutation(n_ * n_);

		// PrintIntArray(random_array, (size_t) (n_ * n_ & 0xFFFFFFFF));

		for (int i = 0; i < n_; i++) {
			for (int j = 0; j < n_; j++) {
				board_[i][j] = random_array[i * n_ + j];
				if (random_array[i * n_ + j] == 0) blank_ = Point(i, j);
			}
		}

		delete[] random_array;
	}


	// Set the state ID.
	for (int i = 0; i < n_; i++) {
		for (int j = 0; j < n_; j++) {
			state_id_str.append(std::to_string(this->board_[i][j]));
		}
	}

	this->state_id_ = state_id_str;
}

State::State(int** b, int n)
	: n_(n)
	, recent_move_cost_(0)
	, total_move_cost_(0)
	, board_(n, vector<int>(n, 0))
	, solution_(n, vector<int>(n, 0))
{
	string state_id_str("");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			board_[i][j] = b[i][j];
			state_id_str.append(std::to_string(this->board_[i][j]));
		}
	}
	this->state_id_ = state_id_str;
	GenerateSolutionState();
}

State::State(State& s, const Move& m) 
	: n_(s.n_)
	, recent_move_cost_(0)
	, total_move_cost_(0)
	, board_(s.n_, vector<int>(s.n_, 0))
	, solution_(s.n_, vector<int>(s.n_, 0))
{
	string state_id_str("");
	for (int i = 0; i < n_; i++) {
		for (int j = 0; j < n_; j++) {
			this->board_[i][j] = s.board_[i][j];
			if (this->board_[i][j] == 0) blank_ = Point(i, j);
		}
	}

	// Copy the previous moves onto this state's move to keep track of the path.
	for (vector<Move>::iterator it = s.moves_.begin();
		it != s.moves_.end(); ++it)
		this->moves_.push_back(*it);

	//this->cost_ = this->MoveBlank(m);
	this->MoveBlank(m);
	this->recent_move_cost_ = m.GetCost();
	this->total_move_cost_ = s.total_move_cost_ + this->recent_move_cost_;

	// Set the state ID.
	for (int i = 0; i < n_; i++) {
		for (int j = 0; j < n_; j++) {
			state_id_str.append(std::to_string(this->board_[i][j]));
		}
	}

	this->state_id_ = state_id_str;
	this->moves_.push_back(m);
	GenerateSolutionState();
}

State& State::operator=(State& s)
{
	this->n_ = s.n_;
	
	for (int i = 0; i < n_; i++) {
		for (int j = 0; j < n_; j++) { 
			board_[i][j] = s.board_[i][j];
			if (this->board_[i][j] == 0) blank_ = Point(i, j);
		}
	}

	moves_.clear();
	
	for (vector<Move>::iterator it = s.moves_.begin();
		it != s.moves_.end(); ++it) {
		moves_.push_back(*it);
	}

	this->state_id_ = s.state_id_;
	this->recent_move_cost_ = s.recent_move_cost_;
	this->total_move_cost_ = s.total_move_cost_;

	GenerateSolutionState();
	return *this;
}

State::State(const State& b)
	: n_(b.n_)
	, recent_move_cost_(0)
	, total_move_cost_(0)
	, board_(b.n_, vector<int>(b.n_, 0))
	, solution_(b.n_, vector<int>(b.n_, 0))
{
	string state_id_str("");
	
	for (int i = 0; i < n_; i++) {
		for (int j = 0; j < n_; j++) {
			this->board_[i][j] = b.board_[i][j];
			if (this->board_[i][j] == 0) blank_ = Point(i, j);
			state_id_str.append(std::to_string(this->board_[i][j]));
		}
	}
	this->recent_move_cost_ = b.recent_move_cost_;
	this->total_move_cost_ = b.total_move_cost_;
	this->state_id_ = state_id_str;
	GenerateSolutionState();
}

State::~State()
{

}


std::vector<shared_ptr<State>> State::GetPossibleStates()
{
	std::vector<shared_ptr<State>> possible_moves;

	int blank_x = blank_.x_;
	int blank_y = blank_.y_;

	if (blank_x > 0)
		possible_moves.emplace_back(
			make_shared<State>(*this, Move(blank_x, blank_y, blank_x - 1, blank_y, 
				board_[blank_x-1][blank_y], "UP")));

	if (blank_x < n_ - 1)
		possible_moves.emplace_back(make_shared<State>(*this, Move(blank_x, blank_y,
			blank_x + 1, blank_y, board_[blank_x + 1][blank_y], "DOWN")));

	if (blank_y > 0)
		possible_moves.emplace_back(make_shared<State>(*this, Move(blank_x, blank_y,
			blank_x, blank_y - 1, board_[blank_x][blank_y-1], "LEFT")));

	if (blank_y < n_ - 1)
		possible_moves.emplace_back(make_shared<State>(*this, Move(blank_x, blank_y,
			blank_x, blank_y + 1, board_[blank_x][blank_y+1], "RIGHT")));

	return possible_moves;
}


std::string State::CurrentStateToString()
{
	std::string result("");

	for (int i = 0; i < n_; i++) {
		for (int j = 0; j < n_; j++) {
			result.append(std::to_string(board_[i][j]));
			result.append("\t");
		}
		result.append("\n");
	}
	return result;
}

bool State::IsGoalState()
{
	if (solution_.empty()) GenerateSolutionState();
	for (int i = 0; i < n_; i++) {
		for (int j = 0; j < n_; j++) {
			if (board_[i][j] != solution_[i][j])
				return false;
		}
	}
	return true;
}

bool State::operator==(const State& s) const
{
	return (this->state_id_ == s.state_id_);
}

bool State::operator!=(const State& s) const
{
	return (this->state_id_ != s.state_id_);
}

bool State::operator>(const State& s) const
{
	return this->total_move_cost_ > s.total_move_cost_;
}

bool State::operator<(const State& s) const
{
	return this->total_move_cost_ < s.total_move_cost_;
}

int State::MoveBlank(const Move& m)
{
	int start_x = m.GetStartPoint().x_;
	int start_y = m.GetStartPoint().y_;

	int end_x = m.GetEndPoint().x_;
	int end_y = m.GetEndPoint().y_;

	board_[start_x][start_y] = board_[end_x][end_y];
	int cost = board_[end_x][end_y];
	board_[end_x][end_y] = 0;
	blank_.x_ = end_x;
	blank_.y_ = end_y;

	return cost;
}

void State::ReverseMove(const Move& m)
{
	int start_x = m.GetStartPoint().x_;
	int start_y = m.GetStartPoint().y_;

	int end_x = m.GetEndPoint().x_;
	int end_y = m.GetEndPoint().y_;

	board_[end_x][end_y] = board_[start_x][start_y];
	board_[start_x][start_y] = 0;
	blank_.x_ = start_x;
	blank_.y_ = start_y;
}

unsigned long long State::GetTotalCostToThisState()
{
	return this->total_move_cost_;
}

void State::GenerateSolutionState()
{
	int totalSize = n_ * n_;
	int top = 0;
	int bottom = n_ - 1;
	int left = 0;
	int right = n_ - 1;
	int index = 0;

	// Initialize
	int* arr = new int[totalSize];
	for (int i = 0; i < totalSize; i++)
		arr[i] = i + 1;
	arr[totalSize - 1] = 0;

	// Initialize solution board if it's not already initialized.
	for (int i = 0; i < n_; i++) {
		for (int j = 0; j < n_; j++) {
			solution_[i][j] = 0;
		}
	}

	while (true) {
		if (left > right) break;

		// Fill out the top row.
		for (int i = left; i <= right; i++) {
			solution_[top][i] = arr[index];
			coordinates_map_.emplace(std::make_pair(solution_[top][i], Point(top, i)));
			index++;
		}
		top++;

		if (top > bottom) break;

		// Fil out the right column.
		for (int i = top; i <= bottom; i++) {
			solution_[i][right] = arr[index];
			coordinates_map_.emplace(std::make_pair(solution_[i][right], Point(i, right)));
			index++;
		}
		right--;

		if (left > right) break;

		// Fill out the bottom row.
		for (int i = right; i >= left; i--) {
			solution_[bottom][i] = arr[index];
			coordinates_map_.emplace(std::make_pair(solution_[bottom][i], Point(bottom, i)));
			index++;
		}
		bottom--;
		if (top > bottom) break;

		// Fill out the left column.
		for (int i = bottom; i >= top; i--) {
			solution_[i][left] = arr[index++];
		}
		left++;

	}

	delete[] arr;
}

int State::GetNumberOfMisplacedTiles()
{
	int num_misplaced_tiles = 0;
	for (int i = 0; i < n_; i++) {
		for (int j = 0; j < n_; j++) {
			if (board_[i][j] != solution_[i][j])
				num_misplaced_tiles++;
		}
	}

	return num_misplaced_tiles;
}

unsigned long long State::SumOfManhattanDistances()
{
	double sumMdist = 0;
	for (int i = 0; i < n_; i++) {
		for (int j = 0; j < n_; j++) {
			sumMdist += coordinates_map_[board_[i][j]].GetManhattanDistance(Point(i, j));
		}
	}
	return sumMdist;
}