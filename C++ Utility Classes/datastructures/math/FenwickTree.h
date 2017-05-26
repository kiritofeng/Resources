/*
 * FenwickTree.h
 *
 *  Created on: May 26, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_FENWICKTREE_H_
#define DATASTRUCTURES_MATH_FENWICKTREE_H_

using namespace std;

/**
 * Created by ricardodpsx@gmail.com on 4/01/15.
 * <p>
 * In {@code Fenwick Tree} structure We arrange the array in an smart way to perform efficient <em>range queries and updates</em>.
 * The key point is this: In a fenwick array, each position "responsible" for storing cumulative data of N previous positions (N could be 1)
 * For example:
 * array[40] stores: array[40] + array[39] ... + array[32] (8 positions)
 * array[32] stores: array[32] + array[31] ... + array[1]  (32 positions)
 * <p>
 * <strong>But, how do you know how much positions a given index is "responsible" for?</strong>
 * <p>
 * To know the number of items that a given array position 'ind' is responsible for
 * We should extract from 'ind' the portion up to the first significant one of the binary representation of 'ind'
 * for example, given ind == 40 (101000 in binary), according to Fenwick algorithm
 * what We want is to extract 1000(8 in decimal).
 * <p>
 * This means that array[40] has cumulative information of 8 array items.
 * But We still need to know the cumulative data bellow array[40 - 8 = 32]
 * 32 is  100000 in binnary, and the portion up to the least significant one is 32 itself!
 * So array[32] has information of 32 items, and We are done!
 * <p>
 * So cummulative data of array[1...40] = array[40] + array[32]
 * Because 40 has information of items from 40 to 32, and 32 has information of items from 32 to  1
 * <p>
 * Memory usage:  O(n)
 *
 * @author Ricardo Pacheco
 */
struct FenwickTree {
private:
	int* array;
	int length;

public:
	FenwickTree(int size) {
		this->length = size + 1;
		array = new int[length];
	}

    /**
     * Range Sum query from 1 to ind
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind index
     * @return sum
     */
	int rsq(int ind) {
		int sum = 0;
		for (int x = ind; x > 0; x -= (x & -x)) {
			sum += array[x];
		}
		return sum;
	}

    /**
     * Range Sum Query from a to b.
     * Search for the sum from array index from a to b
     * a and b are 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  a left index
     * @param  b right index
     * @return sum
     */
	int rsq(int a, int b) {
		return rsq(b) - rsq(a - 1);
	}

    /**
     * Update the array at ind and all the affected regions above ind.
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind   index
     * @param  value value
     */
	void update(int ind, int value) {
		for (int x = ind; x < length; x += (x & -x)) {
			array[x] += value;
		}
	}

	int size() {
		return length - 1;
	}
};

#endif /* DATASTRUCTURES_MATH_FENWICKTREE_H_ */
