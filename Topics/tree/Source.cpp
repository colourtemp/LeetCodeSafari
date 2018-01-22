#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using namespace std;

//====================================================Tree easy=====================================================================

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

 ////////////////////////////////////////////////////////////////////////////
 //310. Minimum Height Trees
 //modified topological sort BFS
 //keep deleting leaves layer-by-layer, until reach the root.
 vector<int> findMinHeightTrees_0(int n, vector<pair<int, int>>& edges) {
	 if (n == 1)
		 return{ 0 };
	 //build adjacent path
	 vector<unordered_set<int>> adj(n);
	 vector<int> height0;
	 for (auto& e : edges) {
		 adj[e.first].insert(e.second);
		 adj[e.second].insert(e.first);
	 }
	 //find leaf, 
	 for (int i = 0; i<n; i++) {
		 if (adj[i].size() == 1)
			 height0.push_back(i);
	 }
	 //delete leaf layer by layer
	 while (true) {
		 vector<int> height1;
		 for (auto& leaf : height0) {
			 for (auto& upper : adj[leaf]) {
				 adj[upper].erase(leaf);
				 if (adj[upper].size() == 1)
					 height1.push_back(upper);
			 }
		 }
		 if (height1.size() == 0)
			 return height0;
		 height0 = height1;
	 }
 }

 //====================================================Tree Medium=====================================================================

 //////////////////////////////////////////////////////////////
 //144. Binary Tree Preorder Traversal
 //recursive
 class Solution144_0 {
 private:
	 void preorder(TreeNode* root, vector<int>& ret) {
		 if (root) {
			 ret.push_back(root->val);
			 preorder(root->left, ret);
			 preorder(root->right, ret);
		 }
	 }
 public:
	 vector<int> preorderTraversal(TreeNode* root) {
		 vector<int> ret;
		 preorder(root, ret);
		 return ret;
	 }
 };

 //iterative
 vector<int> preorderTraversal_1(TreeNode *root) {
	 vector<int> ret;
	 stack<TreeNode*> s;
	 while (root || s.size()) {
		 if (root) {
			 ret.push_back(root->val);//out put at first meet
			 s.push(root);
			 root = root->left;
		 }
		 else {
			 root = s.top();
			 s.pop();
			 root = root->right;
		 }
	 }
	 return ret;
 }

 //Morris traversal ¡ª O(n) time and O(1) space
 //http://www.cnblogs.com/AnnieKim/archive/2013/06/15/MorrisTraversal.html
 vector<int> preorderTraversal_2(TreeNode *root) {
	 vector<int> ret;
	 TreeNode* cur = root, *pre = NULL;//prev node if inorder traverse
	 while (cur) {
		 if (cur->left) {
			 pre = cur->left;
			 while (pre->right&&pre->right != cur)
				 pre = pre->right;
			 if (pre->right == NULL) {//first visit cur
				 ret.push_back(cur->val);
				 pre->right = cur;
				 cur = cur->left;
			 }
			 else if (pre->right == cur) {//second visit cur
				 pre->right = NULL;
				 cur = cur->right;
			 }
		 }
		 else {//left most
			 ret.push_back(cur->val);
			 cur = cur->right;
		 }
	 }
	 return ret;
 }


 ////////////////////////////////////////////////////////////////////////////
 //94. Binary Tree Inorder Traversal
 //Recursive solution ¡ª O(n) time and O(n) space (considering the spaces of function call stack);
 class Solution94_0 {
 private:
	 void inorder(TreeNode* root, vector<int>& ret) {
		 if (!root)
			 return;
		 inorder(root->left, ret);
		 ret.push_back(root->val);
		 inorder(root->right, ret);
	 }
 public:
	 vector<int> inorderTraversal(TreeNode* root) {
		 vector<int> ret;
		 inorder(root, ret);
		 return ret;
	 }
 };

 //Iterative solution using stack ¡ª O(n) time and O(n) space;
 vector<int> inorderTraversal1(TreeNode* root) {
	 vector<int> ret;
	 stack<TreeNode*> s;
	 while (root || s.size()) {
		 if (root) {
			 s.push(root);
			 root = root->left;
		 }
		 else {
			 root = s.top();
			 s.pop();
			 ret.push_back(root->val);//output at second meet
			 root = root->right;
		 }
	 }
	 return ret;
 }

 //Morris traversal ¡ª O(n) time and O(1) space!!!
 //http://www.cnblogs.com/AnnieKim/archive/2013/06/15/MorrisTraversal.html
 vector<int> inorderTraversal_1(TreeNode* root) {
	 //current and previous node by inorder
	 TreeNode* cur = root, *pre = NULL;
	 vector<int> ret;
	 while (cur) {
		 //if cur have left
		 if (cur->left) {
			 //get pre node if inorder traverse
			 pre = cur->left;
			 while (pre->right&&pre->right != cur)
				 pre = pre->right;
			 //if first visit cur
			 //set the trace and go left child
			 if (pre->right == NULL) {
				 pre->right = cur;
				 cur = cur->left;
			 }
			 else {
				 //else is second visit cur
				 //erase the trace and go right
				 ret.push_back(cur->val);
				 pre->right = NULL;
				 cur = cur->right;
			 }
		 }
		 else {
			 //else cur is the left most
			 //go right
			 ret.push_back(cur->val);
			 cur = cur->right;
		 }
	 }
	 return ret;
 }


 //////////////////////////////////////////////////////////////
 //145. Binary Tree Postorder Traversal
 //iterative use a stack and a marker Node
 vector<int> postorderTraversal4(TreeNode* root) {
	 vector<int> ret;
	 stack<TreeNode*> s;
	 //if post order, root will always go direct after it's right child
	 //to check if a root has done right child job, just track last node
	 TreeNode* walker = root;
	 TreeNode* lastPop = NULL;
	 while (walker || s.size()) {
		 if (walker) {
			 s.push(walker);
			 walker = walker->left;
		 }
		 else {
			 TreeNode* topNode = s.top();
			 if (topNode->right&&lastPop != topNode->right)//mean this root need delay
				 walker = topNode->right;
			 else {
				 ret.push_back(topNode->val);
				 s.pop();
				 lastPop = topNode;
			 }
		 }
	 }
	 return ret;
 }

 //Morris traversal- the key is reversing left node to it's right most
 vector<int> postorderTraversal1(TreeNode* root) {
	 vector<int> ret;
	 TreeNode dummy(0);//dummy is necessary if root no child
	 dummy.left = root;
	 TreeNode* cur = &dummy, *pre = NULL;
	 while (cur) {
		 if (cur->left) {
			 pre = cur->left;
			 while (pre->right&&pre->right != cur)
				 pre = pre->right;
			 if (pre->right == NULL) {
				 pre->right = cur;
				 cur = cur->left;
			 }
			 else if (pre->right == cur) {
				 //reverse output the path from cur->left to pre
				 TreeNode* temp = cur->left;
				 /*
				 stack<int> s;
				 while(temp!=cur){
				 s.push(temp->val);
				 temp=temp->right;
				 }
				 while(s.size()){
				 ret.push_back(s.top());
				 s.pop();
				 }  */
				 int pos = ret.size();
				 while (temp != cur) {
					 ret.insert(ret.begin() + pos, temp->val);
					 temp = temp->right;
				 }

				 pre->right = NULL;
				 cur = cur->right;
			 }
		 }
		 else {
			 cur = cur->right;
		 }
	 }
	 return ret;
 }

 //reverse left-right of preorder and reverse output ->Morris
 vector<int> postorderTraversal2(TreeNode* root) {
	 vector<int> ret;
	 TreeNode* cur = root, *pre = NULL;
	 while (cur) {
		 if (cur->right) {
			 pre = cur->right;
			 while (pre->left&&pre->left != cur)
				 pre = pre->left;
			 if (pre->left == NULL) {
				 ret.insert(ret.begin(), cur->val);
				 pre->left = cur;
				 cur = cur->right;
			 }
			 else if (pre->left == cur) {
				 pre->left = NULL;
				 cur = cur->left;
			 }
		 }
		 else {
			 ret.insert(ret.begin(), cur->val);
			 cur = cur->left;
		 }
	 }
	 return ret;
 }

 //reverse left-right of preorder and reverse output ->stack
 vector<int> postorderTraversal3(TreeNode* root) {
	 vector<int> ret;
	 stack<TreeNode*> s;
	 while (root || s.size()) {
		 if (root) {
			 ret.insert(ret.begin(), root->val);
			 s.push(root);
			 root = root->right;
		 }
		 else {
			 root = s.top();
			 s.pop();
			 root = root->left;
		 }
	 }
	 return ret;
 }

 //////////////////////////////////////////////////////////////
 //255. Verify Preorder Sequence in Binary Search Tree

 bool verifyPreorder1(vector<int>& preorder) {
	 /*
	 keeping a stack of nodes (just values) of which we¡¯re 
	 still in the left subtree
	 */
	 stack<int> path;
	 int low = INT_MIN;
	 for (auto& p : preorder) {
		 if (p<low)
			 return false;
		 /*
		 2.
		 If the current number is bigger than the stack top,
		 then we¡¯re their right subtree. So pop all smaller ancestor
		 3.
		 use the popped values as a lower bound, since being in their right subtree 
		 means following number should be bigger than them
		 */
		 while (path.size() && p>path.top()) {
			 low = path.top();
			 path.pop();
		 }
		 /*
		 1.
		 If the current number is smaller than the stack value,
		 then we¡¯re still in the left subtree of all stack nodes
		 */
		 path.push(p);
	 }
	 return true;
 }

 //treating used(will not visit again) part of input vector as stack
 bool verifyPreorder2(vector<int>& preorder) {
	 int leftPa = -1;
	 int low = INT_MIN;
	 for (auto& p : preorder) {
		 if (p<low)
			 return false;
		 while (leftPa >= 0 && p>preorder[leftPa]) {
			 low = preorder[leftPa];
			 leftPa--;
		 }
		 leftPa++;
		 preorder[leftPa] = p;
	 }
	 return true;
 }

 //////////////////////////////////////////////////////////////
 //95. Unique Binary Search Trees II
 //DP
 //be brave enough to modify existing things
 class Solution95_1 {
 private:
	 TreeNode* clone(TreeNode* root) {
		 if (!root)
			 return NULL;
		 TreeNode* clone_root = new TreeNode(root->val);
		 clone_root->left = clone(root->left);
		 clone_root->right = clone(root->right);
		 return clone_root;
	 }
 public:
	 vector<TreeNode*> generateTrees(int n) {
		 if (n == 0)
			 return vector<TreeNode*>();
		 // construct the result of n node tree just from the result of n-1 node tree.
		 vector<TreeNode*> prevTrees(1, NULL);//prevTrees[0]==NULL, when no node, no tree
		 for (int num = 1; num <= n; num++) {//bigger num alway added later can lead 2 cases
			 vector<TreeNode*> curTrees;
			 for (int p = 0; p<prevTrees.size(); p++) {
				 //case 1: The nth node is the new root, so new_root->left = prev_root;
				 TreeNode* prev_root = prevTrees[p];
				 TreeNode* new_root = new TreeNode(num);
				 new_root->left = clone(prev_root);
				 curTrees.push_back(new_root);
				 //case 2: traverse the old tree, use new node to replace the old right child
				 //new_node->left=old_root->right, old_root->right=new_node
				 TreeNode* tem_root = prev_root;
				 while (tem_root != NULL) {
					 //modify the old tree, do clone and store clone
					 TreeNode* new_node = new TreeNode(num);
					 TreeNode* tem_right = tem_root->right;
					 tem_root->right = new_node;
					 new_node->left = tem_right;
					 curTrees.push_back(clone(prev_root));
					 //restore old tree, make tem_root one step further
					 tem_root->right = tem_right;
					 new_node->right = NULL;
					 delete new_node;//avoid memory leak
					 tem_root = tem_right;
				 }
			 }
			 prevTrees = curTrees;
		 }
		 return prevTrees;
	 }
 };

 //////////////////////////////////////////////////////////////
 //96. Unique Binary Search Trees
 class Solution96_0 {
 private:
	 unordered_map<int, int> gesture;//gesture[a]==b : a various number can form b unique BST gesture
 public:
	 int numTrees(int n) {
		 if (n <= 2)
			 return n;
		 auto it = gesture.find(n);
		 if (it != gesture.end())
			 return it->second;
		 int total = 0;
		 for (int root = 1; root <= n; root++) {
			 int left = numTrees(root - 1);
			 int right = numTrees(n - root);
			 if (left&&right)
				 total += left*right;
			 else {
				 total += left;
				 total += right;
			 }
		 }
		 gesture[n] = total;
		 return total;
	 }
 };

 //better DP
 int numTrees1(int n) {
	 vector<int> dp(n + 1, 0);
	 dp[0] = dp[1] = 1;
	 for (int cur = 2; cur <= n; cur++) {
		 for (int temp = 1; temp <= cur; temp++) {
			 dp[cur] += dp[temp - 1] * dp[cur - temp];
		 }
	 }
	 return dp[n];
 }

 //////////////////////////////////////////////////////////////
 //536. Construct Binary Tree from String
 TreeNode* str2tree0(string s) {
	 if (s == "")
		 return NULL;
	 int j = s.find("(");
	 if (j == -1) {
		 TreeNode* node = new TreeNode(stoi(s, nullptr, 10));
		 return node;
	 }
	 int brac = 0;
	 string num = s.substr(0, j);
	 TreeNode* node = new TreeNode(stoi(num, nullptr, 10));
	 string left, right;
	 int i = j;
	 for (; i<s.size(); i++) {
		 if (s[i] == '(')
			 brac++;
		 if (s[i] == ')')
			 brac--;
		 if (brac == 0) {
			 left = s.substr(j + 1, i - j - 1);
			 break;
		 }
	 }
	 if (i == s.size() - 1)
		 right = "";
	 else
		 right = s.substr(i + 2, s.size() - i - 3);
	 node->left = str2tree0(left);
	 node->right = str2tree0(right);
	 return node;
 }

 //recur can only focus on (
 class Solution536_1 {
 private:
	 TreeNode* rec(string& s, int& pos) {
		 if (pos >= s.size())
			 return NULL;
		 int start = pos;
		 while (s[pos] == '-' || isdigit(s[pos]))
			 pos++;
		 TreeNode* node = new TreeNode(stoi(s.substr(start, pos - start)));
		 if (s[pos] == '(') {
			 node->left = rec(s, ++pos);
			 pos++;
		 }
		 if (s[pos] == '(') {
			 node->right = rec(s, ++pos);
			 pos++;
		 }
		 return node;
	 }
 public:
	 TreeNode* str2tree(string s) {
		 int i = 0;
		 return rec(s, i);
	 }
 };

 //iter
 TreeNode* str2tree_2(string s) {
	 if (s == "") return nullptr;
	 stack<TreeNode*> sk;
	 int n = s.size(), pos = 0;
	 while (pos < n && (s[pos] == '-' || isdigit(s[pos])))
		 pos++;
	 TreeNode *root = new TreeNode(stoi(s.substr(0, pos)));
	 sk.push(root);
	 while (pos < n) {
		 if (s[pos++] == ')')
			 sk.pop();
		 else {
			 int start = pos;
			 while (pos < n && (s[pos] == '-' || isdigit(s[pos])))
				 pos++;
			 TreeNode *p = new TreeNode(stoi(s.substr(start, pos - start)));
			 if (sk.top()->left)
				 sk.top()->right = p;
			 else
				 sk.top()->left = p;
			 sk.push(p);
		 }
	 }
	 return root;
 }

 //////////////////////////////////////////////////////////////
 //156. Binary Tree Upside Down
 TreeNode* upsideDownBinaryTree_0(TreeNode* root) {
	 if (!root)
		 return root;
	 stack<TreeNode*> s;
	 while (root->left) {
		 s.push(root);
		 root = root->left;
	 }
	 TreeNode* newRoot = root;

	 while (s.size()) {
		 root->right = s.top();
		 root->left = s.top()->right;
		 root = s.top();
		 s.pop();
	 }
	 root->left = NULL;
	 root->right = NULL;
	 return newRoot;
 }

 //recur to save stack
 TreeNode* upsideDownBinaryTree_1(TreeNode* root) {
	 /** the left most node, when backtrack we need to set the parent node left & right point to null **/
	 if (!root || !root->left) return root;
	 /** record the current left & right node before DFS **/
	 TreeNode* cur_left = root->left;
	 TreeNode* cur_right = root->right;
	 /** DFS call to build the right sub tree **/
	 TreeNode* new_root = upsideDownBinaryTree_1(root->left);
	 cur_left->right = root;
	 cur_left->left = cur_right;
	 /** now the root node become the child node, so we need to clear the left & right sub node to avoid cycles**/
	 root->left = nullptr;
	 root->right = nullptr;
	 return new_root;
 }

 //iter
 TreeNode* upsideDownBinaryTree_2(TreeNode* root) {
	 /** cur   : record the current root node
	 * parent : record the parent node of the current node
	 * right  : record the right child node of the parent node
	 * next   : record the next level left child node **/
	 TreeNode* cur = root;
	 TreeNode* parent = NULL;
	 TreeNode* right = NULL;
	 TreeNode* next = NULL;
	 while (cur != NULL) {
		 next = cur->left;
		 /** set the cur->left point to right and cur->right point to parent **/
		 cur->left = right;
		 right = cur->right;
		 cur->right = parent;
		 parent = cur;
		 /** move left down to the next node **/
		 cur = next;
	 }
	 return parent;
 }

 //////////////////////////////////////////////////////////////
 //655. Print Binary Tree
 //bfs with null node
 class Solution655_0 {
 private:
	 int calDepth(TreeNode * root) {
		 if (!root)
			 return 0;
		 return max(calDepth(root->left), calDepth(root->right)) + 1;
	 }
 public:
	 vector<vector<string>> printTree(TreeNode* root) {
		 int level = calDepth(root);
		 vector<vector<string>> ret(level, vector<string>());
		 vector<int> space(level, 0);//space[l]==space num at left/right of node on level l
		 for (int i = level - 2; i >= 0; i--)
			 space[i] = space[i + 1] * 2 + 1;
		 //bfs
		 vector<TreeNode*> level0{ root };
		 TreeNode temp(INT_MAX);
		 int dep = 0;
		 while (dep<level) {
			 vector<TreeNode*> level1;
			 vector<string> tab(space[dep], "");
			 for (auto& n : level0) {
				 //write space on left
				 ret[dep].insert(ret[dep].end(), tab.begin(), tab.end());
				 //write node
				 string ns = (n == &temp) ? "" : to_string(n->val);
				 ret[dep].push_back(ns);
				 //add next level (NULL children marked by temp)
				 TreeNode* cn = (n->left) ? n->left : &temp;
				 level1.push_back(cn);
				 cn = (n->right) ? n->right : &temp;
				 level1.push_back(cn);
				 //write space on right
				 ret[dep].insert(ret[dep].end(), tab.begin(), tab.end());
				 //write divider space
				 ret[dep].push_back("");
			 };
			 ret[dep].pop_back();//no divider in the end
			 level0 = level1;
			 dep++;
		 }
		 return ret;
	 }
 };

 //no need to store NULL
 class Solution655_1 {
 public:
	 vector<vector<string>> printTree(TreeNode* root) {

		 vector<vector<string>> res;

		 //
		 // create table of empty strings ""
		 // based on the tree height/width
		 //
		 int depth = maxDepth(root);
		 int width = pow(2, depth + 1) - 1;
		 vector<string> row(width, "");
		 for (int i = 0; i <= depth; i++) {
			 res.push_back(row);
		 }

		 //
		 // traverse the tree and fill in table values for non-NULL nodes
		 //
		 fillTable(root, 0, 0, width - 1, res);
		 return res;
	 }

 private:
	 int maxDepth(TreeNode* node) {
		 if (!node) { return -1; }

		 int leftDepth = -1;
		 if (node->left) { leftDepth = maxDepth(node->left); }

		 int rightDepth = -1;
		 if (node->right) { rightDepth = maxDepth(node->right); }

		 return 1 + max(leftDepth, rightDepth);
	 }

	 void fillTable(TreeNode* node, int depth, int start, int end, vector<vector<string>>& table) {
		 if (!node) { return; }

		 int mid = (int)(start + end) / 2;
		 table[depth][mid] = to_string(node->val);

		 if (node->left) { fillTable(node->left, depth + 1, start, mid - 1, table); }
		 if (node->right) { fillTable(node->right, depth + 1, mid + 1, end, table); }
	 }
 };


 //////////////////////////////////////////////////////////////
 //298. Binary Tree Longest Consecutive Sequence
 //leaf to root
 class Solution298_0 {
 private:
	 int ret = 0;
	 int helper(TreeNode* root) {
		 if (!root)
			 return -1;
		 int l = helper(root->left);
		 int r = helper(root->right);
		 int cur = 1;
		 if (root->left&&root->val == root->left->val - 1)
			 cur = l + 1;
		 if (root->right&&root->val == root->right->val - 1)
			 cur = max(cur, r + 1);
		 ret = max(ret, cur);
		 return cur;

	 }
 public:
	 int longestConsecutive(TreeNode* root) {
		 helper(root);
		 return ret;;
	 }
 };

 //root leaf, using two more arg to save a global
 class Solution298_1 {
 private:
	 int helper(TreeNode* root, TreeNode* pa, int length) {
		 if (!root)
			 return length;
		 if (pa&&root->val == pa->val + 1)
			 length++;
		 else
			 length = 1;
		 int l = helper(root->left, root, length);
		 int r = helper(root->right, root, length);
		 return max(length, max(l, r));
	 }
 public:
	 int longestConsecutive(TreeNode* root) {
		 return helper(root, NULL, 0);
	 }
 };

 //////////////////////////////////////////////////////////////
 //549. Binary Tree Longest Consecutive Sequence II
 //take care how to choose the variables right
 class Solution549_1 {
 private:
	 int retMax = 0;
	 pair<int, int> dfs(TreeNode* node) {
		 if (!node)
			 return{ 0,0 };
		 pair<int, int> ID = { 1,1 };//increase/decrease node count including current
		 if (node->left) {
			 pair<int, int> l = dfs(node->left);
			 if (node->val == node->left->val + 1)
				 ID.first = l.first + 1;
			 else if (node->val == node->left->val - 1)
				 ID.second = l.second + 1;
		 }
		 if (node->right) {
			 pair<int, int> r = dfs(node->right);
			 if (node->val == node->right->val + 1)
				 ID.first = max(ID.first, r.first + 1);
			 else if (node->val == node->right->val - 1)
				 ID.second = max(ID.second, r.second + 1);
		 }
		 retMax = max(retMax, ID.first + ID.second - 1);
		 return ID;
	 }
 public:
	 int longestConsecutive(TreeNode* root) {
		 dfs(root);
		 return retMax;
	 }
 };

 //easier to understand but walk tree twice
 class Solution549_2 {
 private:
	 int findPath(TreeNode* root, int prevVal, int diff) {
		 if (root == NULL) return 0;

		 if (root->val == (prevVal + diff))
			 return max(findPath(root->left, root->val, diff), findPath(root->right, root->val, diff)) + 1;
		 else
			 return 0;
	 }
 public:
	 int longestConsecutive(TreeNode* root) {
		 if (root == NULL) return 0;
		 int childMax = 0, cur = 0, l1 = 0, l2 = 0;

		 l1 = findPath(root->left, root->val, -1) + findPath(root->right, root->val, 1) + 1;
		 l2 = findPath(root->left, root->val, 1) + findPath(root->right, root->val, -1) + 1;
		 cur = max(l1, l2);
		 childMax = max(longestConsecutive(root->left), longestConsecutive(root->right));

		 return max(cur, childMax);
	 }

 };

 //////////////////////////////////////////////////////////////
 // 742. Closest Leaf in a Binary Tree
 //if tree better to be solve as graph, make it a graph
 class Solution742_1 {
 public:
	 int findClosestLeaf(TreeNode* root, int k) {
		 unordered_map<TreeNode*, TreeNode*> backMap;   // store all edges that trace node back to its parent
		 queue<TreeNode*> queue;             // the queue used in BFS   
		 set<TreeNode*> seen;

		 TreeNode* kNode = DFS(root, k, backMap);
		 queue.push(kNode);
		 // BFS: find the shortest path
		 while (queue.size()) {
			 TreeNode* curr = queue.front();
			 seen.insert(curr);
			 queue.pop();
			 //visited.insert(kNode);
			 if (curr->left == NULL && curr->right == NULL)
				 return curr->val;
			 if (curr->left != NULL&&seen.count(curr->left) == 0)
				 queue.push(curr->left);
			 if (curr->right != NULL&&seen.count(curr->right) == 0)
				 queue.push(curr->right);
			 if (backMap[curr] && seen.count(backMap[curr]) == 0)   // go alone the back edge
				 queue.push(backMap[curr]);
		 }
		 return -1; // never hit
	 }

 private:
	 // DFS: search for node whoes val == k, draw the back map, stop when hit target
	 TreeNode* DFS(TreeNode* root, int k, unordered_map<TreeNode*, TreeNode*>& backMap) {
		 if (root->val == k)
			 return root;
		 if (root->left != NULL) {
			 backMap[root->left] = root;        // add back edge
			 TreeNode* left = DFS(root->left, k, backMap);
			 if (left != NULL) //stop dfs when meet target
				 return left;
		 }
		 if (root->right != NULL) {
			 backMap[root->right] = root;       // add back edge
			 TreeNode* right = DFS(root->right, k, backMap);
			 if (right != NULL)
				 return right;
		 }
		 return NULL;
	 }
 };

 //////////////////////////////////////////////////////////////
 //236. Lowest Common Ancestor of a Binary Tree
 //post order traversal
 class Solution236_0 {
 private:
	 TreeNode* ret;
	 int dfs(TreeNode* root, TreeNode* p, TreeNode* q) {
		 if (!root)
			 return 0;
		 int count = 0;
		 count += dfs(root->left, p, q);
		 count += dfs(root->right, p, q);
		 if (root == p || root == q)
			 count++;
		 if (count == 2) {
			 ret = root;
			 return 3;
		 }
		 return count;
	 }

 public:
	 TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
		 dfs(root, p, q);
		 return ret;
	 }
 };

 //more elegant
 TreeNode* lowestCommonAncestor_1(TreeNode* root, TreeNode* p, TreeNode* q) {
	 if (root == p || root == q || root == NULL)//order is important
		 return root;//overwrite null or lower target when float
	 TreeNode * parent1 = lowestCommonAncestor(root->left, p, q);
	 TreeNode * parent2 = lowestCommonAncestor(root->right, p, q);
	 if (parent1 && parent2)
		 return root;//if found on both side merge here
	 else
		 return parent1 ? parent1 : parent2;//if found on one side fload up
 }

 //////////////////////////////////////////////////////////////
 //450. Delete Node in a BST
 class Solution450_0 {
 private:
	 void preOrder(TreeNode* node, TreeNode* pa, int& key) {
		 if (!node)
			 return;
		 if (node->val == key) {
			 TreeNode* replacer = NULL;
			 if (node->left&&!node->right)
				 replacer = node->left;
			 else if (node->right&&!node->left)
				 replacer = node->right;
			 else if (node->left&&node->right) {
				 replacer = node->left;
				 TreeNode*temp = node->left;
				 while (temp->right)
					 temp = temp->right;
				 temp->right = node->right;
			 }
			 if (pa->left == node)
				 pa->left = replacer;
			 else
				 pa->right = replacer;
			 delete node;
			 return;
		 }
		 if (node->val>key)
			 preOrder(node->left, node, key);
		 else if (node->val<key)
			 preOrder(node->right, node, key);
	 }
 public:
	 TreeNode* deleteNode(TreeNode* root, int key) {
		 TreeNode t(666);
		 t.left = root;
		 preOrder(root, &t, key);
		 return t.left;
	 }
 };

 //more conscious but did not deleting origin node
 TreeNode* deleteNode_1(TreeNode* root, int key) {
	 if (!root)
		 return nullptr;

	 if (root->val == key) {
		 if (!root->right)
			 return root->left;
		 else {
			 TreeNode* n = root->right;
			 while (n->left)
				 n = n->left;
			 swap(n->val, root->val);

			 root->right = deleteNode_1(root->right, key);
			 return root;
		 }
	 }
	 if (root->val > key)
		 root->left = deleteNode_1(root->left, key);
	 if (root->val < key)
		 root->right = deleteNode_1(root->right, key);
	 return root;
 }

 class Solution450_2 {
 private:
	 //delete node and rebalance, then return substitute node
	 TreeNode* balDel(TreeNode* node) {
		 if (!node)
			 return NULL;
		 TreeNode* sub = NULL;
		 if (node->left&&node->right) {
			 TreeNode* pre = NULL;//parent of substitute node
			 sub = node->right;//choose smallest on right subtree as sub
			 while (sub->left) {
				 pre = sub;
				 sub = sub->left;
			 }
			 sub->left = node->left;//transplant to substitute
			 if (sub != node->right) {//sub is not adj to origin, rotate new tree
				 pre->left = sub->right;
				 sub->right = node->right;
			 }
			 delete node;
			 return sub;
		 }
		 if (!node->left) {
			 sub = node->right;
			 delete node;
			 return sub;
		 }
		 if (!node->right) {
			 sub = node->left;
			 delete node;
			 return sub;
		 }
	 }
 public:
	 TreeNode* deleteNode(TreeNode* root, int key) {
		 TreeNode* cur = root, *pre = NULL;
		 while (cur&&cur->val != key) {
			 pre = cur;
			 if (cur->val>key)
				 cur = cur->left;
			 else
				 cur = cur->right;
		 }
		 if (pre == NULL)
			 return balDel(cur);
		 if (pre->left == cur)
			 pre->left = balDel(cur);
		 else
			 pre->right = balDel(cur);
		 return root;
	 }
 };

 //////////////////////////////////////////////////////////////
 //582. Kill Process
 //bfs
 vector<int> killProcess0(vector<int>& pid, vector<int>& ppid, int kill) {
	 unordered_map<int, unordered_set<int>> map;
	 for (int i = 0; i<pid.size(); i++) {
		 map[ppid[i]].insert(pid[i]);
	 }
	 vector<int> ret;
	 stack<int> wait;
	 wait.push(kill);
	 while (wait.size()) {
		 int nxt = wait.top();
		 wait.pop();
		 ret.push_back(nxt);
		 for (auto & n : map[nxt])
			 wait.push(n);
	 }
	 return ret;
 }


 //////////////////////////////////////////////////////////////
 //545. Boundary of Binary Tree
 class Solution545_0 {
 private:
	 void second(TreeNode* node, vector<int>& ret) {
		 if (!node)
			 return;
		 if (node->left == NULL&&node->right == NULL) {
			 ret.push_back(node->val);
			 return;
		 }
		 second(node->left, ret);
		 second(node->right, ret);
	 }

 public:
	 vector<int> boundaryOfBinaryTree(TreeNode* root) {
		 vector<int> ret;
		 if (!root)
			 return ret;
		 //root
		 ret.push_back(root->val);
		 //left
		 TreeNode* cur = root->left;
		 if (cur) {
			 while (cur->left || cur->right) {
				 ret.push_back(cur->val);
				 if (cur->left)
					 cur = cur->left;
				 else
					 cur = cur->right;
			 }
		 }
		 //leaf
		 if (root->left || root->right)
			 second(root, ret);
		 //right
		 cur = root->right;
		 int pos = ret.size();
		 if (cur) {
			 while (cur->left || cur->right) {
				 ret.insert(ret.begin() + pos, cur->val);
				 if (cur->right)
					 cur = cur->right;
				 else
					 cur = cur->left;
			 }
		 }
		 return ret;
	 }
 };

 //consern of status change
 class Solution545_1 {
	 vector<int> res;
 public:
	 vector<int> boundaryOfBinaryTree(TreeNode* root) {
		 if (!root) return res;
		 res.push_back(root->val);
		 getBd(root->left, true, false);
		 getBd(root->right, false, true);
		 return res;
	 }

	 void getBd(TreeNode* root, bool leftB, bool rightB) {
		 if (!root)
			 return;
		 if (leftB || !root->left && !root->right)
			 res.push_back(root->val);
		 if (root->left)
			 getBd(root->left, leftB, root->right ? false : rightB);
		 if (root->right)
			 getBd(root->right, root->left ? false : leftB, rightB);
		 if ((root->left || root->right) && rightB)
			 res.push_back(root->val);

	 }
 };

 //////////////////////////////////////////////////////////////
 //333. Largest BST Subtree
 class Solution333_0 {
 private:
	 int ret = 0;
	 vector<int> dfs(TreeNode* node) {//return [0] count of BST, [1] MIN, [2] MAX
		 vector<int> cur = { 0,INT_MAX,INT_MIN };
		 if (!node)
			 return cur;
		 vector<int> l = dfs(node->left);
		 vector<int> r = dfs(node->right);
		 if (node->val <= l[2] || node->val >= r[1]) {
			 cur[1] = INT_MIN;
			 cur[2] = INT_MAX;
			 return cur;
		 }
		 else {
			 cur[0] = l[0] + r[0] + 1;
			 cur[1] = min(node->val, l[1]);
			 cur[2] = max(node->val, r[2]);
			 ret = max(ret, cur[0]);
			 return cur;
		 }

	 }
 public:
	 int largestBSTSubtree(TreeNode* root) {
		 dfs(root);
		 return ret;
	 }
 };

 //////////////////////////////////////////////////////////////
 //449. Serialize and Deserialize BST
 // BFS
 // using to_string(int), string.find(",",from), string.substr(from,to)
 class Codec449_1 {
	 string getNextNode(string const& data, int& offset) {
		 int end = data.find(',', offset);
		 if (end == string::npos) {
			 end = data.length();
		 }
		 string sNodeValue = data.substr(offset, end - offset);
		 offset = end + 1;

		 return sNodeValue;
	 }
 public:
	 // Encodes a tree to a single string.
	 string serialize(TreeNode* root) {
		 string result = "";
		 if (!root) return result;
		 queue <TreeNode*> Q;
		 Q.push(root);
		 result += to_string(root->val);
		 result += ',';
		 while (!Q.empty()) {
			 TreeNode* node = Q.front();
			 if (node->left) {
				 result += to_string(node->left->val);
				 result += ',';
				 Q.push(node->left);
			 }
			 else {
				 result += "#,";
			 }
			 if (node->right) {
				 result += to_string(node->right->val);
				 result += ',';
				 Q.push(node->right);
			 }
			 else {
				 result += "#,";
			 }
			 Q.pop();
		 }
		 // triming , and # from end
		 int i = result.size() - 1;
		 for (--i; i >= 0 && result[i] == '#'; i -= 2);
		 return result.substr(0, i + 1);
	 }

	 // Decodes your encoded data to tree.
	 TreeNode* deserialize(string data) {
		 TreeNode* root = nullptr;
		 if (data.empty()) return root;
		 queue<TreeNode*> Q;
		 int offset = 0;
		 int nodeValue = stoi(getNextNode(data, offset));
		 root = new TreeNode(nodeValue);
		 Q.push(root);
		 while (!Q.empty()) {
			 TreeNode* node = Q.front();
			 if (offset < data.length()) {
				 string sValue = getNextNode(data, offset);
				 if (sValue != "#") {
					 int leftNodeValue = stoi(sValue);
					 node->left = new TreeNode(leftNodeValue);
					 Q.push(node->left);
				 }
			 }
			 if (offset < data.length()) {
				 string sValue = getNextNode(data, offset);
				 if (sValue != "#") {
					 int rightNodeValue = stoi(sValue);
					 node->right = new TreeNode(rightNodeValue);
					 Q.push(node->right);
				 }
			 }
			 Q.pop();
		 }
		 return root;
	 }
 };

 //DFS
 //using memcpy(refer_to,refer_from,byte_num) to store int in string
 class Codec449_2 {
 private:
	 inline void preorderDFS(TreeNode* root, string& order) {
		 if (!root)
			 return;
		 char buf[4];
		 //burn the binary of int into 4 chars
		 memcpy(buf, &(root->val), sizeof(int));
		 //record 
		 for (int i = 0; i<4; i++)
			 order.push_back(buf[i]);
		 preorderDFS(root->left, order);
		 preorderDFS(root->right, order);
	 }

	 inline TreeNode* reconstruct(const string& buffer, int& pos, int minValue, int maxValue) {
		 if (pos >= buffer.size())
			 return NULL; //using pos to check whether buffer ends is better than using char* directly.
						  //cast a number from string
		 int value;
		 memcpy(&value, &buffer[pos], sizeof(int));
		 //stop if value not good
		 if (value < minValue || value > maxValue)
			 return NULL;
		 //add node and update pos of next num
		 TreeNode* node = new TreeNode(value);
		 pos += sizeof(int);
		 //go further
		 node->left = reconstruct(buffer, pos, minValue, value);
		 node->right = reconstruct(buffer, pos, value, maxValue);
		 return node;
	 }
 public:
	 // Encodes a tree to a single string.
	 string serialize(TreeNode* root) {
		 string order;
		 preorderDFS(root, order);
		 return order;
	 }
	 // Decodes your encoded data to tree.
	 TreeNode* deserialize(string data) {
		 int pos = 0;
		 return reconstruct(data, pos, INT_MIN, INT_MAX);
	 }
 };

 //////////////////////////////////////////////////////////////
 //652. Find Duplicate Subtrees
 class Solution652_0 {
 private:
	 unordered_map<string, int> remb;
	 vector<TreeNode*> result;
	 string postOrder(TreeNode* node) {
		 if (!node)
			 return "n ";
		 string ret;
		 ret += postOrder(node->left);
		 ret += postOrder(node->right);
		 ret += to_string(node->val);
		 ret += " ";
		 if (ret != "n "&&remb[ret] == 1)
			 result.push_back(node);
		 remb[ret]++;
		 return ret;
	 }
 public:
	 vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
		 postOrder(root);
		 return result;
	 }
 };

 //faster
 class Solution652_1 {
 public:
	 vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
		 unordered_map<string, vector<TreeNode*>> map;
		 vector<TreeNode*> dups;
		 serialize(root, map);
		 for (auto it = map.begin(); it != map.end(); it++)
			 if (it->second.size() > 1) dups.push_back(it->second[0]);
		 return dups;
	 }
 private:
	 string serialize(TreeNode* node, unordered_map<string, vector<TreeNode*>>& map) {
		 if (!node) return "";
		 string s = "(" + serialize(node->left, map) + to_string(node->val) + serialize(node->right, map) + ")";
		 map[s].push_back(node);
		 return s;
	 }
 };

 //////////////////////////////////////////////////////////////
 //662. Maximum Width of Binary Tree
 //index of binary tree is easy to calculate recursively
 class Solution662_1 {
 public:
	 int widthOfBinaryTree(TreeNode* root) {
		 vector<int> left;
		 vector<int> right;
		 if (!root) return 0;

		 TraverseLeft(root, 1, 0, left);
		 TraverseRight(root, 1, 0, right);

		 int res = 0;
		 for (int i = 0; i<left.size(); i++) {
			 if (right[i] - left[i] > res)
				 res = right[i] - left[i];
		 }
		 return res + 1;
	 }

	 void TraverseLeft(TreeNode* p, int level, int index, vector<int>& arr) {
		 if (arr.size() < level)
			 arr.push_back(index);
		 if (p->left)
			 TraverseLeft(p->left, level + 1, index * 2 + 1, arr);
		 if (p->right)
			 TraverseLeft(p->right, level + 1, index * 2 + 2, arr);
	 }

	 void TraverseRight(TreeNode* p, int level, int index, vector<int>& arr) {
		 if (arr.size() < level)
			 arr.push_back(index);
		 if (p->right)
			 TraverseRight(p->right, level + 1, index * 2 + 2, arr);
		 if (p->left)
			 TraverseRight(p->left, level + 1, index * 2 + 1, arr);
	 }
 };

 //more elegant
 class Solution662_2 {
 public:
	 int widthOfBinaryTree(TreeNode* root) {
		 vector<int> lefts; // left most nodes at each level;
		 int maxwidth = 0;
		 dfs(root, 1, 0, lefts, maxwidth);
		 return maxwidth;
	 }
 private:
	 void dfs(TreeNode* node, int id, int depth, vector<int>& lefts, int& maxwidth) {
		 if (!node) return;
		 if (depth >= lefts.size()) lefts.push_back(id);  // add left most node
		 maxwidth = max(maxwidth, id + 1 - lefts[depth]);
		 dfs(node->left, id * 2, depth + 1, lefts, maxwidth);
		 dfs(node->right, id * 2 + 1, depth + 1, lefts, maxwidth);
	 }
 };


 //////////////////////////////////////////////////////////////
 //508. Most Frequent Subtree Sum
 //post order dfs
 class Solution508_0 {
 private:
	 unordered_map<int, int> count;//count[i]==j, i appeared j times
	 int top = 0;
	 int dfs(TreeNode* node) {
		 if (!node)
			 return 0;
		 int cur = node->val;
		 cur += dfs(node->left);
		 cur += dfs(node->right);
		 count[cur]++;
		 top = max(top, count[cur]);
		 return cur;
	 }
 public:
	 vector<int> findFrequentTreeSum(TreeNode* root) {
		 vector<int> ret;
		 dfs(root);
		 for (auto& c : count) {
			 if (c.second == top)
				 ret.push_back(c.first);
		 }
		 return ret;
	 }
 };

 //////////////////////////////////////////////////////////////
 //663. Equal Tree Partition
 //notice 0 and -1%2==-1
 class Solution663_0 {
 private:
	 unordered_map<int, int> got;
	 int dfs(TreeNode* node) {
		 if (!node)
			 return 0;
		 int cur = node->val;
		 cur += dfs(node->left);
		 cur += dfs(node->right);
		 got[cur]++;
		 return cur;
	 }
 public:
		 bool checkEqualTree(TreeNode* root){
			 int sum = dfs(root);
			 if (sum == 0)
				 return got[0] >= 2;
			 if (sum % 2 == 0)
				 return got[sum / 2];
			 else
				 return false;
		 }

 };

 //////////////////////////////////////////////////////////////
 //222. Count Complete Tree Nodes
 //still have re calcul
 int countNodes_1(TreeNode* root) {

	 if (!root) return 0;

	 int hl = 0, hr = 0;

	 TreeNode *l = root, *r = root;

	 while (l) { hl++; l = l->left; }

	 while (r) { hr++; r = r->right; }

	 if (hl == hr) return pow(2, hl) - 1;

	 return 1 + countNodes_1(root->left) + countNodes_1(root->right);

 }

 //check on left height
 class Solution222_2 {
 private:
	 int depth(TreeNode* n) {
		 if (!n)
			 return -1;
		 return depth(n->left) + 1;
	 }

 public:
	 int countNodes(TreeNode* root) {
		 if (!root)
			 return 0;
		 int h = depth(root), count = 0;
		 while (root) {
			 int rh = depth(root->right);
			 if (rh == h - 1) {
				 count += pow(2, h);
				 root = root->right;
			 }
			 else {
				 count += pow(2, h - 1);
				 root = root->left;
			 }
			 h--;
		 }
		 return count;
	 }

 };

 //////////////////////////////////////////////////////////////
 //654. Maximum Binary Tree
 //dfs
 class Solution654_0 {
 private:
	 TreeNode* build(vector<int>& num, int from, int to) {
		 if (from>to)
			 return NULL;
		 int val = num[from];
		 int pos = from;
		 for (int i = from + 1; i <= to; i++) {
			 if (num[i]>val) {
				 val = num[i];
				 pos = i;
			 }
		 }
		 TreeNode* cur = new TreeNode(val);
		 cur->left = build(num, from, pos - 1);
		 cur->right = build(num, pos + 1, to);
		 return cur;
	 }
 public:
	 TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
		 return build(nums, 0, nums.size() - 1);
	 }
 };

//use a stack
 TreeNode* constructMaximumBinaryTree_1(vector<int>& nums) {
	 vector<TreeNode*> stk;
	 for (int i = 0; i < nums.size(); ++i)
	 {
		 TreeNode* cur = new TreeNode(nums[i]);
		 //pre smaller than cur is cur left child
		 while (!stk.empty() && stk.back()->val < nums[i])
		 {
			 cur->left = stk.back();
			 stk.pop_back();
		 }
		 //pre bigger than cur is cur left parent
		 if (!stk.empty())
			 stk.back()->right = cur;
		 stk.push_back(cur);
	 }
	 return stk.front();
 }

 //////////////////////////////////////////////////////////////
 // 666. Path Sum IV
 int pathSum_0(vector<int>& nums) {
	 unordered_map<int, int> id2val;
	 for (auto &n : nums) {
		 int va = n % 10;
		 n /= 10;
		 int idx = n % 10;
		 n /= 10;
		 int level = n % 10;
		 id2val[idx + pow(2, level - 1) - 1] = va;
	 }
	 queue<int> IDs;
	 int sum = 0;
	 IDs.push(1);
	 while (IDs.size()) {
		 int id = IDs.front();
		 IDs.pop();
		 auto it = id2val.find(id * 2);
		 if (it != id2val.end()) {
			 it->second += id2val[id];
			 IDs.push(id * 2);
		 }
		 auto it1 = id2val.find(id * 2 + 1);
		 if (it1 != id2val.end()) {
			 it1->second += id2val[id];
			 IDs.push(id * 2 + 1);
		 }
		 if (it == it1)
			 sum += id2val[id];
	 }
	 return sum;
 }

 //////////////////////////////////////////////////////////////
 //684. Redundant Connection
 //union find
 class Solution684_1 {
 private:
	 vector<int> pa;
	 int find(int a) {
		 while (a != pa[a]) {
			 pa[a] = pa[pa[a]];
			 a = pa[a];
		 }
		 return a;
	 }
 public:
	 vector<int> findRedundantConnection(vector<vector<int>>& edges) {
		 pa.resize(1001);
		 for (int i = 0; i<1001; i++)
			 pa[i] = i;
		 vector<int> ret;
		 for (auto e : edges) {
			 int x = find(e[0]), y = find(e[1]);
			 if (x == y)
				 ret = e;
			 else
				 pa[x] = y;
		 }
		 return ret;
	 }
 };

 //////////////////////////////////////////////////////////////
 //285. Inorder Successor in BST
 //in order of BST always increases
 TreeNode* inorderSuccessor_1(TreeNode* root, TreeNode* p) {
	 if (p->right) {
		 p = p->right;
		 while (p->left)
			 p = p->left;
		 return p;
	 }
	 TreeNode* candidate = NULL;
	 while (root != p)
		 root = (root->val >= p->val) ? (candidate = root)->left : root->right;
	 return candidate;
 }


 //////////////////////////////////////////////////////////////
 //623. Add One Row to Tree
 class Solution623_0 {
 private:
	 void dfs(TreeNode* node, int& v, int& trg, int cur) {
		 if (!node)
			 return;
		 if (cur == trg) {
			 TreeNode* l = new TreeNode(v);
			 TreeNode* r = new TreeNode(v);
			 l->left = node->left;
			 r->right = node->right;
			 node->left = l;
			 node->right = r;
			 return;
		 }
		 dfs(node->left, v, trg, cur + 1);
		 dfs(node->right, v, trg, cur + 1);
	 }
 public:
	 TreeNode* addOneRow(TreeNode* root, int v, int d) {
		 if (d == 1) {
			 TreeNode* nrt = new TreeNode(v);
			 nrt->left = root;
			 return nrt;
		 }
		 dfs(root, v, --d, 1);
		 return root;
	 }
 };

 //////////////////////////////////////////////////////////////
 //250. Count Univalue Subtrees
 class Solution250_0 {
 private:
	 int ret = 0;
	 bool dfs(TreeNode* N) {
		 if (!N)
			 return true;
		 bool l = dfs(N->left);
		 bool r = dfs(N->right);
		 if (l&&r) {
			 if (N->left&&N->val != N->left->val)
				 return false;
			 if (N->right&&N->val != N->right->val)
				 return false;
			 ret++;
			 return true;
		 }
		 return false;
	 }
 public:
	 int countUnivalSubtrees(TreeNode* root) {
		 if (!root)
			 return 0;
		 dfs(root);
		 return ret;
	 }
 };

 //smart use of "|"
 class Solution250_1 {
 public:
	 int countUnivalSubtrees(TreeNode* root) {
		 helper(root, 0);
		 return count;
	 }
 private:
	 int count = 0;
	 bool helper(TreeNode* root, int val) {
		 if (root == NULL) {
			 return true;
		 }
		 //"|" ensures the recursion do both sides 
		 //instead of "||" only dose recursion on left side if it is wrong
		 if (!helper(root->left, root->val) | !helper(root->right, root->val)) {
			 return false;
		 }
		 count++;
		 return root->val == val;
	 }
 };

 //////////////////////////////////////////////////////////////
 //230. Kth Smallest Element in a BST
 //rebalance children of root
 class Solution230_0 {
 private:
	 vector<TreeNode*> order;
	 void dfs(TreeNode* N) {
		 if (!N)
			 return;
		 dfs(N->left);
		 order.push_back(N);
		 dfs(N->right);
	 }
	 TreeNode* rebuild(int a, int b) {
		 if (a>b)
			 return NULL;
		 int cur = a + (b - a) / 2;
		 order[cur]->left = rebuild(a, cur - 1);
		 order[cur]->right = rebuild(cur + 1, b);
		 return order[cur];
	 }
 public:
	 int kthSmallest(TreeNode* root, int k) {
		 int ret;
		 dfs(root->left);
		 root->left = rebuild(0, order.size() - 1);
		 if (k <= order.size())
			 ret = order[k - 1]->val;
		 else if (k == order.size() + 1)
			 ret = root->val;
		 int pos = k - order.size() - 2;
		 order.clear();
		 dfs(root->right);
		 root->right = rebuild(0, order.size() - 1);
		 if (pos >= 0)
			 ret = order[pos]->val;
		 return ret;
	 }
 };

 void main() {
	 //vector<string> maze = { "kid","tag","pup","ail","tun","woo","erg","luz","brr","gay","sip","kay","per","val","mes","ohs","now","boa","cet","pal","bar","die","war","hay","eco","pub","lob","rue","fry","lit","rex","jan","cot","bid","ali","pay","col","gum","ger","row","won","dan","rum","fad","tut","sag","yip","sui","ark","has","zip","fez","own","ump","dis","ads","max","jaw","out","btu","ana","gap","cry","led","abe","box","ore","pig","fie","toy","fat","cal","lie","noh","sew","ono","tam","flu","mgm","ply","awe","pry","tit","tie","yet","too","tax","jim","san","pan","map","ski","ova","wed","non","wac","nut","why","bye","lye","oct","old","fin","feb","chi","sap","owl","log","tod","dot","bow","fob","for","joe","ivy","fan","age","fax","hip","jib","mel","hus","sob","ifs","tab","ara","dab","jag","jar","arm","lot","tom","sax","tex","yum","pei","wen","wry","ire","irk","far","mew","wit","doe","gas","rte","ian","pot","ask","wag","hag","amy","nag","ron","soy","gin","don","tug","fay","vic","boo","nam","ave","buy","sop","but","orb","fen","paw","his","sub","bob","yea","oft","inn","rod","yam","pew","web","hod","hun","gyp","wei","wis","rob","gad","pie","mon","dog","bib","rub","ere","dig","era","cat","fox","bee","mod","day","apr","vie","nev","jam","pam","new","aye","ani","and","ibm","yap","can","pyx","tar","kin","fog","hum","pip","cup","dye","lyx","jog","nun","par","wan","fey","bus","oak","bad","ats","set","qom","vat","eat","pus","rev","axe","ion","six","ila","lao","mom","mas","pro","few","opt","poe","art","ash","oar","cap","lop","may","shy","rid","bat","sum","rim","fee","bmw","sky","maj","hue","thy","ava","rap","den","fla","auk","cox","ibo","hey","saw","vim","sec","ltd","you","its","tat","dew","eva","tog","ram","let","see","zit","maw","nix","ate","gig","rep","owe","ind","hog","eve","sam","zoo","any","dow","cod","bed","vet","ham","sis","hex","via","fir","nod","mao","aug","mum","hoe","bah","hal","keg","hew","zed","tow","gog","ass","dem","who","bet","gos","son","ear","spy","kit","boy","due","sen","oaf","mix","hep","fur","ada","bin","nil","mia","ewe","hit","fix","sad","rib","eye","hop","haw","wax","mid","tad","ken","wad","rye","pap","bog","gut","ito","woe","our","ado","sin","mad","ray","hon","roy","dip","hen","iva","lug","asp","hui","yak","bay","poi","yep","bun","try","lad","elm","nat","wyo","gym","dug","toe","dee","wig","sly","rip","geo","cog","pas","zen","odd","nan","lay","pod","fit","hem","joy","bum","rio","yon","dec","leg","put","sue","dim","pet","yaw","nub","bit","bur","sid","sun","oil","red","doc","moe","caw","eel","dix","cub","end","gem","off","yew","hug","pop","tub","sgt","lid","pun","ton","sol","din","yup","jab","pea","bug","gag","mil","jig","hub","low","did","tin","get","gte","sox","lei","mig","fig","lon","use","ban","flo","nov","jut","bag","mir","sty","lap","two","ins","con","ant","net","tux","ode","stu","mug","cad","nap","gun","fop","tot","sow","sal","sic","ted","wot","del","imp","cob","way","ann","tan","mci","job","wet","ism","err","him","all","pad","hah","hie","aim","ike","jed","ego","mac","baa","min","com","ill","was","cab","ago","ina","big","ilk","gal","tap","duh","ola","ran","lab","top","gob","hot","ora","tia","kip","han","met","hut","she","sac","fed","goo","tee","ell","not","act","gil","rut","ala","ape","rig","cid","god","duo","lin","aid","gel","awl","lag","elf","liz","ref","aha","fib","oho","tho","her","nor","ace","adz","fun","ned","coo","win","tao","coy","van","man","pit","guy","foe","hid","mai","sup","jay","hob","mow","jot","are","pol","arc","lax","aft","alb","len","air","pug","pox","vow","got","meg","zoe","amp","ale","bud","gee","pin","dun","pat","ten","mob" };
	 //vector<int> start = { 0,4 };
	 //vector<int> destination = { 4,4 };

	 //int ret = ladderLength("cet", "ism", maze);
	 //int ooo = ret;
	 //shortestDistance(maze, start, destination);
	 vector<vector<int>> board = { {1,2 },{ 1,3 },{2,3 } };
	 vector<int> click = { 3,0 };
	 TreeNode n(1);
	 //Solution s;
 }



