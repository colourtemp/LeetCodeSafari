#include <iostream>       // std::cout
#include <string>         // std::string
#include <vector>
#include <stack>
#include <algorithm>
#include <map>
#include <unordered_map>
using namespace std;
////////////////////////////////////////////////////////////////////////////////////
//28. Implement strStr()
//definate not expected answer -_-
int strStr0(string haystack, string needle) {
	return haystack.find(needle);
}

//consider edge situation
int strStr1(string haystack, string needle) {
	if (haystack.size() == 0 && needle.size() == 0)
		return 0;
	if (haystack.size()<needle.size())
		return -1;
	for (size_t i = 0; i <= haystack.size() - needle.size(); i++) {
		int j = haystack.compare(i, needle.size(), needle);
		if (j == 0)
			return i;
	}
	return -1;
}

//little bit more hard core   O(n(m-n+1))
int strStr2(string haystack, string needle) {
	int m = haystack.length(), n = needle.length();
	if (!n) return 0;
	for (int i = 0; i < m - n + 1; i++) {
		int j = 0;
		for (; j < n; j++)
			if (haystack[i + j] != needle[j])
				break;
		if (j == n) return i;
	}
	return -1;
}

//KMP(Knuth Morris Pratt) Pattern Searching
//LPS(longest proper prefix which is also suffix), reduce search effort when target partly known
vector<int> getLPS(string needle) {
	size_t N = needle.size();
	vector<int> LPS(N,0);
	int match = 0;//position of prefix already match
	size_t i = 1;//suffix do not include 0, make the "0*******"
	while (i < N) {
		if (needle[match] == needle[i]) {
			match++;
			LPS[i] = match;
			i++;
		}
		else {
			if (match != 0)//in long repeat "ACAAAAA"/"0011111" , make the "**11111"
				match--;
				//match = LPS[match - 1];
			else {//make the "*0*****"
				LPS[i] = 0;
				i++;
			}
		}
	}
	return LPS;
}
//KMP
int findAll(string haystack, string needle) {
	size_t N = haystack.size();
	size_t n = needle.size();
	vector<int> LPS = getLPS(needle);
	size_t i = 0, j = 0;
	while (i <N) {
		if (haystack[i] == needle[j]) {//when one char pair equal, find next i,j
			i++;
			j++;
			if (j == n) {
				printf("Found pattern at index %d \n", i - n);
				j = LPS[n - 1];
			}
		}
		else {//when one char pair not equal, find next i,j
			if (j != 0)
				j = LPS[j - 1];
			else
				i++;
		}
	}
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////

//557. Reverse Words in a String III
//std::reverse [,)
string reverseWords(string s) {
	for (size_t begin = 0; begin<s.size(); begin++) {
		size_t end = begin;
		while (s[end] != ' '&&end<s.size())
			end++;
		reverse(s.begin() + begin, s.begin() + end);
		begin = end;
	}
	return s;
}

//551. Student Attendance Record I
bool checkRecord(string s) {
	int a = 0, l = 0;
	for (size_t i = 0; i<s.size(); i++) {
		if (s[i] == 'A') a++;
		if (s[i] == 'L') l++;
		else l = 0;
		if (a >= 2 || l>2) return false;
	}
	return true;
}

//541. Reverse String II
/**
* 0            k           2k          3k
* |-----------|-----------|-----------|---
* +--reverse--+           +--reverse--+
*/
//don't go step by step if possible
string reverseStr(string s, int k) {

	size_t B = 0, E = 0;
	for (; B<s.size(); B = B + 2 * k) {
		E = min(B + k, s.size());
		reverse(s.begin() + B, s.begin() + E);
	}
	return s;
}

//521. Longest Uncommon Subsequence I
//yeah you go me
int findLUSlength(string a, string b) {
	return a == b ? -1 : max(a.size(), b.size());
}

//13. Roman to Integer
//Ⅰ（1）、Ⅴ（5）、Ⅹ（10）、Ⅼ（50）、Ⅽ（100）、Ⅾ（500）、Ⅿ（1000）
// reverse the order
int romanToInt(string s) {
	unordered_map<char, int> Roman = { { 'I',1 },
	{ 'V',5 },
	{ 'X',10 },
	{ 'L',50 },
	{ 'C',100 },
	{ 'D',500 },
	{ 'M',1000 } };

	int sum = Roman[s[s.size() - 1]];
	for (int i = s.size() - 2; i >= 0; i--) {
		if (Roman[s[i]]<Roman[s[i + 1]])
			sum -= Roman[s[i]];
		else
			sum += Roman[s[i]];
	}
	return sum;
}

//14. Longest Common Prefix
//string+=, consider empty input
string longestCommonPrefix(vector<string>& strs) {
	string got = "";
	if (strs.size() == 0)
		return got;
	for (size_t N = 0;; got += strs[0][N], N++) {
		for (auto str : strs) {
			if (N>str.size() || strs[0][N] != str[N]) {
				return got;
			}
		}
	}
}

//520. Detect Capital
bool detectCapitalUse(string word) {
	int N = (int)word.size() - 1;
	if (word[N] >= 'A'&&word[N] <= 'Z') {
		for (int i = 0; i<N; i++) {
			if (word[i] >= 'a'&&word[i] <= 'z')
				return false;
		}
	}
	else {
		for (int i = 1; i<N; i++) {
			if (word[i] >= 'A'&&word[i] <= 'Z')
				return false;
		}
	}
	return true;
}

//20. Valid Parentheses
bool isValid(string s) {
	stack<char> mystack;
	for (int i = 0; i<(int)s.size(); i++) {
		switch (s[i]) {
		case '(':
			mystack.push(')');
			break;
		case '{':
			mystack.push('}');
			break;
		case '[':
			mystack.push(']');
			break;
		default:
			if (mystack.size() > 0 && s[i] == mystack.top())
				mystack.pop();
			else
				return false;
		}

	}
	if (mystack.size() != 0)
		return false;
	else 
		return true;
}

int main()
{
	std::string str("]");
	std::string str2("AAAA");


	int M = str2.size();

	// create lps[] that will hold the longest prefix suffix
	// values for pattern
	vector<string> strr = { "a","b" };

	isValid(str);

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//						| map					| unordered_map
//-------------------------------------------------------- -
//element ordering		| strict weak			| n / a
//
//common implementation | balanced tree			| hash table
//						| or red - black tree	|
//
//search time			| log(n)				| O(1) if there are no hash collisions
//						|						| Up to O(n) if there are hash collisions
//						|						| O(n) when hash is the same for any key
//
//Insertion time		| log(n) + rebalance	| Same as search
//
//Deletion time			| log(n) + rebalance	| Same as search
//
//needs comparators		| only operator <		| only operator ==
//	
//needs hash function	| no					| yes
//	
//common use case		| when good hash is		| In most other cases.
//						| not possible or		|
//						| too slow.Or when		|
//						| order is required		|
////////////////////////////////////////////////////////////////////////////