
#include <map>
#include <vector>	
#include <algorithm>
#include <set>
#include <unordered_set>
#include <unordered_map>
using namespace std;


//=======================================collision ======================================================
////////////////////////////////////////////////////////////////////////////
//167. Two Sum II - Input array is sorted
vector<int> twoSum(vector<int>& numbers, int target) {
	vector<int> ret;
	if (numbers.size()<2)
		return ret;
	int f = 0, e = numbers.size() - 1;
	while (f<e) {
		if (numbers[f] + numbers[e]>target)
			e--;
		else if (numbers[f] + numbers[e]<target)
			f++;
		else if (numbers[f] + numbers[e] == target) {
			ret.push_back(f + 1);
			ret.push_back(e + 1);
			return ret;
		}
	}
	return ret;
}



////////////////////////////////////////////////////////////////////////////
//611. Valid Triangle Number
int triangleNumber_0(vector<int>& nums) {
	int total = 0;
	int len;
	if ((len = nums.size())<3)
		return total;
	sort(nums.begin(), nums.end());
	for (int i = 0; i<len - 2; i++) {
		for (int j = i + 1; j<len - 1; j++) {
			int a = j + 1, b = len - 1;
			while (nums[j] - nums[i] >= nums[a])
				a++;
			while (nums[j] + nums[i] <= nums[b])
				b--;
			total += b - a >= 0 ? (b - a + 1) : 0;
		}
	}
	return total;
}

int triangleNumber_1(vector<int>& nums) {
	int total = 0;
	int len;
	if ((len = nums.size())<3)
		return total;
	sort(nums.begin(), nums.end());
	for (int i = len - 1; i>1; i--) {//pin on bigest num helps on two pointer
		int a = 0, b = i - 1;
		while (a<b) {
			if (nums[b] + nums[a]>nums[i]) {
				total += b - a;
				b--;
			}
			else
				a++;
		}
	}
	return total;
}

///////////////////////////////////////////////////////////////////////////////
//11. Container With Most Water
int maxArea(vector<int>& height) {
	if (height.size() == 2)
		return min(height[0], height[1]);
	int a = 0, b = height.size() - 1;
	int water = min(height[a], height[b])*(b - a);
	while (a<b) {
		if (height[a]<height[b])
			a++;
		else
			b--;
		water = max(water, min(height[a], height[b])*(b - a));
	}
	return water;
}


///////////////////////////////////////////////////////////////////////
//15. 3Sum
//unique is too slow
vector<vector<int>> threeSum_9(vector<int>& nums) {
	vector<vector<int>> retu;
	if (nums.size()<3)
		return retu;
	sort(nums.begin(), nums.end());
	for (int i = 0; i<nums.size() - 2; i++) {
		int l = i + 1, r = nums.size() - 1;
		while (l<r) {
			if (nums[i] + nums[l] + nums[r]<0)
				l++;
			else if (nums[i] + nums[l] + nums[r]>0)
				r--;
			else {
				vector<int> temp = { nums[i],nums[l],nums[r] };
				retu.push_back(temp);
				l++;
			}
		}
	}
	sort(retu.begin(), retu.end());
	//unique remove consecutive of equal elements, return the begining of empty iterator
	retu.erase(unique(retu.begin(), retu.end()), retu.end());
	return retu;
}

vector<vector<int>> threeSum_0(vector<int>& nums) {
	vector<vector<int>> retu;
	if (nums.size()<3)
		return retu;
	sort(nums.begin(), nums.end());
	set<vector<int>> set;
	for (int i = 0; i<nums.size() - 2; i++) {
		int l = i + 1, r = nums.size() - 1;
		while (l<r) {
			if (nums[i] + nums[l] + nums[r]<0)
				l++;
			else if (nums[i] + nums[l] + nums[r]>0)
				r--;
			else {
				vector<int> temp = { nums[i],nums[l],nums[r] };
				if (set.find(temp) == set.end()) {
					retu.push_back(temp);
					set.insert(temp);
				}
				l++;
			}
		}
	}
	return retu;
}

//do slide is much more faster
vector<vector<int> > threeSum_1(vector<int> &num) {

	vector<vector<int> > res;
	std::sort(num.begin(), num.end());

	for (int i = 0; i < num.size(); i++) {
		int target = -num[i];
		int front = i + 1;
		int back = num.size() - 1;
		while (front < back) {
			int sum = num[front] + num[back];
			// Finding answer which start from number num[i]
			if (sum < target)
				front++;
			else if (sum > target)
				back--;
			else {
				vector<int> triplet(3, 0);
				triplet[0] = num[i];
				triplet[1] = num[front];
				triplet[2] = num[back];
				res.push_back(triplet);
				// Processing duplicates of Number 2
				// Rolling the front pointer to the next different number forwards
				while (front < back && num[front] == triplet[1]) front++;
				// Processing duplicates of Number 3
				// Rolling the back pointer to the next different number backwards
				while (front < back && num[back] == triplet[2]) back--;
			}
		}
		// Processing duplicates of Number 1
		while (i + 1 < num.size() && num[i + 1] == num[i])
			i++;
	}
	return res;
}

/////////////////////////////////////////////////////////////////////////////////////////
//16. 3Sum Closest
int threeSumClosest_0(vector<int>& nums, int target) {
	int ret = 0;
	if (nums.size() <= 3) {
		for (auto n : nums)
			ret += n;
		return ret;
	}
	sort(nums.begin(), nums.end());
	int preDif = INT_MAX;
	int cloest;
	for (int i = 0; i<nums.size() - 2; i++) {
		int l = i + 1, r = nums.size() - 1;
		while (l<r) {
			int sum = nums[i] + nums[l] + nums[r];
			if (abs(sum - target)<preDif) {
				preDif = abs(sum - target);
				cloest = sum;
			}
			if (sum - target<0)
				l++;
			else if (sum - target>0)
				r--;
			else
				return target;
		}
	}
	return cloest;
}

//adding slide is not making it faster
int threeSumClosest_01(vector<int>& nums, int target) {
	int ret = 0;
	if (nums.size()<3) {
		return ret;
	}
	sort(nums.begin(), nums.end());
	int preDif = INT_MAX;
	int cloest;
	for (int i = 0; i<nums.size() - 2;) {
		int l = i + 1, r = nums.size() - 1;
		while (l<r) {
			int sum = nums[i] + nums[l] + nums[r];
			if (abs(sum - target)<preDif) {
				preDif = abs(sum - target);
				cloest = sum;
			}
			if (sum - target<0) {
				l++;
				while (l<r&&nums[l - 1] == nums[l])
					l++;
			}
			else if (sum - target>0) {
				r--;
				while (r>l&&nums[r] == nums[r + 1])
					r--;
			}
			else
				return target;
		}
		i++;
		while (i<nums.size() - 1 && nums[i - 1] == nums[i])//slide on same num
			i++;
	}
	return cloest;
}

////////////////////////////////////////////////////////////////////////////////////
//18. 4Sum
//use slide to skip duplicate
vector<vector<int>> fourSum_0(vector<int>& nums, int target) {
	vector<vector<int>> ret;
	if (nums.size()<4)
		return ret;
	sort(nums.begin(), nums.end());
	for (int a = 0; a<nums.size() - 3;) {
		for (int b = a + 1; b<nums.size() - 2;) {
			int c = b + 1, d = nums.size() - 1;
			while (c<d) {
				int sum = nums[a] + nums[b] + nums[c] + nums[d];
				if (sum<target) {
					c++;
					while (c<d&&nums[c - 1] == nums[c]) c++;
				}
				else if (sum>target) {
					d--;
					while (c<d&&nums[d + 1] == nums[d]) d--;
				}
				else {
					ret.push_back({ nums[a],nums[b],nums[c],nums[d] });
					c++;
					while (c<d&&nums[c - 1] == nums[c]) c++;
					d--;
					while (c<d&&nums[d + 1] == nums[d]) d--;
				}

			}
			b++;
			while (b<nums.size() - 2 && nums[b - 1] == nums[b]) b++;
		}
		a++;
		while (a<nums.size() - 3 && nums[a - 1] == nums[a]) a++;
	}
	return ret;
}

vector<vector<int>> fourSum_1(vector<int>& nums, int target) {
	vector<vector<int>> ret;
	if (nums.size()<4)
		return ret;
	sort(nums.begin(), nums.end());
	int N = nums.size();
	for (int a = 0; a<N - 3; a++) {
		//skip unnecessary range
		if (a>0 && nums[a] == nums[a - 1]) continue;//slide duplicate
		if (nums[a] + nums[a + 1] + nums[a + 2] + nums[a + 3]>target) break;//smallest is too much
		if (nums[a] + nums[N - 3] + nums[N - 2] + nums[N - 1]<target) continue;//biggest is not enough
		for (int b = a + 1; b<N - 2; b++) {
			if (b>a + 1 && nums[b] == nums[b - 1]) continue;
			if (nums[a] + nums[b] + nums[b + 1] + nums[b + 2]>target) break;
			if (nums[a] + nums[b] + nums[N - 2] + nums[N - 1]<target) continue;
			int c = b + 1, d = nums.size() - 1;
			while (c<d) {
				int sum = nums[a] + nums[b] + nums[c] + nums[d];
				if (sum<target)
					c++;//no need to slide if not meet
				else if (sum>target)
					d--;
				else {
					ret.push_back({ nums[a],nums[b],nums[c],nums[d] });
					c++;
					while (c<d&&nums[c - 1] == nums[c]) c++;
					d--;
					while (c<d&&nums[d + 1] == nums[d]) d--;
				}

			}
		}
	}
	return ret;
}


////////////////////////////////////////////////////////////////////////////////////
//259. 3Sum Smaller
//no need to slide in this question
int threeSumSmaller_0(vector<int>& nums, int target) {
	int ret = 0;
	if (nums.size()<3)
		return ret;
	sort(nums.begin(), nums.end());
	int N = nums.size();
	for (int a = 0; a<N - 2; a++) {
		if (nums[a] + nums[a + 1] + nums[a + 2] >= target) break;
		int b = a + 1, c = N - 1;
		while (b<c) {
			int sum = nums[a] + nums[b] + nums[c];
			if (sum<target) {
				ret += c - b;//range
				b++;
			}
			else
				c--;
		}
	}
	return ret;
}

//=======================================partition ======================================================
//////////////////////////////////////////////////////////////////////////////////
//75. Sort Colors
//count sort
void sortColors_00(vector<int>& nums) {
	int r = 0, w = 0, b = 0;
	for (auto n : nums) {
		if (n == 0) r++;
		else if (n == 1) w++;
		else b++;
	}
	int i = 0;
	for (; i<r; i++)
		nums[i] = 0;
	for (; i<r + w; i++)
		nums[i] = 1;
	for (; i<r + w + b; i++)
		nums[i] = 2;
}

//quick sort
class Solution75_0 {
private:
	void quick(vector<int>&nums, int L, int R) {
		if (L >= R)
			return;
		int pivot = nums[L];
		int p = L, l = L, r = R;
		while (l<r) {
			while (r>p&&nums[r] >= pivot) r--;
			nums[p] = nums[r];
			p = r;
			while (l<p&&nums[l] <= pivot) l++;
			nums[p] = nums[l];
			p = l;
		}
		nums[p] = pivot;
		quick(nums, L, p - 1);
		quick(nums, p + 1, R);
	}
public:
	void sortColors(vector<int>& nums) {
		quick(nums, 0, nums.size() - 1);
	}
};

//more specific, also faster
void sortColors_1(vector<int>& nums)
{
	int tmp = 0, low = 0, mid = 0, high = nums.size() - 1;

	while (mid <= high)
	{
		if (nums[mid] == 0)
		{
			tmp = nums[low];
			nums[low] = nums[mid];
			nums[mid] = tmp;
			low++;
			mid++;
		}
		else if (nums[mid] == 1)
		{
			mid++;
		}
		else if (nums[mid] == 2)
		{
			tmp = nums[high];
			nums[high] = nums[mid];
			nums[mid] = tmp;
			high--;
		}
	}
}


//=======================================chase after======================================================
//////////////////////////////////////////////////////////////////////////////////
//209. Minimum Size Subarray Sum
int minSubArrayLen_0(int s, vector<int>& nums) {
	if (nums.size() == 0)
		return 0;
	int l = 0, r = 0, sum = 0;
	int ret = INT_MAX;
	for (; r<nums.size(); r++) {
		sum += nums[r];//expand
		while (sum >= s&&l <= r) {//shrink             
			ret = min(ret, r - l + 1);
			sum -= nums[l++];
		}
	}
	if (ret == INT_MAX)
		return 0;
	return ret;
}

//O(n*log n) extention
class Solution209_1 {
public:
	int minSubArrayLen(int s, vector<int>& nums) {
		vector<int> sums = accumulate(nums);//O(n)
		int n = nums.size(), minlen = INT_MAX;
		for (int i = 1; i <= n; i++) { //O(n)
			if (sums[i] >= s) {
				int p = upper_bound(sums, 0, i, sums[i] - s);//O(log n) binary search
				if (p != -1) minlen = min(minlen, i - p + 1);
			}
		}
		return minlen == INT_MAX ? 0 : minlen;
	}
private:
	// accumulated summations of elements in nums
	vector<int> accumulate(vector<int>& nums) {
		int n = nums.size();
		vector<int> sums(n + 1, 0);
		for (int i = 1; i <= n; i++)
			sums[i] = nums[i - 1] + sums[i - 1];
		return sums;
	}
	// for each element in sums, if it is not less than s, we search for the first element that is greater than sums[i] - s
	int upper_bound(vector<int>& sums, int left, int right, int target) {
		int l = left, r = right;
		while (l < r) {
			int m = l + ((r - l) >> 1);
			if (sums[m] <= target) l = m + 1;
			else r = m;
		}
		return sums[r] > target ? r : -1;
	}
};

//////////////////////////////////////////////////////////////////////////////////
//3. Longest Substring Without Repeating Characters
int lengthOfLongestSubstring_0(string s) {
	if (s.size() <= 1)
		return s.size();
	unordered_set<char> hash;
	int b = 0, g = 0, ret = 0;
	for (; g<s.size(); g++) {
		while (hash.count(s[g]) && b<g) {
			hash.erase(s[b++]);
		}
		hash.insert(s[g]);
		ret = max(ret, g - b + 1);
	}
	return ret;
}

// a little different
int lengthOfLongestSubstring_1(string s) {

	map<char, int> charMap;
	int start = -1;
	int maxLen = 0;

	for (int i = 0; i < s.size(); i++) {
		if (charMap.count(s[i]) != 0) {
			start = max(start, charMap[s[i]]);
		}
		charMap[s[i]] = i;
		maxLen = max(maxLen, i - start);
	}

	return maxLen;
}

//////////////////////////////////////////////////////////////////////////////////
//76. Minimum Window Substring
class Solution76_0 {
private:
	string ret;
	vector<int> thash;
	vector<int> whash;
	bool valid() {
		for (int i = 0; i<256; i++) {
			if (thash[i]>whash[i])
				return false;
		}
		return true;
	}

public:
	string minWindow(string s, string t) {
		ret = s + "a";
		thash.resize(256, 0);
		whash.resize(256, 0);
		for (char c : t)
			thash[c]++;
		int b = 0, g = 0;
		for (; g<s.size(); g++) {
			whash[s[g]]++;//expand
			while (valid() && b <= g) {
				//store
				if (g - b + 1<ret.size())
					ret = s.substr(b, g - b + 1);
				//shrink
				whash[s[b++]]--;
			}
		}
		if (ret != s + "a")
			return ret;
		return "";
	}
};

//using one vector (negative/ positive to represent s/t) is smarter
//using counter is genius
string minWindow_1(string s, string t) {
	vector<int> map(128, 0);//128 i enough for chars
	for (auto c : t) //initialize the hash map here
		map[c]++;
	int counter = t.size(), //num of char checked
		begin = 0, end = 0,
		d = INT_MAX, //sub string length
		head = 0;//sub strig head
	for (; end<s.size(); end++) {
		if (map[s[end]]-->0) counter--; //s[end] check a char in string t
		while (counter == 0) { //valid
			if (end - begin + 1<d) {//update valid status
				d = end - begin + 1;
				head = begin;
			}
			if (map[s[begin++]]++ == 0) counter++;  //make it invalid
		}
	}
	return d == INT_MAX ? "" : s.substr(head, d);
}



//////////////////////////////////////////////////////////////////////////////////
//340. Longest Substring with At Most K Distinct Characters
int lengthOfLongestSubstringKDistinct_0(string s, int k) {
	vector<int> hash(128, 0);//represent the num of each char inside window
	int b = 0, g = 0, count = 0, len = 0;
	for (; g<s.size(); g++) {
		if (hash[s[g]]++ == 0) count++;//new char involved
		while (count>k)//when too much distinct chars
			if (--hash[s[b++]] == 0) count--;//one char totally removed
		len = max(len, g - b + 1);
	}
	return len;
}


int lengthOfLongestSubstringKDistinct_1(string s, int k) {
	unordered_map<char, int> ctr;//same thing as vector + counter above, but slower
	int j = -1, maxlen = 0;
	for (int i = 0; i<s.size(); ++i) {
		++ctr[s[i]];
		while (ctr.size() > k)
			if (--ctr[s[++j]] == 0)
				ctr.erase(s[j]);
		maxlen = max(maxlen, i - j);
	}
	return maxlen;
}




void main() {
	vector<int> o = {2,0,0,2,-2};
	
}