#include <iostream>       // std::cout
#include <string>         // std::string
#include <vector>
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
	int i = 1;//suffix do not include 0, make the "0*******"
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

int main()
{
	std::string str("AAAAABAAAAA");
	std::string str2("AAAA");


	int M = str2.size();

	// create lps[] that will hold the longest prefix suffix
	// values for pattern
	vector<int> lps(M,0);

	findAll(str,str2);

	return 0;
}