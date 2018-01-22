#include<vector>
#include<bitset>
#include<algorithm>//next_permutation
#include<unordered_map>
#include<unordered_set>
#include<time.h>
#include<queue>
#include<set>
using namespace std;



/////////////////////////////////////////////////////////////////////////////////////////////////////
//266. Palindrome Permutation
//bast question, using bitset
bool canPermutePalindrome(string s) {
	bitset<256> check;
	for (auto c : s)
		check.flip(c);
	return check.count()<2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//267. Palindrome Permutation II
//with recursion
class Solution267_0 {
private:
	vector<string> ret;

	void append(unordered_map<char, int> count, string s, int left, int right) {
		if (left >= right) {
			ret.push_back(s);
			return;
		}
		for (auto& c : count) {
			if (c.second>0) {
				c.second -= 2;
				s[left] = c.first;
				s[right] = c.first;
				append(count, s, left + 1, right - 1);
				c.second += 2;
			}
		}
	}

public:
	vector<string> generatePalindromes(string s) {
		int n = s.size();

		unordered_map<char, int> count;
		for (auto c : s)
			count[c]++;

		int oddnum = 0;
		string blank(n, '*');
		for (auto& it : count) {
			if (it.second % 2 == 1) {
				oddnum++;
				blank[n / 2] = it.first;
				it.second--;//might be "aaa"
				if (oddnum>1)
					return ret;
			}
		}
		append(count, blank, 0, n - 1);
		return ret;
	}
};

//next_permutation
class Solution267_1 {
public:
	vector<string> generatePalindromes(string s) {
		vector<string> palindromes;
		unordered_map<char, int> counts;
		for (char c : s)
			counts[c]++;
		int odd = 0;
		char mid;
		//get the repeated half string
		string half;
		for (auto p : counts) {
			if (p.second == 1) {
				odd++, mid = p.first;
				if (odd > 1)
					return palindromes;
			}
			half += string(p.second / 2, p.first);
		}
		//get all permutations of the half
		palindromes = permutations(half);
		//make half into palindrome
		for (string& p : palindromes) {
			string t(p);
			reverse(t.begin(), t.end());
			if (odd) t = mid + t;
			p += t;
		}
		return palindromes;
	}
private:
	vector<string> permutations(string& s) {
		vector<string> perms;
		//get all permutation of a string
		sort(s.begin(), s.end());
		do {
			perms.push_back(s);
		} while (next_permutation(s.begin(), s.end()));
		return perms;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////
//46. Permutations
//next_permutation
vector<vector<int>> permute_0(vector<int>& nums) {
	vector<vector<int>> ret;
	sort(nums.begin(), nums.end());
	do {
		ret.push_back(nums);
	} while (next_permutation(nums.begin(), nums.end()));
	return ret;
}

// do recur swap by hand, little faster
class Solution46_1 {
public:
	vector<vector<int> > permute(vector<int> &num) {
		vector<vector<int> > result;
		permuteRecursive(num, 0, result);
		return result;
	}

	// permute num[begin..end]
	// invariant: num[0..begin-1] have been fixed/permuted
	void permuteRecursive(vector<int> &num, int begin, vector<vector<int> > &result) {
		if (begin >= num.size()) {
			// one permutation instance
			result.push_back(num);
			return;
		}

		for (int i = begin; i < num.size(); i++) {
			swap(num[begin], num[i]);
			permuteRecursive(num, begin + 1, result);
			// reset
			swap(num[begin], num[i]);
		}
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////
//47. Permutations II
class Solution47_0 {
private:
	void rec(vector<int>&nums, int begin, vector<vector<int>>& ret) {
		if (begin >= nums.size()) {
			ret.push_back(nums);
			return;
		}
		unordered_set<int> used;
		for (int i = begin; i<nums.size(); i++) {
			auto it = used.find(nums[i]);
			if (it == used.end()) {
				swap(nums[i], nums[begin]);
				rec(nums, begin + 1, ret);
				used.insert(nums[begin]);
				swap(nums[begin], nums[i]);
			}
		}
	}
public:
	vector<vector<int>> permuteUnique(vector<int>& nums) {
		vector<vector<int>> ret;
		rec(nums, 0, ret);
		return ret;
	}
};


//next_permutation still works for unique
vector<vector<int>> permuteUnique_1(vector<int>& nums) {
	vector<vector<int>> ret;
	sort(nums.begin(), nums.end());
	do {
		ret.push_back(nums);
	} while (next_permutation(nums.begin(), nums.end()));
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//31. Next Permutation
void nextPermutation_0(vector<int>& nums) {
	next_permutation(nums.begin(), nums.end());
}

//hand made next permutation
/*
Find the largest index k such that nums[k] < nums[k + 1]. 
	If no such index exists, the permutation is sorted in descending order, 
	just reverse it to ascending order and we are done. 
	For example, the next permutation of [3, 2, 1] is [1, 2, 3].
Find the largest index l greater than k such that nums[k] < nums[l].
Swap the value of nums[k] with that of nums[l].
Reverse the sequence from nums[k + 1] up to and including the final element nums[nums.size() - 1].
*/
void nextPermutation_01(vector<int>& nums) {
	int posA;//position should be replaces with bigger number
	int posB;//the smallest number bigger than posA in lower digit 

	while (true) {
		for (posA = nums.size() - 2; posA >= 0; posA--)//find the position
			if (nums[posA]<nums[posA + 1]) break;

		if (posA == -1) {//if already the last permutation
			sort(nums.begin(), nums.end());
			break;
		}

		for (posB = nums.size() - 1; posB>posA; posB--)//find the replacer
			if (nums[posB]>nums[posA]) break;

		//do the swap
		swap(nums[posA], nums[posB]);
		sort(nums.begin() + posA + 1, nums.end());
		break;
	}
}

//is_sorted_until, upper_bound
void nextPermutation_1(vector<int>& nums) {
    auto i = is_sorted_until(nums.rbegin(), nums.rend());
	if (i != nums.rend())
		swap(*i, *upper_bound(nums.rbegin(), i, *i));
	reverse(nums.rbegin(), i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//60. Permutation Sequence
string getPermutation_0(int n, int k) {
	string s;
	for (int i = 1; i <= n; i++) {
		s += to_string(i);
	}
	while (--k)
		next_permutation(s.begin(), s.end());
	return s;
}

//surely can skip the iteration, ty to do math
string getPermutation_1(int n, int k) {
	int i, j, f = 1;
	// left part of s is partially formed permutation, right part is the leftover chars.
	string s(n, '0');
	for (i = 1; i <= n; i++) {
		f *= i;//get n!
		s[i - 1] += i; // make s become 1234...n
	}
	for (i = 0, k--; i<n; i++) {
		f /= n - i;//get (n-1)! 
		j = i + k / f; // calculate index of char to put at s[i]
		char c = s[j];
		// remove c by shifting to cover up (adjust the right part).
		for (; j>i; j--)
			s[j] = s[j - 1];
		k %= f;//calculate the k left over
		s[i] = c;//put correct char in position i
	}
	return s;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
//384. Shuffle an Array
//simple rand question
class Solution_384 {
public:
	Solution_384(vector<int> nums) {
		ori = nums;
		n = nums.size();
		reset();
		srand(time(NULL));
	}

	/** Resets the array to its original configuration and return it. */
	vector<int> reset() {
		cal = ori;
		return cal;
	}

	/** Returns a random shuffling of the array. */
	vector<int> shuffle() {
		for (int i = 0; i<n; i++) {
			int j = rand() % (n - i);
			swap(cal[i + j], cal[i]);
		}
		return cal;
	}
private:
	int n;
	vector<int> ori;
	vector<int> cal;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////
//567. Permutation in String
bool checkInclusion_0(string s1, string s2) {
	vector<int> count1(26, 0);
	for (auto c : s1)
		count1[c - 'a']++;
	vector<int> check(count1);
	int n = s1.size();
	if (n>s2.size())
		return false;
	int checkN = n;
	for (int j = 0; j <= s2.size() - n; j++) {
		for (int i = j; i<j + n; i++) {
			if (check[s2[i] - 'a']>0) {
				check[s2[i] - 'a']--;
				if (--checkN == 0)
					return true;
			}
		}
		check = count1;
		checkN = n;
	}
	return false;
}


/*
How do we know string s2 contains a permutation of s1? 
We just need to create a sliding window with length of s1, move from beginning to the end of s2. 
When a character moves in from right of the window, we subtract 1 to that character count from the map. 
When a character moves out from left of the window, we add 1 to that character count. 
So once we see all zeros in the map, meaning equal numbers of every characters between s1 and the substring in window
*/
//sliding window
bool checkInclusion_1(string s1, string s2) {
	int len1 = s1.length(), len2 = s2.length();
	if (len1 > len2) return false;

	vector<int> count(26, 0);
	for (int i = 0; i<len1; ++i) {
		count[s1[i] - 'a']++;
		count[s2[i] - 'a']--;
	}

	int numZeroes = 0;
	for (int i = 0; i<26; ++i) {
		if (count[i] == 0)
			numZeroes++;
	}
	if (numZeroes == 26) return true;

	for (int i = len1; i<len2; ++i) {
		count[s2[i] - 'a']--;
		if (count[s2[i] - 'a'] == 0)
			numZeroes++;
		else if (count[s2[i] - 'a'] == -1)
			numZeroes--;

		count[s2[i - len1] - 'a']++;
		if (count[s2[i - len1] - 'a'] == 0)
			numZeroes++;
		else if (count[s2[i - len1] - 'a'] == 1)
			numZeroes--;

		if (numZeroes == 26) return true;
	}
	return false;
}


/**
* 1. try find a window (i, j) where s2.substr(i, j + 1 - i) contains all chars in s1;
* 2. once found, try reduce window(i, j) such that j + 1 - i == s1.size() while the window still contains all chars in s1 by moving i, return true;
* 3. if windows no longer contain all chars in s1, keep moving j forward;
*/
//extend and then shrink window
bool checkInclusion_2(string s1, string s2) {
	vector<int> cnts(256, 0);
	for (char ch : s1) {
		cnts[ch]++;
	}

	int left = s1.size();
	for (int i = 0, j = 0; j < s2.size(); j++) {
		if (cnts[s2[j]]-- > 0) {
			left--;
		}

		while (left == 0) {
			if (j + 1 - i == s1.size()) {
				return true;
			}
			if (++cnts[s2[i++]] > 0) {
				left++;
			}
		}
	}

	return false;
}



/////////////////////////////////////////////////////////////////////////////////////
//484. Find Permutation
//observe closer
vector<int> findPermutation_1(string s) {
	vector<int> res(1, 1);
	auto b = res.begin();
	int i = 2;
	for (auto c : s) {
		if (c == 'D') {
			b = res.insert(b, i++);
		}
		else {
			res.push_back(i++);
			b = prev(res.end());
		}
	}
	return res;
}


/*
For example, given IDIIDD we start with sorted sequence 1234567
Then for each k continuous D starting at index i we need to reverse [i, i+k] portion of the sorted sequence.*/
vector<int> findPermutation_2(string s) {
	int n = (int)s.length() + 1;
	vector<int> nums(n);
	for (int i = 1; i <= n; ++i) nums[i - 1] = i;
	for (int l = 0, h = 0; l < n - 1; l = h + 1) {
		for (h = l; h < n - 1 && s[h] == 'D'; ++h);
		reverse(nums.begin() + l, nums.begin() + h + 1);
	}
	return nums;
}


