#include<string>
#include<vector>
#include<iostream>
#include<unordered_map>
#include <sstream>      // std::istringstream
#include<map>
#include <algorithm> //min max
#include <bitset>         // std::bitset
#include <unordered_set>
using namespace std;


//////////////////////////////////////////////////////////////
//290. Word Pattern
bool wordPattern(string pattern, string str) {
	vector<string> St;
	string emp = "";
	for (int i = 0; i<str.size(); i++) {
		if (str[i] != ' ')
			emp += str[i];
		if (str[i] == ' ' || i == str.size() - 1) {
			St.push_back(emp);
			emp = "";
		}
	}
	if (St.size() != pattern.size())
		return false;
	for (int j = 0; j<pattern.size(); j++) {
		for (int a = j + 1; a<pattern.size(); a++) {
			if (pattern[j] == pattern[a] && St[a] != St[j])
				return false;
			if (pattern[j] != pattern[a] && St[a] == St[j])
				return false;
		}
	}
	return true;

}

//use of map, set
//use of istringstream
bool wordPattern2(string pattern, string str) {
	unordered_map<char, int> p2i;
	unordered_map<string, int> w2i;
	istringstream in(str);
	int i = 0, n = pattern.size();
	for (string word; in >> word; ++i) {//in>>word will stop th circle at end
		if (i == n || p2i[pattern[i]] != w2i[word])
			return false;
		p2i[pattern[i]] = w2i[word] = i + 1;
	}
	return i == n;
}


//////////////////////////////////////////////////////////////
//599. Minimum Index Sum of Two Lists
vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
	unordered_map<string, int> saw;
	vector<string> common;
	for (size_t j = 0; j<list2.size(); j++)
		saw[list2[j]] = j;

	int isum = 2000;
	for (size_t i = 0; i<list1.size(); i++) {
		if (saw.find(list1[i]) != saw.end()) {
			if (i + saw[list1[i]]<isum) {
				common.clear();
				isum = i + saw[list1[i]];
				common.push_back(list1[i]);
			}
			else if (i + saw[list1[i]] == isum)
				common.push_back(list1[i]);
		}
	}
	return common;
}

//////////////////////////////////////////////////////////////
//594. Longest Harmonious Subsequence
//make sure what the question ask for
int findLHS(vector<int>& nums) {
	if (nums.size() <= 1)
		return 0;
	map<int, int> hash;
	for (auto num : nums) {
		hash[num]++;
	}

	int Max = 0;
	for (map<int, int>::iterator it = hash.begin(); it != hash.end(); it++) {
		int sum = 0;
		sum += it->second;
		map<int, int>::iterator it2 = hash.find(it->first + 1);
		if (it2 != hash.end())
			sum += it2->second;
		else continue;
		Max = max(sum, Max);
	}
	return Max;;
}

//////////////////////////////////////////////////////////////
//575. Distribute Candies
//Set - O(N) time, O(N) space
//unordered is faster
int distributeCandies_0(vector<int>& candies) {
	unordered_set<int> kinds;
	for (int kind : candies) {
		kinds.insert(kind);
	}
	return min(kinds.size(), candies.size() / 2);
}

//Sort - O(N logN) time, O(1) space
int distributeCandies_1(vector<int>& candies) {
	size_t kinds = 0;
	sort(candies.begin(), candies.end());
	for (int i = 0; i < candies.size(); i++) {
		kinds += i == 0 || candies[i] != candies[i - 1];
	}
	return min(kinds, candies.size() / 2);
}

//bitset and test are super fast!
int distributeCandies_2(vector<int>& candies) {
	bitset<200001> hash;
	int count = 0;
	for (int i : candies) {
		if (!hash.test(i + 100000)) {
			count++;
			hash.set(i + 100000);
		}
	}
	int n = candies.size();
	return min(count, n / 2);
}


//////////////////////////////////////////////////////////////
//500. Keyboard Row
//std::string::find_first_of
//Searches the string for the first character that matches any of the characters specified in its arguments.
vector<string> findWords(vector<string>& words)
{
	vector<string> res;

	for (auto str : words)
	{
		bool r1 = str.find_first_of("QWERTYUIOPqwertyuiop") == string::npos ? false : true;
		bool r2 = str.find_first_of("ASDFGHJKLasdfghjkl") == string::npos ? false : true;
		bool r3 = str.find_first_of("ZXCVBNMzxcvbnm") == string::npos ? false : true;

		if (r1 + r2 + r3 == 1)
			res.push_back(str);
	}
	return res;
}


//bit manipulation is fast, use che characteristic of &=
vector<string> findWords_2(vector<string>& words) {
	vector<int> dict(26);
	//Define a mask flag for each row
	//the flag for Row 1 ("QWERTYUIOP") as 1 (001), Row 2 ("ASDFGHJKL") as 2 (010), Row 3 ("ZXCVBNM") as 4 (100).
	vector<string> rows = { "QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM" };
	for (int i = 0; i < rows.size(); i++) {
		for (auto c : rows[i])
			dict[c - 'A'] = 1 << i;
	}
	vector<string> res;
	for (auto w : words) {
		//For each word, us 7 (111) as base, do AND operation on each character.
		int r = 7;
		for (char c : w) {
			r &= dict[toupper(c) - 'A'];
			//If all chars from same row, the final result will be one of (1, 2, 4). 
			//If any character from other row, the final result will be 0.
			if (r == 0) break;
		}
		if (r) res.push_back(w);
	}
	return res;
}

//////////////////////////////////////////////////////////////
//463. Island Perimeter
int islandPerimeter(vector<vector<int>>& grid) {
	if (!grid.size())
		return 0;
	int y = grid.size();
	int x = grid[0].size();
	int sum = 0;
	for (int i = 0; i<y; i++) {
		int prev = 0;
		for (int j = 0; j<x; j++) {
			if (grid[i][j] != prev) {
				sum++;
				prev = grid[i][j];
			}
		}
		if (grid[i][x - 1] == 1)
			sum++;
	}
	for (int j = 0; j<x; j++) {
		int prev = 0;
		for (int i = 0; i<y; i++) {
			if (grid[i][j] != prev || grid[y - 1][j] == 1) {
				sum++;
				prev = grid[i][j];
			}
		}
		if (grid[y - 1][j] == 1)
			sum++;
	}
	return sum;
}

//more math, less cycle
int islandPerimeter_2(vector<vector<int>>& grid) {
	int count = 0, repeat = 0;
	for (int i = 0; i<grid.size(); i++)
	{
		for (int j = 0; j<grid[i].size(); j++)
		{
			if (grid[i][j] == 1)
			{
				count++;
				if (i != 0 && grid[i - 1][j] == 1) repeat++;
				if (j != 0 && grid[i][j - 1] == 1) repeat++;
			}
		}
	}
	return 4 * count - repeat * 2;
}

//////////////////////////////////////////////////////////////
//447. Number of Boomerangs
class Solution447 {
public:
public:
	int numberOfBoomerangs(vector<pair<int, int>>& points) {
		int total = 0;
		unordered_map<int, int> dic;
		for (auto o : points) {
			dic.clear();
			for (auto x : points) {//calculate distance to all point from o
				dic[pow(o.first - x.first, 2) + pow(o.second - x.second, 2)]++;
			}
			for (auto it : dic) {
				if (it.second<2)
					continue;
				else
					total += NCR(it.second, 2) * 2;//select 2 from n
			}
		}
		return total;
	}

	int NCR(int n, int r) {
		if (r>n / 2)
			return NCR(n, n - r);
		int combi = 1;
		for (int k = n, l = r; k>n - r&&r>0; k--, l--) {
			combi *= k;
			combi /= l;//care for divide by zero
		}
		return combi;
	}
};

//2+4+6+8+...+2n = n(n+1)==NCR(n+1,2)*2
//hypot() Returns the hypotenuse of a right-angled triangle whose legs are x and y.
//but the square root part is slow
int numberOfBoomerangs_1(vector<pair<int, int>>& points) {
	int booms = 0;
	for (auto &p : points) {
		unordered_map<double, int> ctr(points.size());
		for (auto &q : points)
			booms += 2 * ctr[hypot(p.first - q.first, p.second - q.second)]++;
	}
	return booms;
}

//////////////////////////////////////////////////////////////
//438. Find All Anagrams in a String
//do summary in the right way
vector<int> findAnagrams_0(string s, string p) {
	vector<int> pv(26, 0), sv(26, 0), res;
	if (s.size() < p.size())
		return res;
	// fill pv, vector of counters for pattern string and sv, vector of counters for the sliding window
	for (int i = 0; i < p.size(); ++i)
	{
		++pv[p[i] - 'a'];
		++sv[s[i] - 'a'];
	}
	if (pv == sv)
		res.push_back(0);

	//here window is moving from left to right across the string. 
	//window size is p.size(), so s.size()-p.size() moves are made 
	for (int i = p.size(); i < s.size(); ++i)
	{
		// window extends one step to the right. counter for s[i] is incremented 
		++sv[s[i] - 'a'];

		// since we added one element to the right, 
		// one element to the left should be forgotten. 
		//counter for s[i-p.size()] is decremented
		--sv[s[i - p.size()] - 'a'];

		// if after move to the right the anagram can be composed, 
		// add new position of window's left point to the result 
		if (pv == sv)
			res.push_back(i - p.size() + 1);
	}
	return res;
}

//////////////////////////////////////////////////////////////
//409. Longest Palindrome
//bit flip is fast
int longestPalindrome(string s) {
	bitset<256> mark;
	for (int i = 0; i<s.size(); i++) {
		mark.flip(s[i]);
	}
	if (mark.count() == 0)//consider edge situation
		return s.size();
	return s.size() - mark.count() + 1;
}


//////////////////////////////////////////////////////////////
//136. Single Number
int singleNumber(vector<int>& nums) {
	for (int i = 1; i<nums.size(); i++) {
		nums[0] = nums[0] ^ nums[i];
	}
	return nums[0];
}


//////////////////////////////////////////////////////////////
//242. Valid Anagram
//use vector to marke again,
//can boot up the speed if consider 26 char over 256 ascii
bool isAnagram(string s, string t) {
	int len = s.size();
	if (t.size() != len)
		return false;
	vector<int> list(256, 0);
	vector<int> empty(256, 0);
	for (int i = 0; i<len; i++) {
		list[s[i]]++;
		list[t[i]]--;
	}
	return list == empty;
}

//sort can be useful in may places, but much slower
bool isAnagram_2(string s, string t) {
	sort(s.begin(), s.end());
	sort(t.begin(), t.end());
	return s == t;
}


//////////////////////////////////////////////////////////////
//205. Isomorphic Strings
bool isIsomorphic(string s, string t) {
	size_t sn = s.size();
	size_t tn = t.size();
	if (sn != tn)
		return false;
	if (!sn&&!tn)
		return true;

	vector<int> smap(256, 0);
	vector<int> tmap(256, 0);

	for (int i = 0; i<sn; i++) {
		//map to each other
		if (smap[s[i]] == 0 && tmap[t[i]] == 0) {
			smap[s[i]] = t[i];
			tmap[t[i]] = s[i];
		}
		//check if follow previous map
		else if (smap[s[i]] != t[i] || tmap[t[i]] != s[i])
			return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////
//645. Set Mismatch
vector<int> findErrorNums(vector<int>& nums) {
	vector<int> re;
	int n = 0;
	bitset<10001> nlist;
	for (int i = 0; i<nums.size(); i++) {
		if (nlist.test(nums[i]))
			re.push_back(nums[i]);
		else {
			nlist.set(nums[i]);
			n = n^nums[i];
		}
		n = n ^ (i + 1);
	}
	re.push_back(n);
	return re;
}

//////////////////////////////////////////////////////////////
//349. Intersection of Two Arrays
vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
	vector<int> re;
	if (!nums1.size() || !nums2.size())
		return re;
	unordered_set<int> hash;
	for (auto num : nums1)
		hash.insert(num);
	for (auto num : nums2)
		if (hash.find(num) != hash.end()) {
			re.push_back(num);
			hash.erase(num);
		}
	return re;
}

// set [InputIterator first, InputIterator last)
vector<int> intersection_2(vector<int>& nums1, vector<int>& nums2) {
	unordered_set<int> m(nums1.begin(), nums1.end());
	vector<int> res;
	for (auto a : nums2)
		if (m.count(a)) {
			res.push_back(a);
			m.erase(a);
		}
	return res;
}

//////////////////////////////////////////////////////////////
//350. Intersection of Two Arrays II
vector<int> intersect_0(vector<int>& nums1, vector<int>& nums2) {
	vector<int> re;
	if (!nums1.size() || !nums2.size())
		return re;
	unordered_map<int, int> map1, map2;

	for (auto num : nums1)
		map1[num]++;
	for (auto num : nums2)
		map2[num]++;
	for (auto pa : map1) {
		unordered_map<int, int>::const_iterator it = map2.find(pa.first);
		if (it != map2.end()) {
			for (int i = 0; i<min(pa.second, it->second); i++)
				re.push_back(pa.first);
		}
	}
	return re;
}

//use plus and mius
//Time: O(m + n) Space: O(m + n)
vector<int> intersect_1(vector<int>& nums1, vector<int>& nums2) {
	unordered_map<int, int> dict;
	vector<int> res;
	for (int i = 0; i < (int)nums1.size(); i++) dict[nums1[i]]++;
	for (int i = 0; i < (int)nums2.size(); i++)
		if (--dict[nums2[i]] >= 0) res.push_back(nums2[i]);
	return res;
}

//if one array is smaller
//Time: O(m + n) Space: O(m) 
vector<int> intersect_2(vector<int>& nums1, vector<int>& nums2) {
	unordered_map<int, int> dict;
	vector<int> res;
	for (int i = 0; i < (int)nums1.size(); i++) dict[nums1[i]]++;
	for (int i = 0; i < (int)nums2.size(); i++)
		if (dict.find(nums2[i]) != dict.end() && --dict[nums2[i]] >= 0) res.push_back(nums2[i]);
	return res;
}

//two pointer method
//sort(begin(),end()) is always useful
vector<int> intersect_3(vector<int>& nums1, vector<int>& nums2) {
	sort(nums1.begin(), nums1.end());
	sort(nums2.begin(), nums2.end());
	int n1 = (int)nums1.size(), n2 = (int)nums2.size();
	int i1 = 0, i2 = 0;
	vector<int> res;
	while (i1 < n1 && i2 < n2) {
		if (nums1[i1] == nums2[i2]) {
			res.push_back(nums1[i1]);
			i1++;
			i2++;
		}
		else if (nums1[i1] > nums2[i2]) {
			i2++;
		}
		else {
			i1++;
		}
	}
	return res;
}

//////////////////////////////////////////////////////////////
//389. Find the Difference
char findTheDifference(string s, string t) {
	char r = 0;
	for (char c : s) r ^= c;
	for (char c : t) r ^= c;
	return r;
}

void main() {

	vector<int> nums = { 1,2 };
	vector<int> nums1 = { 2,1 };
	vector < vector<int>> d2 = { {0, 1, 0, 0},
								 {1, 1, 1, 0},
								 {0, 1, 0, 0},
								 {1, 1, 0, 0} };
	vector<pair<int, int>> points = { {0, 0},{1, 0},{2, 0} };




}