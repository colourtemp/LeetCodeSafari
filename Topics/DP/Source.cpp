#include <vector>
#include <numeric>      // std::partial_sum
#include <algorithm>	//std::max
using namespace std;
/////////////////////////////////////////////////////////////////////////////////
//303. Range Sum Query - Immutable
class NumArray {
public:
	NumArray(vector<int> nums) {
		accu.push_back(0);
		for (int i = 0; i<(int)nums.size(); i++) {
			accu.push_back(accu.back() + nums[i]);
		}
	}

	int sumRange(int i, int j) {
		return accu[j + 1] - accu[i];
	}
private:
	vector<int> accu;
};

//OutputIterator partial_sum (InputIterator first, InputIterator last,
//                               OutputIterator result)
class NumArray1 {
public:
	NumArray1(vector<int> &nums) : psum(nums.size() + 1, 0) {
		partial_sum(nums.begin(), nums.end(), psum.begin() + 1);
	}

	int sumRange(int i, int j) {
		return psum[j + 1] - psum[i];
	}
private:
	vector<int> psum;
};

/////////////////////////////////////////////////////////////////////////////////
//198. House Robber
int rob(vector<int>& nums) {
	vector<int> best(nums.size(), 0);//best result until each store
	if (nums.size() == 0)
		return 0;
	else if (nums.size() == 1)
		return nums[0];
	else if (nums.size() == 2)
		return max(nums[0], nums[1]);
	best[0] = nums[0];
	best[1] = max(nums[0], nums[1]);
	//For each store,
	//if robbing, d[i] = d[i-2] + nums[i].
	//if not robbing, d[i] = d[i-1];
	for (size_t i = 2; i<nums.size(); i++) {
		best[i] = max(best[i - 2] + nums[i], best[i - 1]);
	}
	return best[nums.size() - 1];
}


/////////////////////////////////////////////////////////////////////////////////
//70. Climbing Stairs
//looking at the situation in reverse order
int climbStairs(int n) {
	// base cases
	if (n <= 0) return 0;
	if (n == 1) return 1;
	if (n == 2) return 2;

	int one_step_before = 2;
	int two_steps_before = 1;
	int all_ways = 0;

	for (int i = 2; i<n; i++) {
		all_ways = one_step_before + two_steps_before;
		two_steps_before = one_step_before;
		one_step_before = all_ways;
	}
	return all_ways;
}

//go direct to fibonacci number
//a tells you the number of ways to reach the current step, 
//and b tells you the number of ways to reach the next step. 
//So for the situation one step further up, the old b becomes the new a, and the new b is the old a+b, 
//since that new step can be reached by climbing 1 step from what b represented or 2 steps from what a represented.
int climbStairsN(int n) {
	int a = 1, b = 1;
	while (n--) {
		b = a+b;
		a = b - a;
	}
	return a;
}
/////////////////////////////////////////////////////////////////////////////////

void main()
{
	vector<int> nums = { -2, 0, 3, -5, 2, -1 };


	climbStairs(7);
	
}

/////////////////////////////////////////////////////////////////////////////////
//get the starting situation 
//and the relationship 
//between current result and previous adjacency results
/////////////////////////////////////////////////////////////////////////////////