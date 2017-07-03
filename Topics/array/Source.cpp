#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <array>
#include <algorithm>

using namespace std;

//����inputΪ��
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

//1		unordered_map��hash �����ڿ��ٲ���
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

//set�����Զ����ظ�����  ���Ǳ߼����
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
		return *tops.begin();//end�� past-the-end ����ֱ����
	}
	else {
		return *tops.rbegin();
	}
}

//���ø����������ý�ʡ�ռ�
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

//169	���������޶��������㷨
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

//������ divide & conquer
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
	bool flag = 0;//������bool �ӿ�����ٶ�
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

//�����ֵķ���
int maxSubArray(vector<int>& A) {
	int ans = A[0], sum = 0;
	for (size_t i = 0; i<A.size(); i++) {
		sum += A[i];
		ans = max(sum, ans);
		sum = max(sum, 0);
	}
	return ans;
}

//����edge situation
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

//�ú�����XOR��ת����
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

//��������+-*/%�����򻯴���
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

//��ֿ��Ǳ߼����  ʹ֮��̬��
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

//219	�������Ⱥźܹؼ�
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

//217	����set ��unordered_map
bool containsDuplicate(vector<int>& nums) {
	return nums.size() > set<int>(nums.begin(), nums.end()).size();
}

//624	set.end()�����һ��null��λ�ã��������.rbegin()
//���ҵ���С��󣬱��ұ���
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

//121	����ֱ��ȡֵvector[0],�����ǿռ�
int maxProfit(vector<int> &prices) {
	int maxPro = 0;
	int minPrice = INT_MAX;
	for (size_t i = 0; i < prices.size(); i++) {
		minPrice = min(minPrice, prices[i]);
		maxPro = max(maxPro, prices[i] - minPrice);
	}
	return maxPro;
}

//122	for (int i = 1; i<prices.size(); i++)����ɱ���ռ�����
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

//118	ֱ�Ӳ��ݾ���ûʲô����
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
	vector<int> nums = {};
	int myarr[] = { 1,2,3,4,5 };
	vector<vector<int>> vet = { { 1,2,3 },
	{ 4,5 },
	{ 1,2,3 } };


	getRow(1);



	std::cout << 1 << std::endl;
}