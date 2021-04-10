#include <sstream>
#include <time.h>
#include <stdlib.h>

#include "board.h"
#include "utils.h"

Board::Board(unsigned int n)
	: n_(n)
	, blank_(nullptr)
{
	board_ = new Cell* [n_];
	
	// Temporary solution: Generate a random permutation of an array of size n_ * n_
	// 	containing numbers from 0 to (n_ * n_ - 1) and copy them to the board_.
	// 	   A better solution would be extending a random permutation of an 2d array of
	// 	   dimension n_ x n_. But this should do for now.
	// Masked off with 64 - bits to avoid overflow.
	unsigned int* random_array = RandomPermutation(n_ * n_ & 0xFFFFFFFF);

	// PrintIntArray(random_array, (size_t) (n_ * n_ & 0xFFFFFFFF));

	for (unsigned int i = 0; i < n_; i++) {
		board_[i] = new Cell[n_];
		for (unsigned int j = 0; j < n_; j++) {
			board_[i][j] = Cell(random_array[i * n_ + j], i, j);
			if (random_array[i * n_ + j] == 0) blank_ = &board_[i][j];
		}
	}
	
	delete[] random_array;
}

Board::Board(const Board& b)
	: n_(b.n_)
	, blank_(nullptr)
{
	board_ = new Cell * [n_];

	// Perform a deep copy of the board.
	for (unsigned int i = 0; i < n_; i++) {
		board_[i] = new Cell[n_];
		for (unsigned int j = 0; j < n_; j++) {
			board_[i][j] = b.board_[i][j];
			if (board_[i][j].GetValue() == 0) blank_ = &board_[i][j];
		}
	}
}

Board& Board::operator=(const Board& b)
{
	for (unsigned int i = 0; i < n_; i++)
		delete[] board_[i];
	
	delete[] board_;

	n_ = b.n_;
	board_ = new Cell * [n_];

	for (unsigned int i = 0; i < n_; i++) {
		board_[i] = new Cell[n_];
		for (unsigned int j = 0; j < n_; j++) {
			board_[i][j] = b.board_[i][j];
			if (board_[i][j].GetValue() == 0) blank_ = &board_[i][j];
		}
	}
	
	return *this;
}

Board::~Board()
{
	for (unsigned int i = 0; i < n_; i++)
		delete[] board_[i];

	delete[] board_;
}

void Board::GetPossibleMoves()
{

}

std::string Board::ToString()
{
	std::string result("");
	
	for (unsigned int i = 0; i < n_; i++) {
		for (unsigned int j = 0; j < n_; j++) {
			result.append(board_[i][j].ToString());
			result.append("\t");
		}
		result.append("\n");
	}

	return result;
}