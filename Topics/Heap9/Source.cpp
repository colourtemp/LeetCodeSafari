#include<vector>
#include<algorithm>
#include<queue>
#include<iostream>
#include <functional>//greater
#include <unordered_map>
#include <set>//multi set

using namespace std;



////////////////////////////////////////////////////////////////////////////////////
//42. Trapping Rain Water
//imagain water level grows up, works like shrimping the border
class Solution42_0{
public:
	int trap(vector<int>& height) {
		if (height.size() == 0)
			return 0;
		size_t L = 0, R = height.size() - 1;//maintain L/R walker from border
		int Lbar = 0, Rbar = 0;//current max bar of current walker
		int trapped = 0;
		while (L != R) {
			if (height[L] <= height[R]) {
				Lbar = max(Lbar, height[L++]);
				trapped += Lbar - height[L]>0 ? Lbar - height[L] : 0;
			}
			else {
				Rbar = max(Rbar, height[R--]);
				trapped += Rbar - height[R]>0 ? Rbar - height[R] : 0;
			}
		}
		return trapped;
	}
};

////////////////////////////////////////////////////////////////////////////////////
//407. Trapping Rain Water II
//make sure shrimped border is still connected, need fill all neighbor cells
class Solution407_0 {
public:
	int trapRainWater(vector<vector<int>>& heightMap) {
		int line, colum;
		if ((line = heightMap.size())<3 || (colum = heightMap[0].size())<3)
			return 0;
		//the circle {h,line,colum}, lowest at top
		priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
		//store visited, make sure shrink the circle
		vector<vector<bool>> novisited(line, vector<bool>(colum, false));
		//initialize outer circle
		for (int i = 1; i<colum - 1; i++) {
			pq.push({ heightMap[0][i],0,i });
			pq.push({ heightMap[line - 1][i],line - 1,i });

		}
		for (int i = 1; i<line - 1; i++) {
			pq.push({ heightMap[i][0],i,0 });
			pq.push({ heightMap[i][colum - 1],i,colum - 1 });
			for (int j = 1; j<colum - 1; j++)
				novisited[i][j] = true;
		}
		int trapped = 0;
		vector<pair<int, int>> shift = { { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 } };
		while (pq.size()>0) {//shrink the circle
			vector<int> low = pq.top();//get the short bord point
			pq.pop();
			int l = low[1];
			int c = low[2];
			for (auto s : shift) {//fill all of its inner neighbor and make them part of new circle
				if ((l + s.first) < 0 || (c + s.second) < 0 || (l + s.first) > line - 1 || (c + s.second) > colum - 1)
					continue;
				if (novisited[l + s.first][c + s.second]) {
					int ln = l + s.first;
					int cn = c + s.second;
					novisited[ln][cn] = false;
					if (low[0] - heightMap[ln][cn]>0) {
						trapped += low[0] - heightMap[ln][cn];
						heightMap[ln][cn] = low[0];
					}
					pq.push({ heightMap[ln][cn],ln,cn });
				}
			}
		}
		return trapped;

	}
};

//
class Solution407_1 {
public:
	int trapRainWater(vector<vector<int>>& heightMap) {
		if (heightMap.size() == 0) return 0;
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> que;
		int row = heightMap.size(), col = heightMap[0].size();
		vector<vector<int>> visited(row, vector<int>(col, 0));
		int ans = 0, Max = INT_MIN;
		//initialize outer circle more elegent
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (!(i == 0 || i == row - 1 || j == 0 || j == col - 1)) continue;
				que.push(make_pair(heightMap[i][j], i*col + j)); //number transfer elegent
				visited[i][j] = 1;
			}
		}
		vector<vector<int>> dir{ { 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 } };
		while (!que.empty())
		{
			auto val = que.top(); que.pop();
			int height = val.first, x = val.second / col, y = val.second%col;
			Max = max(Max, height);
			for (auto d : dir)
			{
				int x2 = x + d[0], y2 = y + d[1];
				if (x2 >= row || x2<0 || y2<0 || y2 >= col || visited[x2][y2]) continue;
				visited[x2][y2] = 1;
				if (heightMap[x2][y2] < Max) ans += Max - heightMap[x2][y2];
				que.push(make_pair(heightMap[x2][y2], x2*col + y2));
			}
		}
		return ans;
	}
};

/////////////////////////////////////////////////////////////////////////////////
//295. Find Median from Data Stream
class MedianFinder295_0 {
public:
	/** initialize your data structure here. */
	MedianFinder295_0() {
		right.push(INT_MAX);
		left.push(INT_MIN);
	}

	void addNum(int num) {
		if (num>right.top())
			right.push(num);
		else
			left.push(num);
		if ((int)(left.size() - right.size())>1) {//take care of size_t
			right.push(left.top());
			left.pop();
		}
		else if (right.size()>left.size()) {
			left.push(right.top());
			right.pop();
		}
	}

	double findMedian() {
		if (left.size() == right.size())
			return (double)(left.top() + right.top()) / 2;//take care of int 
		else
			return left.top();
	}

	priority_queue<int, vector<int>, greater<int>> right;
	priority_queue<int> left;//one element more than right
};

/*
Supporting both min- and max-heap is more or less cumbersome, 
depending on the language, so I simply negate the numbers in the heap 
in which I want the reverse of the default order. 
To prevent this from causing a bug with -2^31 (which negated is itself, when using 32-bit ints), 
I use integer types larger than 32 bits.

Using larger integer types also prevents an overflow error when taking the mean 
of the two middle numbers. I think almost all solutions posted previously have that bug.
*/
class MedianFinder295_1 {
	priority_queue<long> small, large;
public:

	void addNum(int num) {
		small.push(num);
		large.push(-small.top());
		small.pop();
		if (small.size() < large.size()) {
			small.push(-large.top());
			large.pop();
		}
	}

	double findMedian() {
		return small.size() > large.size()? small.top(): (small.top() - large.top()) / 2.0;
	}
};

///////////////////////////////////////////////////////////////////////////////
//480. Sliding Window Median
// -2147483648*-1==-2147483648
class hash_heap {//just add remove, not really hash
private:
	vector<long> V;
public:
	void push(long p) {//take care of overflow
		V.push_back(p);
		push_heap(V.begin(), V.end());
	}
	void pop() {
		pop_heap(V.begin(), V.end());
		V.pop_back();
	}
	int size() {
		return V.size();
	}
	long top() {
		if (V.size())
			return V[0];
		return INT_MAX;
	}
	bool remove(long r) {
		int i = 0;
		for (; i<V.size(); i++) {
			if (V[i] == r) {
				V[i] = INT_MAX;
				make_heap(V.begin(), V.end());
				pop_heap(V.begin(), V.end());
				V.pop_back();
				return true;
			}
		}
		return false;
	}

};

class Solution480_0 {
private:
	hash_heap left;
	hash_heap right;
public:
	vector<double> medianSlidingWindow(vector<int>& nums, int k) {
		for (int i = 0; i<k; i++) {
			left.push(nums[i]);
			right.push(-1 * left.top());
			left.pop();
			while (left.size()<right.size()) {
				left.push(-1 * right.top());
				right.pop();
			}
		}
		int rem = 0;
		vector<double> ret;
		for (; k<nums.size(); k++, rem++) {
			if (left.size() == right.size())
				ret.push_back((left.top() - right.top()) / 2.0);
			else
				ret.push_back(left.top());

			if (nums[rem] <= left.top())
				left.remove(nums[rem]);
			else
				right.remove(-1 * nums[rem]);

			left.push(nums[k]);
			right.push(-1 * left.top());
			left.pop();
			while (left.size()<right.size()) {
				left.push(-1 * right.top());
				right.pop();
			}
		}
		if (left.size() == right.size())
			ret.push_back((left.top() - right.top()) / 2.0);
		else
			ret.push_back(left.top());

		return ret;
	}
};



/*
there is the issue of numbers moving out of the window, 
and it turns out that a hash table that records these numbers will just work (and is surprisingly neat). 
The recorded numbers will only be deleted when they come to the top of the heaps.
*/
class Solution480_1 {
public:
	vector<double> medianSlidingWindow(vector<int>& nums, int k) {
		vector<double> medians;
		unordered_map<int, int> hash;                          // count numbers to be deleted
		priority_queue<int, vector<int>> bheap;                // heap on the bottom
		priority_queue<int, vector<int>, greater<int>> theap;  // heap on the top

		int i = 0;

		// Initialize the heaps
		while (i < k) { bheap.push(nums[i++]); }
		for (int count = k / 2; count > 0; --count) {
			theap.push(bheap.top()); bheap.pop();
		}

		while (true) {
			// Get median
			if (k % 2) medians.push_back(bheap.top());
			else medians.push_back(((double)bheap.top() + theap.top()) / 2);

			if (i == nums.size()) break;
			int m = nums[i - k], n = nums[i++], balance = 0;

			// What happens to the number m that is moving out of the window
			if (m <= bheap.top()) { --balance;  if (m == bheap.top()) bheap.pop(); else ++hash[m]; }
			else { ++balance;  if (m == theap.top()) theap.pop(); else ++hash[m]; }

			// Insert the new number n that enters the window
			if (!bheap.empty() && n <= bheap.top()) { ++balance; bheap.push(n); }
			else { --balance; theap.push(n); }

			// Rebalance the bottom and top heaps
			if (balance < 0) { bheap.push(theap.top()); theap.pop(); }
			else if (balance > 0) { theap.push(bheap.top()); bheap.pop(); }

			// Remove numbers that should be discarded at the top of the two heaps
			while (!bheap.empty() && hash[bheap.top()]) { --hash[bheap.top()]; bheap.pop(); }
			while (!theap.empty() && hash[theap.top()]) { --hash[theap.top()]; theap.pop(); }
		}

		return medians;
	}
};

//black technology!!
class Solution480_2 {
public:
	vector<double> medianSlidingWindow(vector<int>& nums, int k) {
		multiset<int> window(nums.begin(), nums.begin() + k);//use multiset to sort and maintain window
															 //initialize
		auto mid = next(window.begin(), k / 2);//use next(iterator, num) to move iterator
		vector<double> medians;
		for (int i = k; ; i++) {

			// Push the current median.
			medians.push_back((double(*mid) + *prev(mid, 1 - k % 2)) / 2);//us k%2 to decide using next or not

																		  // If all done, return.
			if (i == nums.size())
				return medians;

			// Insert nums[i]. 
			window.insert(nums[i]);
			if (nums[i] < *mid)//move mid position based on inserted num
				mid--;

			// Erase nums[i-k].
			if (nums[i - k] <= *mid)//move mid position based on erased num
				mid++;
			window.erase(window.lower_bound(nums[i - k]));
		}
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
//239. Sliding Window Maximum
//all element goes in deque O(n),all goes out O(n),total O(2n)
class Solution239_0 {
public:
	vector<int> maxSlidingWindow(vector<int>& nums, int k) {
		vector<int> ret;
		if (nums.empty())
			return ret;
		deque<pair<int, int>> mem;//stores all candidate of MAX, origin order keeped
		//from 0 to k-1
		mem.push_back({ nums[0],0 });
		for (int i = 1; i<k; i++) {
			while (mem.size() && nums[i]>mem.back().first)
				mem.pop_back();
			mem.push_back({ nums[i],i });
		}
		ret.push_back(mem.front().first);
		//from k to end
		for (int j = k; j<nums.size(); j++) {
			//when window end at j, the element earlier than j-k is no longer considered
			if (mem.front().second == j - k)
				mem.pop_front();
			//when nums[j] comes into consideration, all element smaller than it will never be a max
			while (mem.size() && nums[j]>mem.back().first)
				mem.pop_back();
			mem.push_back({ nums[j],j });
			//now the front is the max in window
			ret.push_back(mem.front().first);
		}
		return ret;
	}
};

void main() {
	vector<vector<int>> heightMap = { {1, 4, 3, 1, 3, 2},
	{3, 2, 1, 3, 2, 4},{2, 3, 3, 2, 3, 1} };



}
