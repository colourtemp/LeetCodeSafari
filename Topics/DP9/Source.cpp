#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
//====================================================rolling array save space=============================
///////////////////////////////////////////////////////////////////////////////////////////////////
//64. Minimum Path Sum
//origin
int minPathSum_0(vector<vector<int>>& grid) {
	int l, c;
	if ((l = grid.size()) == 0 || (c = grid[0].size()) == 0)
		return 0;
	vector<vector<int>> minPath(l, vector<int>(c, 0));
	minPath[0][0] = grid[0][0];
	for (int i = 1; i<c; i++)
		minPath[0][i] = minPath[0][i - 1] + grid[0][i];
	for (int i = 1; i<l; i++) {
		minPath[i][0] = minPath[i - 1][0] + grid[i][0];
		for (int j = 1; j<c; j++) 
			minPath[i][j] = min(minPath[i - 1][j], minPath[i][j - 1]) + grid[i][j];
	}
	return minPath[l - 1][c - 1];
}

//rolling array
int minPathSum_01(vector<vector<int>>& grid) {
	int l, c;
	if ((l = grid.size()) == 0 || (c = grid[0].size()) == 0)
		return 0;
	vector<vector<int>> minPath(2, vector<int>(c, 0));
	minPath[0][0] = grid[0][0];
	for (int i = 1; i<c; i++)
		minPath[0][i] = minPath[0][i - 1] + grid[0][i];
	for (int i = 1; i<l; i++) {
		minPath[i % 2][0] = minPath[(i - 1) % 2][0] + grid[i][0];
		for (int j = 1; j<c; j++) 
			minPath[i % 2][j] = min(minPath[(i - 1) % 2][j], minPath[i % 2][j - 1]) + grid[i][j];
	}
	return minPath[(l - 1) % 2][c - 1];
}

////////////////////////////////////////////////////////////////////////////////////////////////
//221. Maximal Square
int maximalSquare_0(vector<vector<char>>& matrix) {
	int l, c, Marx = 0;
	if ((l = matrix.size()) == 0 || (c = matrix[0].size()) == 0)
		return 0;
	vector<vector<int>> Sq(2, vector<int>(c, 0));//store square size of two lines        
	for (int j = 0; j<c; j++) {
		if (matrix[0][j] == '1') {
			Sq[0][j] = 1;
			Marx = max(Marx, 1);
		}
	}
	for (int i = 1; i<l; i++) {
		if (matrix[i][0] == '1') {
			Sq[i % 2][0] = 1;
			Marx = max(Marx, 1);
		}
		else//don't forget
			Sq[i % 2][0] = 0;
		for (int j = 1; j<c; j++) {
			if (matrix[i][j] == '1') {
				Sq[i % 2][j] = min(Sq[(i - 1) % 2][j - 1], min(Sq[(i - 1) % 2][j], Sq[i % 2][j - 1])) + 1;
				Marx = max(Marx, Sq[i % 2][j]);
			}
			else
				Sq[i % 2][j] = 0;
		}
	}
	return Marx*Marx;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//62. Unique Paths
int uniquePaths_0(int m, int n) {
	vector<vector<int>> paths(2, vector<int>(n, 0));
	for (int i = 0; i<n; i++)
		paths[0][i] = 1;
	for (int i = 1; i<m; i++) {
		paths[i % 2][0] = 1;
		for (int j = 1; j<n; j++) {
			paths[i % 2][j] = paths[(i - 1) % 2][j] + paths[i % 2][(j - 1)];
		}
	}
	return paths[(m - 1) % 2][n - 1];
}


//three states still works with 2 dimention
int minDistance(string word1, string word2) {

	int a = word1.size(), b = word2.size();
	vector<vector<int>> dis(2, vector<int>(b + 1, INT_MAX));
	for (int i = 0; i <= b; i++)
		dis[0][i] = i;
	for (int i = 1; i <= a; i++) {
		dis[i % 2][0] = i;
		for (int j = 1; j <= b; j++) {
			if (word1[i - 1] == word2[j - 1])
				dis[i % 2][j] = dis[(i - 1) % 2][j - 1];
			else
				dis[i % 2][j] = min(dis[(i - 1) % 2][j - 1], min(dis[i % 2][j - 1], dis[(i - 1) % 2][j])) + 1;
		}
	}
	return dis[a % 2][b];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//152. Maximum Product Subarray
int maxProduct_0(vector<int>& nums) {
	//minTill[i] == mininum Product Subarray end with nums[i]
	vector<int> minTill(nums.size(), 0);
	vector<int> maxTill(nums.size(), 0);
	minTill[0] = nums[0];
	maxTill[0] = nums[0];

	int GlobalMax = nums[0];
	//considering negative num may envolve,check both max and min
	for (int i = 1; i<nums.size(); i++) {
		minTill[i] = min(nums[i], min(minTill[i - 1] * nums[i], maxTill[i - 1] * nums[i]));
		maxTill[i] = max(nums[i], max(minTill[i - 1] * nums[i], maxTill[i - 1] * nums[i]));
		if (maxTill[i]>GlobalMax)
			GlobalMax = maxTill[i];
	}
	return GlobalMax;
}

//add rolling array 
int maxProduct_1(vector<int>& nums) {
	//minTill[i] == mininum Product Subarray end with nums[i]
	vector<int> minTill(2, 0);
	vector<int> maxTill(2, 0);
	minTill[0] = nums[0];
	maxTill[0] = nums[0];

	int GlobalMax = nums[0];
	//considering negative num may envolve,check both max and min
	for (int i = 1; i<nums.size(); i++) {
		minTill[i % 2] = min(nums[i], min(minTill[(i - 1) % 2] * nums[i], maxTill[(i - 1) % 2] * nums[i]));
		maxTill[i % 2] = max(nums[i], max(minTill[(i - 1) % 2] * nums[i], maxTill[(i - 1) % 2] * nums[i]));
		if (maxTill[i % 2]>GlobalMax)
			GlobalMax = maxTill[i % 2];
	}
	return GlobalMax;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//188. Best Time to Buy and Sell Stock IV
//one transaction means a buy and a sell
/*
208 / 211 test cases passed.
Status: Time Limit Exceeded
*/
int maxProfit_09(int k, vector<int>& prices) {
	if (prices.size()<2 || k<1)
		return 0;

	//MP[i][j] = max profit till day i after j trans
	vector<vector<long>> MP(prices.size(), vector<long>(k + 1, 0));//equals global max

	//max profit of a trade from day a to day b
	auto MPbt = [&](int a, int b) {
		if (a == b)
			return 0;
		int minPrice = prices[a++];
		int maxPro = INT_MIN;
		for (; a <= b; a++) {
			maxPro = max(maxPro, prices[a] - minPrice);
			minPrice = min(prices[a], minPrice);
		}
		if (maxPro<0)//if must loose money, give up this trade
			return 0;
		return maxPro;
	};

	for (int i = 1; i<prices.size(); i++) {
		for (int j = 1; j <= k; j++) {
			for (int n = 0; n <= i; n++) {//try all possible divide point
				//n is the date for j-1 th trans, plus 1 more trans get a possible MP value
				MP[i][j] = max(MP[i][j], MP[n][j - 1] + MPbt(n, i));
			}
		}
	}
	return MP[prices.size() - 1][k];
}

/*
terminate called after throwing an instance of 'std::bad_alloc'
what():  std::bad_alloc
*/
int maxProfit_099(int k, vector<int>& prices) {
	if (prices.size()<2 || k<1)
		return 0;

	//local[i][j] = max profit till day i after j trans, must have trans finish on i th time
	vector<vector<int>> local(prices.size(), vector<int>(k + 1, 0));
	//not require trans finish on i th time
	vector<vector<int>> global(prices.size(), vector<int>(k + 1, 0));

	for (int i = 1; i<prices.size(); i++) {
		int lastday = prices[i] - prices[i - 1];
		for (int j = 1; j <= k; j++) {
			local[i][j] = max(global[i - 1][j - 1] + lastday, local[i - 1][j] + lastday);
			global[i][j] = max(global[i - 1][j], local[i][j]);
		}
	}
	return global[prices.size() - 1][k];
}

//what ??
int maxProfit_1(int k, vector<int> &prices) {
	int len = prices.size();
	if (len<2) return 0;
	if (k>len / 2) { // simple case
		int ans = 0;
		for (int i = 1; i<len; ++i) {
			ans += max(prices[i] - prices[i - 1], 0);
		}
		return ans;
	}
	vector<int> hold(k + 1);
	vector<int> rele(k + 1);
	for (int i = 0; i <= k; ++i) {
		hold[i] = INT_MIN;
		rele[i] = 0;
	}
	int cur;
	for (int i = 0; i<len; ++i) {
		cur = prices[i];
		for (int j = k; j>0; --j) {
			rele[j] = max(rele[j], hold[j] + cur);
			hold[j] = max(hold[j], rele[j - 1] - cur);
		}
	}
	return rele[k];
}

//====================================================memory search==============================================
///////////////////////////////////////////////////////////////////////////////////////////////////
//674. Longest Continuous Increasing Subsequence
class Solution674_0 {
private:
	vector<int> DP;
	int search(vector<int>& nums, int i) {
		if (DP[i] >= 1)//visited
			return DP[i];
		else {//not visited
			if (nums[i - 1] >= nums[i])//smallest state
				DP[i] = 1;
			else//search further
				DP[i] = search(nums, i - 1) + 1;
		}
		return DP[i];
	}
public:
	int findLengthOfLCIS(vector<int>& nums) {
		int n = nums.size();
		if (n == 0)
			return 0;
		DP.resize(n, 0);
		DP[0] = 1;
		int MaxLen = 1;
		for (int i = 0; i<n; i++) {
			DP[i] = search(nums, i);
			//cout<<"DP["<<i<<"]="<<DP[i]<<endl;
			if (DP[i]>MaxLen)
				MaxLen = DP[i];
		}
		return MaxLen;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//87. Scramble String
//memory not intruduced
bool isScramble_0(string s1, string s2) {
	int len = s1.size();
	if (len == 0)
		return true;
	//check if have same chars
	string S1 = s1;
	string S2 = s2;
	sort(S1.begin(), S1.end());
	sort(S2.begin(), S2.end());
	if (S1 != S2)
		return false;
	//check terminal status
	if (len <= 2 && S1 == S2)
		return true;
	//try all kinds of cut then compare sub string
	for (int i = 1; i<len; i++) {
		string s11 = s1.substr(0, i);
		string s12 = s1.substr(i, len - i);
		//if "gr|eat"->"rg|eat"
		string s211 = s2.substr(0, i);
		string s212 = s2.substr(i, len - 1);
		//if "gr|eat"->"eat|gr"
		string s221 = s2.substr(len - i, i);
		string s222 = s2.substr(0, len - i);
		//consider two cutting method
		if (isScramble_0(s11, s211) && isScramble_0(s12, s212))
			return true;
		if (isScramble_0(s11, s221) && isScramble_0(s12, s222))
			return true;
	}
	return false;
}

//add memory, time reduced to O(n^3)
class Solution87_1 {
private:
	string str1, str2;
	bool checkScramble(int x, int y, int k, vector<vector<vector<int>>>& memo) {
		if (memo[x][y][k] == 1)
			return true;
		if (memo[x][y][k] == -1)
			return false;
		if (k == 0) {
			memo[x][y][k] = 1;
			return true;
		}
		string string1 = str1.substr(x, k);
		string string2 = str2.substr(y, k);
		sort(string1.begin(), string1.end());
		sort(string2.begin(), string2.end());
		if (string1 != string2) {
			memo[x][y][k] = -1;
			return false;
		}
		if (k <= 2 && string1 == string2) {
			memo[x][y][k] = 1;
			return true;
		}
		for (int i = 1; i<k; i++) {
			if (checkScramble(x, y, i, memo) && checkScramble(x + i, y + i, k - i, memo)) {
				memo[x][y][k] = 1;
				return true;
			}
			if (checkScramble(x, y + k - i, i, memo) && checkScramble(x + i, y, k - i, memo)) {
				memo[x][y][k] = 1;
				return true;
			}
		}
		memo[x][y][k] = -1;
		return false;
	}

public:
	bool isScramble(string s1, string s2) {
		str1 = s1;
		str2 = s2;
		int n = s1.size();
		if (n <= 1 && s1 == s2)
			return true;
		//memo[x][y][k] store the relationship of k length substrs 
		//of str1(from x th char) and str2(from y th char)
		//0 is not visited,1 is scramble, -1 is not scramble
		vector<vector<vector<int>>> memo(n, vector<vector<int>>(n, vector<int>(n + 1, 0)));
		return checkScramble(0, 0, n, memo);
	}

};
void main() {
	vector<vector<char>> m = { 
							{ '1','1','1','1','1','1','1', '1' },  
							{ '1','1','1','1','1','1','1', '0' } ,
							{ '1','1','1','1','1','1','1', '0' },
							{ '1','1','1','1','1','0','0', '0' },
							{ '0','1','1','1','1','0','0', '0' } };
	vector<int> a = { 3,2,6,5,0,3 };

}