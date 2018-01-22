#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <sstream>//stringstream
#include <iostream>
#include <functional>//function
#include <set>//ultiset
#include <unordered_set>


using namespace std;

//=================================stack easy=========================================================================

//////////////////////////////////////////////////////////////
//232. Implement Queue using Stacks
class MyQueue232_0 {
public:
	// Push element x to the back of queue.
	void push(int x) {
		pushHelper(x);
	}
	void pushHelper(int x) {
		if (s.size() == 0) {
			s.push(x);
			return;
		}
		int data;
		data = s.top();
		s.pop();
		pushHelper(x);
		s.push(data);
		return;
	}

	// Removes the element from in front of queue.
	int pop(void) {
		int t = peek();
		s.pop();
		return t;
	}

	// Get the front element.
	int peek(void) {
		return s.top();
	}

	// Return whether the queue is empty.
	bool empty(void) {
		return (s.size() == 0);
	}
private:
	stack<int> s;
};

class MyQueue232_1 {
	stack<int> input, output;
public:
	void push(int x) {
		input.push(x);
	}

	int pop(void) {
		int t = peek();
		output.pop();
		return t;
	}

	int peek(void) {
		if (output.empty())
			while (input.size()) {
				output.push(input.top());
				input.pop();
			}
		return output.top();
	}

	bool empty(void) {
		return input.empty() && output.empty();
	}
};

//////////////////////////////////////////////////////////////
//225. Implement Stack using Queues
//do recur is not necessary
class MyStack_stupid {
public:
	/** Initialize your data structure here. */
	MyStack_stupid() {

	}

	/** Push element x onto stack. */
	void push(int x) {
		helpush(x);
	}

	//push new always in front
	void helpush(int x) {
		//put x in most front
		if (Q.empty()) {
			Q.push(x);
			return;
		}
		//take out oldest/last nod in Q;
		queue<int> T;
		while (Q.size()>1) {
			T.push(Q.front());
			Q.pop();
		}
		int old = Q.front();
		Q.pop();
		Q = T;
		//recur
		helpush(x);
		//restore old nod
		Q.push(old);
		return;
	}

	/** Removes the element on top of the stack and returns that element. */
	int pop() {
		int t = Q.front();
		Q.pop();
		return t;

	}

	/** Get the top element. */
	int top() {
		return Q.front();
	}

	/** Returns whether the stack is empty. */
	bool empty() {
		return Q.empty();
	}

private:
	queue<int> Q;
};

//think smart
class Stack_0 {
public:
	queue<int> que;
	// Push element x onto stack.
	void push(int x) {
		que.push(x);
		for (int i = 0; i<que.size() - 1; ++i) {
			que.push(que.front());
			que.pop();
		}
	}

	// Removes the element on top of the stack.
	void pop() {
		que.pop();
	}

	// Get the top element.
	int top() {
		return que.front();
	}

	// Return whether the stack is empty.
	bool empty() {
		return que.empty();
	}
};

//////////////////////////////////////////////////////////////
//155. Min Stack
class MinStack {
public:
	void push(int x) {
		S.push(x);
		M[x]++;
	}
	void pop() {
		M[S.top()]--;
		S.pop();
	}
	int top() {
		return S.top();
	}
	int getMin() {
		it = M.begin();
		while (it->second == 0)
			it++;
		return it->first;
	}
private:
	stack<int> S;
	map<int, int> M;
	map<int, int>::iterator it;
};

//faster, no need to track everything
class MinStack_1 {
private:
	stack<int> s1;
	stack<int> s2;
public:
	void push(int x) {
		s1.push(x);
		//only smaller new nod need to be tracked 
		if (s2.empty() || x <= getMin())  
			s2.push(x);
	}
	void pop() {
		if (s1.top() == getMin())//bigger nod poped will not influence on MIN  
			s2.pop();
		s1.pop();
	}
	int top() {
		return s1.top();
	}
	int getMin() {
		return s2.top();
	}
};

//////////////////////////////////////////////////////////////
//496. Next Greater Element I
vector<int> nextGreaterElement(vector<int>& findNums, vector<int>& nums) {
	stack<int> s;
	unordered_map<int, int> m;
	/*
	421		(number in stack are decracing) 
	421->3  (one bigger number may pop a adjacency list in stack)
	43		(and then become another decreacing stack)
	*/
	for (int n : nums) {
		while (s.size() && s.top() < n) {
			m[s.top()] = n;
			s.pop();
		}
		s.push(n);
	}
	vector<int> ans;
	for (int n : findNums)
		ans.push_back(m.count(n) ? m[n] : -1);
	return ans;
}


//=================================stack mediun=========================================================================

//////////////////////////////////////////////////////////////
//173. Binary Search Tree Iterator
 struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
	
};

//breaks in - order traversal into two steps
//Each node will be visited at most twice->average O(1) run time.
//The stack will store at most h nodes->O(h) space.
class BSTIterator_1 {
	stack<TreeNode *> myStack;
public:
	BSTIterator_1(TreeNode *root) {
		pushAll(root);//store all left nod of rot in stack
	}

	/** @return whether we have a next smallest number */
	//O(1) time
	bool hasNext() {
		return !myStack.empty();
	}

	/** @return the next smallest number */
	//amortized O(1) time because total called n time and total walk through all n node
	int next() {
		TreeNode *tmpNode = myStack.top();//get the left most
		myStack.pop();
		pushAll(tmpNode->right);//then process righ sub tree
		return tmpNode->val;
	}

private:
	//only one side of branch so O(h) memory
	void pushAll(TreeNode *node) {
		while (node != NULL) {
			myStack.push(node);
			node = node->left;
		}
	}
};


//////////////////////////////////////////////////////////////
//636. Exclusive Time of Functions
vector<int> exclusiveTime(int n, vector<string>& logs) {
	stack<pair<int, int>> Stack;
	vector<int> ret(n, 0);
	//recort last spot's status and timing
	int pre_time = 0;
	string pre_sta = "start";

	Stack.push(make_pair(111, 0));//simulate main fun

	for (auto str : logs) {
		//figure out the info in log string
		size_t a = str.find(":");
		size_t b = str.rfind(":");//reverse find           

		//int id, time;
		//stringstream(str.substr(0, a)) >> id;//use stream to do type cast  
		//stringstream(str.substr(b + 1)) >> time;
		int id = stoi(str.substr(0, a));// use stoi() to do type casting, much faster
		int time = stoi(str.substr(b + 1));
		string sta = str.substr(a + 1, b - a - 1);

		//aline the timing for calcul
		if (sta == "start")
			time--;

		//add elapsed time
		Stack.top().second += time - pre_time;

		//control
		if (Stack.top().first != id || (Stack.top().first == id&&pre_sta == sta)) {//if a function called(first time or recur)              
			Stack.push(make_pair(id, 0));
			pre_sta = sta;
		}
		else if (Stack.top().first == id) {//if a function returns
			ret[Stack.top().first] += Stack.top().second;
			Stack.pop();
			pre_sta = "start";
		}
		pre_time = time;
	}
	return ret;
}

/**
* Every time end an function, deduce its life span from its parent
*/
class Solution636 {
public:
	vector<int> exclusiveTime(int n, vector<string>& logs) {
		vector<int> times(n, 0);
		stack<pair<int, int>> starters;
		for (int i = 0; i < logs.size(); i++) {
			Line line = getLine(logs[i]);
			if (line.start) {
				starters.push({ line.fid, line.time });
			}
			else {
				pair<int, int> starter = starters.top();
				int lifespan = line.time + 1 - starter.second;
				starters.pop();
				times[line.fid] += lifespan;
				if (!starters.empty()) {
					times[starters.top().first] -= lifespan;//deduce inner funciton's life ahead of time
				}
			}
		}
		return times;
	}

	struct Line {
		int fid;
		bool start;
		int time;
		Line(int fid, bool start, int time) : fid(fid), start(start), time(time) {};
	};

	Line getLine(string s) {
		int colon1 = s.find(":", 0);
		int colon2 = s.find(":", colon1 + 1);
		string fid = s.substr(0, colon1);
		string start = s.substr(colon1 + 1, colon2 - (colon1 + 1));
		string time = s.substr(colon2 + 1);
		return Line(stoi(fid), start == "start", stoi(time));
	}

};

//////////////////////////////////////////////////////////////
//71. Simplify Path
string simplifyPath_0(string path) {
	if (path == "")
		return "/";
	string temp = "";
	stack<string> sta;
	//always add / at end
	if (*path.rbegin() != '/')
		path = path + "/";
	//walk folder
	for (int i = 0; i<path.size(); i++) {
		if (path[i] == '/'&&temp != "") {//when get a dir
			if (temp == "..") {
				if (!sta.empty())
					sta.pop();
			}
			else if (temp != ".")
				sta.push(temp);
			temp = "";
		}
		else if (path[i] != '/') {//writing a dir
			temp = temp + path[i];
		}
	}
	//write out in string format
	temp = "";
	if (!sta.empty()) {
		temp = sta.top();
		sta.pop();
	}
	while (!sta.empty()) {
		temp = sta.top() + "/" + temp;
		sta.pop();
	}
	temp = "/" + temp;
	return temp;
}

//istream& getline(istream& is, string& str, char delim);
//Extracts characters from is and stores them into str until the delimitation character delim is found (or the newline character, '\n', for (2)).
//If the delimiter is found, it is extracted and discarded (i.e. it is not stored and the next input operation will begin after it).
string simplifyPath_1(string path) {
	string res, tmp;
	vector<string> stk;
	stringstream ss(path);
	while (getline(ss, tmp, '/')) {
		if (tmp == "" || tmp == ".")
			continue;
		if (tmp == ".." && !stk.empty())
			stk.pop_back();//vector.pop_back()
		else if (tmp != "..")
			stk.push_back(tmp);
	}
	for (auto str : stk)
		res += "/" + str;
	return res.empty() ? "/" : res;
}

//////////////////////////////////////////////////////////////
//402. Remove K Digits
class Solution402_0 {
public:
	string removeKdigits(string num, int k) {
		len = num.size();
		if (num.size() == 0 || k == len)
			return "0";
		k = len - k;//k == number need to choose
		origin = num;
		//do choose job
		choose(0, k);
		//delete leading zeros
		while (ret.size()>0 && ret[0] == '0')
			ret = ret.substr(1);
		if (ret == "")//consider edge situation
			return "0";
		return ret;

	}
private:
	string origin;
	int len;
	string ret;
	void choose(int start, int& count) {
		//edge situation
		if (count == 0)
			return;
		else if (len - start == count) {
			ret += origin.substr(start);
			return;
		}
		//find the minnum
		int min = start;
		for (; start <= len - count; start++) {
			if (origin[min] == '0')
				break;
			if (origin[start] - '0'<origin[min] - '0')
				min = start;
		}
		//add choosen digit to string
		ret += origin[min];
		//go find next digit
		choose(min + 1, --count);
	}
};

//scan from left to right, and remove the first "peak" digit; 
//the peak digit is larger than its right neighbor. 
//do it k times
//complexity O(k*n)
string removeKdigits_1(string num, int k) {
	while (k > 0) {
		int n = num.size();
		int i = 0;
		while (i + 1<n && num[i] <= num[i + 1])
			i++;
		num.erase(i, 1);//string.erase(pos,num)
		k--;
	}
	// trim leading zeros
	int s = 0;
	while (s<(int)num.size() - 1 && num[s] == '0')
		s++;
	num.erase(0, s);

	return num == "" ? "0" : num;
}

//when the result stack (i.e. res) pop a digit, it is equivalent as remove that "peak" digit.
string removeKdigits_2(string num, int k) {
	string res;
	int keep = num.size() - k;
	//choose a ascending string
	for (int i = 0; i<num.size(); i++) {
		while (res.size()>0 && res.back()>num[i] && k>0) {//make sure not delete too much digit
			res.pop_back();//string.pop_back();
			k--;
		}
		res.push_back(num[i]);//string.push_back(char);
	}
	//take leading digit
	res.erase(keep);

	// trim leading zeros
	int s = 0;
	while (s<(int)res.size() - 1 && res[s] == '0')  
		s++;
	res.erase(0, s);//string.erase(pos,num)

	return res == "" ? "0" : res;
}

//////////////////////////////////////////////////////////////
//331. Verify Preorder Serialization of a Binary Tree
bool isValidSerialization(string preorder) {
	string nod;
	vector<string> tree;
	stringstream ss(preorder);
	while (getline(ss, nod, ',')) {
		tree.push_back(nod);
		size_t n = tree.size();
		while (n >= 3 && tree[n - 1] == "#"&&tree[n - 2] == "#"&&tree[n - 3] != "#") {
			tree.erase(tree.begin() + n - 3, tree.begin() + n - 1);
			n = tree.size();
		}
	}
	if (tree.size() == 1 && tree[0] == "#")
		return true;
	return false;
}

//////////////////////////////////////////////////////////////
//385. Mini Parser

 //  try look at the class interface before get started
 // This is the interface that allows for creating nested lists.
 // You should not implement it, or speculate about its implementation
class NestedInteger {
   public:
     // Constructor initializes an empty nested list.
     NestedInteger();

     // Constructor initializes a single integer.
     NestedInteger(int value);

     // Return true if this NestedInteger holds a single integer, rather than a nested list.
     bool isInteger() const;

     // Return the single integer that this NestedInteger holds, if it holds a single integer
     // The result is undefined if this NestedInteger holds a nested list
     int getInteger() const;

     // Set this NestedInteger to hold a single integer.
     void setInteger(int value);

     // Set this NestedInteger to hold a nested list and adds a nested integer to it.
     void add(const NestedInteger &ni);

     // Return the nested list that this NestedInteger holds, if it holds a nested list
     // The result is undefined if this NestedInteger holds a single integer
     const vector<NestedInteger> &getList() const;
 };

//think smart for recur
//get number from string using string>>int then clear flag
class Solution385_1 {
public:
	NestedInteger deserialize(string s) {
		stringstream in(s);
		return deserialize(in);
	}
private:
	NestedInteger deserialize(stringstream &in) {
		int number;
		//if in lead with number, return a number obj
		if (in >> number) {//cast leading num in stream in to int,set flag 
			cout << number << endl;
			return NestedInteger(number);
		}
		in.clear();//clear the error flag if >> failed
		in.get();//Extracts a single character from the stream.

				 //else it must be a number after[, need to create new obj     
		NestedInteger list;
		while (in.peek() != ']') {//keep add childred recursively until most inside finish
			list.add(deserialize(in));
			if (in.peek() == ',')
				in.get();
		}
		in.get();
		//return new nested obj
		return list;
	}
};


//Time: O(n), Space: O(n)
//using stack is easier
//get number from string using find_if_not(it,it,lambda)
class Solution385_2 {
public:
	NestedInteger deserialize(string s) {
		auto isnumber = [](char c) { return (c == '-') || isdigit(c); };

		stack<NestedInteger> stk;
		stk.push(NestedInteger());

		for (auto it = s.begin(); it != s.end();) {
			const char & c = (*it);
			if (isnumber(c)) {
				//Returns an iterator to the first element in the range [first,last) for which pred returns false. 
				//If no such element is found, the function returns last.
				auto it2 = find_if_not(it, s.end(), isnumber);
				int val = stoi(string(it, it2));//Copies the sequence of characters in the range [first,last)
				stk.top().add(NestedInteger(val));
				it = it2;
			}
			else {
				if (c == '[') {
					stk.push(NestedInteger());
				}
				else if (c == ']') {
					NestedInteger ni = stk.top();
					stk.pop();
					stk.top().add(ni);
				}
				++it;
			}
		}

		NestedInteger result = stk.top().getList().front();
		return result;
	}
};


//////////////////////////////////////////////////////////////
//503. Next Greater Element II
vector<int> nextGreaterElements_0(vector<int>& nums) {
	nums.insert(nums.end(), nums.begin(), nums.end());
	cout << nums.size() << endl;
	vector<int> ret(nums.size() / 2, -1);
	for (size_t n = 0; n<nums.size() / 2; n++) {
		for (size_t m = n + 1; m<nums.size(); m++) {
			if (nums[n]<nums[m]) {
				ret[n] = nums[m];
				break;
			}
		}
	}
	return ret;
}

//We use a stack to keep a decreasing sub-sequence, 
//whenever we see a number x greater than stack.peek() we pop all elements less than x 
//and for all the popped ones, their next greater element is x
vector<int> nextGreaterElements(vector<int>& nums) {
	int n = nums.size();
	vector<int> next(n, -1);
	stack<int> s; // index stack
	for (int i = 0; i < n * 2; i++) {
		int num = nums[i % n];
		while (!s.empty() && nums[s.top()] < num) {
			next[s.top()] = num;
			s.pop();
		}
		if (i < n)
			s.push(i);
	}
	return next;
}

//////////////////////////////////////////////////////////////
//394. Decode String
//string.find_first_of("any_of_these_char")
//string.substr(start,length)
string decodeString_0(string s) {
	stack<bool> Stack;
	int n = s.find_first_of("[]");
	//if pure string
	if (n == -1)
		return s;
	//get num
	int i0 = s.find_first_of("0123456789");
	int x = stoi(s.substr(i0, n - i0));
	//find '[' & ']'
	int b0 = n;
	int b1;
	while (n != -1) {
		if (s[n] == '[')
			Stack.push(0);
		else if (s[n] == ']'&&Stack.size() == 1) {
			b1 = n;
			break;
		}
		else if (s[n] == ']'&&Stack.top() == 0) {
			Stack.pop();
		}
		n = s.find_first_of("[]",n+1);
	}
	//copy repeat string
	string first = "";
	if (i0>0)
		first = s.substr(0, i0);
	string repeat = s.substr(b0 + 1, b1 - b0 - 1);
	string last = "";
	if (b1<s.size() - 1)
		last = s.substr(b1 + 1);
	s = first;
	while (x--) 
		s = s + repeat;
	s = s + last;
	//decode inside scope
	return decodeString_0(s);
}

//decode inside job first
//isdigit() to check num char
class Solution394 {
public:
	string decodeString(const string& s, int& i) {//pass by const reference is faster
		string res;

		while (i < s.length() && s[i] != ']') {//if s[i]!=']' keep going
			if (!isdigit(s[i]))	//if s[i]==a~z add to res
				res += s[i++];
			else {				//if s[i]==num or [ 
				//get num n
				int n = 0;
				while (i < s.length() && isdigit(s[i]))
					n = n * 10 + s[i++] - '0';
				// step over '['
				i++; 
				//decode inside string
				string t = decodeString(s, i);
				// step over ']'
				i++; 
				//copy repeat string
				while (n-- > 0)
					res += t;
			}
		}
		return res;
	}

	string decodeString(string s) {
		int i = 0;
		return decodeString(s, i);
	}
};


//////////////////////////////////////////////////////////////
//456. 132 Pattern
/*
(...,s1,...,s2,...s3,...)&s1<s3<s2
from right to left, find the biggest s3 meet the requirement (...,s2,...s3,...)&s3<s2
check if s1 on left meet the requirement s1<s3
*/
bool find132pattern_1(vector<int>& nums) {
	int s3 = INT_MIN;
	stack<int> st;
	for (int i = nums.size() - 1; i >= 0; i--) {
		if (nums[i] < s3) return true;
		else while (!st.empty() && nums[i] > st.top()) {
			s3 = st.top(); st.pop();
		}
		st.push(nums[i]);
	}
	return false;
}

//////////////////////////////////////////////////////////////
//150. Evaluate Reverse Polish Notation
int evalRPN(vector<string>& tokens) {
	if (tokens.empty())
		return 0;
	if (tokens.size() == 1)
		return stoi(tokens[0]);
	stack<int> s;
	for (auto token : tokens) {
		if (isdigit(*token.rbegin()))//make sure negative num works
			s.push(stoi(token));
		else {
			int b = s.top();
			s.pop();
			int a = s.top();
			s.pop();
			if (token == "+")
				s.push(a + b);
			else if (token == "-")
				s.push(a - b);
			else if (token == "*")
				s.push(a*b);
			else if (token == "/")
				s.push(a / b);
		}
	}
	return s.top();
}

//lambda
int evalRPN_1(vector<string>& tokens) {
	unordered_map<string, function<int(int, int) > > map = {
		{ "+" , [](int a, int b) { return a + b; } },
		{ "-" , [](int a, int b) { return a - b; } },
		{ "*" , [](int a, int b) { return a * b; } },
		{ "/" , [](int a, int b) { return a / b; } }
	};
	std::stack<int> stack;
	for (string& s : tokens) {
		if (!map.count(s)) {
			stack.push(stoi(s));
		}
		else {
			int op1 = stack.top();
			stack.pop();
			int op2 = stack.top();
			stack.pop();
			stack.push(map[s](op2, op1));
		}
	}
	return stack.top();
}

//////////////////////////////////////////////////////////////
//341. Flatten Nested List Iterator
/*
// This is the interface that allows for creating nested lists.
// You should not implement it, or speculate about its implementation
class NestedInteger {
   public:
     // Return true if this NestedInteger holds a single integer, rather than a nested list.
	   bool isInteger() const;

     // Return the single integer that this NestedInteger holds, if it holds a single integer
     // The result is undefined if this NestedInteger holds a nested list
	   int getInteger() const;

     // Return the nested list that this NestedInteger holds, if it holds a nested list
     // The result is undefined if this NestedInteger holds a single integer
	   const vector<NestedInteger> &getList() const;
 };

 //ver0
class NestedIterator {
public:
	NestedIterator(vector<NestedInteger> &nestedList) {
		parse(nestedList);
	}

	int next() {
		int i = N.front();
		N.pop();
		return i;
	}

	bool hasNext() {
		return !N.empty();
	}

	void parse(vector<NestedInteger> nestedList) {
		if (nestedList.empty())
			return;
		while (!nestedList.empty()) {
			NestedInteger NI = *nestedList.begin();
			nestedList.erase(nestedList.begin());
			if (NI.isInteger())
				N.push(NI.getInteger());
			else {
				parse(NI.getList());
			}
		}
		return;
	}

	queue<int> N;

};


//shouldn't copy the entire data
class NestedIterator {
public:
	NestedIterator(vector<NestedInteger> &nestedList) {
		begins.push(nestedList.begin());
		ends.push(nestedList.end());
	}

	int next() {
		hasNext();//so next will not go error if user forget to call hasNext
		return (begins.top()++)->getInteger();
	}

	bool hasNext() {//tries to find an integer
		while (begins.size()) {
			if (begins.top() == ends.top()) {
				begins.pop();
				ends.pop();
			}
			else {
				auto x = begins.top();
				if (x->isInteger())
					return true;
				begins.top()++;
				begins.push(x->getList().begin());
				ends.push(x->getList().end());
			}
		}
		return false;
	}

private:
	stack<vector<NestedInteger>::iterator> begins, ends;
};

//ver2  save the copy time
class NestedIterator {
public:
    NestedIterator(vector<NestedInteger> &nestedList) {
        parse(nestedList.begin(),nestedList.end());
    }

    int next() {
        if(N.empty())
            return 66666;
		int i = N.front();
		N.pop();
		return i;
    }

    bool hasNext() {
        return !N.empty();
    }

private:
   void parse(vector<NestedInteger>::iterator begin,vector<NestedInteger>::iterator end) {
		while (begin!=end) {
			if (begin->isInteger())
				N.push(begin->getInteger());
			else 
				parse(begin->getList().begin(),begin->getList().end());
            begin++;
		}
		return;
	}
    
    queue<int> N;
    };
*/



//////////////////////////////////////////////////////////////
//103. Binary Tree Zigzag Level Order Traversal
//always remember to check empty input
class Solution103 {
public:
	vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
		vector<vector<int>> ret;
		vector<TreeNode*> que;//store the node of one level
		if (!root)
			return ret;
		que.push_back(root);//store level 0
		bool rev = 0;
		while (!que.empty()) {
			//zigzag and store
			ret.push_back(level(que, rev));
			rev = !rev;
			//get nodes of next level
			for (int i = 0, j = que.size(); i<j; i++) {
				if (que[0]->left)
					que.push_back(que[0]->left);
				if (que[0]->right)
					que.push_back(que[0]->right);
				que.erase(que.begin());
			}
		}
		return ret;
	}

	//turn a vector of node in to int, with given order
	vector<int> level(const vector<TreeNode*>& que, bool rev) {
		vector<int> l;
		if (!rev)
			for (auto n : que)
				l.push_back(n->val);
		else
			for (auto n : que)
				l.insert(l.begin(), n->val);
		return l;
	}
};

class Solution103_1 {
public:
	vector<vector<int> > zigzagLevelOrder(TreeNode* root) {
		if (root == NULL) {
			return vector<vector<int> >();
		}
		vector<vector<int> > result;

		queue<TreeNode*> nodesQueue;
		nodesQueue.push(root);
		bool leftToRight = true;

		while (!nodesQueue.empty()) {
			int size = nodesQueue.size();
			vector<int> row(size);
			for (int i = 0; i < size; i++) {
				TreeNode* node = nodesQueue.front();
				nodesQueue.pop();

				// find position to fill node's value
				int index = (leftToRight) ? i : (size - 1 - i);//calculate index is another option

				row[index] = node->val;
				if (node->left) {
					nodesQueue.push(node->left);
				}
				if (node->right) {
					nodesQueue.push(node->right);
				}
			}
			// after this level
			leftToRight = !leftToRight;
			result.push_back(row);
		}
		return result;
	}
};


//=================================heap medium=========================================================================
//659. Split Array into Consecutive Subsequences
//use a smarter way to keep trak of the sequences
/*
keep track of consecutive sequences
Now, iterate through each num in nums. 
if there exists a sequence which ends with element num-1, 
then append num to the end of the shortest such sequence; 
otherwise, create a new sequence that begins with num.
*/
bool isPossible_1(vector<int>& nums)
{
	/*backs[key] returns a priority queue representing all sequences that end with key*/
	/*priority_queue<int, vector<int>, std::greater<int>> 
		smallest value at top
		each value represent the size of a sequence ending with key
	*/
	unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>> backs;

	// Keep track of the number of sequences with size < 3
	int need_more = 0;

	for (int num : nums)
	{
		if (!backs[num - 1].empty())// There exists a sequence that ends in num-1
		{			
			int count = backs[num - 1].top();// get the size of shortest sequence ending with num-1
			backs[num - 1].pop();// Remove the existing shortest sequence
			backs[num].push(++count);// Add a new sequence ending in 'num' with size incremented by 1 

			if (count == 3)
				need_more--;
		}
		else// There is no sequence that ends in num-1
		{	
			// Create a new sequence with size 1 that ends with 'num'
			backs[num].push(1);

			need_more++;
		}
	}
	return need_more == 0;
}

//Trick: a new straight always start at duplicating positions
//count the frequencies of all numbers and record them in cnt
/*
tails[i] means the number of consecutive subsequences we've found so far, 
who are longer than 3 , and tailed by number i,
*/
bool isPossible_2(vector<int>& nums) {
	unordered_map<int, int> cnt, tails;
	for (int &i : nums) cnt[i]++;
	//second circle
	for (int &i : nums) {
		//check if this num is been used
		if (!cnt[i]) continue;
		cnt[i]--;
		if (tails[i - 1] > 0) {//if a good que can append do it
			tails[i - 1]--;
			tails[i]++;
		}
		else if (cnt[i + 1] && cnt[i + 2]) {//else create a good queue
			cnt[i + 1]--;
			cnt[i + 2]--;
			tails[i + 2]++;
		}
		else return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////
//502. IPO
//Any project with P[i] = 0 is useless and should be filtered away immediately (the problem only guarantees all inputs non-negative).
//only need to store necessary info (sorted if necessary)
int findMaximizedCapital(int k, int W, vector<int>& P, vector<int>& C) {
	priority_queue<int> low;      // sorted possible profits(big on top)
								  /*multiset: multiple elements can have equivalent values.
								  typically implemented as binary search trees.*/
	multiset<pair<int, int>> high; // undoable project with profit(small C in front)


	for (int i = 0; i < P.size(); ++i) // initialize low and high
		if (P[i] > 0) {
			if (C[i] <= W)
				low.push(P[i]);
			else
				high.emplace(C[i], P[i]);
		}

	while (k-- && low.size()) {
		W += low.top();// greedy to work on most profitable first
		low.pop();
		for (auto i = high.begin(); high.size() && i->first <= W; i = high.erase(i))                low.push(i->second);
	}
	return W;
}

//////////////////////////////////////////////////////////////
//347. Top K Frequent Elements
vector<int> topKFrequent_0(vector<int>& nums, int k) {
	unordered_map<int, int> hav;
	for (auto num : nums) 
		hav[num]++;
	
	priority_queue<pair<int, int>> sor;//allows same key
	for (auto it = hav.begin(); it != hav.end(); ++it)
		sor.emplace(it->second, it->first);
	vector<int> ret;
	while (k--) {
		ret.push_back(sor.top().second);
		sor.pop();
	}
	return ret;
}

vector<int> topKFrequent_1(vector<int>& nums, int k) {
	unordered_map<int, int> map;
	for (int num : nums) {
		map[num]++;
	}

	vector<int> res;
	// pair<first, second>: first is frequency,  second is number
	priority_queue<pair<int, int>> pq;
	for (auto it = map.begin(); it != map.end(); it++) {
		pq.push(make_pair(it->second, it->first));
		//size of priority_queue is (n - k), every operation in priority_queue takes O(log(n-k))
		if (pq.size() > (int)map.size() - k) {
			res.push_back(pq.top().second);
			pq.pop();
		}
	}
	return res;
}

class Solution_347 {
private:
	struct bigger {
		bool operator()(pair<int, int> &one, pair<int, int>two) {
			return one.second > two.second;
		}
	};
public:
	vector<int> topKFrequent(vector<int>& nums, int k) {
		/* Use unordered_map and priority_queue(minheap) solution.
		*
		* Use unordered_map to avoid red-black hash implement, which takes almost O(n*lgn).
		* We build a min heap with size k, so the time complexity is O(nlgk).
		*/
		unordered_map<int, int> num_count;
		for (const auto &n : nums) {
			num_count[n] += 1;
		}

		priority_queue<pair<int, int>, vector<pair<int, int>>, bigger> frequent_heap;
		// Build the min-heap with size k.
		for (auto it = num_count.begin(); it != num_count.end(); it++) {
			if (frequent_heap.size() < k) {
				frequent_heap.push(*it);
			}
			else if (it->second >= frequent_heap.top().second) {
				frequent_heap.pop();
				frequent_heap.push(*it);
			}
		}

		vector<int> ans;
		while (!frequent_heap.empty()) {
			auto top = frequent_heap.top();
			frequent_heap.pop();
			ans.push_back(top.first);
		}
		return ans;
	}
};

//////////////////////////////////////////////////////////////
// 215. Kth Largest Element in an Array
//priority_queue big always on top
int findKthLargest_0(vector<int>& nums, int k) {
	priority_queue<int> my(nums.begin(), nums.end());
	while (--k)
		my.pop();
	return my.top();
}

//O(nlogn) complexity
int findKthLargest_1(vector<int>& nums, int k) {
	sort(nums.begin(), nums.end());
	return nums[k - 1];
}

//multiset small always in front
int findKthLargest_2(vector<int>& nums, int k) {
	multiset<int> mset;
	int n = nums.size();
	for (int i = 0; i < n; i++) {
		mset.insert(nums[i]);
		if (mset.size() > k)
			mset.erase(mset.begin());
	}
	return *mset.begin();
}

//partial quick sort
/*
Initialize left to be 0 and right to be nums.size() - 1;
Partition the array, if the pivot is at the k - 1 - th position, return it(we are done);
If the pivot is right to the k - 1 - th position, update right to be the left neighbor of the pivot;
Else update left to be the right neighbor of the pivot.
Repeat 2.
*/
class Solution215 {
public:
	int partition(vector<int>& nums, int left, int right) {
		int pivot = nums[left];
		int l = left + 1, r = right;
		while (l <= r) {//must have = or can't deal with 2 element vector
			if (nums[l] < pivot && nums[r] > pivot)
				swap(nums[l++], nums[r--]);
			if (nums[l] >= pivot) l++;//must have = or can't deal with 2 equal elements vector
			if (nums[r] <= pivot) r--;
		}
		swap(nums[left], nums[r]);
		return r;
	}

	int findKthLargest(vector<int>& nums, int k) {
		int left = 0, right = nums.size() - 1;
		while (true) {
			int pos = partition(nums, left, right);
			if (pos == k - 1) return nums[pos];
			if (pos > k - 1) right = pos - 1;
			else left = pos + 1;
		}
	}
};
// quick sort in another way
class Solution215_0 {
public:
	//each call is O(n) time, total O(n) on average
	void partition(vector<int>& nums, int left, int right) {
		if (left >= right)
			return;
		int pivot = nums[left];
		int p = left;
		int l = left, r = right;
		while (l != r) {
			while (nums[r] <= pivot&&r>p) 
				r--;//must equal or can't deal with 2 equal elements vector{99,99}
			nums[p] = nums[r];
			p = r;
			while (nums[l] >= pivot&&l<p) 
				l++;
			nums[p] = nums[l];
			p = l;
		}
		nums[l] = pivot;
		partition(nums, left, l - 1);
		partition(nums, l + 1, right);
	}

	int findKthLargest(vector<int>& nums, int k) {
		partition(nums, 0, nums.size() - 1);
		for (auto n : nums)
			cout << n << ",";
		return nums[--k];
	}
};
//Heapsort
class Solution215_1 {
public:
	inline int left(int idx) {
		return (idx << 1) + 1;
	}
	inline int right(int idx) {
		return (idx << 1) + 2;
	}
	void max_heapify(vector<int>& nums, int idx) {
		int largest = idx;
		int l = left(idx), r = right(idx);
		if (l < heap_size && nums[l] > nums[largest]) largest = l;
		if (r < heap_size && nums[r] > nums[largest]) largest = r;
		if (largest != idx) {
			swap(nums[idx], nums[largest]);
			max_heapify(nums, largest);
		}
	}
	void build_max_heap(vector<int>& nums) {
		heap_size = nums.size();
		for (int i = (heap_size >> 1) - 1; i >= 0; i--)
			max_heapify(nums, i);
	}
	int findKthLargest(vector<int>& nums, int k) {
		build_max_heap(nums);
		for (int i = 0; i < k; i++) {
			swap(nums[0], nums[heap_size - 1]);
			heap_size--;
			max_heapify(nums, 0);
		}
		return nums[heap_size];
	}
private:
	int heap_size;
};

//////////////////////////////////////////////////////////////
//451. Sort Characters By Frequency
//memory limit exceed
string frequencySort_0(string s) {
	unordered_map<char, long> map;
	for (char c : s)
		map[c]++;
	priority_queue<pair<long, char>> ord;
	for (auto it = map.begin(); it != map.end(); it++)
		ord.emplace(it->second, it->first);
	string r;
	while (!ord.empty()) {
		pair<long, char> p = ord.top();
		ord.pop();
		long i = p.first;
		char c = p.second;
		while (i--)
			r = r + c;
	}
	return r;
}


string frequencySort_1(string s) {
	unordered_map<char, int> freq;
	vector<string> bucket(s.size() + 1, "");//use vector position to store num
	string res;

	//count frequency of each character
	for (char c : s) freq[c]++;
	//put character into frequency bucket
	for (auto& it : freq) {
		int n = it.second;
		char c = it.first;
		bucket[n].append(n, c);//append n times of c
	}
	//form descending sorted string
	for (int i = s.size(); i>0; i--) {
		if (!bucket[i].empty())
			res.append(bucket[i]);
	}
	return res;
}

//array of pair, sort with lambda 
string frequencySort_2(string s)
{
	string ans;
	pair<char, int> freq[256] = { { 0, 0 } };
	for (const auto &c : s) {
		freq[c].first = c;
		++freq[c].second;
	}
	sort(begin(freq), end(freq),
		[](pair<char, int>& l, pair<char, int>& r) {
		return l.second > r.second;
	});
	for (auto it = begin(freq); it != end(freq); ++it)
		ans.append(it->second, it->first);
	return ans;
}

//sort with static member function
class Solution451 {
public:
	string frequencySort(string s){
		string ans;
		pair<char, int> freq[256] = { { 0, 0 } };
		for (const auto &c : s) {
			freq[c].first = c;
			++freq[c].second;
		};
		sort(begin(freq), end(freq), myfunction);
		for (auto it = begin(freq); it != end(freq); ++it)
			ans.append(it->second, it->first);
		return ans;
	}
private:
	static bool myfunction(pair<char, int> l, pair<char, int> r) {
		return l.second > r.second;
	}
};

//sort unordered_map with lambda
string frequencySort_3(string s) {
	unordered_map<char, int> hist;
	for (auto c : s) {
		++hist[c];
	}

	auto pred = [&hist](char a, char b) {
		return hist[a] > hist[b] || (hist[a] == hist[b] && a > b);
	};

	sort(s.begin(), s.end(), pred);

	return s;
}

//////////////////////////////////////////////////////////////
//373. Find K Pairs with Smallest Sums
//O(mim(m, k) mim(n, k)logk)
//direct operator in structor into priority_queue
class Solution373 {
private:
	struct mycompare {
		bool operator()(pair<int, int>& p1, pair<int, int>& p2) {//use < then max at the top(like default)
			return p1.first + p1.second < p2.first + p2.second;
		}
	};
public:
	vector<pair<int, int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
		vector<pair<int, int>> res;
		priority_queue<pair<int, int>, vector<pair<int, int> >, mycompare> pq;
		//walk through leading k of each list
		for (int i = 0; i < min((int)nums1.size(), k); i++) {
			for (int j = 0; j < min((int)nums2.size(), k); j++) {
				if (pq.size() < k)
					pq.push(make_pair(nums1[i], nums2[j]));
				else if (nums1[i] + nums2[j] < pq.top().first + pq.top().second) {//make use of the priority queue
					pq.push(make_pair(nums1[i], nums2[j]));
					pq.pop();
				}
			}
		}
		while (!pq.empty()) {
			res.push_back(pq.top());
			pq.pop();
		}
		return res;
	}

};

//direct lambda into priority_queue
vector<pair<int, int>> kSmallestPairs_1(vector<int>& nums1, vector<int>& nums2, int k)
{
	vector<pair<int, int>> v;
	if (nums1.empty() || nums2.empty()) return v;

	auto cmp = [](const pair<int, int>& a, const pair<int, int>& b) {
		return a.first + a.second > b.first + b.second; };

	priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> minHeap(cmp);

	for (auto n : nums1)
		for (auto m : nums2)
			minHeap.emplace(n, m);
	while (minHeap.size() && k--)
	{
		v.emplace_back(minHeap.top());
		minHeap.pop();
	}
	return v;
}

//////////////////////////////////////////////////////////////
//313. Super Ugly Number
/*Every ugly num is created by multiplying a existing ugly num and a prime num.
To walk through all possibility, must try each (prime)*(smallest unused ugly num) when adding a new ugly num.
To keep track of the smallets existing un-used ugly number as the factor (avoid duplication),
need to store a index pointing to the next smallest ugly num can be used by each prime, starting index are all 0
because the ugly is stored ascend, we can just use ++ to keep updating the index
*/
int nthSuperUglyNumber(int n, vector<int>& primes) {
	int K = primes.size();
	vector<int> index(K, 0);//index of the smallest ugly numbers can be used by each prime
	//list of ugly nums, all except ugly[0] is set to max
	vector<int> ugly(n, INT_MAX);
	ugly[0] = 1;

	for (int i = 1; i<n; i++) {//generate n unknown ugly
		//walk through all prime*(smallest ugly can be used by this prime), choose the min
		for (int j = 0; j<K; j++) 
			ugly[i] = min(ugly[i], ugly[index[j]] * primes[j]);
		//update the index of used prime
		for (int j = 0; j<K; j++)
			index[j] += (ugly[i] == ugly[index[j]] * primes[j]);
	}
	return ugly[n - 1];
}

//////////////////////////////////////////////////////////////
//264. Ugly Number II
int nthUglyNumber(int n) {
	vector<int> primes = { 2,3,5 };
	vector<int> index(3, 0);
	vector<int> ugly(n, INT_MAX);
	ugly[0] = 1;

	for (size_t i = 1; i<n; i++) {
		for (size_t j = 0; j<3; j++)
			ugly[i] = min(ugly[i], ugly[index[j]] * primes[j]);
		for (size_t j = 0; j<3; j++)
			index[j] += (ugly[i] == ugly[index[j]] * primes[j]);
	}
	return ugly[n - 1];
}

//////////////////////////////////////////////////////////////
//355. Design Twitter
//std::make/push/pop_heap provide finer control than std::priority_queue.
//make_heap(h.begin(),h.end(),lambda)
class Twitter_1
{
	struct Tweet//make local struct is very helpful
	{
		int time;
		int id;
		Tweet(int time, int id) : time(time), id(id) {}
	};

	std::unordered_map<int, std::vector<Tweet>> tweets; // [u] = array of tweets by u
	std::unordered_map<int, std::unordered_set<int>> following; // [u] = array of users followed by u

	int time;

public:
	Twitter_1() : time(0) {}

	void postTweet(int userId, int tweetId)
	{
		tweets[userId].emplace_back(time++, tweetId);
	}

	std::vector<int> getNewsFeed(int userId)
	{
		//h stores tweets from all followed users(his oldest and newest in memory)
		std::vector<std::pair<Tweet*, Tweet*>> h; // pair of pointers (begin, current)

		//put all interested pointers in h
		for (auto& u : following[userId])//auto& get the u as reference
		{
			auto& t = tweets[u];
			if (t.size() > 0)
				h.emplace_back(t.data(), t.data() + t.size() - 1);//data direct get the interanl pointer
		}
		auto& t = tweets[userId]; 
		if (t.size() > 0)
			h.emplace_back(t.data(), t.data() + t.size() - 1);

		//always sort the h using newest tweet* 
		auto f = [](const std::pair<Tweet*, Tweet*>& x, const std::pair<Tweet*, Tweet*>& y) {
			return x.second->time < y.second->time;
		};
		std::make_heap(h.begin(), h.end(), f);

		const int n = 10;
		std::vector<int> o;
		o.reserve(n);
		for (int i = 0; (i < 10) && !h.empty(); ++i)
		{
			//get the user who have the newest tweet
			std::pop_heap(h.begin(), h.end(), f);
			auto& hb = h.back();
			//store the newest tweet id in output
			o.push_back(hb.second->id);

			//move out the newest tweet from this user
			if (hb.first == hb.second--)//if tweets from this user is used up 
				h.pop_back();//pop him
			else//else put this user back to heap
				std::push_heap(h.begin(), h.end(), f);
		}
		return o;
	}

	void follow(int followerId, int followeeId)
	{
		if (followerId != followeeId)
			following[followerId].insert(followeeId);
	}

	void unfollow(int followerId, int followeeId)
	{
		following[followerId].erase(followeeId);
	}
};


void partition(vector<int>& nums, int left, int right) {
	if (left >= right)
		return;
	int pivot = nums[left];
	int p = left;
	int l = left, r = right;
	while (l != r) {
		while (nums[r]<pivot&&r>p) r--;
		nums[p] = nums[r];
		p = r;
		while (nums[l]>pivot&&l<p) l++;
		nums[p] = nums[l];
		p = l;
	}
	nums[l] = pivot;
	partition(nums, left, l - 1);
	partition(nums, l + 1, right);

}

#include<string>
#include<iostream>
void main() {

	vector<int> findNums = { 2, 7, 13, 19 };
	vector<int> nums = {99, 99 };
	//removeKdigits("10200", 1);
	string s = "abababababababababababbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbabababa";

	nthSuperUglyNumber(12, findNums);

	Twitter_1 obj;
	obj.postTweet(1,5);
	vector<int> param_2 = obj.getNewsFeed(1);
	obj.follow(1,2);
	obj.postTweet(2,6);
	vector<int> param_3 = obj.getNewsFeed(1);
	obj.unfollow(1,2);
	vector<int> param_4 = obj.getNewsFeed(1);
	partition(nums,0, 1);
	
	//string s = "[123,[456,[789]]]";
	//stringstream in(s);
	//in.get();
	//if (in >> number)
	//	cout << "good";

	//decodeString("3[a2[c]]");
	
	//TreeNode* a = new TreeNode(3);
	//TreeNode* b = new TreeNode(9);
	//TreeNode* c = new TreeNode(20);
	//TreeNode* d = new TreeNode(15);
	//TreeNode* e = new TreeNode(7);

	//a->left = b;
	//a->right = c;
	//c->left = d;
	//c->right = e;

	//Solution103 myS;
	//myS.zigzagLevelOrder(a);
}


/*
string raw_pattern = R"((\+|-)?[[:digit:]]+)";//raw string
regex integer(raw_pattern);//Regular Expression
cin >> input;
if (regex_match(input, rr))
      cout << "match" << endl;
*/

//lambda
/*
[](input_paramter_declaration)->returned_type {body_of_the_lambda_expression}(parameters)
auto func = [](int a, int b) -> int { return a+b; };
*/

//use of lambda
/*
for_each(begin(l), end(l),lambda)

find_if_not(begin(l), end(l),lambda)

sort(begin(l), end(l),lambda)

*/