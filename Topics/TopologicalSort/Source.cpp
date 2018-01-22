#include<vector>
#include<queue>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include<map>



using namespace std;



///////////////////////////////////////////////////////////////////////////////////////////////////////
//444. Sequence Reconstruction
class Solution_444_0 {
public:
	bool sequenceReconstruction(vector<int>& org, vector<vector<int>>& seqs) {
		int n = org.size();
		vector<set<int>> tree(n + 1, set<int>());
		int pathn = 0;
		//modify to parend - child path graph
		for (auto& seq : seqs) {
			for (int i = 0; i<seq.size(); i++) {
				if (seq[i]>n || seq[i]<1)//validating node value
					return false;
				if (i + 1<seq.size())
					tree[seq[i]].insert(seq[i + 1]);
				pathn++;
			}
		}
		//if no path exist
		if (pathn == 0) {
			if (seqs.size() == 0)//can be empty seqs
				return false;
			for (auto& seq : seqs) {//can be single node seqs
				if (seq != org)
					return false;
			}
			return true;
		}
		//check and erase target path in graph
		for (int i = 0; i<n - 1; i++) {
			int fa = org[i], ch = org[i + 1];
			auto it = tree[fa].find(ch);
			if (it == tree[fa].end())
				return false;
			tree[fa].erase(it);
		}
		//check the redundant path
		for (int fa = 1; fa <= n; fa++) {
			auto it_ch = tree[fa].begin();
			for (; it_ch != tree[fa].end(); it_ch++) {
				if (!check(org, fa, *it_ch))
					return false;
			}
		}
		return true;

	}
private:
	//return true if the father-child relationship exist in org
	bool check(vector<int>& org, int fa, int ch) {
		int it1 = 0, it2 = 0;
		for (int i = 0; i<org.size(); i++) {
			if (org[i] == fa)
				it1 = i;
			if (org[i] == ch)
				it2 = i;
		}
		if (it2 - it1>0)
			return true;
		return false;
	}
};

//little modify of topological sort
/*
L ¡û Empty list that will contain the sorted elements
S ¡û Set of all nodes with no incoming edge
while S is non-empty do
remove a node n from S
add n to tail of L
for each node m with an edge e from n to m do
remove edge e from the graph
if m has no other incoming edges then
insert m into S
if graph has edges then
return error (graph has at least one cycle)
else
return L (a topologically sorted order)
*/
class Solution_444_1 {
public:
	bool sequenceReconstruction(vector<int>& org, vector<vector<int>>& seqs) {
		/* BFS topological sort */
		int n = org.size();
		vector<unordered_set<int>> edges(n + 1, unordered_set<int>());
		vector<int> degrees(n + 1, 0);//count how may parent does one node has

		//transfer to a list of all node's child edges
		bool empty = true; /* to handle test case [1] [[ ], [ ]] */
		for (vector<int> seq : seqs) {
			if (seq.empty())
				continue;
			empty = false;
			if (seq[0] < 1 || seq[0] > n)
				return false;
			for (int i = 1; i < seq.size(); i++) {
				if (seq[i] < 1 || seq[i] > n)
					return false;
				if (edges[seq[i - 1]].count(seq[i]) == 0) {
					edges[seq[i - 1]].insert(seq[i]);
					degrees[seq[i]]++;
				}
			}
		}

		if (empty)
			return false;

		queue<int> myq;//queue of nodes with no parent edges
		for (int i = 1; i <= n; i++)
			if (degrees[i] == 0)
				myq.push(i);

		// In order to get a unique sequence, myq.size is always 1, also the order is the same as org
		int idx = 0;
		while (!myq.empty()) {
			if (myq.size() > 1)
				return false;
			int k = myq.front();
			myq.pop();
			if (k != org[idx++]) //matching the origin in order
				return false;
			for (int i : edges[k]) {
				if (--degrees[i] == 0)
					myq.push(i);
			}
		}
		return idx == n;
	}
};

/*
For org to be uniquely reconstructible from seqs we need to satisfy 2 conditions:

1:Every sequence in seqs should be a subsequence in org. This part is obvious.
2:Every 2 consecutive elements in org should be consecutive elements in some sequence from seqs.

Why is that? Well, suppose condition 1 is satisfied. Then for 2 any consecutive elements x and y in org we have 2 options.
2.1 We have both x and y in some sequence from seqs.
Then (as condition 1 is satisfied) they must be consequtive elements in this sequence.
2.2 There is no sequence in seqs that contains both x and y.
In this case we cannot uniquely reconstruct org from seqs as sequence with x and y switched would also be a valid original sequence for seqs.
*/
class Solution_444_2 {
public:
	bool sequenceReconstruction(vector<int>& org, vector<vector<int>>& seqs) {
		int n = org.size();
		vector<int> pos(n + 1, 0);//pos[i]==x, num i is in x th position of org
		vector<int> pair(n + 1, 0);;//pair[i]==-1, the number direct following num i in org have been found in seqs
		for (int i = 1; i < n; i++)
			pos[org[i]] = i;
		for (vector<int>& seq : seqs) {
			if (seq.empty())
				continue;
			if (seq[0] < 1 || seq[0] > n)
				return false;
			for (int i = 1; i < seq.size(); i++) {
				if (seq[i] < 1 || seq[i] > n)
					return false;
				if (pos[seq[i]] <= pos[seq[i - 1]]) //check cond 1
					return false;
				if (pos[seq[i]] == pos[seq[i - 1]] + 1) //record cond 2
					pair[seq[i - 1]] = -1;
			}
			if (seq.back() == org[n - 1]) //record cond 2 for last node 
				pair[org[n - 1]] = -1;
		}
		for (int i = 1; i <= n; i++) {//check last cond 2
			if (pair[i] != -1)
				return false;
		}
		return true;
	}
};



///////////////////////////////////////////////////////////////////////////////////////////////////////
//210. Course Schedule II
//real topological sort
vector<int> findOrder(int numCourses, vector<pair<int, int>>& prerequisites) {
	vector<int> order;

	vector<unordered_set<int>> childs(numCourses, unordered_set<int>());
	unordered_map<int, int> parents;
	for (auto& pre : prerequisites) {
		parents[pre.first]++;
		childs[pre.second].insert(pre.first);
	}

	queue<int> independents;
	for (int i = 0; i<numCourses; i++) {
		if (parents[i] == 0)
			independents.push(i);
	}

	while (independents.size()) {
		int pa = independents.front();
		independents.pop();
		order.push_back(pa);
		for (auto& ch : childs[pa]) {
			if (--parents[ch] == 0)
				independents.push(ch);
		}
	}

	for (auto&p : parents) {
		if (p.second != 0) {
			vector<int> emptyOrder;
			return emptyOrder;
		}
	}
	return order;
}

//BFS 
class Solution210_1 {
public:
	vector<int> findOrder(int numCourses, vector<pair<int, int>>& prerequisites) {
		vector<unordered_set<int>> graph = make_graph(numCourses, prerequisites);
		vector<int> degrees = compute_indegree(graph);
		queue<int> zeros;
		for (int i = 0; i < numCourses; i++)
			if (!degrees[i]) zeros.push(i);
		vector<int> toposort;
		for (int i = 0; i < numCourses; i++) {//count number
			if (zeros.empty()) return{};//when empty before reach number
			int zero = zeros.front();
			zeros.pop();
			toposort.push_back(zero);
			for (int neigh : graph[zero]) {
				if (!--degrees[neigh])
					zeros.push(neigh);
			}
		}
		return toposort;
	}
private:
	vector<unordered_set<int>> make_graph(int numCourses, vector<pair<int, int>>& prerequisites) {
		vector<unordered_set<int>> graph(numCourses);
		for (auto pre : prerequisites)
			graph[pre.second].insert(pre.first);
		return graph;
	}
	vector<int> compute_indegree(vector<unordered_set<int>>& graph) {
		vector<int> degrees(graph.size(), 0);
		for (auto neighbors : graph)
			for (int neigh : neighbors)
				degrees[neigh]++;
		return degrees;
	}
};

//DFS
/*
Another way to think about it is the last few in the order must be those which are not prerequisites of other courses.
Thinking it recursively means if one node has unvisited child node,
you should visit them first before you put this node down in the final order array.
*/
class Solution210_2 {
public:
	vector<int> findOrder(int numCourses, vector<pair<int, int>>& prerequisites) {
		vector< vector<int> > graph(numCourses, vector<int>());
		for (auto item : prerequisites) {
			graph[item.second].push_back(item.first);
		}
		vector<int> res;
		vector<bool> visited(numCourses, false);//if one node has unvisited child node
		vector<bool> on_path(numCourses, false);//if one node is already on a recur call of dfs
		for (int cid = 0; cid<numCourses; ++cid) {
			if (!visited[cid] && !dfs(res, cid, graph, visited, on_path)) {
				// Not visited and can't make dfs without infinite loop
				return{};
			}
		}
		std::reverse(res.begin(), res.end());
		return res;
	}
private:
	// Return true if dfs succeed(no cycle).
	bool dfs(vector<int> & res,
		int cid, //course id
		vector< vector<int> > & graph,
		vector<bool> & visited,
		vector<bool> & on_path) {
		if (on_path[cid]) // On the same dfs path
			return false;
		if (visited[cid]) // Visited, no need to go further
			return true;
		visited[cid] = on_path[cid] = true;
		for (int num : graph[cid]) {
			if (on_path[num] || !dfs(res, num, graph, visited, on_path)) {
				// If the node has been on the recursive path,
				// or call dfs on it return false, we can return false directly
				// since a cycle has been detected.
				return false;
			}
		}
		res.push_back(cid); //push current course if all it's children have been added
		on_path[cid] = false; //rewind on_path
		return true;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
//207. Course Schedule
//BFS
bool canFinish_0(int numCourses, vector<pair<int, int>>& prerequisites) {
	vector<unordered_set<int>> edges(numCourses);
	vector<int> prereq(numCourses, 0);
	for (auto & p : prerequisites) {
		edges[p.second].insert(p.first);
		prereq[p.first]++;
	}
	queue<int> ready;
	for (int i = 0; i<numCourses; i++) {
		if (prereq[i] == 0)
			ready.push(i);
	}
	while (ready.size()) {
		int c = ready.front();
		ready.pop();
		for (auto& e : edges[c]) {
			if (--prereq[e] == 0)
				ready.push(e);
		}
	}
	for (auto& p : prereq) {
		if (p>0)
			return false;
	}
	return true;
}

//DFS
class Solution207_0 {
private:
	bool DFS(vector<unordered_set<int>>& edges, int course, vector<bool>& visited, vector<bool>& onpath) {
		if (onpath[course])//must goes first
			return false;
		if (visited[course])
			return true;
		visited[course] = onpath[course] = true;
		for (auto& e : edges[course])
			if (!DFS(edges, e, visited, onpath))
				return false;
		onpath[course] = false;
		return true;
	}
public:
	bool canFinish(int numCourses, vector<pair<int, int>>& prerequisites) {
		vector<unordered_set<int>> edges(numCourses);
		for (auto & p : prerequisites)
			edges[p.second].insert(p.first);

		vector<bool> visited(numCourses, false);
		vector<bool> onpath(numCourses, false);
		for (int i = 0; i<numCourses; i++) {
			if (!DFS(edges, i, visited, onpath))
				return false;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
//269. Alien Dictionary
//BFS alarm for no duplicate edges
string alienOrder(vector<string>& words) {
	if (words.size()<1)//may empty
		return "";

	string ret;
	string word1;
	string word2;
	unordered_map<char, unordered_set<char>> map;
	unordered_map<char, unordered_set<char>> pa;//may have duplicate parent appears
	for (char c : words[0]) {
		map[c];
		pa[c];
	}
	for (int i = 0; i<words.size() - 1; i++) {
		word1 = words[i];
		word2 = words[i + 1];
		for (char c : word2) {
			map[c];
			pa[c];
		}
		int len = min(word1.size(), word2.size());
		for (int i = 0; i<len; i++) {
			if (word1[i] != word2[i]) {
				map[word1[i]].insert(word2[i]);
				pa[word2[i]].insert(word1[i]);
				break;
			}
		}
	}

	if (pa.size() == 1)//may have only one char
		return ret += pa.begin()->first;

	queue<char> grad;
	for (auto it = pa.begin(); it != pa.end(); it++)
		if (it->second.size() == 0)
			grad.push(it->first);

	while (grad.size()) {
		char g = grad.front();
		ret += g;
		grad.pop();
		for (auto &m : map[g]) {
			pa[m].erase(g);
			if (pa[m].size() == 0)
				grad.push(m);
		}
	}

	for (auto it = pa.begin(); it != pa.end(); it++)
		if (it->second.size()>0)
			return "";

	return ret;
}
//BFS add a set to mark all chars
string alienOrder(vector<string>& words) {
	map<char, set<char>> suc, pre;
	set<char> chars;
	string s;
	for (string t : words) {
		chars.insert(t.begin(), t.end());
		for (int i = 0; i<min(s.size(), t.size()); ++i) {
			char a = s[i], b = t[i];
			if (a != b) {
				suc[a].insert(b);
				pre[b].insert(a);
				break;
			}
		}
		s = t;
	}
	set<char> free = chars;
	for (auto p : pre)
		free.erase(p.first);
	string order;
	while (free.size()) {
		char a = *begin(free);
		free.erase(a);
		order += a;
		for (char b : suc[a]) {
			pre[b].erase(a);
			if (pre[b].empty())
				free.insert(b);
		}
	}
	return order.size() == chars.size() ? order : "";
}

void main(){
	vector<string> m = { "za","zb","ca","cb" };

	alienOrder(m);
}