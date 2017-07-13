#include <math.h> //sqrt
#include <climits>//INT_MAX
#include <vector>
#include <algorithm>// min max
#include <unordered_set>
#include <numeric>//accumulate
#include <set>
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
//do some simple math works    -_-#
int minMoves0(vector<int>& nums) {
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

//////////////////////////////////////////////////////////////////////////////
//441. Arranging Coins
int arrangeCoins0(int n) {
	if (n<2)
		return n;
	for (int i = 2; i>0; i++)
	{
		double times = (double)i / 2;
		double sum = times*(i + 1);//consider overflow
		if (sum>n)
			return i - 1;
		else if ((sum == n))
			return i;
	}
}

//math!
/*
-> (1+x)x/2 = n
-> x ^ 2 + x = 2n
->x ^ 2 + x + 1 / 4 = 2n + 1 / 4
-> (x + 1 / 2) ^ 2 = 2n + 1 / 4
-> (x + 0.5) = sqrt(2n + 0.25)
->x = -0.5 + sqrt(2n + 0.25)
*/
int arrangeCoins(int n) {
	return -0.5 + sqrt((double)2 * n + 0.25);
}

//////////////////////////////////////////////////////////////////////////////
//258. Add Digits
//math trivia
int addDigits(int num) {
	if (num == 0)
		return 0;
	else if (num % 9 == 0)
		return 9;
	else
		return num % 9;
}

//////////////////////////////////////////////////////////////////////////////
//263. Ugly Number
//this will get all prime number but slow for current question
//also notice : abs(-2147483648)==-2147483648 INT_MAX==2147483647, INT_MIN=-2147483648
bool isUgly(int num) {
	if (num == 1)
		return true;
	set<int> myset;
	if (num <= 0)
		return false;
	while (num>1) {
		for (int i = 2; i <= num; i++) {
			if (num%i == 0) {
				num = num / i;
				myset.insert(i);
				break;
			}
		}
	}
	myset.erase(2);
	myset.erase(3);
	myset.erase(5);
	if (myset.size()>0)
		return false;
	else
		return true;
}

//smarter
bool isUgly1(int num) {
	for (int i = 2; i<6 && num; i++) {//2,3,4(represent 2),5
		while (num%i == 0)
			num = num / i;
	}
	return num == 1;
}


//////////////////////////////////////////////////////////////////////////////
//231. Power of Two
bool isPowerOfTwo(int n) {
	if (n <= 0)
		return false;
	return !(n&(n - 1));
}

//////////////////////////////////////////////////////////////////////////////
//7. Reverse Integer
int reverse(int x) {
	long res = 0;//consider overflow
	while (x) {
		res = res * 10 + x % 10;
		x /= 10;
	}
	return (res<INT_MIN || res>INT_MAX) ? 0 : res;
}


//////////////////////////////////////////////////////////////////////////////
//326. Power of Three
//it has loop
bool isPowerOfThree0(int n) {
	if (n <= 0)
		return false;
	while (n % 3 == 0)
		n = n / 3;
	if (n == 1)
		return true;
	return false;
}

//math,math,math
//this will not work for none prime numbers
bool isPowerOfThree1(int n) {
	if (n <= 0)
		return false;
	//(int)(log(INT_MAX) / log(3)); get the biggest possible power in INT
	int MaxPowThree = pow(3, (int)(log(INT_MAX) / log(3)));
	if (MaxPowThree%n == 0)
		return true;
	return false;
}

//cannot use log (natural log) here, because it will generate round off error for n=243
bool isPowerOfThree(int n) {
	if (n <= 0)
		return false;
	double m = log10(n) / log10(3);
	if (m - (int)m>0)
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//204. Count Primes
//not fast enough
int countPrimes0(int n) {
	if (n == 0)
		return 0;
	int count = 0;
	for (int i = 2; i < n; i++) {
		//no need to go over i^0.5  the upper side number have be checked by pair until i^0.5
		for (int j = 2; j <= sqrt(i); j++)
			if (i%j == 0) {
				count--;
				break;
			}
		count++;
	}
	return count;
}

//Sieve of Eratosthenes, kick out all the nonprime
int countPrimes(int n) {
	vector<bool> prime(n, true);
	prime[0] = false, prime[1] = false;
	for (int i = 0; i <= sqrt(n); ++i) {//no need to go over sqrt
		if (prime[i]) {
			for (int j = i*i; j < n; j += i) {//n*prime is not prime
				prime[j] = false;
			}
		}
	}
	return count(prime.begin(), prime.end(), true);
}

void main() {
	int b = INT_MIN;

	int t = 2147483649;
	float xx = (log(100) / log(3));
	float x = log(0x7fffffff);
	float y= log(3);
	float a = log(0x7fffffff) / log(3);
	float Max = pow(3, a);
	float Max3 = pow(3, (int)a);

	vector<int> ve = { 1,2,3};
	int A = countPrimes(7);
}