#ifndef SLIDING_PUZZLE_MODEL_UTILS_HPP_
#define SLIDING_PUZZLE_MODEL_UTILS_HPP_

#include <vector>

/*
* Swap(a, b) --
*
* Utility function to swap 2 unsigned integers.
*/
void Swap(int* a, int* b);

/*
* RandomPermutation(n) --
* 
* Generate a random permutation of 0 to N-1 using the Fisher-Yates shuffle algorithm.
* Time complexity: O(n).
*/
int* RandomPermutation(int n);

void PrintIntArray(int* arr, size_t n);

template <typename T>
void PrintVector(std::vector<T> v)
{
	for (std::vector<T>::iterator it = v.begin(); it != v.end(); ++it) {
		printf("%s\n", (**it).ToString().c_str());
	}
	printf("\n");
};

template <typename T>
void DeleteObjectsVector(std::vector<T*> v)
{
	for (std::vector<T*>::iterator it = v.begin(); it != v.end(); ++it) {
		delete *it;
	}
}

void DeleteIntArray(int** arr, size_t n);
#endif // SLIDING_PUZZLE_MODEL_UTILS_HPP_