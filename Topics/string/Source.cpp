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
////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////
//521. Longest Uncommon Subsequence I
//yeah you go me
int findLUSlength(string a, string b) {
	return a == b ? -1 : max(a.size(), b.size());
}
////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////
//459. Repeated Substring Pattern
/*
str + str means doubling, (str + str).substr(1, str.size() * 2 - 2) means removing the first char of the first half and the last char of the second half.

If there is no pattern, both of the first copy and the second copy will be changed, so str will not be found in(str + str).substr(1, str.size() * 2 - 2).
If there is a pattern, the first char of str can still be found in the first half, and the last char of str can also be found in the second half.
Here is an example : abcabc is the original string, and (bcabc abcab) includes abcabc.
*/
bool repeatedSubstringPattern0(string s) {
	string doubleS = s + s;
	return (doubleS).substr(1, s.size() * 2 - 2).find(s) != -1;
}

//string.substr(start,length);
bool repeatedSubstringPattern(string str) {
	int n = str.length();
	for (int i = 1; i <= n / 2; i++)
		if (n % i == 0 && str.substr(i) == str.substr(0, n - i))//shift left and right should be the same
			return true;
	return false;
}

////////////////////////////////////////////////////////////////////////////////////
//606. Construct String from Binary Tree
// Definition for a binary tree node.
 struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 };

//to_string();
string tree2str(TreeNode* t) {
	string ret;
	if (t == NULL)
		return ret;
	ret += to_string(t->val);

	if (t->left) {
		ret += "(" + tree2str(t->left) + ")";
	}
	else if (t->right) {
		ret += "()";
	}

	if (t->right) {
		ret += "(" + tree2str(t->right) + ")";
	}

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////
//434. Number of Segments in a String
//consider extreme input(empty or overwhelm), modify the input to avoid edge situation
int countSegments(string s) {
	int res = 0;
	s.push_back(' ');
	for (int i = 1; i < (int)s.size(); ++i)
		if (s[i] == ' ' && s[i - 1] != ' ') ++res;
	return res;
}

////////////////////////////////////////////////////////////////////////////////////
//38. Count and Say
string seq(string str) {
	string got;
	char cur = str[0];
	int count = 1;
	for (size_t i = 1; i<str.size(); i++) {
		if (str[i] == cur)
			count++;
		else {
			got += to_string(count) + cur;
			cur = str[i];
			count = 1;
		}
	}
	got += to_string(count) + cur;
	return got;
}
string countAndSay(int n) {
	string str = "1";
	while (--n>0)
		str = seq(str);
	return str;
}

////////////////////////////////////////////////////////////////////////////////////
//383. Ransom Note
//slow but notice find_first_of(char,n) find_last_of(char,n)
bool canConstruct(string ransomNote, string magazine) {
	size_t r = ransomNote.size();
	size_t m = magazine.size();
	if (r>m)
		return false;

	sort(ransomNote.begin(), ransomNote.end());
	sort(magazine.begin(), magazine.end());
	for (size_t i = 0; i<r;) {
		int begin = ransomNote.find_first_of(ransomNote[i]);
		int end = ransomNote.find_last_of(ransomNote[i]);

		int begin1 = magazine.find_first_of(ransomNote[i]);
		int end1 = magazine.find_last_of(ransomNote[i]);

		if (begin1==-1||end - begin>end1 - begin1)
			return false;
		i = end + 1;
	}
	return true;
}

bool canConstruct1(string ransomNote, string magazine) {
	unordered_map<char, int> map(26);
	for (size_t i = 0; i < magazine.size(); ++i)
		++map[magazine[i]];
	for (size_t j = 0; j < ransomNote.size(); ++j)
		if (--map[ransomNote[j]] < 0)
			return false;
	return true;
}

bool canConstruct2(string ransomNote, string magazine) {
	vector<int> vec(26, 0);
	for (size_t i = 0; i < magazine.size(); ++i)
		++vec[magazine[i] - 'a'];
	for (size_t j = 0; j < ransomNote.size(); ++j)
		if (--vec[ransomNote[j] - 'a'] < 0)
			return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////
//345. Reverse Vowels of a String
//find_first_of(target chars, start point),  template <class T> void swap (T& a, T& b)
string reverseVowels(string s) {
	vector<size_t> Vowels;
	int found = s.find_first_of("aeiouAEIOU");
	while (found != -1) {
		Vowels.push_back(found);
		found = s.find_first_of("aeiouAEIOU", found + 1);
	}
	for (int j = 0, k = (int)Vowels.size() - 1; j<k; j++, k--) {
		swap(s[Vowels[j]], s[Vowels[k]]);
	}
	return s;
}

//more clean
string reverseVowels1(string s) {
	int i = 0, j = s.size() - 1;
	while (i < j) {
		i = s.find_first_of("aeiouAEIOU", i);
		j = s.find_last_of("aeiouAEIOU", j);
		if (i < j) {
			swap(s[i++], s[j--]);
		}
	}
	return s;
}

////////////////////////////////////////////////////////////////////////////////////
//344. Reverse String
string reverseString(string s) {
	for (int i = 0, j = (int)s.size() - 1; i<j; i++, j--) {
		swap(s[i], s[j]);
	}
	return s;
}

////////////////////////////////////////////////////////////////////////////////////
//58. Length of Last Word
//cinsider 0-1 overflow
int lengthOfLastWord(string s) {
	s += ' ';
	int space1 = s.size();
	do {
		space1 = s.find_last_of(' ', space1 - 1);
	} while (space1>0 && s[space1 - 1] == ' ');

	int space0 = s.find_last_of(' ', space1 - 1);

	if (space1 == 0)//no char
		return 0;
	else if (space0 == -1)//word with no leading space
		return space1;
	else
		return space1 - space0 - 1;
}

//smarter
int lengthOfLastWord1(string s) {
	int len = 0, tail = s.length() - 1;
	while (tail >= 0 && s[tail] == ' ') tail--;
	while (tail >= 0 && s[tail] != ' ') {
		len++;
		tail--;
	}
	return len;
}

////////////////////////////////////////////////////////////////////////////////////
//67. Add Binary
//reverse
string addBinary(string a, string b) {
	string res;
	size_t l = max(a.size(), b.size());
	int A = a.size() - 1;
	int B = b.size() - 1;
	int carry = 0, sum = 0;
	while (l-->0) {
		sum = 0;
		if (A >= 0 && a[A--] == '1')
			sum++;
		if (B >= 0 && b[B--] == '1')
			sum++;
		sum += carry;

		if (sum == 0)
			res += '0';
		else if (sum == 1) {
			res += '1';
			carry = 0;
		}
		else if (sum == 2) {
			res += '0';
			carry = 1;
		}
		else if (sum == 3) {
			res += '1';
			carry = 1;
		}
	}
	if (carry == 1)
		res += '1';
	reverse(res.begin(), res.end());
	return res;
}

//know ASCII better
string addBinary1(string a, string b)
{
	string s = "";

	int c = 0, i = a.size() - 1, j = b.size() - 1;
	while (i >= 0 || j >= 0 || c == 1)
	{
		c += i >= 0 ? a[i--] - '0' : 0;
		c += j >= 0 ? b[j--] - '0' : 0;
		s = char(c % 2 + '0') + s;
		c /= 2;
	}

	return s;
}

//know true==1,false==0 and XOR better
string addBinary2(string a, string b)
{
	string result = "";
	int apos = a.size() - 1;
	int bpos = b.size() - 1;
	int adigit, bdigit, carry = 0;

	while (apos >= 0 || bpos >= 0 || carry == 1)
	{
		adigit = bdigit = 0;

		if (apos >= 0) adigit = a[apos--] == '1';
		if (bpos >= 0) bdigit = b[bpos--] == '1';

		// Another way: the digit is 1 if adigit + bdigit + carry == 1 or == 3, but I noticed that
		// XOR is more concise:
		result = static_cast<char>(adigit ^ bdigit ^ carry + '0') + result;
		carry = adigit + bdigit + carry >= 2;
	}

	return result;
}
////////////////////////////////////////////////////////////////////////////////////
//125. Valid Palindrome
//always consider empty input
//isalnum(char),toupper(char)
bool isPalindrome(string s) {
	for (int i = 0, j = s.size() - 1; i < j; i++, j--) { // Move 2 pointers from each end until they collide
		while (isalnum(s[i]) == false && i < j) i++; // Increment left pointer if not alphanumeric
		while (isalnum(s[j]) == false && i < j) j--; // Decrement right pointer if no alphanumeric
		if (toupper(s[i]) != toupper(s[j])) return false; // Exit and return error if not match
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
// 22. Generate Parentheses
class Solution {
public:
	vector<string> generateParenthesis(int n) {
		N = 2 * n;
		Add("(", 1, 0);
		return ret;

	}
private:
	void Add(string s, int lef, int rig) {
		if (lef<rig || lef>N / 2 || rig>N / 2)
			return;
		if (lef + rig == N)
			ret.push_back(s);
		string copy = s;
		Add(s += '(', ++lef, rig);
		Add(copy += ')', --lef, ++rig);
	}
	int N;
	vector<string> ret;
};

int main()
{
	std::string str("a");
	std::string str2("b");


	int M = str2.size();

	// create lps[] that will hold the longest prefix suffix
	// values for pattern
	vector<string> strr = { "a","b" };

	Solution myS;
	myS.generateParenthesis(3);

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