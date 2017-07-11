#include <math.h> //sqrt
#include <climits>//INT_MAX
#include <vector>
#include <algorithm>// min max
#include <unordered_set>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//69. Sqrt(x)
//definate not expected answer;   sqrt()
int mySqrt0(int x) {
	return (int)sqrt(x);
}
//not expected answer;   pow()
int mySqrt1(int x) {
	return (int)pow(x,0.5);
}

//alg play
//INT_MAX
int mySqrt2(int x) {
	if (x == 0)
		return 0;
	int left = 1, right = INT_MAX;
	while (true) {
		int mid = left + (right - left) / 2;
		if (mid>x / mid) {//INT_MAX*INT_MAX will cause overfloating must use divide
			right = mid - 1;
		}
		else {
			if ((mid + 1)>x / (mid + 1))
				return mid;
			left = mid + 1;
		}
	}
}

//bit play
//bit manipulation, add from highest possible bit
int mySqrt3(int x) {
	long res = 0;//avoid overflow
	for (int i = 15; i >= 0; i--)
	{
		if ((res + (1 << i)) * (res + (1 << i)) <= x)
			res += (1 << i);
	}
	return res;
}

//math play
//Newton¡¯s method
//f(x)=x^2-a; //f(x)==0, x is a's sqrt
//http://blog.csdn.net/tclxspy/article/details/51034653
//closer_x=x-f(x)/2x
//        =(2x^2-x^2+a)/2x
//        =(x^2+a)/2x
int mySqrt4(int x) {
	long res = x;
	while (res*res>x) {
		res = (res*res + x) / (2 * res);
	}
	return res;
}


//////////////////////////////////////////////////////////////////////////////
//633. Sum of Square Numbers
bool judgeSquareSum(int c) {
	if (c == 0)//what?_?
		return true;
	int sc = (int)sqrt(c);
	for (long i = 1; i <= sc; i++) {
		int l = c - (int)pow(i, 2);
		int n = (int)sqrt(l);
		if (l == pow(n, 2))
			return true;
	}
	return false;
}

//narrow down is faster
bool judgeSquareSum1(int c) {
	if (c < 0) {
		return false;
	}
	int left = 0, right = (int)sqrt(c);
	while (left <= right) {
		int cur = left * left + right * right;
		if (cur < c) {
			left++;
		}
		else if (cur > c) {
			right--;
		}
		else {
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////
//9. Palindrome Number
//stupid but faster
bool isPalindrome(int x) {
	if (x<0)
		return false;
	long long dig = 0, ten = 1;//100  3  100
	while (x / ten>0) {
		dig++;
		ten = ten * 10;
	}
	ten = ten / 10;
	if (ten == 1 || ten == 0)
		return true;
	int lead, end, left = x;
	for (int i = 0; i <dig / 2; i++) {
		lead = (int)(x / ten);
		lead = lead % 10;
		ten = ten / 10;
		end = left % 10;
		left = left / 10;
		if (lead != end)
			return false;
	}
	return true;
}

//reverse half of the number
bool isPalindrome0(int x) {
	if (x<0 || (x>0 && x % 10 == 0)) return false;
	int rev = 0;
	while (rev<x) {       //121           |1221
		rev = rev * 10 + x % 10;//1     12      |1       12
		x /= 10;          //12    1       |122     12
	}
	if (rev == x || rev / 10 == x)
		return true;
	return false;
}
//////////////////////////////////////////////////////////////////////////////
//598. Range Addition II
//no addtion variable needed
int maxCount(int m, int n, vector<vector<int>>& ops) {
	for (auto op : ops) {
		m = min(op[0], m);
		n = min(op[1], n);
	}
	return m*n;
}

//////////////////////////////////////////////////////////////////////////////
//507. Perfect Number
//perfet number very limited, exhaustion make sense   -_-#
//set.count();
//Integer	4B	-2,147,483,648	2,147,483,647
bool checkPerfectNumber(int num) {
	static unordered_set<int> n = { 6, 28, 496, 8128, 33550336 };
	return (bool)n.count(num);
}

bool checkPerfectNumber1(int num) {
	if (num == 1) return false;

	int sum = 0;
	for (int i = 2; i <= sqrt(num); i++) {
		if (num % i == 0) {
			sum += i;
			if (i != num / i) sum += num / i;
		}
	}
	sum++;

	return sum == num;
}

//////////////////////////////////////////////////////////////////////////////
//453. Minimum Moves to Equal Array Elements
//do some simple math works noy   -_-#
int minMoves(vector<int>& nums) {
	int n = nums.size();
	if (n <= 1)
		return 0;
	int mn = nums[0];
	long sum = nums[0];
	for (int i = 1; i < n; ++i)
	{
		mn = min(mn, nums[i]);
		sum += nums[i];
	}
	return sum - long(mn)*long(n);
}

void main() {
	int b = INT_MAX;

	vector<int> ve = { 1,2,3 };
	int A = minMoves(ve);
}