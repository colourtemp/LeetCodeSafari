#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <string>
#include <stack>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
//111. Minimum Depth of Binary Tree
 //Definition for a binary tree node.
 struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

 //go all the way down, consider empty input
 int minDepth(TreeNode *root) {
	 if (!root) return 0;
	 if (!root->left) return 1 + minDepth(root->right);
	 if (!root->right) return 1 + minDepth(root->left);
	 return 1 + min(minDepth(root->left), minDepth(root->right));
 }


/////////////////////////////////////////////////////////////////////////////
//235. Lowest Common Ancestor of a Binary Search Tree
 TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
	 if ((root->val - p->val)*(root->val - q->val) <= 0) {
		 return root;
	 }
	 if ((root->val - p->val)>0)
		 return lowestCommonAncestor(root->left, p, q);
	 else
		 return lowestCommonAncestor(root->right, p, q);
 }

 //iterative
 TreeNode* lowestCommonAncestor1(TreeNode* root, TreeNode* p, TreeNode* q) {
	 TreeNode* cur = root;
	 while (true) {
		 if (p->val < cur->val && q->val < cur->val)
			 cur = cur->left;
		 else if (p->val > cur->val && q->val > cur->val)
			 cur = cur->right;
		 else return cur;
	 }
 }

/////////////////////////////////////////////////////////////////////////////
//501. Find Mode in Binary Search Tree
/*
no matter which situation, must do left current right can secure the continous of same num
	2
2		2
	2
1		2
	2
2		3
	2
1		4
*/
 class Solution501 {
 public:
	 vector<int> findMode(TreeNode* r) {
		 int mfq/*max freq*/, pre/*previous val*/, cnt/*duplicates count*/;
		 getMaxFreq(r, mfq = 0, pre, cnt = 0); // in-order traversal to get max frequency
		 getMode(r, mfq, pre, cnt = 0);      // in-order traversal to get all modes
		 return modes;
	 }
 private:
	 vector<int> modes;//on stack

	 void getMaxFreq(TreeNode* r, int& mfq, int& pre, int& cnt) {//pass by reference to save heap
		 if (!r) return;
		 //left
		 getMaxFreq(r->left, mfq, pre, cnt);
		 //current
		 cnt = cnt*(r->val == pre) + 1;
		 pre = r->val;
		 mfq = max(mfq, cnt);
		 //right
		 getMaxFreq(r->right, mfq, pre, cnt);
	 }

	 void getMode(TreeNode* r, const int mfq, int& pre, int& cnt) {
		 if (!r) return;
		 //left
		 getMode(r->left, mfq, pre, cnt);
		 //current
		 cnt = cnt*(r->val == pre) + 1;
		 pre = r->val;
		 if (mfq == cnt)
			 modes.push_back(r->val);
		 //right
		 getMode(r->right, mfq, pre, cnt);
	 }
 };

 /////////////////////////////////////////////////////////////////////////////
 // 543. Diameter of Binary Tree
 class Solution543 {
 public:
	 int diameterOfBinaryTree(TreeNode* root) {
		 if (root == 0)
			 return 0;
		 int rdep = (getDepth(root->left) + getDepth(root->right));
		 int cdep = max(diameterOfBinaryTree(root->left), diameterOfBinaryTree(root->right));
		 return max(rdep, cdep);
	 }
 private:
	 int getDepth(TreeNode* root) {
		 if (root == 0)
			 return 0;
		 return 1 + max(getDepth(root->left), getDepth(root->right));
	 }
 };

////////////////////////////////////////////////////////////////////////////
//437. Path Sum III
 class Solution437 {
 public:
	 int pathSum(TreeNode* root, int sum) {
		 if (root == 0)//makesure not using a null pointer
			 return 0;
		 return nodeSum(root, sum) + pathSum(root->left, sum) + pathSum(root->right, sum);
	 }
 private:
	 int nodeSum(TreeNode* root, int sum) {//num of correct path starting from current node
		 if (root == 0)
			 return 0;
		 return (sum - root->val == 0) + nodeSum(root->left, sum - root->val) + nodeSum(root->right, sum - root->val);
		 //utilizaing a==b give 1 or 0
	 }
 };

 //smarter, keep track of previous solution
 class Solution437_ {
 public:
	 vector<int> s;
	 int pathSum(TreeNode* root, int sum) {
		 if (!root) return 0;
		 s.push_back(root->val);
		 int count = pathSum(root->left, sum) + pathSum(root->right, sum);
		 int tmp_sum = 0;
		 for (int i = s.size() - 1; i >= 0; i--) {
			 tmp_sum += s[i];
			 if (tmp_sum == sum) count++;
		 }
		 s.pop_back();
		 return count;
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
 //404. Sum of Left Leaves
 int sumOfLeftLeaves(TreeNode* root) {
	 if (root == 0)
		 return 0;
	 if (root->left != 0 && root->left->left == 0 && root->left->right == 0)//when left is a leave
		 return root->left->val + sumOfLeftLeaves(root->right);
	 return sumOfLeftLeaves(root->left) + sumOfLeftLeaves(root->right);//whe left is not leave
 }

 ////////////////////////////////////////////////////////////////////////////
 //538. Convert BST to Greater Tree
 //for BST, right->middle->left goes from big to small
 class Solution538 {
 private:
	 int cur_sum = 0;
 public:
	 void travel(TreeNode* root) {
		 if (!root) return;
		 //right
		 if (root->right) travel(root->right);
		 //curr
		 root->val = (cur_sum += root->val);
		 //left
		 if (root->left) travel(root->left);
	 }
	 TreeNode* convertBST(TreeNode* root) {
		 travel(root);
		 return root;
	 }
 };


 ////////////////////////////////////////////////////////////////////////////
 //637. Average of Levels in Binary Tree
 class Solution637 {
 public:
	 vector<double> averageOfLevels(TreeNode* root) {
		 go(root, 1);
		 for (int i = 0; i<sum.size(); i++)
			 sum[i] /= count[i];
		 return sum;
	 }
 private:
	 vector<double> sum;
	 vector<int>count;
	 void go(TreeNode* node, int level) {
		 if (!node)
			 return;
		 if (sum.size()<level)
			 sum.push_back(0);
		 if (count.size()<level)
			 count.push_back(0);
		 count[level - 1] += 1;
		 sum[level - 1] += node->val;
		 go(node->left, level + 1);
		 go(node->right, level + 1);
		 return;
	 }
 };

 //put each level in a queue and seperate them by nullptr
 vector<double> averageOfLevels(TreeNode* root) {
	 vector<double> ret;
	 double sum = 0, count = 0;
	 queue<TreeNode*> q;
	 q.push(root);
	 q.push(nullptr);
	 while (!q.empty()) {
		 TreeNode* t = q.front();
		 q.pop();
		 if (t == nullptr) {
			 ret.push_back(sum / count);
			 sum = count = 0;
			 if (!q.empty()) q.push(nullptr);
		 }
		 else {
			 sum += t->val;
			 ++count;
			 if (t->left) q.push(t->left);
			 if (t->right) q.push(t->right);
		 }
	 }
	 return ret;
 }


 ////////////////////////////////////////////////////////////////////////////
 //112. Path Sum
 bool hasPathSum(TreeNode* root, int sum) {
	 //check empty root
	 if (!root)
		 return false;
	 //caculate
	 sum -= root->val;
	 //end situation
	 if (!root->left&&!root->right)
		 return sum==0;
	 //recur
	 return (hasPathSum(root->left, sum) || hasPathSum(root->right, sum));
 }

 ////////////////////////////////////////////////////////////////////////////
//257. Binary Tree Paths
 class Solution257 {
 public:
	 vector<string> binaryTreePaths(TreeNode* root) {
		 if (root == 0)
			 return pathCollection;
		 conti("", root);
		 return pathCollection;
	 }
 private:
	 vector<string>pathCollection;
	 void conti(string prev, TreeNode* node) {
		 if (prev != "")
			 prev += "->";
		 prev += to_string(node->val);
		 if (!node->left&&!node->right) {
			 pathCollection.push_back(prev);
			 return;
		 }
		 if (node->left)
			 conti(prev, node->left);
		 if (node->right)
			 conti(prev, node->right);
	 }
 };

 //faster
 class Solution257_2 {
 public:
	 vector<string> binaryTreePaths(TreeNode* root) {
		 if (root == 0)
			 return pathCollection;
		 conti(to_string(root->val), root);
		 return pathCollection;
	 }
 private:
	 vector<string>pathCollection;
	 void conti(string prev, TreeNode* node) {//simplify the recur function
		 if (!node->left&&!node->right) {
			 pathCollection.push_back(prev);
			 return;
		 }
		 if (node->left)
			 conti(prev + "->" + to_string(node->left->val), node->left);
		 if (node->right)
			 conti(prev + "->" + to_string(node->right->val), node->right);
	 }
 };


 ////////////////////////////////////////////////////////////////////////////
 //110. Balanced Binary Tree
 class Solution110 {
 public:
	 bool isBalanced(TreeNode* root) {
		 if (!root)
			 return true;
		 stack<TreeNode*> stack;//loop through tree
		 while (!stack.empty() || root) {
			 while (root) {
				 if (!nodbalance(root))
					 return false;
				 stack.push(root);
				 root = root->left;
			 }
			 if (!stack.empty()) {
				 root = stack.top()->right;
				 stack.pop();
			 }
		 }
		 return true;
	 }

	 bool nodbalance(TreeNode* root) {
		 if (!root)
			 return true;
		 return abs(depth(root->left, 1) - depth(root->right, 1)) <= 1;
	 }

	 int depth(TreeNode* root, int dep) {//accept empty node
		 if (!root)
			 return dep;
		 return max(depth(root->left, dep + 1), depth(root->right, dep + 1));
	 }
 };

 //shorter, recursive call,  O(N^2)
 //calling depth() for its left and right children actually has to access all of its children, thus the complexity is O(N). 
 // We do this for each node in the tree, so the overall complexity of isBalanced will be O(N^2)
 class Solution110_1 {
 public:
	 int depth(TreeNode *root) {
		 if (root == NULL) return 0;
		 return max(depth(root->left), depth(root->right)) + 1;//use +1 so save a arg
	 }

	 bool isBalanced(TreeNode *root) {
		 if (root == NULL) return true;

		 int left = depth(root->left);
		 int right = depth(root->right);

		 return abs(left - right) <= 1 && isBalanced(root->left) && isBalanced(root->right);//recur
	 }
 };

 //smarter, each node in the tree only need to be accessed once O(N)
 class Solution110_2 {
 public:
	 int dfsHeight(TreeNode *root) {
		 if (root == NULL) return 0;

		 //use -1 as flag, return when 
		 int leftHeight = dfsHeight(root->left);
		 if (leftHeight == -1) return -1;
		 int rightHeight = dfsHeight(root->right);
		 if (rightHeight == -1) return -1;

		 if (abs(leftHeight - rightHeight) > 1)  return -1;
		 return max(leftHeight, rightHeight) + 1;
	 }
	 bool isBalanced(TreeNode *root) {
		 return dfsHeight(root) != -1;
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
//108. Convert Sorted Array to Binary Search Tree
 //walk through all middle number
 class Solution108 {
 public:
	 TreeNode* sortedArrayToBST(vector<int>& nums) {
		 if (nums.size() == 0)
			 return 0;
		 Nums = nums;
		 int middle = (nums.size() - 1 - 0) / 2;
		 TreeNode* root = new TreeNode(nums[middle]);
		 growTree(root, middle, 0, nums.size() - 1);
		 return root;
	 }

 private:
	 vector<int> Nums;
	 //left and right are not used number, mid=left+(right-left)/2
	 void growTree(TreeNode* node, int mid, int left, int right) {
		 
		 if (mid == left && mid == right)//end situation is three number all equal
			 return;
		 
		 if (mid != left) {//if one side finished, stop recur
			 int L = left + (mid - 1 - left) / 2;
			 TreeNode* LNode = new TreeNode(Nums[L]);
			 node->left = LNode;
			 growTree(LNode, L, left, mid - 1);//or keep recur on unused range
		 }
		 
		 if (mid != right) {
			 int R = mid + 1 + (right - mid - 1) / 2;
			 TreeNode* RNode = new TreeNode(Nums[R]);
			 node->right = RNode;
			 growTree(RNode, R, mid + 1, right);
		 }
	 }
 };

 //same idea 
 //brute force /2 is ok to go through all middle
 //std::vector<std::string> words2(words1.begin(), words1.end());  //not including last iterator 
 //use more of vactor can save numbering issue
 class Solution108_2 {
 public:
	 TreeNode *sortedArrayToBST(vector<int> &num) {
		 if (num.size() == 0) return NULL;//end situation
		 if (num.size() == 1)//end situation
		 {
			 return new TreeNode(num[0]);
		 }

		 int middle = num.size() / 2;//brute force size/2 
		 TreeNode* root = new TreeNode(num[middle]);

		 vector<int> leftInts(num.begin(), num.begin() + middle);//go through not used vector
		 vector<int> rightInts(num.begin() + middle + 1, num.end());

		 root->left = sortedArrayToBST(leftInts);
		 root->right = sortedArrayToBST(rightInts);

		 return root;
	 }
 };


 ////////////////////////////////////////////////////////////////////////////
//107. Binary Tree Level Order Traversal II
 //walk through the tree without recur
 //  myvector.insert ( myvector.begin() , 200 );//insert to any place in vect
 class Solution107 {
 public:
	 vector<vector<int>> levelOrderBottom(TreeNode* root) {
		 vector<vector<int>> res;
		 if (!root)
			 return res;

		 queue<TreeNode*> NodeQ;
		 NodeQ.push(root);
		 NodeQ.push(0);
		 vector<int> curValue;
		 while (root || NodeQ.size()>1) {
			 while (root) {
				 curValue.push_back(root->val);
				 if (root->left)
					 NodeQ.push(root->left);
				 if (root->right)
					 NodeQ.push(root->right);
				 NodeQ.pop();
				 root = NodeQ.front();
			 }
			 while (NodeQ.size() > 0&& NodeQ.front() == 0 )
				 NodeQ.pop();
			 NodeQ.push(0);
			 root = NodeQ.front();
			 res.insert(res.begin(), curValue);
			 curValue.clear();			 
		 }
		 return res;
	 }
 };

 //recur
 class Solution107_2 {
 public:
	 vector<vector<int> > levelOrderBottom(TreeNode *root) {
		 DFS(root, 0);
		 return vector<vector<int> >(res.rbegin(), res.rend());
	 }
 private:
	 vector<vector<int> > res;
	 void DFS(TreeNode* root, int level)
	 {
		 if (root == NULL) return;
		 if (level == res.size()) // The level does not exist in output
		 {
			 res.push_back(vector<int>()); // Create a new level
		 }

		 res[level].push_back(root->val); // Add the current value to its level
		 DFS(root->left, level + 1); // Go to the next level
		 DFS(root->right, level + 1);
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
 //226. Invert Binary Tree
 TreeNode* invertTree(TreeNode* root) {
	 if (root == 0)
		 return 0;
	 TreeNode* temp = invertTree(root->left);
	 root->left = invertTree(root->right);
	 root->right = temp;
	 return root;
 }

 //non-recur
 TreeNode* invertTree1(TreeNode* root) {
	 std::stack<TreeNode*> stk;
	 stk.push(root);

	 while (!stk.empty()) {
		 TreeNode* p = stk.top();
		 stk.pop();
		 if (p) {
			 stk.push(p->left);
			 stk.push(p->right);
			 std::swap(p->left, p->right);
		 }
	 }
	 return root;
 }

 ////////////////////////////////////////////////////////////////////////////
//572. Subtree of Another Tree
 class Solution572 {
 public:
	 bool isSubtree(TreeNode* s, TreeNode* t) {
		 if (!s&&!t)
			 return true;
		 if (!s || !t)
			 return false;
		 if (sameTree(s, t))
			 return true;
		 return (isSubtree(s->left, t) || isSubtree(s->right, t));

	 }
 private:
	 bool sameTree(TreeNode* s, TreeNode* t) {
		 if (!s&&!t)
			 return true;
		 if (!s || !t)
			 return false;
		 if (s->val != t->val)
			 return false;
		 return (sameTree(s->left, t->left) && sameTree(s->right, t->right));
	 }
 };

 //do some pre check can boot up the speed
 class Solution572_1 {
	 vector<TreeNode*> nodes;

 public:
	 bool isSubtree(TreeNode* s, TreeNode* t) {
		 if (!s && !t) return true;
		 if (!s || !t) return false;

		 getDepth(s, getDepth(t, -1));

		 for (TreeNode* n : nodes)
			 if (identical(n, t))
				 return true;

		 return false;
	 }

	 int getDepth(TreeNode* r, int d) {
		 if (!r)
			 return -1;

		 int depth = max(getDepth(r->left, d), getDepth(r->right, d)) + 1;

		 // Check if depth equals required value
		 // Require depth is -1 for tree t (only return the depth, no push)
		 if (depth == d)
			 nodes.push_back(r);//never wast time to walk through the tree again

		 return depth;
	 }

	 bool identical(TreeNode* a, TreeNode* b) {
		 if (!a && !b) return true;
		 if (!a || !b || a->val != b->val) return false;

		 return identical(a->left, b->left) && identical(a->right, b->right);
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
//104. Maximum Depth of Binary Tree
 //time complexity is O(n), space complexities is O(N) when tree like linked-list
 int maxDepth(TreeNode* root) {
	 if (!root)
		 return 0;
	 else
		 return 1 + max(maxDepth(root->left), maxDepth(root->right));
 }

 //BFS 
 //time complexity is O(n), space complexities is O(N) 
 int maxDepth1(TreeNode *root)
 {
	 if (root == NULL)
		 return 0;

	 int res = 0;
	 queue<TreeNode *> q;
	 q.push(root);
	 while (!q.empty())
	 {
		 ++res;
		 for (int i = 0, n = q.size(); i < n; ++i)
		 {
			 TreeNode *p = q.front();
			 q.pop();

			 if (p->left != NULL)
				 q.push(p->left);
			 if (p->right != NULL)
				 q.push(p->right);
		 }
	 }

	 return res;
 }


 ////////////////////////////////////////////////////////////////////////////
//101. Symmetric Tree
 bool isSymmetric(TreeNode* root) {
	 if (!root)
		 return true;
	 vector<TreeNode*> current;
	 current.push_back(root->left);
	 current.push_back(root->right);
	 while (current.size()>0)
	 {
		 vector<TreeNode*> nex;
		 int csize = current.size();
		 for (int i = 0; i<csize / 2; i++) {
			 if (current[i]) {
				 nex.push_back(current[i]->left);
				 nex.push_back(current[i]->right);
			 }
			 if (!current[i] && !current[csize - 1 - i])
				 continue;
			 if (!current[i] || !current[csize - 1 - i])
				 return false;
			 if (current[i]->val != current[csize - 1 - i]->val)
				 return false;
		 }
		 for (int i = csize / 2; i<csize; i++) {
			 if (current[i]) {
				 nex.push_back(current[i]->left);
				 nex.push_back(current[i]->right);
			 }
		 }
		 current = nex;
	 }
	 return true;
 }

 ////////////////////////////////////////////////////////////////////////////
 //100. Same Tree
 bool isSameTree(TreeNode* p, TreeNode* q) {
	 if (!p || !q)
		 return p==q;//use == return true or false to save a if()
	 if (p->val != q->val)
		 return false;
	 return (isSameTree(p->left, q->left) && isSameTree(q->right, p->right));
 }

 ////////////////////////////////////////////////////////////////////////////
 //563. Binary Tree Tilt
 //easy bus slow
 class Solution563 {
 public:
	 int findTilt(TreeNode* root) {
		 if (!root)
			 return 0;
		 else
			 return abs(getSum(root->left) - getSum(root->right)) + findTilt(root->left) + findTilt(root->right);
	 }

	 int getSum(TreeNode* root) {
		 if (!root)
			 return 0;
		 else
			 return root->val + getSum(root->left) + getSum(root->right);
	 }
 };

 //faster
 class Solution563_2 {
 public:
	 int findTilt(TreeNode* root) {
		 int res = 0;
		 postorder(root, res);
		 return res;
	 }
 private:
	 int postorder(TreeNode* root, int& res) {//do all the job in one circle
		 if (root == NULL) return 0;
		 int leftsum = postorder(root->left, res);
		 int rightsum = postorder(root->right, res);
		 res += abs(leftsum - rightsum);
		 return leftsum + rightsum + root->val;
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
 //617. Merge Two Binary Trees
 //not to create new heap if possible
 TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
	 if (!t1) return t2;
	 if (!t2) return t1;

	 TreeNode* node = new TreeNode(t1->val + t2->val);
	 node->left = mergeTrees(t1->left, t2->left);
	 node->right = mergeTrees(t1->right, t2->right);
	 return node;
 }


 void main() {
	 TreeNode N0(1);
	 TreeNode N1(2);
	 TreeNode N2(2);
	 TreeNode N3(3);
	 TreeNode N4(4);
	 TreeNode N5(4);
	 TreeNode N6(3);

	 N0.left = &N1;
	 N0.right = &N2;
	 N1.left = &N3;
	 N1.right = &N4;
	 N2.left = &N5;
	 N2.right = &N6;


	

	 bool n = isSymmetric(&N0);
 }

