#ifndef SLIDING_PUZZLE_PUZZLE_TESTS_H_
#define SLIDING_PUZZLE_PUZZLE_TESTS_H_

#include <assert.h>

#include "board_tests.hpp"

namespace Test
{
	void RunAllTests(PuzzleController &c)
	{
		Test::RunTests(c);
	}
}

#endif SLIDING_PUZZLE_PUZZLE_TESTS_H_ // SLIDING_PUZZLE_PUZZLE_TESTS_H_