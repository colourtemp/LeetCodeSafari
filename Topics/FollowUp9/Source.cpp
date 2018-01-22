#include <vector>
#include <queue>
#include <functional>//std::greater
#include <unordered_map>
#include <set>
#include <algorithm>


using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////
//162. Find Peak Element
//binary search
int findPeakElement(vector<int>& nums) {
	int n = nums.size();
	int min = 0, max = n - 1;
	while (min<max - 1) {//make sure mid-1>min
		int mid = min + (max - min) / 2;
		if (nums[mid]>nums[mid - 1] && nums[mid]>nums[mid + 1])
			return mid;
		if (nums[mid]>nums[mid - 1])
			min = mid + 1;
		else
			max = mid - 1;
	}
	return nums[min]>nums[max] ? min : max;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//378. Kth Smallest Element in a Sorted Matrix
class Solution378_0 {
private:
	int line;
	int colum;
	int lc2n(int l, int c) {
		return l*colum + c;
	}
public:
	int kthSmallest(vector<vector<int>>& matrix, int k) {
		if ((line = matrix.size()) == 0)
			return 0;
		colum = matrix[0].size();
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> PQ;//value, position num
		for (int i = 0; i<colum; i++)
			PQ.emplace(matrix[i][0], lc2n(i, 0));
		while (--k) {
			int t = PQ.top().second;
			//cout<<"pop"<<PQ.top().first<<endl;
			int nl = t / colum;
			int nc = t%colum + 1;
			PQ.pop();
			if (nc<colum)
				PQ.emplace(matrix[nl][nc], lc2n(nl, nc));
		}
		return PQ.top().first;
	}
};

//binary search 
int kthSmallest_1(vector<vector<int>>& matrix, int k) {
	int n = matrix.size();
	int l = matrix[0][0], r = matrix[n - 1][n - 1], mid;
	//Because the loop invariant is left<=Solution<=right 
	//when while stops l must be left==Solution==right 
	while (l<r) {
		mid = l + r >> 1;//equals (l+r)/2
		int cnt = 0, j = n - 1;
		for (int i = 0; i<n; i++) {
			while (j >= 0 && matrix[i][j]>mid)
				j--;
			cnt += j + 1;
		}
		if (cnt<k)
			l = mid + 1;
		else
			r = mid;
	}
	return l;
}

/////////////////////////////////////////////////////////////////////////////////////////
//560. Subarray Sum Equals K
int subarraySum(vector<int>& nums, int k) {
	int n = nums.size();
	for (int i = 1; i<n; i++)
		nums[i] = nums[i] + nums[i - 1];
	unordered_map<int, int> hash;//presum, number of presum
	hash.emplace(0, 1);
	int ret = 0;
	for (int j = 0; j<n; j++) {
		if (hash.find(nums[j] - k) != hash.end())
			ret += hash[nums[j] - k];
		hash[nums[j]]++;
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////
//363. Max Sum of Rectangle No Larger Than K
class Solution363_0 {
private:
	int ret;
	void refresh(const vector<int>& merge, long k) {
		for (int i = merge.size() - 1; i >= 1; i--) {
			for (int j = i - 1; j >= 0; j--) {
				long subsum = merge[i] - merge[j];//use long to prevent overflow
				if ((k - subsum) >= 0 && (k - subsum<k - ret))
					ret = subsum;
			}
		}
	}
public:
	int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
		int l, c;
		ret = INT_MIN;
		if ((l = matrix.size())<1 || (c = matrix[0].size())<1)
			return ret;
		//do presum
		vector<vector<int>> presum = matrix;;
		for (int L = 1; L<l; L++) {
			for (int C = 0; C<c; C++)
				presum[L][C] = presum[L][C] + presum[L - 1][C];
		}
		//walk through all line choices
		for (int i = 0; i<l; i++) {
			for (int j = i; j<l; j++) {
				//merge selected lines to one presum line
				vector<int> merge(c, 0);
				for (int k = 0; k<c; k++) {
					merge[k] = (i == 0) ? presum[j][k] : presum[j][k] - presum[i - 1][k];
					if (k>0)
						merge[k] = merge[k] + merge[k - 1];
				}
				merge.insert(merge.begin(), 0);
				//search for all max no larger than
				refresh(merge, k);
			}
		}
		return ret;
	}
};

//set.lower_bound(n); it<=n;
//set.upper_bound(n); it>n;
int maxSumSubmatrix_01(vector<vector<int>>& matrix, int k) {
	int l, c;
	int ret = INT_MIN;
	if ((l = matrix.size())<1 || (c = matrix[0].size())<1)
		return ret;
	//do presum
	vector<vector<int>> presum = matrix;;
	for (int L = 1; L<l; L++) {
		for (int C = 0; C<c; C++)
			presum[L][C] = presum[L][C] + presum[L - 1][C];
	}
	presum.insert(presum.begin(), vector<int>(c, 0));
	//walk through all line choices
	for (int i = 1; i <= l; i++) {
		for (int j = i; j <= l; j++) {
			//merge selected lines to one presum line
			vector<int> merge(c, 0);
			for (int m = 0; m<c; m++) {
				merge[m] = presum[j][m] - presum[i - 1][m];
				if (m>0)
					merge[m] = merge[m] + merge[m - 1];
			}
			//search for max no larger than
			set<int> prevSet;
			prevSet.insert(0);
			for (int n = 0; n<c; n++) {
				auto it = prevSet.lower_bound(merge[n] - k);//return smaller or euqal to 
				if (it != prevSet.end())
					ret = max(ret, merge[n] - *it);
				prevSet.insert(merge[n]);
			}
		}
	}
	return ret;
}

//not doing pre sum, swap colum and line
//O(l^2*r*log r)
int maxSumSubmatrix_1(vector<vector<int>>& matrix, int k) {
	if (matrix.empty()) return 0;
	int row = matrix.size(), col = matrix[0].size(), res = INT_MIN;
	for (int l = 0; l < col; ++l) {//walk through start colum --O(l)
		vector<int> sums(row, 0);//merged selected colums as one
		for (int r = l; r < col; ++r) {//walk though ending colum --O(l)
			for (int i = 0; i < row; ++i) {//--O(r)
				sums[i] += matrix[i][r];
			}
			// Find the max subarray no more than K 
			set<int> accuSet;
			accuSet.insert(0);
			int curSum = 0, curMax = INT_MIN;
			for (int sum : sums) {//--O(r)
				curSum += sum;
				set<int>::iterator it = accuSet.lower_bound(curSum - k);//-O(log r)
				if (it != accuSet.end()) curMax = std::max(curMax, curSum - *it);
				accuSet.insert(curSum);
			}
			res = std::max(res, curMax);
		}
	}
	return res;
}