#include<vector>
#include<map>
#include<algorithm>
#include<set>
#include<utility>
#include <iostream>
#include <string>
#include <unordered_set>
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////
//252. Meeting Rooms

// Definition for an interval.
struct Interval {
    int start;
    int end;
    Interval() : start(0), end(0) {}
    Interval(int s, int e) : start(s), end(e) {}
};

class Solution252_0 {
public:
	bool canAttendMeetings(vector<Interval>& intervals) {
		if (intervals.size() == 0)
			return true;
		multimap<int, int> sorted;//auto sort with key, allow same key exist
		for (auto i : intervals) {
			sorted.emplace(i.start, 1);
			sorted.emplace(i.end, -1);
		}
		int c = 0;//count of meeting num
		int last = sorted.begin()->first;;
		for (auto it = sorted.begin(); it != sorted.end(); it++) {
			//summarize the condition after consider all change in one time point
			if (it->first != last&&c>1)
				return false;
			//then update current change
			c += it->second;
			last = it->first;
		}
		return true;
	}
};

//this is simple, can use sort
class Solution252_1 {
public:
	bool canAttendMeetings(vector<Interval>& intervals) {
		if (intervals.size() == 0)
			return true;
		sort(intervals.begin(), intervals.end(), [](Interval& left, Interval& right) { return left.start<right.start; });
		for (int i = 0; i<intervals.size() - 1; i++)
			if (intervals[i].end>intervals[i + 1].start)
				return false;
		return true;
	}
};


/////////////////////////////////////////////////////////////////////////////////////
//253. Meeting Rooms II
class Solution253_0 {
public:
	int minMeetingRooms(vector<Interval>& intervals) {
		multimap<int, int> ready;
		for (auto i : intervals) {
			ready.emplace(i.start, 1);
			ready.emplace(i.end, -1);
		}
		int c = 0;
		int ret = 0;
		int pre = ready.begin()->first;
		for (auto r : ready) {
			if (r.first != pre)
				ret = max(ret, c);
			c += r.second;
			pre = r.first;
		}
		return ret;
	}
};


//no need for multimap
class Solution253_1 {
public:
	int minMeetingRooms(vector<Interval>& intervals) {
		map<int, int> changes;
		for (auto i : intervals) {
			changes[i.start] += 1;
			changes[i.end] -= 1;
		}
		int rooms = 0, maxrooms = 0;
		for (auto change : changes)
			maxrooms = max(maxrooms, rooms += change.second);
		return maxrooms;
	}
};

////////////////////////////////////////////////////////////////////////
//218. The Skyline Problem
//multiset<pair<int, int>> also sort pair with same key
class Solution218_1 {
public:
	vector<pair<int, int>> getSkyline(vector<vector<int>>& buildings) {
		pair<int, int> curr({ 0,0 });
		vector<pair<int, int>> result;
		multiset<pair<int, int>> seq;//all building edges
		for (auto p : buildings) {
			seq.emplace(make_pair(p[0], -p[2]));//left edge {x1
			seq.emplace(make_pair(p[1], p[2]));//right edge {x2
		}
		multiset<int> height({ 0 });//all building height above current edge

		for (auto p : seq) {//for each edge
			//cout << "x£º" << p.first << " y:" << p.second << endl;
			if (p.second<0)
				height.emplace(-p.second);//add height on left edge
			else
				height.erase(height.find(p.second));//remove height on right edge
			if (*height.rbegin() != curr.second) {//if heightest y not euqal to last y
				curr.first = p.first;//update x
				curr.second = *height.rbegin();//update y
				result.push_back(curr);//store
			}
		}
		return result;
	}
};
/*
Your input
[[0,2,3],[2,5,3]]
Your stdout
x£º0 y:3	¡¾height: 3¡¿	[0,3]
x£º2 y:-3	¡¾height: ¡¿	[2,0]
x£º2 y:3	¡¾height: 3¡¿	[2,3]
x£º5 y:-3	¡¾height: ¡¿	[5,0]
Your answer
[[0,3],[2,0],[2,3],[5,0]]
Expected answer
[[0,3],[5,0]]


Your input
[[0,2,3],[2,5,3]]
Your stdout
x£º0 y:-3	¡¾height: 3¡¿	[0,3]
x£º2 y:-3	¡¾height: 3£¬3¡¿[no action]
x£º2 y:3	¡¾height: 3¡¿	[no action]
x£º5 y:3	¡¾height: 0¡¿	[5,0]
Your answer
[[0,3],[5,0]]
Expected answer
[[0,3],[5,0]]

------------------------------------
Your input
[[0,2,3],[0,2,6]]
Your stdout
x£º0 y:3
x£º0 y:6
x£º2 y:-6
x£º2 y:-3
Your answer
[[0,3],[0,6],[2,3],[2,0]]
Expected answer
[[0,6],[2,0]]

Your input

[[0,2,3],[0,2,6]]
Your stdout
x£º0 y:-6
x£º0 y:-3
x£º2 y:3
x£º2 y:6
Your answer
[[0,6],[2,0]]
Expected answer
[[0,6],[2,0]]
*/


////////////////////////////////////////////////////////////////////////
//391. Perfect Rectangle
//=============================doing surface check===============================
//45 / 46 test cases passed.
class Solution391_9 {
private:
	multiset<vector<int>> cy;//divided cells with same x edges
	set<int> div;//divider x position
	void divide(vector<int> rec) {//divide a rectangle with given x edges
		for (auto d : div) {
			if (rec[0]<d&&d<rec[2]) {
				cy.insert({ rec[0],rec[1],d,rec[3] });
				divide({ d,rec[1],rec[2],rec[3] });
				return;
			}
		}
		cy.insert(rec);
	}
public:
	bool isRectangleCover(vector<vector<int>>& rectangles) {
		//get all x divide position
		for (auto & rec : rectangles) {
			div.insert(rec[0]);
			div.insert(rec[2]);
		}
		//divide all rectangle into same wide cells
		for (auto & rec : rectangles)
			divide(rec);
		//merge cell into colums
		vector<vector<int>> col;
		while (cy.size()>0) {
			//get one cell(the first)
			vector<int> first = *cy.begin();
			cy.erase(cy.begin());
			//if this colum has finsised merge
			if (cy.size() == 0 || (*cy.begin())[0] != first[0])
				col.push_back(first);
			//if two colums are not adjacent
			else if (cy.size() != 0 && first[3] != (*cy.begin())[1])
				return false;
			//if next cell is in same colum
			else {
				//merge the cell into next 
				vector<int> tem = *cy.begin();
				tem[1] = first[1];
				cy.erase(cy.begin());
				cy.insert(tem);
			}
		}
		//check if all colums can be merged
		int x1 = col[0][2];
		int y0 = col[0][1];
		int y1 = col[0][3];
		for (int i = 1; i<col.size(); i++) {
			if (col[i][0] != x1 || col[i][1] != y0 || col[i][3] != y1)
				return false;
			x1 = col[i][2];
		}
		return true;
	}
};


//45 / 46 test cases passed.
class Solution391_99 {
private:
	set<int> div;//divider x position(in order)
	multiset<vector<int>> cell;//divided cells with edges on div (ordered)
	void divide(const vector<int> & rec) {//divide a rectangle with given x edges, store in 
		for (auto d : div) {
			if (rec[0]<d&&d<rec[2]) {
				cell.insert({ rec[0],rec[1],d,rec[3] });
				divide({ d,rec[1],rec[2],rec[3] });
				return;
			}
		}
		cell.insert(rec);
	}

public:
	bool isRectangleCover(vector<vector<int>>& rectangles) {
		//get all x divide position
		for (auto & rec : rectangles) {
			div.insert(rec[0]);
			div.insert(rec[2]);
		}
		//divide all rectangle into same wide cells
		for (auto & rec : rectangles)
			divide(rec);
		//merge cell into colums
		set<int> Ys;//store all Y border met on merged colum
		int preX = ((*cell.begin())[0]);//X of last cell(name of colum)
		int preX2 = ((*cell.begin())[2]);//end of last colum
		int preY = ((*cell.begin())[1]);//top Y of last cell
		Ys.insert(preY);//insert first botttom Y
		for (auto it = cell.begin(); it != cell.end(); it++) {
			if ((*it)[0] == preX) {//when on same colum
				if ((*it)[1] != preY)//cells in this colum not connected
					return false;
			}
			else {//new colum
				cout << "reset" << endl;
				if ((*it)[0] != preX2)//different colum not neighbored 
					return false;
				Ys.insert(preY);//insert last top Y                
				preX = ((*it)[0]);//reset preX
				Ys.insert((*it)[1]);//insert this bottol Y
			}
			preX2 = (*it)[2];
			preY = (*it)[3];
		}
		//
		if (Ys.size()>2)
			return false;
		return true;
	}
};

//============================doing line check======================================
//do not over simplify the concept, may miss edge situations
class Solution391_0 {
private:
	multimap<int, pair<int, int>> Xsorted;//x=int, y=<start,end>£¬begining edges vertical to x 
	multimap<int, pair<int, int>> rXsorted;//ending edges vertical to y
	multimap<int, pair<int, int>> Ysorted;
	multimap<int, pair<int, int>> rYsorted;
	set<int> X, Y;//where border exist

	//on position x==?/y==? merge all begining/ending edges 
	vector<pair<int, int>> getlineOn(const multimap<int, pair<int, int>>& sorted, int x) {
		vector<pair<int, int>> lines;
		//sorted lines on x==?/y==?
		auto range = sorted.equal_range(x);
		multiset<pair<int, int>> S;
		for (auto it = range.first; it != range.second; it++) {
			S.emplace(it->second.first, it->second.second);
		}
		//try merge all short lines 
		for (auto it = S.begin(); it != S.end(); it++) {//for all short lines
			bool ext = false;
			for (auto& p : lines) {//check one existing lines
				if (p.second == it->first) {//if can attach to any
					p.second = it->second;
					ext = true;
					break;
				}
			}
			if (!ext) //if can't attach to existing lines
				lines.push_back({ it->first,it->second });
		}
		return lines;
	};

	//check if all inner edges(vertiacl to x/y) do not overlap or gap 
	bool goodInner(const set<int> &X, multimap<int, pair<int, int>>& Xsorted, multimap<int, pair<int, int>>& rXsorted) {
		for (auto& it : X) {
			if (it != (*X.begin()) && it != (*X.rbegin())) {
				vector<pair<int, int>> Xedge = getlineOn(Xsorted, it);
				vector<pair<int, int>> rXedge = getlineOn(rXsorted, it);
				int n = Xedge.size();
				int rn = rXedge.size();
				if (n != rn) {
					return false;
				}
				for (int i = 0; i<n; i++) {//begin and end edge on same x/y should neutralize 
					if (Xedge[i] != rXedge[i])
						return false;
				}
			}
		}
		return true;
	};
public:
	bool isRectangleCover(vector<vector<int>>& rectangles) {
		for (auto & rec : rectangles) {
			//store all border places
			X.insert(rec[0]);
			X.insert(rec[2]);
			Y.insert(rec[1]);
			Y.insert(rec[3]);
			//store all beginning/ending borders vertical to x/y
			pair<int, int> temp = { rec[1],rec[3] };
			Xsorted.emplace(rec[0], temp);
			temp = { rec[1],rec[3] };
			rXsorted.emplace(rec[2], temp);
			temp = { rec[0],rec[2] };
			Ysorted.emplace(rec[1], temp);
			temp = { rec[0],rec[2] };
			rYsorted.emplace(rec[3], temp);
		}

		//check outter borders vertical to x           
		vector<pair<int, int>> Xedge0 = getlineOn(Xsorted, *X.begin());
		vector<pair<int, int>> Xedge1 = getlineOn(rXsorted, *X.rbegin());
		if (Xedge0.size() != 1 || Xedge1.size() != 1 || Xedge0[0] != Xedge1[0])//(should be two parallel line) 
			return false;

		//check outter borders vertical to y
		vector<pair<int, int>> Yedge0 = getlineOn(Ysorted, *Y.begin());
		vector<pair<int, int>> Yedge1 = getlineOn(rYsorted, *Y.rbegin());
		if (Yedge0.size() != 1 || Yedge1.size() != 1 || Yedge0[0] != Yedge1[0])
			return false;

		//check inner borders
		if (!goodInner(X, Xsorted, Xsorted))
			return false;

		if (!goodInner(Y, Ysorted, rYsorted))
			return false;

		return true;
	}
};

//============================doing dot check======================================
/*
counting corner
_ _  _|_  _.
 |    |    |
*/
//unaligned overlap will generate a corner in the interior of another sub-rectangle,
class Solution391_1 {
private:
	string getKey(int x, int y) {
		return to_string(x) + " " + to_string(y);
	}
public:
	bool isRectangleCover(vector<vector<int>>& rectangles) {
		if (rectangles.empty() || rectangles[0].empty())
			return false;
		int x1 = INT_MAX, y1 = INT_MAX, x2 = INT_MIN, y2 = INT_MIN, area = 0;
		unordered_set<string> s;//store all corners by string
		for (auto v : rectangles) {
			x1 = min(x1, v[0]);//left most x
			y1 = min(y1, v[1]);//bottom most y
			x2 = max(x2, v[2]);//right most x
			y2 = max(y2, v[3]);//top most y
			area += (v[2] - v[0]) * (v[3] - v[1]);//add area of current rect
			string a = getKey(v[0], v[1]), b = getKey(v[0], v[3]), c = getKey(v[2], v[1]), d = getKey(v[2], v[3]);
			//unordered_set.erase() returns the number of elements erased
			if (!s.erase(a)) s.insert(a);//effectively cancel every two
			if (!s.erase(b)) s.insert(b);
			if (!s.erase(c)) s.insert(c);
			if (!s.erase(d)) s.insert(d);
		}
		return s.count(getKey(x1, y1))//corner is left
			&& s.count(getKey(x1, y2))
			&& s.count(getKey(x2, y1))
			&& s.count(getKey(x2, y2))
			&& s.size() == 4//only 4 corner not cancelled
			&& area == (x2 - x1) * (y2 - y1);//area meets
	}
};

void main() {

}