/*
Union Find
-to determin groups
-to union groups 
-to determin circle (using connected function)
*/


#include <vector>
#include <numeric>//iota
using namespace std;

/////////////////////////////////////////////////////////////////
//323. Number of Connected Components in an Undirected Graph
/*
to the begining, each node is treated as a set
each time we add edges, check if the two points already belong to the same set, 
if so, we do nothing. 
Otherwise, I call union that effectively remove one set. 
the number of set lefted is the result
*/
//only use compression 
class Solution323_1 {
public:
	int countComponents(int n, vector<pair<int, int>>& edges) {
		vector<int> p(n);//p[a] is direct parent of a
		iota(begin(p), end(p), 0);//set each node as a map
		/* iota store increasing sequence from 0, same as
		for(int i=0;i<n;i++){
		p[i]=i;
		}
		*/
		auto find = [&](int n) {//find the final parent of n
			while (n != p[n]) {
				p[n] = p[p[n]];//do compression
				n = p[n];
			}
			return n;
		};
		/*same as
		function<int(int)> find = [&](int n){
		return n==p[n]?n:find(p[n]);
		};

		*/
		auto unions = [&](const int& a, const int& b) {//union a and b into one map
			int x = find(a);
			int y = find(b);
			p[x] = y;//assign a's final parent to b's 
		};
		for (auto& edge : edges) {
			int p1 = find(edge.first);
			int p2 = find(edge.second);
			if (p1 != p2) {//if this edge connect two stand alone map
				unions(edge.first, edge.second);//union these two map
				n--;//total map number need minus 1
			}
			/* faster than above if, save the time on calling find
				n-=p1!=p2;
				p[p1]=p2;
			*/
		}
		return n;
	}
};

//if not using lambda, just resize the private vector
//with compression and rank,time O(1) space O(n)
	class Solution323_2 {
	private:
		vector<int> p;//parent, origin
		vector<int> r;//rank, tree level
	public:
		//find the final parent/origin of n
		int find(int n) {
			while (n != p[n]) {
				p[n] = p[p[n]];//do compression
				n = p[n];
			}
			return n;
		};

		//union a and b into one tree
		void unions(const int& a, const int& b) {
			int x = find(a);
			int y = find(b);
			if (x == y)
				return;
			if (r[x]<r[y])//assign lower rank to highr rank tree
				p[x] = y;
			else {
				p[y] = x;
				if (r[x] == r[y])//add rank by one if rank equal
					x++;
			}

		};

		int countComponents(int n, vector<pair<int, int>>& edges) {
			p.resize(n);
			r.resize(n);

			for (int i = 0; i<n; i++) {
				p[i] = i;//set origin stand alone sets
				r[i] = 0;//set stand alone set rank to 0
			}

			for (auto& edge : edges) {
				int p1 = find(edge.first);
				int p2 = find(edge.second);
				if (p1 != p2) {//if this edge connect two stand alone map
					unions(edge.first, edge.second);//union these two map
					n--;//total map number need minus 1
				}
			}
			return n;
		}
	};


/////////////////////////////////////////////////////////////////
// 200. Number of Islands
//use union find
class Solution200_0 {
private:
	int l;//line dimention
	int c;//colum dimention
	vector<int> p;//union_find parent
public:
	int numIslands(vector<vector<char>>& grid) {
		if (grid.size() == 0 || grid[0].size() == 0)
			return 0;
		l = grid.size();
		c = grid[0].size();
		//transfer matrix to connection pairs
		int count = 0;//total land area
		vector<pair<int, int>> connects;//edge connect lands
		for (int x = 0; x<l; x++) {
			for (int y = 0; y<c; y++) {
				if (grid[x][y] == '0')
					continue;
				else
					count++;
				if (x<l - 1 && grid[x + 1][y] == '1')
					connects.emplace_back(xy2id(x, y), xy2id(x + 1, y));
				if (y<c - 1 && grid[x][y + 1] == '1')
					connects.emplace_back(xy2id(x, y), xy2id(x, y + 1));
			}
		}
		//initial union-find
		int n = l*c;
		p.resize(n);
		for (int i = 0; i<n; i++)
			p[i] = i;
		//minus unioned land
		for (const auto& connect : connects) {
			int x = find(connect.first);
			int y = find(connect.second);
			if (x != y) {
				p[x] = y;;
				count--;
			}
		}
		return count;
	}

	int find(int x) {
		while (x != p[x]) {
			p[x] = p[p[x]];
			x = p[x];
		}
		return x;
	}

	//not merged in numIslands to save time
	void unions(int a, int b) {
		if ((a = find(a)) == (b = find(b)))
			return;
		p[a] = b;
	}

	//transfer from (x,y) to id
	int xy2id(int x, int y) {
		return x*c + y;
	}

};

//use DFS (can transfer to BFS)
class Solution200_1 {
public:
	int numIslands(vector<vector<char>>& grid) {
		if (grid.empty()) return 0;
		int m = grid.size(), n = grid[0].size(), num = 0;
		vector<vector<bool>> visited(m, vector<bool>(n, false));
		//walk through everywhere
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {				
				if (grid[i][j] == '1' && !visited[i][j]) {
					num++;//a unmarked land means new island
					markIslands(grid, visited, i, j);//then mark everywhere on new island
				}
			}
		}
		return num;
	}
private:
	//mark everything connected to current land as known 
	void markIslands(vector<vector<char>>& grid, vector<vector<bool>>& visited, int r, int c) {
		visited[r][c] = true;
		int m = grid.size(), n = grid[0].size();
		if (r - 1 >= 0 && grid[r - 1][c] == '1' && !visited[r - 1][c])
			markIslands(grid, visited, r - 1, c);
		if (r + 1 < m && grid[r + 1][c] == '1' && !visited[r + 1][c])
			markIslands(grid, visited, r + 1, c);
		if (c - 1 >= 0 && grid[r][c - 1] == '1' && !visited[r][c - 1])
			markIslands(grid, visited, r, c - 1);
		if (c + 1 < n && grid[r][c + 1] == '1' && !visited[r][c + 1])
			markIslands(grid, visited, r, c + 1);
	}
};

/////////////////////////////////////////////////////////////////
//305. Number of Islands II
//union find with compression
class Solution305_0 {
private:
	int l;//line num
	int c;//colum num
	vector<int> pa;//parent
	int island;//current island num

	//turn 2d position as a id
	int xy2id(int x, int y) {
		return x*c + y;
	}
	//find the origin of current land
	int find(int p) {
		while (p != pa[p]) {
			pa[p] = pa[pa[p]];//compression
			p = pa[p];
		}
		return p;
	}
	//once unioned, do not add to island num
	void unions(int a, int b) {
		if ((a = find(a)) == (b = find(b)))
			return;
		pa[a] = b;
		island--;
	}
public:
	vector<int> numIslands2(int m, int n, vector<pair<int, int>>& positions) {
		l = m;
		c = n;
		island = 0;
		vector<int> ret;
		vector<vector<bool>> show(l, vector<bool>(c, false));//store land appeared
		pa.resize(l*c);
		iota(begin(pa), end(pa), 0);//init parent to self
		for (auto& pos : positions) {
			island++;//this land may add to island num
			int x = pos.first;
			int y = pos.second;
			show[x][y] = 1;//mark current positon as a land
						   //check if this land can union as neighbors
			if (x>0 && show[x - 1][y])
				unions(xy2id(x, y), xy2id(x - 1, y));
			if (y>0 && show[x][y - 1])
				unions(xy2id(x, y), xy2id(x, y - 1));
			if (x<l - 1 && show[x + 1][y])
				unions(xy2id(x, y), xy2id(x + 1, y));
			if (y<c - 1 && show[x][y + 1])
				unions(xy2id(x, y), xy2id(x, y + 1));
			ret.push_back(island);
		}
		return ret;
	}
};

class Solution305_1 {
	vector<int> numIslands2(int m, int n, vector<pair<int, int>>& positions) {
		vector<int> res;
		roots = vector<int>(m * n, -1);//to store land appearen & root parent
		vector<pair<int, int>> dirs = { { 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 } };//can make 4 way shift easier
		int island = 0;
		for (auto pos : positions) {
			int x = pos.first, y = pos.second, idx_p = x * n + y;
			roots[idx_p] = idx_p;
			++island;
			for (auto dir : dirs) {
				int row = x + dir.first, col = y + dir.second, idx_new = row * n + col;
				if (row >= 0 && row < m && col >= 0 && col < n && roots[idx_new] != -1) {
					int rootNew = findRoot(idx_new), rootPos = findRoot(idx_p);
					if (rootPos != rootNew) {//merge union function inside
						roots[rootPos] = rootNew;
						--island;
					}
				}
			}
			res.push_back(island);
		}
		return res;
	}

private:
	vector<int> roots;
	int findRoot(int idx) {
		while (idx != roots[idx]) {
			roots[idx] = roots[roots[idx]];
			idx = roots[idx];
		}
		return idx;
	}
};

/////////////////////////////////////////////////////////////////
//130. Surrounded Regions
//DFS
class Solution130_01 {
private:
	int l;//line
	int c;//colum
		  //shift and mark all conected 'O'
	vector<pair<int, int>> shift = { { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 } };
	void Smark(vector<vector<char>>& board, pair<int, int> free) {
		board[free.first][free.second] = 'Q';
		for (auto s : shift) {
			int x = free.first + s.first;
			int y = free.second + s.second;
			if (x>0 && x<l&&y>0 && y<c&&board[x][y] == 'O')
				Smark(board, { x,y });
		}
	}
public:
	void solve(vector<vector<char>>& board) {
		if (board.size()<3 || board[0].size()<3)
			return;
		l = board.size();
		c = board[0].size();
		//get all 'O' on the edge of matrix,extend & mark all neghbor free O
		for (int i = 0; i<l; i++) {
			for (int j = 0; j<c; j++) {
				if ((i == 0 || i == l - 1 || j == 0 || j == c - 1) && (board[i][j] == 'O'))
					Smark(board, { i,j });
			}
		}
		//clean
		for (int i = 0; i<l; i++) {
			for (int j = 0; j<c; j++) {
				if (board[i][j] == 'Q')
					board[i][j] = 'O';
				else
					board[i][j] = 'X';
			}
		}
		return;
	}
};

//union find
//connect all border nod to a extra one, grouping all free maps
class Solution130_02 {
private:
	int line;
	int colum;
	vector<int> pa;
	vector<int> ran;
	int find(int i) {
		while (i != pa[i]) {
			pa[i] = pa[pa[i]];
			i = pa[i];
		}
		return i;
	}
	void unions(int i, int j) {
		int x = find(i);
		int y = find(j);
		if (x == y)
			return;
		pa[y] = x;
	}
	bool connected(int i, int j) {
		return find(i) == find(j);
	}
	int xy2id(int x, int y) {
		return x*colum + y;
	}

public:
	void solve(vector<vector<char> > &board) {
		if ((line = board.size())<3 || (colum = board[0].size())<3)
			return;
		pa.resize(line*colum + 1);//extra node connect all border 'O'
		int extraID = line*colum;
		for (int i = 0; i <= extraID; i++)
			pa[i] = i;
		//shift and connect
		vector<pair<int, int>> shift = { { -1,0 },{ 1,0 },{ 0,1 },{ 0,-1 } };
		for (int x = 0; x<line; x++) {
			for (int y = 0; y<colum; y++) {
				if (board[x][y] == 'O') {
					if (x == 0 || x == line - 1 || y == 0 || y == colum - 1)
						unions(xy2id(x, y), extraID);
					int xx, yy;
					for (auto p : shift) {
						xx = x + p.first;
						yy = y + p.second;
						if ((xx >= 0 && xx<line) && (yy >= 0 && yy<colum) && board[xx][yy] == 'O')
							unions(xy2id(xx, yy), xy2id(x, y));
					}
				}
			}
		}
		//check and clean
		for (int x = 0; x<line; x++) {
			for (int y = 0; y<colum; y++) {
				if (board[x][y] == 'O'&&!connected((xy2id(x, y)), extraID)) {
					board[x][y] = 'X';
				}
			}
		}
	}
};


/////////////////////////////////////////////////////////////////
//261. Graph Valid Tree
//determin circle using union-find
class Solution261_0 {
private:
	vector<int> pa;
	int group;
	int find(int a) {
		while (a != pa[a]) {
			pa[a] = pa[pa[a]];
			a = pa[a];
		}
		return a;
	}
	void unions(int a, int b) {
		if ((a = pa[a]) != (b = pa[b]))
			pa[a] = b;
		group--;
	}
	bool connected(int a, int b) {
		return find(a) == find(b);
	}
	void initUF(int x) {
		pa.resize(x);
		group = x;
		for (int i = 0; i<x; i++)
			pa[i] = i;
	}
public:
	bool validTree(int n, vector<pair<int, int>>& edges) {
		initUF(n);
		for (auto p : edges) {
			if (connected(p.first, p.second))//if a cicle exist, must be a usless edge connects unioned nods
				return false;
			unions(p.first, p.second);
		}
		if (group == 1)
			return true;
		return false;
	}
};

//determin circle using DFS
//if there is n-1 edges total, and we can go through every node using bfs starting from one node, 
//it means that the graph is connected and cannot have any cycle.
class Solution261_1 {
private:

	void DFS(vector<vector<int>>& neighbors, vector<bool>& visited, int nod) {
		if (visited[nod])
			return;
		visited[nod] = true;
		for (auto i : neighbors[nod])
			DFS(neighbors, visited, i);
	}
public:
	bool validTree(int n, vector<pair<int, int>>& edges) {
		if (n != edges.size() + 1)//a tree with n node must have n-1 edges(requires no duplicate edges exist)
			return false;
		vector<vector<int>> neighbors(n);//neighbours of all nodes
		for (auto p : edges) {
			neighbors[p.first].push_back(p.second);
			neighbors[p.second].push_back(p.first);
		}
		vector<bool> visited(n, false);//already visited nodes
		DFS(neighbors, visited, 0);//walk through all edges(should be able to visit all nodes
		bool tree = true;
		for (bool v : visited) {
			tree &= v;
		}
		return tree;
	}
};

void main() {
	Solution130_01 mys;
	vector<vector<char>> grid;
	vector<char> c(4,'X');
	grid.push_back(c);
	c[1] = 'O';
	c[2] = 'O';
	grid.push_back(c);
	c[1] = 'X';
	grid.push_back(c);
	c[1] = 'O';
	c[2] = 'X';
	grid.push_back(c);
	mys.solve(grid);

}