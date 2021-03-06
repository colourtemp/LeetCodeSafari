#include <math.h> //sqrt
#include <climits>//INT_MAX
#include <vector>
#include <algorithm>// min max
#include <unordered_set>
#include <numeric>//accumulate
#include <set>
#include <string> 
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
//Newton��s method
//f(x)=x^2-a; //f(x)==0, x is a's sqrt
//http://blog.csdn.net/tclxspy/article/details/51034653
//closer_x=x-(f(x)/2x)
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
	return n.count(num);
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

//////////////////////////////////////////////////////////////////////////////
//202. Happy Number
//kinda slow
class Solution202 {
public:
	bool isHappy(int n) {
		if (n <= 0)
			return false;
		unordered_set<int> seen;
		while (true) {
			seen.insert(n);
			n = SS(n);
			if (seen.find(n) != seen.end())//not search()
				return false;
			if (n == 1)
				return true;
		}

	}

private:
	int SS(int n) {
		int ss = 0;
		while (n>0) {
			int d = n % 10;
			ss += d*d;
			n /= 10;
		}
		return ss;
	}
};

//////////////////////////////////////////////////////////////////////////////
//172. Factorial Trailing Zeroes
//fail until n==30
int trailingZeroes0(int n) {

	if (n<0)
		n = n*(-1);
	long long Zn = 0, Ln = n;//condiser overflow(still not enough)
	while (n>1) {
		while (Ln % 10 == 0) {
			Ln /= 10;
			Zn++;
		}
		Ln *= (--n);
	}
	return Zn;
}

//math math math
/*
Because all trailing 0 is from factors 5 * 2.
In the n! operation, factors 2 is always ample. So we just count how many 5 factors in all number from 1 to n.
*/
int trailingZeroes1(int n) {
	return n<5 ? 0 : n / 5 + trailingZeroes1(n / 5);
}

//cautious about overflow when multiply
/*
We just count the number of 5 in Equation 1.
Multiple of 5 provides one 5, multiple of 25 provides two 5 and so on.
Note the duplication : multiple of 25 is also multiple of 5, so multiple of 25 only provides one extra 5.
*/
int trailingZeroes2(int n) {
	int result = 0;
	long long i;
	//better than i <= INT_MAX
	for (i = 5; n / i>0; i *= 5) {
		result += (int)(n / i);
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////////
//171. Excel Sheet Column Number
int titleToNumber(string s) {
	int result = 0;
	for (int i = 0; i < s.size(); i++) {
		result = result * 26 + (s.at(i) - 'A' + 1);
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////////
//168. Excel Sheet Column Title
//look into numeric relationship
string convertToTitle(int n) {
	string ret = "";
	while (n>0) {
		char x = 'A' + (n - 1) % 26;
		ret.insert(ret.begin(), x);
		//ret = char((n - 1) % 26 + 'A') + ret;
		n = (n - 1) / 26;;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////////
//367. Valid Perfect Square
//binary search   O(log(n))
bool isPerfectSquare0(int num) {
	int left = 1, right = num;
	while (left <= right) {
		long mid = left + (right - left) / 2;//use long to avoid overflow
		//long mid=(left+right)>>1;
		if (mid*mid>num) {
			right = (int)mid - 1;
		}
		else if (mid*mid<num) {
			left = (int)mid + 1;
		}
		else {
			return true;
		}
	}
	return false;
}

//A square number is 1+3+5+7+...    O(sqrt(n))>O(log(n))
bool isPerfectSquare1(int num) {
	int i = 1;
	while (num>0) {
		num -= i;
		i += 2;
	}
	return num == 0 ? true : false;
}

//Newton method for sqrt
int isPerfectSquare2(int x) {
	long res = x;
	while (res*res>x) {
		res = (res*res + x) / (2 * res);
	}
	return res*res==x;
}

//////////////////////////////////////////////////////////////////////////////
//400. Nth Digit
//std problem solving methodology
int findNthDigit(int n) {
	//get the length of target num
	long len = 1, count = 9;
	while (n - count*len>0) {
		n -= len*count;
		len++;
		count *= 10;//when ever multiplication involves consider overflow
	}
	//get the target number
	int offset = n%len == 0 ? n / len - 1 : n / len;
	int Tnum = pow(10, len - 1) + offset;
	//get the exact index in target number count from right
	int index = n%len;
	if (index == 0)
		index = len;
	for (int i = index; i<len; i++)
		Tnum /= 10;
	return Tnum % 10;
}


//////////////////////////////////////////////////////////////////////////////
//415. Add Strings
//string(1, char)
string addStrings(string num1, string num2) {
	string Long = num1;
	string Short = num2;
	if (num1.size()<num2.size()) {
		Long = num2;
		Short = num1;
	}
	int carry = 0;
	int s = Short.size() - 1, l = Long.size() - 1;
	string result = "";
	for (; s >= 0; s--, l--) {
		int sum = Short[s] + Long[l] - 2 * '0' + carry;
		carry = sum / 10;
		sum %= 10;
		result = string(1, ('0' + sum)) + result;
	}
	for (; l >= 0; l--) {
		int sum = Long[l] - '0' + carry;
		carry = sum / 10;
		sum %= 10;
		result = string(1, ('0' + sum)) + result;
	}
	if (carry>0)
		result = string(1, ('0' + carry)) + result;
	return result;
}

//similar
//to_string(char);reverse(res.begin(), res.end());
string addStrings(string num1, string num2) {
	int i = num1.size() - 1;
	int j = num2.size() - 1;
	int carry = 0;
	string res = "";
	while (i >= 0 || j >= 0 || carry) {
		long sum = 0;
		if (i >= 0) { sum += (num1[i] - '0'); i--; }
		if (j >= 0) { sum += (num2[j] - '0'); j--; }
		sum += carry;
		carry = sum / 10;
		sum = sum % 10;
		res = res + to_string(sum);
	}
	reverse(res.begin(), res.end());
	return res;
}

//less lines
string addStrings1(string num1, string num2) {
	if (num1.size() < num2.size()) return addStrings(num2, num1);//flipe the order smarter
	int carry = 0, i = num1.size() - 1, j = num2.size() - 1;
	for (; i >= 0 && (carry || j >= 0); i--, j--, carry /= 10)//compress all if condition
		num1[i] = (carry += num1[i] - '0' + (j >= 0 ? num2[j] - '0' : 0)) % 10 + '0';
	return (carry ? "1" : "") + num1;
}



/////////////////////////////////////////////////////////////////////////////////////
//634. Find the Derangement of An Array
//refer https://en.wikipedia.org/wiki/Derangement
//math + straight forward DP
int findDerangement_0(int n) {
	if (n == 1)
		return 0;
	if (n == 2)
		return 1;
	int M = 1000000007;
	vector<long long> DP(n + 1);
	DP[1] = 0;
	DP[2] = 1;
	for (int i = 3; i<n + 1; i++) {
		DP[i] = (long long)(i - 1)*(DP[i - 1] + DP[i - 2]) % M;
	}
	return (int)DP[n];
}

//DP add rolling array
int findDerangement_1(int n) {
	if (n == 1)
		return 0;
	if (n == 2)
		return 1;
	int M = 1000000007;
	vector<long long> DP(2);
	DP[0] = 0;
	DP[1] = 1;
	for (int i = 3; i<n + 1; i++) {
		DP[(i - 1) % 2] = (long long)(i - 1)*(DP[0] + DP[1]) % M;
	}
	return (int)DP[(n - 1) % 2];
}

/*******************************************
* about mod 1000000007==pow(10,9)+7        *
********************************************/
/*
A few distributive properties of modulo are as follows:
1. ( a + b ) % c = ( ( a % c ) + ( b % c ) ) % c
2. ( a * b ) % c = ( ( a % c ) * ( b % c ) ) % c
	to avoid overflow either declare a or b as long long int
	OR use explicit type casting (( long long ) a * b ) % c.
3. ( a �C b ) % c = ( ( a % c ) - ( b % c ) ) % c ( see notes below )
	( a �C b ) % c = ( ( a % c ) - ( b % c ) ) %c is fine mathematically.
	But % works differently with negative numbers
	while programming, don't use:
	a=(a%c);
	b=(b%c);
	ans=( a - b )%c;

	instead use:
	a=a%c;
	b=b%c;
	ans = ( a - b + c ) % c;
4. ( a / b ) % c NOT EQUAL TO ( ( a % c ) / ( b % c ) ) % c
	The modulo multiplicative inverse ( MMI ) of a number y is z iff (z * y) % M == 1.

	instead of perform:
		z=(x*(1/y))%M;
	we should perform:
		y2=findMMI(y,M);
		z=(x*y2)%M;

	MMI may be different for different M.
	findMMI using Fermat's Little Theorem: y2==y^(M-2) (mod M)
	int fast_pow(long long base, long long n,long long M) //base^n(mod M)
	{
		if(n==0)
			return 1;
		if(n==1)
			return base;
		long long halfn=fast_pow(base,n/2,M);
		if(n%2==0)
			return ( halfn * halfn ) % M;
		else
			return ( ( ( halfn * halfn ) % M ) * base ) % M;
	}
	int findMMI_fermat(int n,int M)
	{
		return fast_pow(n,M-2,M);//
	}
*/
/*
For M to be a prime number is really important.
1.Because if it is not a prime number then it is possible that the result of a modulo operation may become 0.
Eg. if M=12 and we perform ( 8 * 3 ) % 12, we'll get 0.
But if M is prime then ( ( a % M ) * ( b % M ) ) % M can never be 0 (unless a or b == 0)
2.If M is prime then we can find MMI for any number n such that 1<=n<M
*/


void main() {
	int b = INT_MIN;

//	INT_MAX == 2,147,483,647==2^31-1	   ==0x7FFFFFFF
//	INT_MIN = -2,147,483,648==-((2^31-1)+1)==0x80000000->(-1(0x7FFFFFFF+1))

	vector<int> ve = { 1,2,3};

	float a = sqrt(808201);
	int i= findNthDigit(1000000000);

}