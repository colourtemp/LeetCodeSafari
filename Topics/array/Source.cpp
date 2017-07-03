#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <array>
#include <algorithm>
#include <map>

using namespace std;

//考虑input为空
void plusone(vector<int> &digits)
{
	int n = digits.size();
	for (int i = n - 1; i >= 0; --i)
	{
		if (digits[i] == 9)
		{
			digits[i] = 0;
		}
		else
		{
			digits[i]++;
			return;
		}
	}

	digits.push_back(0);
	digits[0] = 1;
}

//1		unordered_map、hash 适用于快速查找
vector<int> twoSum(vector<int> &numbers, int target)
{
	//Key is the number and value is its index in the vector.
	unordered_map<int, int> hash;
	vector<int> result;
	for (size_t i = 0; i < numbers.size(); i++) {
		int numberToFind = target - numbers[i];

		//if numberToFind is found in map, return them
		if (hash.find(numberToFind) != hash.end()) {
			result.push_back(hash[numberToFind]);
			result.push_back(i);
			return result;
		}

		//number was not found. Put it in the map.
		hash[numbers[i]] = i;
	}
	return result;
}

//set用于自动非重复排序  考虑边际情况
int thirdMax(vector<int>& nums) {
	set<int> tops;
	//sort
	for (size_t i = 0; i<nums.size(); i++) {
		tops.insert(nums[i]);
		if (tops.size()>3) {
			tops.erase(tops.begin());
		}
	}
	if (tops.size() == 3) {
		return *tops.begin();//end是 past-the-end 不能直接用
	}
	else {
		return *tops.rbegin();
	}
}

//利用负号起标记作用节省空间
vector<int> findDisappearedNumbers(vector<int>& nums) {
	vector<int> lost;
	for (size_t i = 0; i<nums.size(); i++) {
		int pos = abs(nums[i]) - 1;
		if (nums[pos]>0)
			nums[pos] = -nums[pos];
	}
	for (size_t i = 0; i<nums.size(); i++) {
		if (nums[i]>0)
			lost.push_back(i + 1);
	}
	return lost;
}


int findMaxConsecutiveOnes(vector<int>& nums) {
	int max = 0, cur = 0;
	for (size_t i = 0; i<nums.size(); i++)
	{
		if (nums[i] == 1)
		{
			if (++cur>max)
				max = cur;
		}
		else cur = 0;
	}
	return max;
}


void moveZeroes(vector<int>& nums) {
	size_t sorted = 0;
	//move non-zero one by one
	for (size_t i = 0; i<nums.size(); i++) {
		if (nums[i] != 0) {
			nums[sorted] = nums[i];
			sorted++;
		}
	}
	//fill the rest 0
	for (; sorted<nums.size(); sorted++) {
		nums[sorted] = 0;
	}
}


vector<int> twoSum1(vector<int>& numbers, int target) {
	int lo = 0, hi = numbers.size() - 1;
	while (numbers[lo] + numbers[hi] != target) {
		if (numbers[lo] + numbers[hi]<target) {
			lo++;
		}
		else {
			hi--;
		}
	}
	return vector<int>({ lo + 1,hi + 1 });
}

//26	
int removeDuplicates(vector<int>& nums) {
	int sorted = 0;
	for (size_t i = 0; i<nums.size(); i++) {
		if (nums[i]>nums[sorted]) {
			nums[++sorted] = nums[i];
		}
	}
	if (nums.size()<2)
		return nums.size();
	else
		return sorted + 1;
}

//int removeDuplicates(vector<int>& nums) {
//	int count = 0;
//	int n = nums.size();
//	for (int i = 1; i < n; i++) {
//		if (nums[i] == nums[i - 1]) count++;
//		else nums[i - count] = nums[i];
//	}
//	return n - count;
//}

//27	
int removeElement(vector<int>& nums, int val) {
	int size = nums.size();
	int i = 0;
	while (i<size) {
		if (nums[i] == val) {
			nums[i] = nums[size - 1];
			size--;
		}
		else
			i++;
	}
	return size;
}

//169	尽量利用限定条件简化算法
int majorityElement(vector<int>& nums) {
	int maj = nums[0];
	int count = 1;
	for (size_t i = 1; i<nums.size(); i++) {
		if (count == 0) {
			maj = nums[i];
			count++;
		}
		else if (maj == nums[i])
			count++;
		else
			count--;
	}
	return maj;
}

void rotate(vector<int>&nums, int n, int k)
{
	if ((n == 0) || (k <= 0))
	{
		return;
	}

	int cntRotated = 0;
	int start = 0;
	int curr = 0;
	int numToBeRotated = nums[0];
	int tmp = 0;
	// Keep rotating the elements until we have rotated n 
	// different elements.
	while (cntRotated < n)
	{
		do
		{
			tmp = nums[(curr + k) % n];
			nums[(curr + k) % n] = numToBeRotated;
			numToBeRotated = tmp;
			curr = (curr + k) % n;
			cntRotated++;
		} while (curr != start);
		// Stop rotating the elements when we finish one cycle, 
		// i.e., we return to start.

		// Move to next element to start a new cycle.
		start++;
		curr = start;
		numToBeRotated = nums[curr];
	}
}

//问题拆分 divide & conquer
void rotate2(int nums[], int n, int k)
{
	k = k%n;

	// Reverse the first n - k numbers.
	// Index i (0 <= i < n - k) becomes n - k - i.
	reverse(nums, nums + n - k);

	// Reverse tha last k numbers.
	// Index n - k + i (0 <= i < k) becomes n - i.
	reverse(nums + n - k, nums + n);

	// Reverse all the numbers.
	// Index i (0 <= i < n - k) becomes n - (n - k - i) = i + k.
	// Index n - k + i (0 <= i < k) becomes n - (n - i) = i.
	reverse(nums, nums + n);
}

//buck sort
int arrayPairSum(vector<int>& nums) {
	vector<int> hashtable(20001, 0);
	for (size_t i = 0; i<nums.size(); i++)
	{
		hashtable[nums[i] + 10000]++;
	}
	int ret = 0;
	bool flag = 0;//尽量用bool 加快代码速度
	for (size_t i = 0; i<20001;) {
		if ((hashtable[i]>0) && (flag == 0)) {
			ret = ret + i - 10000;
			flag = 1;
			hashtable[i]--;
		}
		else if ((hashtable[i]>0) && (flag == 1)) {
			hashtable[i]--;
			flag = 0;
		}
		else i++;
	}
	return ret;
}

//问题拆分的方法
int maxSubArray(vector<int>& A) {
	int ans = A[0], sum = 0;
	for (size_t i = 0; i<A.size(); i++) {
		sum += A[i];
		ans = max(sum, ans);
		sum = max(sum, 0);
	}
	return ans;
}

//考虑edge situation
int findUnsortedSubarray(vector<int>& nums) {
	int shortest = 0;

	size_t left = 0, right = nums.size() - 1;
	while (left < nums.size() - 1 && nums[left] <= nums[left + 1]) { left++; }
	while (right > 0 && nums[right] >= nums[right - 1]) { right--; };

	if (right > left) {
		int vmin = INT_MAX, vmax = INT_MIN;
		for (size_t i = left; i <= right; ++i) {
			if (nums[i] > vmax) {
				vmax = nums[i];
			}
			if (nums[i] < vmin) {
				vmin = nums[i];
			}
		}

		while (left >= 0 && nums[left] > vmin) { left--; };
		while (right < nums.size() && nums[right] < vmax) { right++; };

		shortest = right - left - 1;
	}

	return shortest;
}

//好好利用XOR翻转功能
int missingNumber(vector<int>& nums) {

	int result = 0;
	int i = 0;
	for (size_t j = 0; j<nums.size(); j++) {
		result ^= nums[j];
		result ^= i++;
	}
	result ^= nums.size();
	return result;
}

//利用数字+-*/%特征简化代码
vector<vector<int>> matrixReshape(vector<vector<int>>& nums, int r, int c) {
	int row = nums.size();
	int colum = nums[0].size();
	if (row*colum != r*c)
		return nums;
	vector<vector<int>> result;
	result.assign(r, vector < int >(c, 0));
	for (int i = 0; i<r*c; i++) {
		result[i / c][i%c] = nums[i / colum][i%colum];
	}
	return result;

}

//充分考虑边际情况  使之常态化
bool canPlaceFlowers(vector<int>& flowerbed, int n) {
	flowerbed.insert(flowerbed.begin(), 0);
	flowerbed.push_back(0);
	for (size_t i = 1; i < flowerbed.size() - 1; ++i)
	{
		if (flowerbed[i - 1] + flowerbed[i] + flowerbed[i + 1] == 0)
		{
			--n;
			++i;
		}

	}
	return n <= 0;
}

//219	带不带等号很关键
bool containsNearbyDuplicate(vector<int>& nums, int k) {
	unordered_map<int, int> hash;
	for (size_t i = 0; i<nums.size(); i++) {
		if (hash.find(nums[i]) != hash.end()) {
			if (i - hash[nums[i]] <= (size_t)k)
				return true;
		}
		hash[nums[i]] = i;
	}
	return false;
}

//217	利用set 或unordered_map
bool containsDuplicate(vector<int>& nums) {
	return nums.size() > set<int>(nums.begin(), nums.end()).size();
}

//624	set.end()是最后一个null的位置，最大数用.rbegin()
//逐步找到最小最大，边找边算
int maxDistance(vector<vector<int>>& arrays) {
	int left = arrays[0][0], right = arrays[0].back(), dif = 0;
	for (size_t i = 1; i < arrays.size(); i++)
	{
		dif = max(dif, max(abs(arrays[i].back() - left), abs(right - arrays[i][0])));
		left = min(left, arrays[i][0]);
		right = max(right, arrays[i].back());
	}
	return dif;
}

//121	避免直接取值vector[0],可能是空集
int maxProfit(vector<int> &prices) {
	int maxPro = 0;
	int minPrice = INT_MAX;
	for (size_t i = 0; i < prices.size(); i++) {
		minPrice = min(minPrice, prices[i]);
		maxPro = max(maxPro, prices[i] - minPrice);
	}
	return maxPro;
}

//122	for (int i = 1; i<prices.size(); i++)本身可避免空集问题
int maxProfit1(vector<int>& prices) {
	int Sum = 0;
	int Pro = 0;
	for (size_t i = 1; i<prices.size(); i++) {
		Pro = prices[i] - prices[i - 1];
		if (Pro>0) {
			Sum += Pro;
		}
	}
	return Sum;
}


//119
vector<int> getRow(int rowIndex) {
	vector<int> tri = { 1 };
	if (rowIndex<0)
		return tri;
	for (int i = 1; i <= rowIndex; i++) {
		tri.push_back(1);
		int temp = i - 1;
		while (temp >= 1) {
			tri[temp] = tri[temp] + tri[temp - 1];
			temp--;
		}
	}
	return tri;
}

//118	直接操纵矩阵没什么不好
vector<vector<int> > generate(int numRows) {
	vector<vector<int>> r(numRows);

	for (int i = 0; i < numRows; i++) {
		r[i].resize(i + 1);
		r[i][0] = r[i][i] = 1;

		for (int j = 1; j < i; j++)
			r[i][j] = r[i - 1][j - 1] + r[i - 1][j];
	}

	return r;
}

//88. Merge Sorted Array
//reverse the order
void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
	int M = m - 1, N = n - 1, X = m + n - 1;
	while (M >= 0 && N >= 0) {
		nums1[X--] = nums1[M]>nums2[N] ? nums1[M--] : nums2[N--];
	}
	if (N >= 0) {
		while (X >= 0)
			nums1[X--] = nums2[N--];
	}
}

//628. Maximum Product of Three Numbers
//Sorts the elements in the range [first,last) into ascending order.   N*log2(N)
int maximumProduct(vector<int>& nums) {
	sort(nums.begin(), nums.end());
	size_t n = nums.size();

	int one = nums[0] * nums[1] * nums[2];
	int another = nums[n - 3] * nums[n - 2] * nums[n - 1];

	int ret = max(one, another);

	return ret;
}

//532. K - diff Pairs in an Array
//check naughty input
int findPairs(vector<int>& nums, int k) {
	if (k<0)
		return 0;
	unordered_map<int, int> hash;
	set<int> total;
	for (size_t i = 0; i<nums.size(); i++) {
		int target = nums[i] - k;
		int target2 = nums[i] + k;
		if (hash.find(target) != hash.end()) {
			total.insert(target + nums[i]);
		}
		if (hash.find(target2) != hash.end()) {
			total.insert(target2 + nums[i]);
		}
		hash[nums[i]] = 1;
	}
	return total.size();
}

//532. 
int findPairs1(vector<int>& nums, int k) {
	if (k<0) return 0;
	int res = 0;
	sort(nums.begin(), nums.end());
	for (int i = 0, j = 1, n = nums.size(); i<n && j<n; ) {
		if (i == j || nums[j] - nums[i]<k) j++;
		else if (nums[j] - nums[i]>k) i++;
		else {
			res++;
			while (j<n && nums[j] - nums[i] == k) j++;//slide on duplicate target
			while (j<n - 1 && nums[j] == nums[j + 1]) j++;//slide on duplicate
			int i0 = i;
			while (i<j && nums[i0] == nums[i]) i++;//slide on duplicate
		}
	}
	return res;
}

int main()
{
	//int i = 1;
	//std::cout << isPowerOfFour(10)<<std::endl;

	//vector<int> nums = { 3,1,0 };
	//std::cout << missingNumber(nums) << std::endl;

	//std::cout << largest_power(9) << std::endl;

	//vector<int> nums = { 2,3,4 };
	//int result = 6;

	//twoSum(nums,result);
	//removeElement(nums,3);
	vector<int> nums = {1,2,3,4,5};
	vector<int> nums1 = {};
	int myarr[] = { 1,2,3,4,5 };
	vector<vector<int>> vet = { { 1,2,3 },
	{ 4,5 },
	{ 1,2,3 } };


	findPairs(nums,3);



	std::cout << 1 << std::endl;
}
