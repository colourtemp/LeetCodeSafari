//Trie tree
#include <vector>
#include <set>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//212. Word Search II
class node {
public:
	int count;//if is a word
	node* child[26];
	node() {
		count = 0;
		for (int i = 0; i < 26; i++)
			child[i] = NULL;
	}
};

class trie {
public:
	trie() {
		root = new node;
	}
	~trie() {
		freeTree(root);
	}
	void insert(string word) {
		int n = word.size();
		node* p = root;
		if (n == 0 || !root)
			return;
		for (int i = 0; i < n; i++) {
			int x = word[i] - 'a';
			if (p->child[x]==NULL) //add when not exist
				p->child[x] = new node;
			p = p->child[x];			
		}
		p->count = 1;
	}
	bool search(string word) {
		int n = word.size();
		node* p = root;
		if (n == 0 || !root)
			return false;
		for (int i = 0; i < n; i++) {
			int x = word[i] - 'a';
			if (p->child[x]) {//exist go
				p = p->child[x];
			}
			else
				return false;
		}
		if (p->count)
			return true;
		return false;
	}
	bool prefix(string word) {
		int n = word.size();
		node* p = root;
		if (n == 0 || !root)
			return false;
		for (int i = 0; i < n; i++) {
			int x = word[i] - 'a';
			if (p->child[x]) {//exist go
				p = p->child[x];
			}
			else
				return false;
		}
		return true;
	}
private:
	node* root;
	void freeTree(node* node) {
		for (int i = 0; i<26; i++) {
			if (node->child[i] != NULL) 
				freeTree(node->child[i]);			
		}
		delete node;
	}
};

class Solution212_0 {
private:
	trie* myTrie;
	int colum;
	int line;
	vector<string> found;

	//walk starting from cur,put everything in found
	vector<pair<int, int>> shifts = { { -1,0 },{ 1,0 },{ 0,1 },{ 0,-1 } };
	void DFS(vector<vector<char>>& board, vector<bool> used, pair<int, int> cur, string word) {
		if ((cur.first<0 || cur.first>line - 1) || (cur.second<0 || cur.second>colum - 1))
			return;
		if (used[cur.first*colum + cur.second])
			return;
		word = word + board[cur.first][cur.second];
		if (!myTrie->prefix(word))
			return;
		used[cur.first*colum + cur.second] = true;//mark after make sure this nod will be used
		if (myTrie->search(word))
			found.push_back(word);
		for (auto s : shifts)
			DFS(board, used, { cur.first + s.first,cur.second + s.second }, word);
		return;
	}
public:
	vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
		vector<string> ret;
		set<string> set;
		if (board.size() == 0 || board[0].size() == 0)
			return ret;
		line = board.size();
		colum = board[0].size();
		//build trie dictionary
		myTrie = new trie;
		for (auto word : words) 
			myTrie->insert(word);
		//for each starting point
		for (int i = 0; i < line; i++) {
			for (int j = 0; j < colum; j++) {
				vector<bool> used(line*colum, false);
				found.clear();//clear found
				DFS(board, used, { i,j }, "");//put all possible in found
											  //store found, avoid duplicate
				for (auto f : found)
					set.insert(f);
			}
		}
		//transfer to vector
		for (auto s : set)
			ret.push_back(s);
		delete myTrie;
		return ret;
	}
};
///////////////////////////////////////////////////////////////////
//do lot of things smarter
const int MAX_CHARS = 26;

class TrieNode0 {
public:
	TrieNode0(string s) : isWord(false), word(s) {
		memset(children, 0, sizeof(children));
	}
public:
	/*** store-the-root-cur-pos-word ***/
	string word;//so no need to pass string in recur
	bool isWord;
	TrieNode0* children[MAX_CHARS];
};

class TrieTree {
public:
	TrieTree() :root(new TrieNode0("")) {}
	~TrieTree() { freeTree(root); }
	TrieNode0* getRoot() {
		return root;
	}

	void addWord(string& s) {
		TrieNode0* node = root;
		string t;
		for (int i = 0; i<s.size(); i++) {
			t += s[i];
			if (node->children[s[i] - 'a'] == NULL) {
				node->children[s[i] - 'a'] = new TrieNode0(t);
			}
			node = node->children[s[i] - 'a'];
		}
		node->isWord = true;
	}
private:
	void freeTree(TrieNode0* node) {
		for (int i = 0; i<MAX_CHARS; i++) {
			if (node->children[i] != NULL) {
				freeTree(node->children[i]);
			}
		}
		delete node;
	}
	TrieNode0* root;
};

class Solution212_1 {
public:
	vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
		TrieTree t;
		for (int i = 0; i<words.size(); i++)   t.addWord(words[i]);
		vector<string> result;
		for (int i = 0; i<board.size(); i++) {
			for (int j = 0; j<board[i].size(); j++) {
				help(board, t.getRoot(), i, j, result);
			}
		}
		return result;
	}
	//do DFS smarter so can pass by reference
	void help(vector<vector<char>>& board, TrieNode0* root, int row, int col, vector<string>& result) {
		if (row<0 || col<0 || row >= board.size() || col >= board[0].size() || board[row][col] == '*')  return;
		char ch = board[row][col];//save pre condition
		root = root->children[ch - 'a'];
		if (root == NULL)   return;
		if (root->isWord) {
			result.push_back(root->word);
			root->isWord = false;//cut off the found ones, can save effort trying to get it again
		}
		board[row][col] = '*';//mark on board
		help(board, root, row + 1, col, result);
		help(board, root, row - 1, col, result);
		help(board, root, row, col + 1, result);
		help(board, root, row, col - 1, result);
		board[row][col] = ch;//change it back 
	}
};

/////////////////////////////////////////////////////////////////////////
//decent and generious
class TrieNode1 {
public:
	bool is_end;
	vector<TrieNode1*> children;
	TrieNode1() {
		is_end = false;
		children = vector<TrieNode1*>(26, NULL);

	}
	/*	node* child[26];
	node() {
	count = 0;
	for (int i = 0; i < 26; i++)
	child[i] = NULL;
	}
	*/
};

class Trie {
public:
	Trie(vector<string>& words) {
		root = new TrieNode1();
		for (int i = 0; i<words.size(); ++i)
			addWord(words[i]);
	}

	~Trie() {
		delTree(root);
	}

	TrieNode1* getRoot() {
		return root;
	}

	void addWord(const string& word) {
		TrieNode1* cur = root;
		for (int i = 0; i<word.size(); ++i) {
			int index = word[i] - 'a';
			if (cur->children[index] == NULL)//if not in trie add new nod
				cur->children[index] = new TrieNode1();
			cur = cur->children[index];
		}
		cur->is_end = true;
	}
private:
	TrieNode1* root;
	void delTree(TrieNode1 *root) {
		for (int i = 0; i < 26; ++i) {
			if (root->children[i])
				delTree(root->children[i]);
		}
		free(root);
		//delete root;
	}
};

class Solution212_0_2 {
public:
	vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
		Trie* trie = new Trie(words);
		TrieNode1* root = trie->getRoot();
		//try all possible starting point
		for (int x = 0; x<board.size(); ++x)
			for (int y = 0; y<board[0].size(); ++y)
				findWords(board, x, y, root, "");

		vector<string> result;
		for (auto it : result_set)    result.push_back(it);
		return result;
	}
private:
	set<string> result_set;//avoid storing dupliate 
						   // (board,current x,y on board, current nod on Trie, current word formed, 
	void findWords(vector<vector<char>>& board, int x, int y, TrieNode1* root, string word) {
		if (x<0 || x >= board.size() || y<0 || y >= board[0].size() || board[x][y] == ' ') //if x,y out of board orused in word
			return;
		//if current char on board[x][y] is in trie
		if (root->children[board[x][y] - 'a'] != NULL) {
			word = word + board[x][y];
			root = root->children[board[x][y] - 'a'];
			if (root->is_end) //work direct on trie pointer is faster
				result_set.insert(word);
			char c = board[x][y];
			board[x][y] = ' ';//mark as used for sub search
			findWords(board, x + 1, y, root, word);
			findWords(board, x - 1, y, root, word);
			findWords(board, x, y + 1, root, word);
			findWords(board, x, y - 1, root, word);
			board[x][y] = c;//unmark so not interrupe future start point
		}
	}
};

///////////////////////////////////////////////////////////////////////////////
//211. Add and Search Word - Data structure design
class Node221 {
public:
	Node221() {
		term = false;
		children = vector<Node221*>(26, NULL);
	}
	bool term;//is terminal
	vector<Node221*> children;
};

class WordDictionary221_0 {
public:
	/** Initialize your data structure here. */
	WordDictionary221_0() {
		root = new Node221;
	}

	~WordDictionary221_0() {
		burnTree(root);
	}

	/** Adds a word into the data structure. */
	void addWord(string word) {
		Node221* nod = root;
		for (int i = 0; i<word.size(); i++) {
			int index = word[i] - 'a';
			if (!nod->children[index])
				nod->children[index] = new Node221;
			nod = nod->children[index];
		}
		nod->term = true;
	}

	/** Returns if the word is in the data structure. A word could contain the dot character '.' to represent any one letter. */
	bool search(string word) {
		return searchFF(root, 0, word);
	}
private:
	Node221* root;
	void burnTree(Node221* nod) {
		if (!nod) return;
		for (int i = 0; i<26; i++) {
			if (nod->children[i])
				burnTree(nod->children[i]);
		}
		delete nod;
	}
	//search From i th char of word, start From sub tree of nod
	bool searchFF(Node221* nod, int i, string word) {
		if (!nod)
			return false;
		for (; i<word.size(); i++) {
			if (word[i] == '.') {
				bool ret = false;
				for (auto c : nod->children)
					ret |= searchFF(c, i + 1, word);
				return ret;
			}
			int index = word[i] - 'a';
			if (!nod->children[index])
				return false;
			nod = nod->children[index];
		}
		if (nod->term)
			return true;
		return false;
	}
};

//pass a char* instead of string to query and it greatly speeds up the code. 
//So the initial call to query is actually query(word.c_str(), root)
class TrieNode {
public:
	bool isKey;
	TrieNode* children[26];
	TrieNode() : isKey(false) {
		memset(children, NULL, sizeof(TrieNode*) * 26);
	}
};

class WordDictionary {
public:
	WordDictionary() {
		root = new TrieNode();
	}

	// Adds a word into the data structure.
	void addWord(string word) {
		TrieNode* run = root;
		for (char c : word) {
			if (!(run->children[c - 'a']))
				run->children[c - 'a'] = new TrieNode();
			run = run->children[c - 'a'];
		}
		run->isKey = true;
	}

	// Returns if the word is in the data structure. A word could
	// contain the dot character '.' to represent any one letter.
	bool search(string word) {
		return query(word.c_str(), root);
	}

private:
	TrieNode* root;

	bool query(const char* word, TrieNode* node) {
		TrieNode* run = node;
		for (int i = 0; word[i]; i++) {
			if (run && word[i] != '.')
				run = run->children[word[i] - 'a'];
			else if (run && word[i] == '.') {
				TrieNode* tmp = run;
				for (int j = 0; j < 26; j++) {
					run = tmp->children[j];
					if (query(word + i + 1, run))
						return true;
				}
			}
			else break;
		}
		return run && run->isKey;
	}
};

void main() {
	Solution212_0 S;
	vector<vector<char>> board;
	vector<char> B = {'a','b'};
	vector<char> B1 = {'c','d'};
	//vector<char> B = { 'i','h','k','r' };
	//vector<char> B = { 'i','f','l','v' };
	board.push_back(B);
	board.push_back(B1);
	vector<string> word = { "cdba" };
	S.findWords(board, word);
}