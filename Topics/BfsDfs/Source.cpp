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
#include <numeric>

using namespace std;

//====================================================BFS medium=============================

////////////////////////////////////////////////////////////////////////////
//490. The Maze
bool hasPath_0(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
	int lin = maze.size();
	int col = maze[0].size();

	if (maze[start[0]][start[1]] == -1)//if already visited
		return false;
	maze[start[0]][start[1]] = -1;

	if (start == destination)
		return true;

	vector<int> shift0 = { 1,-1,0,0 }, shift1 = { 0,0,1,-1 };
	for (int i = 0; i<4; i++) {
		//roll to the wall
		int l = start[0] + shift0[i], c = start[1] + shift1[i];
		while (l >= 0 && l<lin&&c >= 0 && c<col&&maze[l][c] != 1)
			l += shift0[i], c += shift1[i];
		l -= shift0[i], c -= shift1[i];

		vector<int> stop = { l,c };
		if (hasPath_0(maze, stop, destination))
			return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////
//505. The Maze II
//simple modificatio is slow
class Solution505_0 {
private:
	int lin, col;
	vector<int> shift0 = { 1,-1,0,0 }, shift1 = { 0,0,1,-1 };
	bool DFS(vector<vector<int>>& maze, int& l, int& c, vector<int>& destination, vector<vector<int>>& dist, int step) {
		if (step >= dist[l][c])
			return false;
		else
			dist[l][c] = step;

		if (l == destination[0] && c == destination[1])
			return true;

		bool check = false;
		for (int i = 0; i<4; i++) {
			int ss = step, ll = l, cc = c;
			ll += shift0[i], cc += shift1[i];
			while (ll >= 0 && ll<lin&&cc >= 0 && cc<col&&maze[ll][cc] != 1) {
				ss++;
				ll += shift0[i], cc += shift1[i];
			}
			ll -= shift0[i], cc -= shift1[i];
			if (DFS(maze, ll, cc, destination, dist, ss))
				check = true;
		}
		return check;
	}
public:
	int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
		lin = maze.size();
		col = maze[0].size();
		vector<vector<int>> dist(lin, vector<int>(col, INT_MAX));

		if (DFS(maze, start[0], start[1], destination, dist, 0))
			return dist[destination[0]][destination[1]];
		return -1;
	}
};

//using queue instead of recursion is faster here
int shortestDistance_1(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
	int lin = maze.size(), col = maze[0].size();
	vector<int> shift0 = { 1,-1,0,0 }, shift1 = { 0,0,1,-1 };
	vector<vector<int>> dist(lin, vector<int>(col, INT_MAX));
	queue<vector<int>> current;
	current.push(start);
	dist[start[0]][start[1]] = 0;

	while (current.size()) {
		int l = current.front()[0], c = current.front()[1];
		current.pop();
		for (int i = 0; i<4; i++) {
			int dd = dist[l][c], ll = l, cc = c;
			ll += shift0[i], cc += shift1[i];
			while (ll >= 0 && ll<lin&&cc >= 0 && cc<col&&maze[ll][cc] != 1) {
				dd++;
				ll += shift0[i], cc += shift1[i];
			}
			ll -= shift0[i], cc -= shift1[i];
			if (dd<dist[ll][cc]) {
				dist[ll][cc] = dd;
				vector<int> tem = { ll,cc };
				if (ll == destination[0] && cc == destination[1])
					continue;
				current.push(tem);
			}
		}
	}
	if (dist[destination[0]][destination[1]] == INT_MAX)
		return -1;
	return dist[destination[0]][destination[1]];
}

////////////////////////////////////////////////////////////////////////////
//286. Walls and Gates
//
void wallsAndGates_0(vector<vector<int>>& rooms) {
	int lin, col;
	if ((lin = rooms.size())<1 || (col = rooms[0].size())<1)
		return;
	queue<pair<int, int>> curr;
	for (int i = 0; i<lin; i++) {
		for (int j = 0; j<col; j++) {
			if (rooms[i][j] == 0)
				curr.emplace(i, j);
		}
	}
	vector<pair<int, int>> shift = { { -1,0 },{ 1,0 },{ 0,1 },{ 0,-1 } };
	while (curr.size()) {
		int l = curr.front().first;
		int r = curr.front().second;
		curr.pop();
		for (auto& s : shift) {
			int ll = l, rr = r;
			ll += s.first, rr += s.second;
			if (ll<0 || ll >= lin || rr<0 || rr >= col || rooms[ll][rr] == -1)
				continue;
			if (rooms[l][r] + 1<rooms[ll][rr]) {
				rooms[ll][rr] = rooms[l][r] + 1;
				curr.emplace(ll, rr);
			}
		}
	}
}

//DFS si more elegant and faster 
class Solution286 {
private:
	int lin, col;
	void dfs(vector<vector<int>>& rooms, int ll, int rr, int level) {
		if (ll<0 || ll >= lin || rr<0 || rr >= col || rooms[ll][rr] == -1 || level>rooms[ll][rr])
			return;
		rooms[ll][rr] = level;
		dfs(rooms, ll + 1, rr, level + 1);
		dfs(rooms, ll - 1, rr, level + 1);
		dfs(rooms, ll, rr + 1, level + 1);
		dfs(rooms, ll, rr - 1, level + 1);
	}
public:
	void wallsAndGates(vector<vector<int>>& rooms) {
		if ((lin = rooms.size())<1 || (col = rooms[0].size())<1)
			return;
		for (int i = 0; i<lin; i++) {
			for (int j = 0; j<col; j++) {
				if (rooms[i][j] == 0) dfs(rooms, i, j, 0);
			}
		}
	}
};

////////////////////////////////////////////////////////////////////////////
//127. Word Ladder
//BFS notice this graph not having tree characteristics, circle may exist
int ladderLength_0(string beginWord, string endWord, vector<string>& wordList) {
	//circle may exist, neet a record for reference
	unordered_map<string, bool> visited;
	visited[beginWord] = false;
	//make adjancancy list of all words   
	unordered_map<string, unordered_set<string>> adj;
	wordList.push_back(beginWord);//beginWord is not in wordList.             
	int n = wordList.size();
	int len = beginWord.size();
	for (int i = 0; i<n - 1; i++) {//compare every two words
		string word_a = wordList[i];
		visited[word_a] = false;
		for (int j = i + 1; j<n; j++) {
			string word_b = wordList[j];
			int count = 0;
			for (int x = 0; x<len; x++) {
				if (word_a[x] == word_b[x])
					count++;
			}
			if (count == len - 1) {
				adj[word_a].insert(word_b);
				adj[word_b].insert(word_a);
			}
		}
	}
	//BFS level by level
	vector<string> current;
	current.push_back(beginWord);
	int step = 1;
	while (true) {
		vector<string> next;
		step++;
		for (auto& curr : current) {
			visited[curr] = true;
			for (auto& neighbor : adj[curr]) {
				if (neighbor == endWord)
					return step;
				if (!visited[neighbor])
					next.push_back(neighbor);
			}
		}
		if (next.size() == 0)
			break;
		current = next;
	}
	return 0;
}

//because only have lower letter, can search adjancancy words smarter
class Solution127_1 {
public:
	int ladderLength(string beginWord, string endWord, vector<string>& wordDict) {
		unordered_set<string> Dict(wordDict.begin(), wordDict.end());
		queue<string> toVisit;
		addNextWords(beginWord, Dict, toVisit);
		int dist = 1;
		while (!toVisit.empty()) {
			int num = toVisit.size();
			dist++;
			for (int i = 0; i < num; i++) {
				string word = toVisit.front();
				toVisit.pop();
				if (word == endWord)
					return dist;
				addNextWords(word, Dict, toVisit);
			}

		}
		return 0;
	}
private:
	//search for neighbor smarter, 
	void addNextWords(string word, unordered_set<string>& Dict, queue<string>& toVisit) {
		Dict.erase(word);
		for (int p = 0; p < (int)word.length(); p++) {
			char letter = word[p];
			for (int k = 0; k < 26; k++) {
				word[p] = 'a' + k;
				if (Dict.find(word) != Dict.end()) {
					toVisit.push(word);
					Dict.erase(word);
				}
			}
			word[p] = letter;
		}
	}
};

//two-end BFS
/*
Search from both ends.
'prevWords' will alway be the smaller set in two ends (for speed up)
From 'prevWords', find the set of words which are one character from 'prevWords'.
Check each word adj to 'prevWords'(by changing one character), if it is in 'nextWord' we are done.
Otherwise, update the 'prevWords'
*/
int ladderLength_2(string beginWord, string endWord, vector<string>& wordDict) {
	unordered_set<string> Dict(wordDict.begin(), wordDict.end());//unordered_set is easier to manipulate
																 //Dict.erase(beginWord);//beginWord is not a transformed word
	if (Dict.erase(endWord) == 0)//endWord may not part of the Dict
		return 0;
	unordered_set<string> prevWords;//start side group
	unordered_set<string> nextWords;//stop side group
	prevWords.insert(beginWord);
	nextWords.insert(endWord);
	int ladder = 2, len = beginWord.length();
	while (!nextWords.empty() && !prevWords.empty()) {
		if (nextWords.size() < prevWords.size())//always do search from the end with smaller elements
			swap(nextWords, prevWords);
		unordered_set<string> temp;
		//for (auto itr = prevWords.begin(); itr != prevWords.end(); itr++) {
		for (const auto& w : prevWords) {
			//string word = *itr;
			string word(w);
			Dict.erase(word);
			for (int p = 0; p < len; p++) {
				char letter = word[p];
				for (int j = 0; j < 26; j++) {
					word[p] = 'a' + j;
					if (nextWords.find(word) != nextWords.end())//when two end meets, it's done
						return ladder;
					if (Dict.find(word) != Dict.end()) {//if not met, update the prevWords group
						temp.insert(word);
					}
				}
				word[p] = letter;
			}
		}
		swap(prevWords, temp);
		ladder++;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//743. Network Delay Time
//real Dijkstra
int networkDelayTime_1(vector<vector<int>>& times, int V, int K) {
	vector<vector<pair<int, int>>> graph(V + 1, vector<pair<int, int>>());
	for (auto &edge : times)
		graph[edge[0]].emplace_back(edge[1], edge[2]); // graph[from] = { to, time }

	vector<int> timeTo(V + 1, INT_MAX);
	timeTo[K] = 0;
	set<pair<int, int>> pq{ { 0, K } }; // { time, to }, makes a non duplicated pq
	while (!pq.empty()) {
		int v = pq.cbegin()->second;
		pq.erase(pq.begin());
		for (auto &adj : graph[v]) {
			int w = adj.first, time = adj.second;
			if (timeTo[v] + time < timeTo[w]) {
				pq.erase({ timeTo[w], w });
				timeTo[w] = timeTo[v] + time;
				/* Adding w into set if and only if w was relaxed succesfully
				is the key to guarantee good time complexity O(ElogV)
				and don't need to create visited array */
				pq.emplace(timeTo[w], w);
			}
		}
	}
	int ret = 0;
	for (int v = 1; v <= V; ++v)
		ret = max(ret, timeTo[v]);
	return ret == INT_MAX ? -1 : ret;
}

//Bellman Ford
int networkDelayTime(vector<vector<int>>& times, int N, int K) {
	vector<int> dist(N + 1, INT_MAX);
	dist[K] = 0;
	for (int i = 0; i < N; i++) {
		for (vector<int> e : times) {
			int u = e[0], v = e[1], w = e[2];
			if (dist[u] != INT_MAX && dist[v] > dist[u] + w) {//avoid overflow
				dist[v] = dist[u] + w;
			}
		}
	}

	int maxwait = 0;
	for (int i = 1; i <= N; i++)
		maxwait = max(maxwait, dist[i]);
	return maxwait == INT_MAX ? -1 : maxwait;
}

////////////////////////////////////////////////////////////////////////////
//542. 01 Matrix
vector<vector<int>> updateMatrix_0(vector<vector<int>>& matrix) {
	int lin, col;
	if ((lin = matrix.size()) == 0 || (col = matrix[0].size()) == 0)
		return matrix;
	vector<vector<int>> ret(lin, vector<int>(col, INT_MAX));
	vector<pair<int, int>> current;
	for (int l = 0; l<lin; l++) {
		for (int c = 0; c<col; c++) {
			if (matrix[l][c] == 0) {
				current.emplace_back(l, c);//vector should use emplace_back
				ret[l][c] = 0;
			}
		}
	}

	vector<int> shiftl = { -1,1,0,0 }, shiftc = { 0,0,-1,1 };
	int level = 1;
	while (current.size()) {
		vector<pair<int, int>> next;
		for (auto& c : current) {
			for (int s = 0; s<4; s++) {
				int line = c.first + shiftl[s], colum = c.second + shiftc[s];
				if (line >= 0 && line<lin&&colum >= 0 && colum<col&&ret[line][colum] == INT_MAX) {
					next.emplace_back(line, colum);
					ret[line][colum] = level;
				}
			}
		}
		current = next;
		level++;
	}
	return ret;
}

//DP is another thought
/*
The first pass makes sure we have distance to nearest zero from top-left direction.
The second pass makes sure we have distance to nearest zero from bottom-right direction.
*/
vector<vector<int>> updateMatrix(vector<vector<int>>& matrix) {
	int m = matrix.size(), n = matrix[0].size();
	vector<vector<int>> ans(m, vector<int>(n, INT_MAX));

	// top-left to bottom-right DP         
	for (int r = 0; r < m; r++) {
		for (int c = 0; c < n; c++) {
			if (matrix[r][c] == 0)
				ans[r][c] = 0;
			else {
				if (r > 0 && ans[r - 1][c] != INT_MAX && 1 + ans[r - 1][c]<ans[r][c])
					ans[r][c] = 1 + ans[r - 1][c];
				if (c > 0 && ans[r][c - 1] != INT_MAX && 1 + ans[r][c - 1]<ans[r][c])
					ans[r][c] = 1 + ans[r][c - 1];
			}
		}
	}

	// bottom-right to top-left DP
	for (int r = m - 1; r >= 0; r--) {
		for (int c = n - 1; c >= 0; c--) {
			if (matrix[r][c] == 0)
				ans[r][c] = 0;
			else {
				if (r < m - 1 && ans[r + 1][c] != INT_MAX && 1 + ans[r + 1][c]<ans[r][c])
					ans[r][c] = 1 + ans[r + 1][c];
				if (c < n - 1 && ans[r][c + 1] != INT_MAX && 1 + ans[r][c + 1]<ans[r][c])
					ans[r][c] = 1 + ans[r][c + 1];
			}
		}
	}
	return ans;
}

////////////////////////////////////////////////////////////////////////////
//102. Binary Tree Level Order Traversal
struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

vector<vector<int>> levelOrder(TreeNode* root) {
	vector<TreeNode*> level0;
	vector<vector<int>> ret;
	if (!root)
		return ret;
	level0.push_back(root);
	while (level0.size()) {
		vector<TreeNode*> level1;
		vector<int> levelrec;
		for (auto& node : level0) {
			levelrec.push_back(node->val);
			if (node->left)
				level1.push_back(node->left);
			if (node->right)
				level1.push_back(node->right);
		}
		ret.push_back(levelrec);
		level0 = level1;
	}
	return ret;
}

//a clean recur
class Solution102_1 {
public:
	vector<vector<int>> ret;

	void buildVector(TreeNode *root, int depth)
	{
		if (root == NULL) return;
		if (ret.size() == depth)
			ret.push_back(vector<int>());

		ret[depth].push_back(root->val);
		buildVector(root->left, depth + 1);
		buildVector(root->right, depth + 1);
	}

	vector<vector<int> > levelOrder(TreeNode *root) {
		buildVector(root, 0);
		return ret;
	}
};


////////////////////////////////////////////////////////////////////////////
//279. Perfect Squares
//BFS
int numSquares_0(int n) {
	unordered_set<int> cand;// all perfect square numbers which <= n.
	vector<int> current;//current level for BFS
	vector<bool> triedNum(n + 1, false);//if a number's numSquares already known or in the queue to be calculated

	int sqrtn = sqrt(n);
	if (sqrtn*sqrtn == n)
		return 1;

	for (int i = 1; i <= sqrtn; i++) {
		cand.insert(i*i);
		current.push_back(i*i);
		triedNum[i*i] = true;
	}

	int count = 2;
	while (true) {
		vector<int> next;//next level for BFS
		for (auto& cur : current) {
			for (auto & can : cand) {
				if (can + cur == n)
					return count;
				if (can + cur<n&&!triedNum[can + cur]) {//avoid repeat effort
					next.push_back(can + cur);
					triedNum[can + cur] = true;
				}
			}
		}
		current = next;
		count++;
	}
	return count;
}

//math
/*knowing Lagrange's four-square theorem
telling there are only 4 possible results: 1, 2, 3, 4.*/
/*knowing Legendre's three-square theorem.
telling the result is 4 if and only if n can be
written in the form of 4^k*(8*m + 7).*/
class Solution279_1 {
public:
	int is_square(int n) {
		int temp = (int)sqrt(n);
		return temp * temp == n;
	}
	int numSquares(int n) {
		//A natural number is a sum of three squares 
		//if and only if it's not of the form 4^a(8b+7) with integers a and b.
		while ((n & 3) == 0) //n%4 == 0  
			n >>= 2;  //n /= 4
		if ((n & 7) == 7) //n % 8 == 7  
			return 4;
		//brute force one square
		if (is_square(n))
			return 1;
		//brute force sum of two squares 
		int sqrt_n = (int)sqrt(n);
		for (int i = 1; i <= sqrt_n; i++) {
			if (is_square(n - i*i))
				return 2;
		}
		//known left case
		return 3;
	}
};

//DP
/*the truth of DP: know few of begining status
know the relation between latter status and earlier status*/
//static can take advantage of the online judgement method 
int numSquares_2(int n)
{
	if (n <= 0)
	{
		return 0;
	}

	// cntPerfectSquares[i] = the least number of perfect square numbers 
	// which sum to i. 
	static vector<int> cntPerfectSquares({ 0 });//static can save previous effort if called multiple times

												// While cntPerfectSquares.size() <= n, we need to incrementally 
												// calculate the next result until we get the result for n.
	while (cntPerfectSquares.size() <= n)
	{
		int m = cntPerfectSquares.size();
		int cntSquares = INT_MAX;
		for (int i = 1; i*i <= m; i++)
		{
			cntSquares = min(cntSquares, cntPerfectSquares[m - i*i] + 1);
		}

		cntPerfectSquares.push_back(cntSquares);
	}

	return cntPerfectSquares[n];
}


////////////////////////////////////////////////////////////////////////////
//417. Pacific Atlantic Water Flow
//BFS
vector<pair<int, int>> pacificAtlantic_0(vector<vector<int>>& matrix) {
	int lin, col;
	vector<pair<int, int>> ret;
	if ((lin = matrix.size()) == 0 || (col = matrix[0].size()) == 0)
		return ret;
	//get the starting point of P and A
	queue<pair<int, int>> currentP, currentA;
	for (int j = 0; j<col; j++) {
		currentP.emplace(0, j);
		currentA.emplace(lin - 1, j);
	}
	for (int i = 0; i<lin; i++) {
		currentP.emplace(i, 0);
		currentA.emplace(i, col - 1);
	}
	//mark anything can be climbed up to from P and A
	vector<vector<bool>> toP(lin, vector<bool>(col, false)), toA(lin, vector<bool>(col, false));
	vector<int> shiftX = { 0,0,-1,1 }, shiftY = { -1,1,0,0 };
	while (currentP.size()) {
		int x = currentP.front().first, y = currentP.front().second;
		toP[x][y] = true;
		currentP.pop();
		for (int i = 0; i<4; i++) {
			int xx = x + shiftX[i], yy = y + shiftY[i];
			if (xx<0 || xx >= lin || yy<0 || yy >= col || toP[xx][yy])
				continue;
			if (matrix[xx][yy] >= matrix[x][y]) {
				currentP.emplace(xx, yy);
			}
		}
	}
	while (currentA.size()) {
		int x = currentA.front().first, y = currentA.front().second;
		toA[x][y] = true;
		currentA.pop();
		for (int i = 0; i<4; i++) {
			int xx = x + shiftX[i], yy = y + shiftY[i];
			if (xx<0 || xx >= lin || yy<0 || yy >= col || toA[xx][yy])
				continue;
			if (matrix[xx][yy] >= matrix[x][y]) {
				currentA.emplace(xx, yy);
			}
		}
	}
	//get the area can be reached from both side
	for (int l = 0; l<lin; l++) {
		for (int c = 0; c<col; c++) {
			if (toP[l][c] && toA[l][c])
				ret.emplace_back(l, c);
		}
	}
	return ret;
}

//DFS
//using 1+2==3 smart
class Solution417_1 {
public:
	vector<pair<int, int>> res;
	vector<vector<int>> visited;//(1 = reachable to Pac, 2 = Atl, 3 = both)

	void dfs(int i, int j, vector<vector<int>>& matrix, int pre, int label) {
		if (i<0 || i>matrix.size() - 1 || j<0 || j>matrix[0].size() - 1 || matrix[i][j]<pre || visited[i][j] == 3 || visited[i][j] == label)
			return;
		visited[i][j] += label;
		if (visited[i][j] == 3)
			res.push_back({ i,j });
		dfs(i + 1, j, matrix, matrix[i][j], label);
		dfs(i - 1, j, matrix, matrix[i][j], label);
		dfs(i, j + 1, matrix, matrix[i][j], label);
		dfs(i, j - 1, matrix, matrix[i][j], label);
	}

	vector<pair<int, int>> pacificAtlantic(vector<vector<int>>& matrix) {
		if (matrix.empty())
			return res;
		int m = matrix.size();
		int n = matrix[0].size();
		visited.resize(m, vector<int>(n, 0));
		for (int j = 0; j<n; j++) {
			dfs(0, j, matrix, INT_MIN, 1);
			dfs(m - 1, j, matrix, INT_MIN, 2);
		}

		for (int i = 0; i<m; i++) {
			dfs(i, 0, matrix, INT_MIN, 1);
			dfs(i, n - 1, matrix, INT_MIN, 2);
		}
		return res;
	}
};

////////////////////////////////////////////////////////////////////////////
//199. Binary Tree Right Side View
vector<int> rightSideView(TreeNode* root) {
	vector<int> view;
	if (!root)
		return view;
	vector<TreeNode*> current;
	current.push_back(root);
	while (current.size()) {
		vector<TreeNode*> nxt;
		view.push_back(current.back()->val);
		for (auto& c : current) {
			if (c->left)
				nxt.push_back(c->left);
			if (c->right)
				nxt.push_back(c->right);
		}
		current = nxt;
	}
	return view;
}

//modified preorder traversal, solv binary problem more binary
class Solution199_1 {
public:
	void recursion(TreeNode *root, int level, vector<int> &res)
	{
		if (root == NULL)
			return;

		if (res.size()<level) //the first on hit in the next level is pushed to result
			res.push_back(root->val);
		recursion(root->right, level + 1, res);//right first
		recursion(root->left, level + 1, res);
	}

	vector<int> rightSideView(TreeNode *root) {
		vector<int> res;
		recursion(root, 1, res);
		return res;
	}
};

//513. Find Bottom Left Tree Value
//very like Question 199
class Solution513_0 {
public:
	int findBottomLeftValue(TreeNode* root) {
		if (!root)
			return 0;
		vector<int> leftView;
		traversal(root, 0, leftView);
		return leftView.back();
	}
	void traversal(TreeNode* root, int level, vector<int>& leftView) {
		if (!root)
			return;
		if (level == leftView.size())
			leftView.push_back(root->val);
		traversal(root->left, level + 1, leftView);
		traversal(root->right, level + 1, leftView);
	}
};

//BFS
int findBottomLeftValue_1(TreeNode* root) {
	int result = 0;
	queue<TreeNode*> queuee;
	queuee.push(root);

	while (queuee.size()) {
		int size = queuee.size();
		for (int i = 0; i < size; i++) {
			TreeNode* node = queuee.front(); queuee.pop();
			if (i == 0) result = node->val;
			if (node->left != NULL) queuee.push(node->left);
			if (node->right != NULL) queuee.push(node->right);
		}
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////
//529. Minesweeper
//BFS
vector<vector<char>> updateBoard_0(vector<vector<char>>& board, vector<int>& click) {
	queue<vector<int>> current;
	int lin = board.size(), col = board[0].size();
	vector<vector<bool>> queued(lin, vector<bool>(col, false));//remember points already in current queue
	current.push(click);
	queued[click[0]][click[1]] = true;
	vector<int> shiftl = { -1,-1,-1,0,0,1,1,1 }, shiftc = { -1,0,1,-1,1,-1,0,1 };
	while (current.size()) {
		int l = current.front()[0], c = current.front()[1];
		current.pop();
		if (board[l][c] == 'M')
			board[l][c] = 'X';
		else if (board[l][c] == 'E') {
			int count = 0;
			vector<vector<int>> adjEs;
			for (int i = 0; i<8; i++) {
				int ll = l + shiftl[i], cc = c + shiftc[i];
				if (ll<0 || ll >= lin || cc<0 || cc >= col || queued[ll][cc])//avoid duplicate
					continue;
				if (board[ll][cc] == 'M')
					count++;
				else if (board[ll][cc] == 'E')
					adjEs.push_back({ ll,cc });
			}
			if (count>0)
				board[l][c] = '0' + count;
			else {
				board[l][c] = 'B';
				for (auto adj : adjEs) {
					current.push(adj);
					queued[adj[0]][adj[1]] = true;
				}
			}
		}
	}
	return board;
}


////////////////////////////////////////////////////////////////////////////
//515. Find Largest Value in Each Tree Row

class Solution515_0 {
private:
	void MLR(vector<int>& ret, TreeNode* node, int depth) {
		if (!node)
			return;
		if (depth == ret.size())//the trick of same depth and size
			ret.push_back(node->val);
		else
			ret[depth] = max(ret[depth], node->val);
		MLR(ret, node->left, depth + 1);
		MLR(ret, node->right, depth + 1);
	}
public:
	vector<int> largestValues(TreeNode* root) {
		vector<int> ret;
		MLR(ret, root, 0);
		return ret;
	}
};

//n preorder solution the vector have been constantly resized, and each time add 1 because we don't know how deep the tree is. 
//If change to post order, we can resize the vector only at leaf node, this should improve the performance.
class Solution515_1 {
public:
	vector<int> largestValues(TreeNode* root) {
		vector<int> res;
		dfs(root, 1, res);
		return res;
	}

private:
	void dfs(TreeNode* node, int depth, vector<int>& res) {
		if (!node) {
			return;
		}
		dfs(node->left, depth + 1, res);
		dfs(node->right, depth + 1, res);
		if (depth > res.size()) {
			res.resize(depth, INT_MIN);
		}
		res[depth - 1] = max(res[depth - 1], node->val);
	}
};

////////////////////////////////////////////////////////////////////////////
//133. Clone Graph
//Definition for undirected graph.
struct UndirectedGraphNode {
	int label;
	vector<UndirectedGraphNode *> neighbors;
	UndirectedGraphNode(int x) : label(x) {};
};
//remember to check visited nodes in BFS
class Solution133_0 {
public:
	UndirectedGraphNode *cloneGraph(UndirectedGraphNode *node) {
		if (!node)
			return node;
		queue<UndirectedGraphNode *> current;//queue for BFS
		current.push(node);
		unordered_set<int> queued;//all node already in queue, helping BFS
		queued.insert(node->label);
		unordered_map<int, UndirectedGraphNode *> replica;  //cloned node      
		unordered_map<int, vector<int>> connects; //relation of nodes
		while (current.size()) {
			UndirectedGraphNode * org = current.front();
			current.pop();
			UndirectedGraphNode * rep = new UndirectedGraphNode(org->label);
			replica.emplace(rep->label, rep);
			for (auto& adj : org->neighbors) {
				connects[org->label].push_back(adj->label);
				if (queued.count(adj->label) == 0) {//avoid put duplicate node into curent queue
					queued.insert(adj->label);
					current.push(adj);
				}
			}
		}
		for (auto &con : connects) {
			int pa = con.first;
			vector<int> kids = con.second;
			for (auto& kid : kids)
				replica[pa]->neighbors.push_back(replica[kid]);
		}
		return replica[node->label];
	}
};

//better BFS
class Solution133_1 {
public:
	UndirectedGraphNode *cloneGraph(UndirectedGraphNode *node) {
		if (!node) return NULL;
		UndirectedGraphNode* copy = new UndirectedGraphNode(node->label);
		mp[node] = copy;
		queue<UndirectedGraphNode*> toVisit;
		toVisit.push(node);
		while (!toVisit.empty()) {
			UndirectedGraphNode* cur = toVisit.front();
			toVisit.pop();
			vector<UndirectedGraphNode *> curneighbors = cur->neighbors;
			for (UndirectedGraphNode* neigh : curneighbors) {
				if (mp.find(neigh) == mp.end()) {
					UndirectedGraphNode* neigh_copy = new UndirectedGraphNode(neigh->label);
					mp[neigh] = neigh_copy;
					toVisit.push(neigh);
				}
				mp[cur]->neighbors.push_back(mp[neigh]);
			}
		}
		return copy;
	}
private:
	unordered_map<UndirectedGraphNode*, UndirectedGraphNode*> mp;
};

//better DFS
class Solution133_2 {
public:
	UndirectedGraphNode *cloneGraph(UndirectedGraphNode *node) {
		if (!node) return NULL;
		if (mp.find(node) == mp.end()) {
			mp[node] = new UndirectedGraphNode(node->label);
			for (UndirectedGraphNode* neigh : node->neighbors)
				mp[node]->neighbors.push_back(cloneGraph(neigh));
		}
		return mp[node];
	}
private:
	unordered_map<UndirectedGraphNode*, UndirectedGraphNode*> mp;
};


//====================================================DFS medium=============================

////////////////////////////////////////////////////////////////////////////
//337. House Robber III
//DFS with DP
class Solution337_0 {
private:
	pair<int, int> traverse(TreeNode* node) {
		pair<int, int> cur(0, 0);
		if (!node)
			return cur;

		pair<int, int> lp = traverse(node->left);
		pair<int, int> rp = traverse(node->right);

		cur.first = node->val + lp.second + rp.second;//first is the max including current
		cur.second = max(lp.first, lp.second) + max(rp.first, rp.second);//second is the max not including current

		return cur;
	}
public:
	int rob(TreeNode* root) {
		pair<int, int> rootp = traverse(root);
		return max(rootp.first, rootp.second);
	}
};

////////////////////////////////////////////////////////////////////////////
//721. Accounts Merge
//DFS 
class Solution721 {
private:
	unordered_map<string, unordered_set<string>> graph;//recording all edges
	unordered_map<string, bool> visited; //recording visited node   
										 //dfs/bfs on unvisited node
	void dfs(string current, vector<string>& merged) {
		merged.push_back(current);
		visited[current] = true;
		for (auto& adj : graph[current]) {
			if (!visited[adj])
				dfs(adj, merged);
		}
	}
public:
	vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
		//build undirected graph using all pieces
		//using leading email as center, all rests as neighbors
		for (auto & account : accounts) {
			visited[account[1]] = false;
			for (int i = 2; i<account.size(); i++) {
				graph[account[i]].insert(account[1]);
				graph[account[1]].insert(account[i]);
				visited[account[i]] = false;
			}
		}
		vector<vector<string>> summarized;
		//walk through all pieces and do merge
		for (auto &account : accounts) {
			if (!visited[account[1]]) {
				//because dfs will mark all related as visited, 
				//whenever touch a unvisited piece means a new person
				vector<string> merged;
				dfs(account[1], merged);
				//little bit of formating
				sort(merged.begin(), merged.end());
				merged.insert(merged.begin(), account[0]);
				summarized.push_back(merged);
			}
		}
		return summarized;
	}
};

//union-find
class Solution721_1 {
public:
	vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
		//initial all individual
		for (auto & account : accounts) {
			for (int i = 1; i<account.size(); i++) {
				papa[account[i]] = account[i];
				owner[account[i]] = account[0];
			}
		}
		//unions every edge
		for (auto & account : accounts) {
			for (int i = 2; i<account.size(); i++) {
				unions(account[i - 1], account[i]);
			}
		}
		//sumamrize every group by representing email
		unordered_map<string, unordered_set<string>> groups;
		for (auto & account : accounts) {
			for (int i = 1; i<account.size(); i++) {
				groups[find(account[i])].insert(account[i]);
			}
		}
		//walk through all group
		vector<vector<string>> summarized;
		for (auto &group : groups) {
			vector<string> merged(group.second.begin(), group.second.end());
			sort(merged.begin(), merged.end());
			merged.insert(merged.begin(), owner[group.first]);
			summarized.push_back(merged);
		}
		return summarized;
	}
private:
	unordered_map<string, string> papa;
	unordered_map<string, string> owner;
	string find(string name) {
		while (name != papa[name]) {
			papa[name] = papa[papa[name]];
			name = papa[name];
		}
		return name;
	}
	void unions(string one, string another) {
		one = find(one);
		another = find(another);
		if (one == another)
			return;
		papa[one] = another;
	}
};


////////////////////////////////////////////////////////////////////////////
//106. Construct Binary Tree from Inorder and Postorder Traversal
/*
			13
		  /    \
		 2      3
		/ \    /
	   5   6  7
			 / \
			8   9
				 \
				 10
				/
			  12

5,  2,  6,  13,  8,  7,  9,  12,  10,  3
---left--- root  ---------right---------  //inorder tell the num of left and right subtree

5,  6,  2,  8,  12,  10,  9,  7,  3,  13
---left---  ---------right---------- root //posorder tell the position of root
*/
class Solution106_1 {
private:
	//because no duplicate node can use map
	unordered_map<int, int> posOfNum;//posOfNum[i] ==position of num i in inorder set

	TreeNode* dfs(vector<int>& inorder, int ileft, int iright, vector<int>& postorder, int pleft, int pright) {
		if (ileft > iright)
			return 0;
		int val = postorder[pright];
		TreeNode *root = new TreeNode(val);
		if (ileft == iright)
			return root;
		int posOfRoot = posOfNum[val];
		int lenOfLeft = posOfRoot - ileft;
		int lenOfRight = iright - posOfRoot;
		root->left = dfs(inorder, ileft, posOfRoot - 1, postorder, pleft, pleft + lenOfLeft - 1);
		root->right = dfs(inorder, posOfRoot + 1, iright, postorder, pleft + lenOfLeft, pright - 1);

		return root;
	}
public:
	TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
		int n = inorder.size();
		for (int i = 0; i<n; i++)
			posOfNum[inorder[i]] = i;
		return dfs(inorder, 0, n - 1, postorder, 0, n - 1);
	}
};

////////////////////////////////////////////////////////////////////////////
//98. Validate Binary Search Tree
class Solution98_0 {
private:
	bool DFS(TreeNode* root, long bottom, long roof) {
		//check current
		if (!root)
			return true;
		if (root->val <= bottom || root->val >= roof)
			return false;
		//check childs
		if (root->left&&!DFS(root->left, bottom, min(roof, (long)root->val)))
			return false;
		if (root->right&&!DFS(root->right, max((long)root->val, bottom), roof))
			return false;
		return true;
	}
public:
	bool isValidBST(TreeNode* root) {
		return DFS(root, (long)INT_MIN - 1, (long)INT_MAX + 1);//one extreme is root val==2147483647
	}
};

//avoid buggy INT_MAX
class Solution98_1 {
public:
	bool isValidBST(TreeNode* root) {
		return isValidBST(root, NULL, NULL);
	}

	bool isValidBST(TreeNode* root, TreeNode* minNode, TreeNode* maxNode) {
		if (!root)
			return true;
		if (minNode && root->val <= minNode->val || maxNode && root->val >= maxNode->val)
			return false;
		return isValidBST(root->left, minNode, root) && isValidBST(root->right, root, maxNode);
	}
};

//in order traversal using a stack
//inorder a BST must be ascending
bool isValidBST_2(TreeNode* root) {
	stack<TreeNode*> S;
	TreeNode* prev = NULL;
	while (root || S.size()) {
		//go all the way down, get left most
		while (root) {
			S.push(root);
			root = root->left;
		}
		root = S.top();
		S.pop();
		//check ascending at current
		if (prev&&prev->val >= root->val)
			return false;
		prev = root;
		//continue to right
		root = root->right;
	}
	return true;
}

int findBlackPixel(vector<vector<char>>& picture, int N) {
	int row, col;
	if ((row = picture.size()) == 0 || (col = picture[0].size()) == 0)
		return 0;
	unordered_map<int, unordered_set<int>> RBcount, CBcount;
	for (int r = 0; r<row; r++) {
		int n = 0;
		for (int c = 0; c<col; c++) {
			if (picture[r][c] == 'B')
				n++;
		}
		RBcount[n].insert(r);
	}
	for (int c = 0; c<col; c++) {
		int n = 0;
		for (int r = 0; r<row; r++) {
			if ((picture[r][c] == 'B'))
				n++;
		}
		CBcount[n].insert(c);
	}
	vector<pair<int, int>> candidate;
	int maxEqual = min(col, row);
	for (int i = 0; i <= maxEqual; i++) {
		if (RBcount[i].size() && CBcount[i].size()) {
			for (auto& r : RBcount[i]) {
				for (auto& c : CBcount[i]) {
					candidate.emplace_back(r, c);
				}
			}
		}
	}
	int ret = 0;
	for (auto& p : candidate) {
		int r = p.first, c = p.second;
		if (picture[r][c] == 'W')
			ret++;
		else {
			bool same = true;
			for (auto & row : picture) {
				if (row[c] == 'B'&&row != picture[r]) {
					same = false;
					break;
				}
			}
			if (same)
				ret++;
		}
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////
//547. Friend Circles
//DFS
class Solution547_0 {
public:
	int findCircleNum(vector<vector<int>>& M) {
		N = M.size();
		if (N<2)
			return N;
		vector<bool> visited(N, false);
		int circle = 0;
		for (int i = 0; i<N; i++) {
			if (!visited[i]) {
				circle++;
				dfs(M, i, visited);
			}
		}
		return circle;
	}
private:
	int N;
	//visit the frients of a unvisited people
	void dfs(vector<vector<int>>& M, int person, vector<bool>& visited) {
		visited[person] = true;
		for (int i = 0; i<N; i++) {
			if (M[person][i] == 1 && !visited[i])
				dfs(M, i, visited);
		}
	}
};

//union find
class Solution547_01 {
public:
	int findCircleNum(vector<vector<int>>& M) {
		N = M.size();
		if (N<2)
			return N;
		for (int i = 0; i<N; i++)
			pa.push_back(i);

		int circle = N;
		for (int i = 0; i<N; i++) {
			for (int j = i; j<N; j++) {
				if (M[i][j] == 1 && find(i) != find(j)) {
					circle--;
					unions(i, j);
				}
			}
		}
		return circle;
	}
private:
	int N;//num of people
	vector<int> pa;
	int find(int n) {
		while (n != pa[n]) {
			pa[n] = pa[pa[n]];
			n = pa[n];
		}
		return n;
	}
	void unions(int a, int b) {
		a = find(a);
		b = find(b);
		if (a == b)
			return;
		pa[a] = b;
	}
};


//Definition for binary tree with next pointer.
 struct TreeLinkNode {
  int val;
  TreeLinkNode *left, *right, *next;
  TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
 };
 ////////////////////////////////////////////////////////////////////////////
 //116. Populating Next Right Pointers in Each Node
 //notice nieghbor tree connection
 class Solution_1 {
 public:
	 void connect(TreeLinkNode *root) {
		 if (!root || !root->left)  return;
		 help(root->left, root->right);
	 }

	 void help(TreeLinkNode* a, TreeLinkNode* b) {
		 a->next = b;
		 if (a->left) {
			 help(a->left, a->right);
			 help(a->right, b->left);
			 help(b->left, b->right);
		 }
	 }
 };

 //use the next pointer samrt
 void connect_2(TreeLinkNode *root) {
	 if (!root)
		 return;
	 while (root->left)
	 {
		 TreeLinkNode *p = root;
		 while (p)
		 {
			 p->left->next = p->right;
			 if (p->next)
				 p->right->next = p->next->left;
			 p = p->next;
		 }
		 root = root->left;
	 }
 }

 ////////////////////////////////////////////////////////////////////////////
 //494. Target Sum
 //DFS iteration with repeat calcul is slow, O(2^n)
 class Solution494_0 {
 public:
	 int findTargetSumWays(vector<int>& nums, int S) {
		 int n = nums.size();
		 return num(nums, 0, n, S);
	 }

	 int num(vector<int>& nums, int head, int& tail, int S) {
		 if (head == tail) 
			 return S == 0;		 
		 return num(nums, head + 1, tail, S - nums[head]) + num(nums, head + 1, tail, S + nums[head]);
	 }
 };

 //O(ns) DFS with Memoization
 class Solution494_1 {
 public:
	 int findTargetSumWays(vector<int>& nums, int S) {
		 vector<unordered_map<int, int>> mem(nums.size());
		 return find(0, nums, S, mem);
	 }
	 int find(int p, vector<int>& nums, int sum, vector<unordered_map<int, int>>& mem) {
		 if (p == nums.size()) return sum == 0;
		 auto it = mem[p].find(sum);
		 if (it != mem[p].end()) return it->second;
		 return mem[p][sum] = find(p + 1, nums, sum + nums[p], mem) + find(p + 1, nums, sum - nums[p], mem);
	 }
 };

 //O(ns) dp
 int findTargetSumWays_2(vector<int>& nums, int S) {
	 int n = nums.size();
	 vector<unordered_map<int, int>> dp(n + 1);
	 dp[0][0] = 1;
	 for (int i = 0; i<n; i++)
		 for (auto &p : dp[i]) {
			 dp[i + 1][p.first + nums[i]] += p.second;
			 dp[i + 1][p.first - nums[i]] += p.second;
		 }
	 return dp[n][S];
 }

// O(ns) time, linear space dp.
 //when need to swap a container, swap pointer is better
 int findTargetSumWays_3(vector<int>& nums, int S) {
	 unordered_map<int, int> cur({ { 0,1 } }), nxt, *p_cur = &cur, *p_nxt = &nxt;
	 for (int i = 0; i<nums.size(); i++) {
		 for (auto &p : *p_cur) {
			 (*p_nxt)[p.first + nums[i]] += p.second;
			 (*p_nxt)[p.first - nums[i]] += p.second;
		 }
		 swap(p_cur, p_nxt);
		 p_nxt->clear();
	 }
	 return (*p_cur)[S];
 }

 //adding a tast of math cut one way of DP
 /*
 let's see how this can be converted to a subset sum problem:
 sum(P) - sum(N) = target
 sum(P) + sum(N) + sum(P) - sum(N) = target + sum(P) + sum(N)
 2 * sum(P) = target + sum(nums)
 So the original problem has been converted to a subset sum problem as follows:
 Find a subset P of nums such that sum(P) = (target + sum(nums)) / 2

 Note that the above formula has proved that target + sum(nums) must be even
 We can use that fact to quickly identify inputs that do not have a solution
 */
 class Solution494_4 {
 public:
	 int findTargetSumWays(vector<int>& nums, int s) {
		 int sum = 0;
		 for (auto &n : nums)
			 sum += n;
		 //never make it
		 if (sum<s)
			 return 0;
		 //must be even, (s + sum) & 1 --> (s+sum)%2
		 if ((s + sum) & 1)
			 return 0;
		 //(s + sum) >> 1  -->   (s + sum)/2
		 return subsetSum(nums, (s + sum) >> 1);
	 }

	 /*int subsetSum(vector<int>& nums, int s) {
	 vector<int> dp0(s + 1, 0);//DP rollng array,
	 dp0[0] = 1;//means selecting 0 num, only one situation exist and its sum==0
	 vector<int> dp1(dp0);
	 //dp[i]-->number of combo whos sum==i if choosing number from 0 until num[n-1]
	 for (int n=0;n<nums.size();n++){//choosing number until nums[n],
	 for (int target = nums[n]; target <=s; target++){
	 dp1[target] += dp0[target-nums[n]];
	 }
	 dp0=dp1;
	 }
	 return dp0[s];
	 }
	 */

	 //more elegant
	 //because summery of pos num always go up, refresh from right can save dp0
	 int subsetSum(vector<int>& nums, int s) {
		 vector<int> dp(s + 1, 0);
		 dp[0] = 1;
		 for (int n : nums)
			 for (int i = s; i >= n; i--)
				 dp[i] += dp[i - n];
		 return dp[s];
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
 //114. Flatten Binary Tree to Linked List
 //naive DFS
 class Solution114_0 {
 private:
	 void dfs(TreeNode* root, vector<TreeNode*>& R) {
		 R.push_back(root);
		 if (root->left)
			 dfs(root->left, R);
		 if (root->right)
			 dfs(root->right, R);
	 }
 public:
	 void flatten(TreeNode* root) {
		 if (!root)
			 return;
		 vector<TreeNode*> R;
		 dfs(root, R);
		 for (int i = 0; i<R.size() - 1; i++) {
			 R[i]->right = R[i + 1];
			 R[i]->left = NULL;
		 }
	 }
 };

 //post order is elegant
 class Solution114_1 {
	 TreeNode* pre = NULL;
 public:
	 void flatten(TreeNode* root) {
		 if (!root)
			 return;
		 flatten(root->right);
		 flatten(root->left);
		 root->right = pre;
		 root->left = NULL;
		 pre = root;
	 }
 };

 //flap the tree
 void flatten_2(TreeNode *root) {
	 TreeNode*now = root;
	 while (now)
	 {
		 if (now->left)
		 {
			 //Find current node's prenode that links to current node's right subtree
			 TreeNode* pre = now->left;
			 while (pre->right)
			 {
				 pre = pre->right;
			 }
			 pre->right = now->right;
			 //Use current node's left subtree to replace its right subtree(original right 
			 //subtree is already linked by current node's prenode
			 now->right = now->left;
			 now->left = NULL;
		 }
		 now = now->right;
	 }
 }


 ////////////////////////////////////////////////////////////////////////////
 //117. Populating Next Right Pointers in Each Node II
 class Solution117_0 {
 private:
	 vector<TreeLinkNode *> prevs;
	 void dfs(TreeLinkNode *root, int level) {
		 if (!root)
			 return;
		 if (level == prevs.size())
			 prevs.push_back(root);
		 else {
			 prevs[level]->next = root;
			 prevs[level] = root;
		 }
		 dfs(root->left, level + 1);
		 dfs(root->right, level + 1);
	 }
 public:
	 void connect(TreeLinkNode *root) {
		 if (!root)
			 return;
		 dfs(root, 0);
	 }
 };

 //post order resize vector less, should be faster
 class Solution117_01 {
 private:
	 vector<TreeLinkNode *> prevs;
	 void dfs(TreeLinkNode *root, int level) {
		 if (!root)
			 return;
		 dfs(root->left, level + 1);
		 dfs(root->right, level + 1);
		 if (level + 1>prevs.size())
			 prevs.resize(level + 1, NULL);
		 if (prevs[level]) {
			 prevs[level]->next = root;
		 }
		 prevs[level] = root;
	 }
 public:
	 void connect(TreeLinkNode *root) {
		 if (!root)
			 return;
		 dfs(root, 0);
	 }
 };

 //BFS space O(1)
 void connectII_2(TreeLinkNode *root) {
	 TreeLinkNode *now, *lowHead, *lowTail;
	 now = root;
	 lowHead = lowTail = NULL;
	 while (now) {
		 if (now->left) {
			 if (lowTail) {
				 lowTail->next = now->left;
				 lowTail = now->left;
			 }
			 else {
				 lowHead = now->left;
				 lowTail = now->left;
			 }
		 }
		 if (now->right) {
			 if (lowTail) {
				 lowTail->next = now->right;
				 lowTail = now->right;
			 }
			 else {
				 lowHead = now->right;
				 lowTail = now->right;
			 }
		 }
		 now = now->next;
		 if (!now) {
			 now = lowHead;
			 lowHead = NULL;
			 lowTail = NULL;
		 }
	 }
 }
 
 ////////////////////////////////////////////////////////////////////////////
 //332. Reconstruct Itinerary
 //naive dfs, slow
 class Solution332_0 {
 private:
	 vector<pair<string, string>> valid;
	 unordered_map<string, multiset<string>> graph;
	 bool moreDfs(vector<string>& current) {
		 if (valid.size() == 0)
			 return false;
		 string prev = current.back();
		 for (auto & nxt : graph[prev]) {
			 pair<string, string> need = { prev,nxt };
			 auto it = find(valid.begin(), valid.end(), need);
			 if (it == valid.end())
				 continue;
			 valid.erase(it);
			 current.push_back(nxt);
			 if (!moreDfs(current))
				 return false;
			 valid.push_back(need);
			 current.pop_back();
		 }
		 return true;
	 }
 public:
	 vector<string> findItinerary(vector<pair<string, string>> tickets) {
		 valid = tickets;
		 for (auto& t : tickets)
			 graph[t.first].insert(t.second);
		 vector<string> cur = { "JFK" };
		 moreDfs(cur);
		 return cur;
	 }
 };

 //Eulerian path
 /*
 The nodes which have odd edges (in + out) are the entrance or exit. 
 If there are no nodes have odd degrees, we could follow any path without stuck until hit the last exit node
 The reason we got stuck is because that we hit the exit
 */
 class Solution332_1 {
 public:
	 vector<string> findItinerary(vector<pair<string, string>> tickets) {
		 for (auto ticket : tickets)
			 targets[ticket.first].insert(ticket.second);
		 visit("JFK");
		 return vector<string>(route.rbegin(), route.rend());
	 }
 private:
	 map<string, multiset<string>> targets;
	 vector<string> route;
	 void visit(string airport) {
		 while (targets[airport].size()) {
			 string next = *targets[airport].begin();
			 targets[airport].erase(targets[airport].begin());
			 visit(next);
		 }
		 route.push_back(airport);
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
 //113. Path Sum II
 //naive dfs
 class Solution113_0 {
 private:
	 void dfs(TreeNode* root, int target, vector<int> cur, vector<vector<int>>& ret) {
		 target -= root->val;
		 cur.push_back(root->val);
		 if (target == 0 && !root->left&&!root->right) {
			 ret.push_back(cur);
		 }
		 if (root->left) dfs(root->left, target, cur, ret);
		 if (root->right) dfs(root->right, target, cur, ret);
		 cur.pop_back();
	 }

 public:
	 vector<vector<int>> pathSum(TreeNode* root, int sum) {
		 vector<int> cur;
		 vector<vector<int>> ret;
		 if (root) dfs(root, sum, cur, ret);
		 return ret;
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
 //129. Sum Root to Leaf Numbers
 class Solution129_0 {
 private:
	 int deeper(TreeNode* root, int sum) {
		 if (!root)
			 return 0;
		 sum = 10 * sum + root->val;
		 if (!root->left&&!root->right)
			 return sum;
		 return deeper(root->left, sum) + deeper(root->right, sum);
	 }
 public:
	 int sumNumbers(TreeNode* root) {
		 return deeper(root, 0);
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
 //694. Number of Distinct Islands
 //need a proper way to represent the question
 //record the path of DFS using dir+level
 class Solution694_1 {
 public:
	 // we need the depth to differentiate between [[1,0],[1,1],[1,0]] (dddr) and [1,0],[1,0],[1,1] (dddr) 
	 void colorIsland(vector<vector<int>>& grid, int i, int j, int color, string &x, int d) {
		 if (!grid[i][j]) {
			 return;
		 }
		 x += to_string(d);
		 grid[i][j] = color;
		 if (i < grid.size() - 1 && grid[i + 1][j] == 1) {
			 x = x + "d";
			 colorIsland(grid, i + 1, j, color, x, d + 1);
		 }
		 if (i > 0 && grid[i - 1][j] == 1) {
			 x = x + "u";
			 colorIsland(grid, i - 1, j, color, x, d + 1);
		 }
		 if (j < grid[i].size() - 1 && grid[i][j + 1] == 1) {
			 x = x + "r";
			 colorIsland(grid, i, j + 1, color, x, d + 1);
		 }
		 if (j > 0 && grid[i][j - 1] == 1) {
			 x = x + "l";
			 colorIsland(grid, i, j - 1, color, x, d + 1);
		 }
	 }

	 int numDistinctIslands(vector<vector<int>>& grid) {
		 int count = 0;
		 unordered_set<string> shape;
		 for (int i = 0; i<grid.size(); i++) {
			 for (int j = 0; j<grid[i].size(); j++) {
				 if (grid[i][j] == 1) {
					 string x;
					 colorIsland(grid, i, j, 2, x, 0);
					 if (shape.find(x) == shape.end()) {
						 shape.insert(x);
						 count++;
					 }
				 }
			 }
		 }
		 return count;
	 }
 };

 //a brute force way to do record
 class Solution694_2 {
 public:
	 int numDistinctIslands(vector<vector<int>>& grid) {
		 int m = grid.size(), n = grid[0].size();
		 set<vector<vector<int>>> islands;
		 for (int i = 0; i < m; i++) {
			 for (int j = 0; j < n; j++) {
				 vector<vector<int>> island;
				 if (dfs(i, j, i, j, grid, m, n, island))
					 islands.insert(island);
			 }
		 }
		 return islands.size();
	 }

 private:
	 int delta[4][2] = { 0, 1, 1, 0, 0, -1, -1, 0 };

	 bool dfs(int i0, int j0, int i, int j, vector<vector<int>>& grid, int m, int n, vector<vector<int>>& island) {
		 if (i < 0 || m <= i || j < 0 || n <= j || grid[i][j] <= 0) return false;
		 island.push_back({ i - i0, j - j0 });
		 grid[i][j] *= -1;
		 for (int d = 0; d < 4; d++) {
			 dfs(i0, j0, i + delta[d][0], j + delta[d][1], grid, m, n, island);
		 }
		 return true;
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
 //364. Nested List Weight Sum II
 //DFS
/*
 class Solution364_0 {
 private:
	 void dfs(vector<NestedInteger>& nestedList, int level, vector<int>& NBL) {
		 if (level >= NBL.size())
			 NBL.push_back(0);
		 for (auto& N : nestedList) {
			 if (N.isInteger())
				 NBL[level] += N.getInteger();
			 else
				 dfs(N.getList(), level + 1, NBL);
		 }
	 }
 public:
	 int depthSumInverse(vector<NestedInteger>& nestedList) {
		 vector<int> NBL;//number by level
		 dfs(nestedList, 0, NBL);
		 int l = NBL.size();
		 int ret = 0;
		 for (int i = 0; i<l; i++)
			 ret += (l - i)*NBL[i];
		 return ret;
	 }
 };
*/

 //BFS 
 //Instead of multiplying by depth, add integers multiple times 
 //(by going level by level and adding the unweighted sum to the weighted sum after each level).
 /*
     int depthSumInverse(vector<NestedInteger>& nestedList) {
        int unweighted = 0, weighted = 0;
        while(!nestedList.empty()){
            vector<NestedInteger> tmp;
            for(auto& iter : nestedList){
                if(iter.isInteger())
                    unweighted += iter.getInteger();
                else{
                    vector<NestedInteger>& t = iter.getList();
                    for(int i = 0; i < t.size(); i++)
                        tmp.push_back(t[i]);
                }
            }
            weighted += unweighted;
            nestedList = tmp;
        }
        return weighted;
    }
 */


 ////////////////////////////////////////////////////////////////////////////
 //576. Out of Boundary Paths
 //BFS may use too much memory
 //remember visited node is necessary
 // O(m*n*N)
 class Solution576_1 {
 private:
	 int M;
	 vector<int> shiftL = { -1,1,0,0 }, shiftC = { 0,0,-1,1 };
	 int dfs(int& m, int& n, int N, int i, int j, vector<vector<vector<int>>>& known) {
		 if (i<0 || i >= m || j<0 || j >= n)
			 return 1;
		 if (N == 0)
			 return 0;
		 if (known[i][j][N] != -1)
			 return known[i][j][N];
		 known[i][j][N] = ((dfs(m, n, N - 1, i - 1, j, known) % M + dfs(m, n, N - 1, i + 1, j, known) % M) % M +
			 (dfs(m, n, N - 1, i, j - 1, known) % M + dfs(m, n, N - 1, i, j + 1, known) % M) % M) % M;
		 return known[i][j][N];
	 }
 public:
	 int findPaths(int m, int n, int N, int i, int j) {
		 M = pow(10, 9) + 7;
		 vector<vector<vector<int>>> known(m, vector<vector<int>>(n, vector<int>(N + 1, -1)));
		 return dfs(m, n, N, i, j, known);
	 }
 };

 /*
 The number of paths for N moves is the sum of paths for N - 1 moves from the adjacent cells.
 If an adjacent cell is out of the border, the number of paths is 1.
 */
 //REVERSE walk process, start from the border
 int findPaths_2(int m, int n, int N, int i, int j) {
	 int dp[51][50][50] = {};//dp[a][b][c] = num of ways to reach (b,c) from border in a steps
	 for (auto Ni = 1; Ni <= N; ++Ni)
		 for (auto mi = 0; mi < m; ++mi)
			 for (auto ni = 0; ni < n; ++ni)
				 dp[Ni][mi][ni] = (((mi == 0 ? 1 : dp[Ni - 1][mi - 1][ni]) + (mi == m - 1 ? 1 : dp[Ni - 1][mi + 1][ni])) % 1000000007
					 + ((ni == 0 ? 1 : dp[Ni - 1][mi][ni - 1]) + (ni == n - 1 ? 1 : dp[Ni - 1][mi][ni + 1])) % 1000000007) % 1000000007;
	 return dp[N][i][j];
 }

 //with rolling array
 int findPaths_21(int m, int n, int N, int i, int j) {
	 unsigned int g[2][50][50] = {};
	 while (N-- > 0)
		 for (auto k = 0; k < m; ++k)
			 for (auto l = 0, nc = (N + 1) % 2, np = N % 2; l < n; ++l)
				 g[nc][k][l] = ((k == 0 ? 1 : g[np][k - 1][l]) + (k == m - 1 ? 1 : g[np][k + 1][l])
					 + (l == 0 ? 1 : g[np][k][l - 1]) + (l == n - 1 ? 1 : g[np][k][l + 1])) % 1000000007;
	 return g[1][i][j];
 }

 ////////////////////////////////////////////////////////////////////////////
 //638. Shopping Offers
 //dfs with memo
 class Solution638_0 {
 private:
	 int min = INT_MAX, offerNum, itemNum;
	 set<vector<int>> calculated;//unordered_set will not hash vector<int>
	 void dfs(const vector<int>& price, const vector<vector<int>>& offers, vector<int> needs, vector<int> usage) {
		 if (calculated.find(usage) != calculated.end())
			 return;
		 bool offerUse = false;
		 for (int o = 0; o<offerNum; o++) {//walk through all offers
			 vector<int> needsB = needs;
			 bool usable = true;//check if current offer usable
			 for (int i = 0; i<itemNum; i++) {
				 needsB[i] -= offers[o][i];
				 if (needsB[i]<0) {
					 usable = false;
					 break;
				 }
			 }
			 if (usable) {//if usable, go deeper search
				 offerUse = true;
				 usage[o]++;
				 dfs(price, offers, needsB, usage);
				 usage[o]--;
			 }
		 }
		 if (!offerUse) {//if no more offer can be used, calculate the cost
			 int total = 0;
			 for (int i = 0; i<offerNum; i++)
				 total += usage[i] * offers[i][itemNum];
			 for (int n = 0; n<itemNum; n++)
				 total += price[n] * needs[n];
			 min = total<min ? total : min;
		 }
		 calculated.insert(usage);//memorize calculated bill
	 }
 public:
	 int shoppingOffers(vector<int>& price, vector<vector<int>>& special, vector<int>& needs) {
		 itemNum = price.size();
		 vector<vector<int>> real_offer;
		 for (auto& s : special) {
			 int exp = 0;
			 for (int i = 0; i<itemNum; i++)
				 exp += price[i] * s[i];
			 if (s[itemNum]<exp)
				 real_offer.push_back(s);
		 }
		 offerNum = real_offer.size();
		 vector<int> usage(offerNum, 0);//usage[i]= used num of i th offer 
		 dfs(price, real_offer, needs, usage);
		 return min;
	 }
 };

 //more elegant version
 int shoppingOffers_1(vector<int>& price, vector<vector<int>>& special, vector<int>& needs) {
	 int withOffer = INT_MAX;
	 for (auto &offer : special) {
		 // try to use this offer
		 vector<int> temp = needs;
		 bool isValidOffer = true;
		 for (int i = 0; i < temp.size(); ++i) {
			 if (temp[i] - offer[i] < 0) {//use if else is shorter
				 isValidOffer = false;
				 break;
			 }
			 else {
				 temp[i] -= offer[i];
			 }
		 }
		 if (!isValidOffer) { continue; }
		 withOffer = min(withOffer, shoppingOffers_1(price, special, temp) + offer.back());
	 }
	 int withoutOffer = 0;
	 for (int i = 0; i < needs.size(); ++i) {
		 withoutOffer += needs[i] * price[i];
	 }
	 return min(withOffer, withoutOffer);
 }


 ////////////////////////////////////////////////////////////////////////////
 //366. Find Leaves of Binary Tree
 //post order DFS
 class Solution366_0 {
 private:
	 vector<vector<int>> ret;
	 int dfs(TreeNode* root) {
		 int level = -1;
		 if (!root)
			 return level + 1;
		 level = max(level, dfs(root->left));
		 level = max(level, dfs(root->right));
		 if (level == ret.size())
			 ret.resize(level + 1, vector<int>());
		 ret[level].push_back(root->val);
		 return level + 1;
	 }
 public:
	 vector<vector<int>> findLeaves(TreeNode* root) {
		 dfs(root);
		 return ret;
	 }
 };


 ////////////////////////////////////////////////////////////////////////////
 //737. Sentence Similarity II
 //DFS
 class Solution737_0 {
 private:
	 bool dfs(unordered_map<string, unordered_set<string>>& graph, string from, string& to, unordered_set<string>& visited) {
		 if (from == to)
			 return true;
		 visited.insert(from);
		 for (auto& s : graph[from]) {
			 if (visited.find(s) == visited.end())
				 if (dfs(graph, s, to, visited)) {
					 graph[from].insert(to);
					 return true;
				 }
		 }
		 return false;
	 }
 public:
	 bool areSentencesSimilarTwo(vector<string>& words1, vector<string>& words2, vector<pair<string, string>> pairs) {
		 int len1 = words1.size(), len2 = words2.size();
		 if (len1 != len2)
			 return false;
		 unordered_map<string, unordered_set<string>> graph;
		 for (auto& p : pairs) {
			 graph[p.first].insert(p.second);
			 graph[p.second].insert(p.first);
		 }
		 for (int i = 0; i<len1; i++) {
			 unordered_set<string> visited;
			 if (!dfs(graph, words1[i], words2[i], visited))
				 return false;
		 }
		 return true;
	 }
 };
 //union-find
 class Solution_01 {
 private:
	 unordered_map<string, string> pa;
	 string find(string bby) {
		 while (bby != pa[bby]) {
			 pa[bby] = pa[pa[bby]];
			 bby = pa[bby];
		 }
		 return bby;
	 }
 public:
	 bool areSentencesSimilarTwo(vector<string>& words1, vector<string>& words2, vector<pair<string, string>> pairs) {
		 int len1 = words1.size(), len2 = words2.size();
		 if (len1 != len2)
			 return false;
		 for (int i = 0; i<len1; i++) {
			 pa[words1[i]] = words1[i];
			 pa[words2[i]] = words2[i];
		 }
		 for (auto& p : pairs) {
			 string pap1 = find(p.first), pap2 = find(p.second);
			 if (pap1 != pap2)
				 pa[pap1] = pap2;
		 }
		 for (int i = 0; i<len1; i++) {
			 if (find(words1[i]) != find(words2[i]))
				 return false;
		 }
		 return true;
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
 //439. Ternary Expression Parser
 //notice the extreme nesting
 class Solution439_0 {
 public:
	 string parseTernary(string expression) {
		 string cont, left, right;
		 cont = expression.substr(0, 1);
		 if (expression.find(":") == string::npos)
			 return cont;
		 if (expression[3] == ':') {
			 left = expression.substr(2, 1);
			 right = expression.substr(4);
		 }
		 else {
			 int count = 0;
			 for (int i = 3; i<expression.size(); i++) {
				 if (expression[i] == ':'&&count == 0) {
					 left = expression.substr(2, i - 2);
					 right = expression.substr(i + 1);
				 }
				 else {
					 if (expression[i] == '?')
						 count++;
					 if (expression[i] == ':')
						 count--;
				 }
			 }
		 }
		 if (cont == "T")
			 return parseTernary(left);
		 else
			 return parseTernary(right);
	 }
 };

 //more wise to do right to left
 class Solution439_1 {
 public:
	 string parseTernary(string expression) {
		 int n = expression.size();
		 stack<char> sk;
		 for (int i = n - 1; i >= 0; i--) {
			 if (expression[i] == '?') {
				 if (expression[i - 1] == 'F')
					 sk.pop();
				 else {
					 char first = sk.top();
					 sk.pop();
					 sk.pop();
					 sk.push(first);
				 }
				 i--;
			 }
			 else if (expression[i] != ':') {
				 sk.push(expression[i]);
			 }
		 }
		 string ans(1, sk.top());
		 return ans;
	 }
 };


 ////////////////////////////////////////////////////////////////////////////
 //105. Construct Binary Tree from Preorder and Inorder Traversal
 class Solution105_0 {
 public:
	 TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
		 if (!preorder.size())
			 return NULL;
		 int root_val = preorder[0];
		 TreeNode* root = new TreeNode(root_val);
		 int root_pos;
		 for (int i = 0; i<inorder.size(); i++) {
			 if (inorder[i] == root_val) {
				 root_pos = i;
				 break;
			 }
		 }
		 vector<int> left_in(inorder.begin(), inorder.begin() + root_pos);
		 vector<int> right_in(inorder.begin() + root_pos + 1, inorder.end());
		 vector<int> left_pre(preorder.begin() + 1, preorder.begin() + 1 + left_in.size());
		 vector<int> right_pre(preorder.begin() + 1 + left_in.size(), preorder.end());
		 root->left = buildTree(left_pre, left_in);
		 root->right = buildTree(right_pre, right_in);
		 return root;
	 }
 };

 //the no copy version iiis faster
 class Solution105_1 {
 public:
	 /* from Preorder and Inorder Traversal */
	 TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
		 return helper(preorder, 0, preorder.size(), inorder, 0, inorder.size());
	 }
 private:
	 TreeNode* helper(vector<int>& preorder, int i, int j, vector<int>& inorder, int ii, int jj)
	 {
		 // tree        8 4 5 3 7 3
		 // preorder    8 [4 3 3 7] [5]
		 // inorder     [3 3 4 7] 8 [5]
		 if (i >= j || ii >= j)
			 return NULL;

		 int mid = preorder[i];
		 auto f = find(inorder.begin() + ii, inorder.begin() + jj, mid);

		 int dis = f - inorder.begin() - ii;

		 TreeNode* root = new TreeNode(mid);
		 root->left = helper(preorder, i + 1, i + 1 + dis, inorder, ii, ii + dis);
		 root->right = helper(preorder, i + 1 + dis, j, inorder, ii + dis + 1, jj);
		 return root;
	 }
 };

 ////////////////////////////////////////////////////////////////////////////
 //473. Matchsticks to Square
 //dfs may not always works like a tree/map
 class Solution473_1 {
 private:
	 bool dfs(vector<int>&edge, int& roof, int pos, vector<int>&nums) {
		 if (pos == nums.size()) {
			 if (edge[0] == edge[1] && edge[1] == edge[2] && edge[2] == edge[3])
				 return true;
			 return false;
		 }
		 for (int i = 0; i<4; i++) {
			 //skip unnecessary edges
			 if (edge[i] + nums[pos]>roof)
				 continue;
			 //skip checked situaion
			 int tem = i;
			 while (--tem >= 0)
				 if (edge[tem] == edge[i])
					 break;
			 if (tem != -1)//is same length edge been tried before
				 continue;//skip dfs on the same situation
						  //dfs
			 edge[i] += nums[pos];
			 if (dfs(edge, roof, pos + 1, nums))
				 return true;
			 edge[i] -= nums[pos];
		 }
		 return false;

	 }
 public:
	 bool makesquare(vector<int>& nums) {
		 int roof = 0;
		 if (nums.size()<4)
			 return false;
		 //sort(nums.begin(), nums.end(), greater<int>());
		 sort(nums.rbegin(), nums.rend());//big at first make roof breaking faster
		 for (auto &n : nums)
			 roof += n;
		 if (roof % 4 != 0)
			 return false;
		 roof /= 4;
		 vector<int> edge(4, 0);
		 return dfs(edge, roof, 0, nums);
	 }
 };

 //bit masking + DP solution with detailed comments
 //A bitmask is used as a representation of a subset. 
 //For example if nums = {1,1,2,2,2}, then a bitmask = 01100 represents the subset {1,2}.
 class Solution473_2 {
 public:
	 bool makesquare(vector<int>& nums) {
		 int n = nums.size();
		 long sum = accumulate(nums.begin(), nums.end(), 0l);//#include <numeric>
		 if (sum % 4)
			 return false;
		 long sideLen = sum / 4;
		 //goodMasks stores all subsets can meet the target length 
		 vector<int> goodMasks;
		 // validHalfSubsets[i] == true if the subset represented by bitmask i has sum == 2*sideLen, 
		 // AND the bitmask i can be further partitioned into two submasks has sum == sideLen
		 vector<bool> validHalfSubsets(1 << n, false);

		 // E.g., if n = 5, (1 << 5 - 1) = 11111 represents the whole set
		 int all = (1 << n) - 1;

		 for (int mask = 0; mask <= all; mask++) {// go through all possible subsets
			 long subsetSum = 0;
			 for (int i = 0; i < 15; i++) {// 15 because num limit of matchstick known 
				 if ((mask >> i) & 1)
					 subsetSum += nums[i];
			 }

			 if (subsetSum == sideLen) {// if this subset has target length
				 for (int goodMask : goodMasks) {
					 if ((goodMask & mask) == 0) {// if this mask and a usedMask are mutually exclusive
						 int validHalf = goodMask | mask;
						 validHalfSubsets[validHalf] = true;//they make a validHalfSubsets
						 if (validHalfSubsets[all ^ validHalf])//if another half is also known valid, finish
							 return true;
					 }
				 }
				 goodMasks.push_back(mask);
			 }
		 }
		 return false;
	 }
 };

void main() {
	vector<vector<int>> picture = { { 1,1,0 },{ 0,1,1 },{ 0,0,0 },{ 1,1,1 },{ 0,1,0 } };
	vector<int> nums = { 1, 1, 1, 1, 1 };
	//Solution S;
	vector<int> price = { 2,5 };
	vector<vector<int>> special = { { 3,0,5 },{ 1,2,10 } };
	vector<int> needs = { 3,2 };






}