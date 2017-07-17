#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

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
 class Solution {
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

 void main() {
	 TreeNode N0(2);
	 TreeNode N1(1);
	 TreeNode N2(2);
	 N0.left = &N1;
	 N0.right = &N2;

 }

