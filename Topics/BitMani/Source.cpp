#include<iostream>
#include<string>
#include<algorithm>
#include<vector> //emplace_back
#include <bitset>         // std::bitset

using namespace std;
/*
Hex:	0x	 
Binary: 0b
Octal:	0	
Decimal:do not start with 'zero'.
*/


//////////////////////////////////////////////////////////////////////
//342. Power of Four
bool isPowerOfFour(int num) {
	if (num <= 0)
		return false;
	if ((num&(num - 1)) != 0)
		return false;
	//0b01010101010101010101010101010101=1431655765=0x55555555
	if ((num & 0x55555555) == 0)//== as priority over &
		return false;
	return true;
}

//math trick
/*
(4^n - 1) % 3 == 0
another proof:
(1) 4^n - 1 = (2^n + 1) * (2^n - 1)
(2) among any 3 consecutive numbers, there must be one that is a multiple of 3
among (2^n-1), (2^n), (2^n+1), one of them must be a multiple of 3, and (2^n) cannot be the one, 
therefore either (2^n-1) or (2^n+1) must be a multiple of 3, 
and 4^n-1 must be a multiple of 3 as well.
*/
bool isPowerOfFour_1(int num) {
	return num > 0 && (num & (num - 1)) == 0 && (num - 1) % 3 == 0;
}

//////////////////////////////////////////////////////////////////////
//191. Number of 1 Bits
int hammingWeight_1(uint32_t n) {//shift and check last bit
	int sum = 0;
	while (n>0) {
		if ((n & 1) == 1)
			sum++;
		n /= 2;;
	}
	return sum;
}

//faster
int hammingWeight_2(uint32_t n) {//shift and check last bit
	int res = 0;
	for (int i = 0; i<32; i++) {
		if (((n >> i) & 1) == 1) {
			res++;
		}
	}
	return res;
}

//Each time of "n &= n - 1", we delete one '1' from n.
int hammingWeight_3(uint32_t n)
{
	int res = 0;
	while (n)
	{
		n &= n - 1;
		++res;
	}
	return res;
}

//n ^= n&-n delete one '1' from n.
int hammingWeight_4(uint32_t n)
{
	int res = 0;
	while (n)
	{
		n ^= n&(~n+1);
		++res;
	}
	return res;
}

//folding technic
/*eg:
0000 0000 0000 0000 0000 0000 1100 1011		n=203
0101 0101 0101 0101 0101 0101 0101 0101		0x55555555
------------------------------------------
0000 0000 0000 0000 0000 0000 0100 0001			a=65
0000 0000 0000 0000 0000 0000 0100 0101			b=69

0000 0000 0000 0000 0000 0000 10|00 01|10	n=134
							  2 + 0  1+2
0011 0011 0011 0011 0011 0011 0011 0011		0x33333333
--------------------------------------------
0000 0000 0000 0000 0000 0000 0000 0010			a=2
0000 0000 0000 0000 0000 0000 0010 0001			b=33

0000 0000 0000 0000 0000 0000 0010| 0011	n=35
								2 +   3
0000 1111 0000 1111 0000 1111 0000 1111		0x0F0F0F0F
---------------------------------------------
0000 0000 0000 0000 0000 0000 0000 0011			a=3
0000 0000 0000 0000 0000 0000 0000 0010 		b=2

0000 0000 0000 0000 0000 0000 |0000 0101	n=5
									 5
*/
int hammingWeight(uint32_t n)
{
	n = (n & 0x55555555) + (n >> 1 & 0x55555555); // fold every 2 bits
	//0101 0101 0101 0101 0101 0101 0101 0101
	n = (n & 0x33333333) + (n >> 2 & 0x33333333); // fold every 4 bits 
	//0011 0011 0011 0011 0011 0011 0011 0011
	n = (n & 0x0F0F0F0F) + (n >> 4 & 0x0F0F0F0F); // fold every 8 bits 
	//0000 1111 0000 1111 0000 1111 0000 1111
	n = (n & 0x00FF00FF) + (n >> 8 & 0x00FF00FF); // fold every 16 bits 
	//0000 0000 1111 1111 0000 0000 1111 1111
	n = (n & 0x0000FFFF) + (n >> 16 & 0x0000FFFF);// fold every 32 bits 
	//0000 0000 0000 0000 1111 1111 1111 1111
	return n;
}


//////////////////////////////////////////////////////////////////////
//461. Hamming Distance
int hammingDistance_0(int x, int y) {
	x = x^y;//don't allocate more varient if possible
	int c = 0;
	while (x) {
		c++;
		x &= (x - 1);
	}
	return c;
}

/*
int hammingDistance_1(int x, int y) {
	x ^= y;
	//There is NO __builtin_popcount in c++, it's a built in function of GCC.
	//It returns the numbers of set bits in an integer
	return __builtin_popcount(x);
}
*/


//////////////////////////////////////////////////////////////////////
//405. Convert a Number to Hexadecimal
const string HEX = "0123456789abcdef";//prestore the known info
class Solution {
public:
	string toHex(int num) {
		if (num == 0) return "0";
		string result;
		int count = 0;
		//must count length, >>will fill left with f, may create endless loop
		while (num && count++ < 8) {
			result = HEX[(num & 0xf)] + result;
			num >>= 4;
		}
		return result;
	}
};

//////////////////////////////////////////////////////////////////////
//190. Reverse Bits
//O(log sizeof(int))
uint32_t reverseBits_0(uint32_t n) {
	n = ((n >> 1) & 0x55555555) | ((n << 1) & 0xaaaaaaaa);
	n = ((n >> 2) & 0x33333333) | ((n << 2) & 0xcccccccc);
	n = ((n >> 4) & 0x0f0f0f0f) | ((n << 4) & 0xf0f0f0f0);
	n = ((n >> 8) & 0x00ff00ff) | ((n << 8) & 0xff00ff00);
	n = ((n >> 16) & 0x0000ffff) | ((n << 16) & 0xffff0000);
	return n;
}

//in other words
uint32_t  reverseBits_1(uint32_t n) {
	uint32_t result = 0;
	for (int i = 0; i<32; i++)
		result = (result << 1) + (n >> i & 1);
	return result;
}


//////////////////////////////////////////////////////////////////////
//476. Number Complement
int findComplement_0(int num) {
	uint32_t ret = ~num;
	int big;
	while (num>0) {
		big = (num&-num);
		num = (num&(num - 1));
	}
	return ret%big;
}

/*
num          = 00000101
mask         = 11111111
					^ ^ overlap!
num          = 00000101
mask         = 11111110
					^   overlap!
num          = 00000101
mask         = 11111100
					^   overlap!
num          = 00000101
mask         = 11111000
						clear!
*/
int findComplement_1(int num) {
	unsigned mask = ~0;
	while (num & mask) mask <<= 1;
	return ~mask & ~num;
}

//////////////////////////////////////////////////////////////////////
//401. Binary Watch
/*
std::bitset<16> foo;							//foo: 0000000000000000
std::bitset<16> bar (0xfa2);					//bar: 0000111110100010
std::bitset<16> baz (std::string("0101111001"));//baz: 0000000101111001

bitset.count();

//save the effort to construct then copy/move the obj which push_back uses
vector.emplace_back()    //cpp11
*/
vector<string> readBinaryWatch_0(int num) {
	vector<string> rs;
	for (int h = 0; h < 12; h++)
		for (int m = 0; m < 60; m++)
			if (bitset<10>(h << 6 | m).count() == num)
				rs.emplace_back(to_string(h) + (m < 10 ? ":0" : ":") + to_string(m));
	return rs;
}


//////////////////////////////////////////////////////////////////////
//371. Sum of Two Integers
int getSum_0(int a, int b) {
	if (b == 0)
		return a;
	return getSum_0(a^b, (a&b) << 1);
}

//no recur
int getSum(int a, int b) {
	int sum = a;

	while (b != 0)
	{
		sum = a ^ b;//calculate sum of a and b without thinking the carry 
		b = (a & b) << 1;//calculate the carry
		a = sum;//add sum(without carry) and carry
	}

	return sum;
}

void main() {
	cout << "testing" << endl;

	int x = ((int)~0) >> 1;// == 11111111111111111111111111111111
	int y = ((int)~0) << 1;// == 11111111111111111111111111111110

	int t = 1, T = 1;
	//result after shifting left(or right) too much is undefined
	t >>= 1;	//0000 0000 0000 0000==0
	T <<= 33;	//0000 0000 0000 0010==2  this go over the 32 bit and goes like circle

	//right shifting operations on negative values are undefined
	t = -2;		//1111 1111 1111 1110
	t >>= 2;	//1111 1111 1111 1111 ==-1 insert 1 to the left of int

	//The & and | operators have lower precedence than comparison operators
	t =(0 == 1 | 3);	//t=3
	t =(0==(1 | 3));	//t=0
}

//other tricks
//=================================================================================
//Swap two intergers without any extra space
//a ^= b; b ^= a; a ^= b;	

//return first(right most) bit set
//c & -c // c & (~c + 1)	

//return first unset bit
//~c & (c + 1)				

//Count the number of bits in a bitfield
//fold by 2.4.8.16 bits

/*
n = (n & 0x55555555) + ((n & 0xaaaaaaaa) >> 1);
n = (n & 0x33333333) + ((n & 0xcccccccc) >> 2);
n = (n & 0x0f0f0f0f) + ((n & 0xf0f0f0f0) >> 4);
n = (n & 0x00ff00ff) + ((n & 0xff00ff00) >> 8);
n = (n & 0x0000ffff) + ((n & 0xffff0000) >> 16);
*/


//Reverse the bits in a word.
//swap left right by 1.2.4.8.16.32
/*
n = ((n >>  1) & 0x55555555) | ((n <<  1) & 0xaaaaaaaa);
n = ((n >>  2) & 0x33333333) | ((n <<  2) & 0xcccccccc);
n = ((n >>  4) & 0x0f0f0f0f) | ((n <<  4) & 0xf0f0f0f0);
n = ((n >>  8) & 0x00ff00ff) | ((n <<  8) & 0xff00ff00);
n = ((n >> 16) & 0x0000ffff) | ((n << 16) & 0xffff0000);
*/

//unsigned x = ((unsigned)~0) >> 1;// == 01111111111111111111111111111111==2^31-1
//unsigned y = ((unsigned)~0) << 1;// == 11111111111111111111111111111110==2^32-1-1

//int x = ((int)~0) >> 1;// == 11111111111111111111111111111111
//int y = ((int)~0) << 1;// == 11111111111111111111111111111110

/*
Set union			A | B
Set intersection	A & B
Set subtraction		A & ~B
Set negation		ALL_BITS ^ A or ~A
Set bit				A |= 1 << bit
Clear bit			A &= ~(1 << bit)
Test bit			(A & 1 << bit) != 0
Extract last bit	A&-A or A&~(A - 1) or x ^ (x&(x - 1))
Remove last bit		A&(A - 1)
Get all 1 - bits	~0
*/
