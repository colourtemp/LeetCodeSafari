#include <stddef.h> //NULL
#include <stack>
#include <vector>
#include <algorithm>//sort
using namespace std;
// Definition for singly - linked list.
 struct ListNode {
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(NULL) {}
};


 ///////////////////////////////////////////////////////////////////////////
 //141. Linked List Cycle
 //No extra space usually means constant space
 //use the characteristic of circle
 /*
 if there's a circle, the faster one will finally "catch" the slower one.
 (the distance between these 2 pointers will decrease one every time.)
 if there's no circle, the faster runner will reach the end of linked list. (NULL)
 */
 bool hasCycle(ListNode *head) {
	 if (head == NULL) return false;
	 ListNode* walker = head;
	 ListNode* runner = head;
	 while (runner->next != NULL && runner->next->next != NULL) {
		 walker = walker->next;
		 runner = runner->next->next;
		 if (walker == runner) return true;
	 }
	 return false;
 }

 ///////////////////////////////////////////////////////////////////////////
 //237. Delete Node in a Linked List
 void deleteNode(ListNode* node) {
	 node->val = node->next->val;
	 node->next = node->next->next;
 }

 ///////////////////////////////////////////////////////////////////////////
 // 83. Remove Duplicates from Sorted List
 ListNode* deleteDuplicates(ListNode* head) {
	 ListNode* cur = head;
	 while (cur) {
		 while (cur->next && cur->val == cur->next->val) {//remember to check NULL ptr
			 delete cur->next;//remember to free memory
			 cur->next = cur->next->next;
		 }
		 cur = cur->next;
	 }
	 return head;
 }


 ///////////////////////////////////////////////////////////////////////////
 //160. Intersection of Two Linked Lists
 //dame smart
 ListNode *getIntersectionNode(ListNode *headA, ListNode *headB)
 {
	 ListNode *p1 = headA;
	 ListNode *p2 = headB;

	 if (p1 == NULL || p2 == NULL) return NULL;

	 while (p1 != NULL && p2 != NULL && p1 != p2) {
		 p1 = p1->next;
		 p2 = p2->next;
		 
		 // Any time they collide or reach end together without colliding 
		 // then return any one of the pointers.

		 // Will also meet on NULL when have two separate link list
		 // image NULL as the intercept point
		 if (p1 == p2) 
			 return p1;
		 
		 // If one of them reaches the end earlier then reuse it 
		 // by moving it to the beginning of other list.
		 // Once both of them go through reassigning, 
		 // they will be equidistant from the collision point.
		 if (p1 == NULL) 
			 p1 = headB;
		 if (p2 == NULL) 
			 p2 = headA;
	 }

	 return p1;
 }

 ///////////////////////////////////////////////////////////////////////////
 //203. Remove Linked List Elements
 ListNode* removeElements_0(ListNode* head, int val) {
	 ListNode* walk = head;
	 if (head == NULL)
		 return NULL;

	 while (walk != NULL&&walk->val == val) {
		 head = walk->next;
		 walk = head;
	 }

	 while (walk != NULL&&walk->next != NULL) {
		 if (walk->next->val == val) {
			 walk->next = walk->next->next;
		 }
		 else
			 walk = walk->next;
	 }
	 return head;
 }

 ///////////////////////////////////////////////////////////////////////////
 //206. Reverse Linked List
 ListNode* reverseList(ListNode* head) {
	 stack<ListNode*> s;
	 while (head) {
		 s.push(head);
		 head = head->next;
	 }
	 if (s.empty())
		 return NULL;
	 ListNode* ret = s.top();
	 s.pop();
	 ListNode* walk = ret;
	 while (!s.empty()) {
		 walk->next = s.top();
		 s.pop();
		 walk = walk->next;
	 }
	 walk->next = NULL;
	 return ret;
 }

 //more elegent
 ListNode* reverseList_1(ListNode* head) {
	 ListNode* pre = NULL;
	 while (head) {
		 ListNode* next = head->next;
		 head->next = pre;
		 pre = head;
		 head = next;
	 }
	 return pre;
 }

 //recur has the feature of stack
 ListNode* reverseList_2(ListNode* head) {
	 if (!head || !(head->next)) return head;
	 ListNode* node = reverseList(head->next);
	 head->next->next = head;
	 head->next = NULL;
	 return node;
 }

 ///////////////////////////////////////////////////////////////////////////
 //234. Palindrome Linked List
 class Solution234 {
 public:
	 bool isPalindrome(ListNode* head) {
		 ListNode* slow = head;
		 ListNode* fast = head;
		 while (fast != NULL&&fast->next != NULL) {//fast slow trick again
			 slow = slow->next;
			 fast = fast->next->next;
		 }
		 ListNode* re = reverseList(slow);
		 while (re != NULL) {
			 if (head->val != re->val)
				 return false;
			 head = head->next;
			 re = re->next;
		 }
		 return true;

	 }
	 //local revers is useful
	 ListNode* reverseList(ListNode* head) {
		 ListNode* prev = NULL;
		 ListNode* nex = NULL;
		 while (head != NULL) {
			 nex = head->next;
			 head->next = prev;
			 prev = head;
			 head = nex;
		 }
		 return prev;
	 }
 };

 ///////////////////////////////////////////////////////////////////////////
 //21. Merge Two Sorted Lists
 //use recur smart, 
 ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
	 if (l1 == NULL) return l2;
	 if (l2 == NULL) return l1;

	 if (l1->val < l2->val) {
		 l1->next = mergeTwoLists(l1->next, l2);
		 return l1;
	 }
	 else {
		 l2->next = mergeTwoLists(l2->next, l1);
		 return l2;
	 }
 }

 //dummy is brillant!
 ListNode *mergeTwoLists_2(ListNode *l1, ListNode *l2) {
	 ListNode dummy(0);
	 ListNode* tail = &dummy;
	 while (l1 != NULL&&l2 != NULL) {
		 if (l1->val<l2->val) {
			 tail->next = l1;
			 l1 = l1->next;
		 }
		 else {
			 tail->next = l2;
			 l2 = l2->next;
		 }
		 tail = tail->next;
	 }
	 tail->next = l1 ? l1 : l2;
	 return dummy.next;
 }

 class Solution21 {
 public:
	 ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
		 while (l2 != NULL) {
			 ListNode* tem = l2->next;
			 sortedInsert(&l1, l2);
			 l2 = tem;
		 }
		 return l1;
	 }
	 //use of reference, save the space of dummy
	 void sortedInsert(ListNode **headRef, ListNode *newNode) {
		 ListNode **currRef = headRef;//play with the origin pointer instead of the copy
		 while (*currRef != nullptr && (*currRef)->val < newNode->val)
			 currRef = &((*currRef)->next);
		 newNode->next = *currRef;
		 *currRef = newNode;//put newNode into the origin pointer's target
	 }
	 //use of dummy
	 void sortedInsert_0(ListNode **headRef, ListNode *newNode) {
		 ListNode dummy(0), *tail = &dummy;
		 dummy.next = *headRef;
		 //keep using the ->next because need know both side of the origin
		 while (tail->next != NULL && tail->next->val < newNode->val)
			 tail = tail->next;
		 newNode->next = tail->next;
		 tail->next = newNode;
		 *headRef = dummy.next;
	 }
 };


 //MIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMIDMID

 ///////////////////////////////////////////////////////////////////////////
 //61. Rotate List
 ListNode* rotateRight_0(ListNode* head, int k) {
	 if (head == NULL || k == 0)
		 return head;
	 ListNode* cont0 = head;
	 ListNode* cont1 = head;
	 ListNode* cont2 = head;
	 ListNode* cont3 = head;
	 int i = 1;
	 for (; i <= k; i++) {
		 if (cont3->next != NULL) {
			 cont3 = cont3->next;
		 }
		 else
			 return rotateRight_0(head, k%i);
	 }
	 while (cont3->next != NULL) {
		 cont3 = cont3->next;
		 cont1 = cont1->next;
	 }
	 cont2 = cont1->next;
	 cont3->next = cont0;
	 cont1->next = NULL;
	 return cont2;
 }

 //simply count length, a litte faster
 ListNode* rotateRight_1(ListNode* head, int k) {
	 if (!head) return NULL;
	 ListNode* pseu = new ListNode(0);
	 pseu->next = head;
	 ListNode* fast = pseu;
	 int len = 0;
	 while (fast->next) {
		 len++;
		 fast = fast->next;
	 }
	 k = len - k % len;
	 if (k == len) return head;
	 ListNode* last = fast;
	 fast = pseu;
	 while (k) {
		 fast = fast->next;
		 k--;
	 }
	 ListNode* next = fast->next;
	 last->next = pseu->next;
	 fast->next = NULL;
	 return next;

 }

 ///////////////////////////////////////////////////////////////////////////
 //148. Sort List
 //normal merge sort, space complexity is O(logn)
 class Solution148_0 {
 public:
	 //been called log(n) times
	 ListNode* sortList(ListNode* head) {
		 if (head == NULL || head->next == NULL)
			 return head;
		 ListNode* slow = head;
		 ListNode* fast = head->next->next;
		 //slow & fast tachnic to find the middle
		 while (fast != NULL&&fast->next != NULL) {
			 slow = slow->next;
			 fast = fast->next->next;
		 }
		 fast = slow->next;
		 slow->next = NULL;
		 //key point of merge sort, when cut to one node list, the list is sorted
		 ListNode* left = sortList(head);
		 ListNode* right = sortList(fast);
		 //then question become merge sorted list
		 return merge(left, right);
	 }
	 //been called log(n) times
	 ListNode *merge(ListNode*h1, ListNode*h2)
	 {
		 ListNode dummy(0);
		 ListNode* tail = &dummy;
		 while (h1 != NULL&&h2 != NULL) {
			 if (h1->val<h2->val) {
				 tail->next = h1;
				 h1 = h1->next;
			 }
			 else {
				 tail->next = h2;
				 h2 = h2->next;
			 }
			 tail = tail->next;
		 }
		 tail->next = h1 ? h1 : h2;
		 return dummy.next;
	 }
 };

 /**
 * Merge sort use bottom-up policy,
 * so Space Complexity is O(1)
 * Time Complexity is O(NlgN)
 * stable sort
 */
 class Solution148_1 {
 public:
	 ListNode *sortList(ListNode *head) {
		 if (!head || !(head->next))
			 return head;
		 //get the linked list's length
		 ListNode* cur = head;
		 int length = 0;
		 while (cur) {
			 length++;
			 cur = cur->next;
		 }
		 //sort by 2 node groups->4 node groups->8 node....kind like reverese string
		 ListNode dummy(0);
		 dummy.next = head;
		 ListNode *left, *right, *tail;
		 for (int step = 1; step < length; step <<= 1) {
			 cur = dummy.next;
			 tail = &dummy;
			 while (cur) {
				 left = cur;
				 right = split(left, step);
				 cur = split(right, step);
				 tail = merge(left, right, tail);
			 }
		 }
		 return dummy.next;
	 }
 private:
	 /**
	 * Divide the linked list into two lists,
	 * while the first list contains first n ndoes
	 * return the second list's head
	 */
	 ListNode* split(ListNode *head, int n) {
		 //if(!head) return NULL;
		 for (int i = 1; head && i < n; i++)
			 head = head->next;
		 if (!head)
			 return NULL;
		 ListNode *second = head->next;
		 head->next = NULL;
		 return second;
	 }
	 /**
	 * merge the two sorted linked list l1 and l2,
	 * then append the merged sorted linked list to the node head
	 * return the tail of the merged sorted linked list
	 */
	 ListNode* merge(ListNode* l1, ListNode* l2, ListNode* head) {
		 ListNode *cur = head;
		 while (l1 && l2) {
			 if (l1->val > l2->val) {
				 cur->next = l2;
				 cur = l2;
				 l2 = l2->next;
			 }
			 else {
				 cur->next = l1;
				 cur = l1;
				 l1 = l1->next;
			 }
		 }
		 cur->next = (l1 ? l1 : l2);
		 while (cur->next) cur = cur->next;
		 return cur;
	 }
 };


 ///////////////////////////////////////////////////////////////////////////
 //147. Insertion Sort List
 ListNode* insertionSortList_0(ListNode* head) {
	 if (head == NULL || head->next == NULL)
		 return head;

	 ListNode dummy(INT_MIN);

	 while (head != NULL) {
		 ListNode** p = &dummy.next;
		 ListNode* hnxt = head->next;
		 while ((*p) != NULL && (*p)->val<head->val)
			 p = &((*p)->next);//this is changing pointer of pointer
		 head->next = *p;
		 *p = head;//this is change the origin position
		 head = hnxt;
	 }
	 return dummy.next;
 }

 //little more space, faster
 ListNode* insertionSortList_1(ListNode* head) {
	 ListNode* new_head = new ListNode(0);
	 new_head->next = head;
	 ListNode* pre = new_head;
	 ListNode* cur = head;
	 while (cur) {
		 if (cur->next && cur->next->val < cur->val) {
			 while (pre->next && pre->next->val < cur->next->val)
				 pre = pre->next;
			 /* Insert cur -> next after pre.*/
			 ListNode* temp = pre->next;
			 pre->next = cur->next;
			 cur->next = cur->next->next;
			 pre->next->next = temp;
			 /* Move pre back to new_head. */
			 pre = new_head;
		 }
		 else cur = cur->next;
	 }
	 ListNode* res = new_head->next;
	 delete new_head;
	 return res;
 }

 //You could for sure talk about it in the interview and clear it out before jumping into sorting a list.
 // you could talk about the following comparison of copying a list to array vs sorting the list
 /*
 Depending on a number of factors, it may actually be faster to copy the list to an array and then use a Quicksort.
 array has much better cache performance than a linked list. If the nodes in the list are dispersed in memory, 
 you may be generating cache misses all over the place. 
 Then again, if the array is large you will get cache misses anyway.

 Mergesort parallelises better, so it may be a better choice if that is what you want. 
 It is also much faster if you perform it directly on the linked list.
 */
 //So it might be better to actually copy the values into an array and sort them there.
 ListNode* insertionSortList_2(ListNode* head) {
	 if (head == nullptr) return head;
	 vector<int> v;
	 ListNode *node = head;
	 while (node) {
		 v.push_back(node->val);
		 node = node->next;
	 }
	 sort(v.begin(), v.end());
	 node = head;
	 ListNode *h = node;
	 node->val = v[0];
	 for (int i = 1; i < v.size(); ++i) {
		 node->next = new ListNode(v[i]);
		 node = node->next;
	 }

	 head = h;
	 return head;
 }

 ///////////////////////////////////////////////////////////////////////////
 // 143. Reorder List
 void reorderList_0(ListNode* head) {
	 if (head == NULL || head->next == NULL || head->next->next == NULL)
		 return;
	 //get midddle
	 ListNode* slow = head;
	 ListNode* fast = head->next->next;
	 while (fast != NULL&&fast->next != NULL) {
		 fast = fast->next->next;
		 slow = slow->next;
	 }
	 fast = slow->next;
	 slow->next = NULL;
	 ListNode* Rhead = NULL;
	 while (fast != NULL) {
		 ListNode* N = fast->next;
		 fast->next = Rhead;
		 Rhead = fast;
		 fast = N;
	 }
	 //merge
	 ListNode dummy(0);
	 ListNode* tail = &dummy;
	 while (head != NULL&&Rhead != NULL) {
		 tail->next = head;
		 tail = tail->next;
		 head = head->next;

		 tail->next = Rhead;
		 tail = tail->next;
		 Rhead = Rhead->next;
	 }
	 tail->next = head ? head : Rhead;
	 head = dummy.next;
 }

 // more elegent faster
 // O(N) time, O(1) space in total
 void reorderList_1(ListNode *head) {
	 if (!head || !head->next) return;

	 // find the middle node: O(n)
	 ListNode *p1 = head, *p2 = head->next;
	 while (p2 && p2->next) {
		 p1 = p1->next;
		 p2 = p2->next->next;
	 }

	 // cut from the middle and reverse the second half: O(n)
	 ListNode *head2 = p1->next;
	 p1->next = NULL;

	 p2 = head2->next;
	 head2->next = NULL;
	 while (p2) {
		 p1 = p2->next;
		 p2->next = head2;
		 head2 = p2;
		 p2 = p1;
	 }

	 // merge two lists: O(n)   
	 for (p1 = head, p2 = head2; p1; ) {
		 auto t = p1->next;
		 p1 = p1->next = p2;//????
		 p2 = t;
	 }

	 //for (p1 = head, p2 = head2; p2; ) {
	 //    auto t = p1->next;
	 //    p1->next = p2;
	 //    p2 = p2->next;
	 //    p1 = p1->next->next = t;
	 //}
 }

 ///////////////////////////////////////////////////////////////////////////
 //142. Linked List Cycle II
 //brute force, slow
 ListNode *detectCycle_0(ListNode *head) {
	 if (head == NULL)
		 return NULL;
	 //check if circle exist
	 ListNode* slow = head;
	 ListNode* fast = head;
	 ListNode* origin = head;
	 bool got = false;
	 int step = 0;
	 while (fast != NULL&&fast->next != NULL) {
		 step += 2;
		 slow = slow->next;
		 fast = fast->next->next;
		 if (slow == fast) {
			 got = true;
			 break;
		 }
	 }
	 if (!got)
		 return NULL;
	 //try start point, should be found in n steps
	 while (true) {
		 for (int i = 0; i<step; i++) {
			 head = head->next;
			 if (head == origin) {
				 return origin;
			 }
			 if (head == slow)//should be smaller than slow
				 break;
		 }
		 origin = origin->next;
	 }
	 return origin;
 }
 
 /* do some math

 L1 is defined as the distance between the head point and entry point
 L2 is defined as the distance between the entry point and the meeting point
 C is defined as the length of the cycle

 total distance of the slow pointer traveled when encounter is L1 + L2 + n1C
 total distance of the fast pointer traveled when encounter is L1 + L2 + n2C
 2(L1 + L2 + n1C£©= L1 + L2 + n2C
 L1= (n2-2*n1)C - L2 = n3C - L2
 L1 is equal to the steps which meeting point needed to rejoint with entry point
 */
 ListNode *detectCycle_1(ListNode *head) {
	 ListNode *slow = head, *fast = head, *begin = head;
	 while (fast&&fast->next)
	 {
		 slow = slow->next;
		 fast = fast->next->next;
		 if (slow == fast)
		 {
			 while (slow != begin)
			 {
				 slow = slow->next;
				 begin = begin->next;
			 }
			 return begin;
		 }
	 }
	 return nullptr;
 }


 ///////////////////////////////////////////////////////////////////////////
 //445. Add Two Numbers II
 //listNode will over flow even unsigned long long
 ListNode* addTwoNumbers_overflow(ListNode* l1, ListNode* l2) {
	 ListNode* p1 = l1;
	 ListNode* p2 = l2;//P1 should be shorter or equal
	 unsigned long long i1 = p1->val;
	 while (p1->next != NULL) {
		 i1 *= 10;
		 p1 = p1->next;
		 i1 += p1->val;
	 }
	 unsigned long long i2 = p2->val;
	 while (p2->next != NULL) {
		 i2 *= 10;
		 p2 = p2->next;
		 i2 += p2->val;
	 }
	 i1 = i1 + i2;
	 ListNode* N = NULL;
	 while (i1 > 0) {
		 ListNode* nod = new ListNode(i1 % 10);
		 nod->next = N;
		 N = nod;
		 i1 /= 10;
		 i1 = -1;
	 }
	 return N;
 }

 ListNode* addTwoNumbers_0(ListNode* l1, ListNode* l2) {
	 //use stack to reverse 
	 stack<int> s1, s2;
	 while (l1 != NULL) {
		 s1.push(l1->val);
		 l1 = l1->next;
	 }
	 while (l2 != NULL) {
		 s2.push(l2->val);
		 l2 = l2->next;
	 }
	 //reverse grow the new list digit by digit, avoid overflow
	 int sum = 0;
	 ListNode* N = NULL;
	 while (!s1.empty() || !s2.empty()) {
		 if (!s1.empty()) {
			 sum += s1.top();
			 s1.pop();
		 }
		 if (!s2.empty()) {
			 sum += s2.top();
			 s2.pop();
		 }
		 {
			 ListNode* nod = new ListNode(sum % 10);
			 nod->next = N;
			 N = nod;
			 sum /= 10;
		 }
	 }
	 //take care of extra carry digit
	 while (sum>0) {
		 ListNode* nod = new ListNode(sum % 10);
		 nod->next = N;
		 N = nod;
		 sum /= 10;
	 }

	 return N;
 }

 //dame smart
 class Solution445 {
 public:
	 ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
		 int n1 = 0, n2 = 0, carry = 0;
		 ListNode *curr1 = l1, *curr2 = l2, *res = NULL;
		 while (curr1) { curr1 = curr1->next; n1++; }
		 while (curr2) { curr2 = curr2->next; n2++; }
		 curr1 = l1; curr2 = l2;
		 //count digit to align
		 while (n1 > 0 && n2 > 0) {
			 int sum = 0;
			 if (n1 >= n2) { sum += curr1->val; curr1 = curr1->next; n1--; }
			 if (n2 > n1) { sum += curr2->val; curr2 = curr2->next; n2--; }
			 res = addToFront(sum, res);//build half-cooked list like input 
		 }
		 //reverse half-cooked list, take care of carry digit
		 curr1 = res; res = NULL;
		 while (curr1) {
			 curr1->val += carry; carry = curr1->val / 10;
			 res = addToFront(curr1->val % 10, res);
			 curr2 = curr1;
			 curr1 = curr1->next;
			 delete curr2;
		 }
		 if (carry) res = addToFront(1, res);
		 return res;
	 }
	 ListNode* addToFront(int val, ListNode* head) {
		 ListNode* temp = new ListNode(val);
		 temp->next = head;
		 return temp;
	 }
 };

 //learn the recur
 class Solution {
 public:
	 ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
		 int n1 = length(l1), n2 = length(l2);
		 int carry = 0;
		 ListNode* h = new ListNode(1);//prepared carrying digit
		 h->next = n1 > n2 ? add_aux(l1, l2, n1 - n2, carry) : add_aux(l2, l1, n2 - n1, carry);//judge the size
		 return carry == 1 ? h : h->next;//judge to use the extra digit or not
	 }
 private:
	 int length(ListNode* l) {
		 int len = 0;
		 while (l != nullptr) {
			 len++;
			 l = l->next;
		 }
		 return len;
	 }
	 //recur may use O(n) extra space
	 //use the reverse nature of recursion
	 ListNode* add_aux(ListNode* l1, ListNode* l2, int k, int& carry) {//k indicate the extra digit of l1
		 if (l2 == nullptr)
			 return nullptr;
		 ListNode* p = new ListNode(l1->val);
		 if (k > 0) {
			 p->next = add_aux(l1->next, l2, k - 1, carry);
		 }
		 else {
			 p->val += l2->val;
			 p->next = add_aux(l1->next, l2->next, k, carry);
		 }
		 p->val += carry;
		 carry = p->val / 10;
		 p->val %= 10;
		 return p;
	 }
 };

 ///////////////////////////////////////////////////////////////////////////
 // 138. Copy List with Random Pointer

 
  //Definition for singly-linked list with a random pointer.
  struct RandomListNode {
      int label;
      RandomListNode *next, *random;
      RandomListNode(int x) : label(x), next(NULL), random(NULL) {}
  };
 
  //space complexity of this solution is O(N)
 //might faster with map
 RandomListNode *copyRandomList_0(RandomListNode *head) {
	 //always remember empty input
	 if (head == NULL)
		 return NULL;
	 //put origin linked list in vector for position caculation
	 vector<RandomListNode*> list;
	 while (head != NULL) {
		 list.push_back(head);
		 head = head->next;

	 }

	 vector<int> positon;
	 vector<RandomListNode*> newlist;
	 RandomListNode dummy(0);
	 RandomListNode* tail = &dummy;
	 for (auto nod : list) {
		 //get the relative position of random pointer
		 if (nod->random == NULL)
			 positon.push_back(-1);
		 else
			 positon.push_back(distance(list.begin(), find(list.begin(), list.end(), nod->random)));
		 //build new list with out random pointer, put new list in vector
		 RandomListNode* newnod = new RandomListNode(nod->label);
		 newlist.push_back(newnod);
		 tail->next = newnod;
		 tail = tail->next;
	 }
	 //set random pointer according to relative position
	 for (int i = 0; i<newlist.size(); i++) {
		 if (positon[i] == -1)
			 newlist[i]->random = NULL;
		 else
			 newlist[i]->random = newlist[positon[i]];
	 }
	 return dummy.next;
 }

//The idea is to associate the original node with its copy node in a single linked list.
// O(n) space, O(1) extra space and linear time complexity O(N)
 RandomListNode *copyRandomList_1(RandomListNode *head) {
	 if (!head) 
		 return NULL;
	 RandomListNode* run = head;
	 /* Insert the copy of each node after it. */
	 /*
	 origin		1 - 2 - 3 - NULL
	 after copy	1   2   3   NULL
				| / | / | /
				1   2	3
	*/
	 while (run) {
		 RandomListNode* copy = new RandomListNode(run->label);
		 copy->next = run->next;
		 run->next = copy;
		 run = run->next->next;
	 }
	 /* Set the random pointer for each copy. */
	 run = head;
	 while (run) {
		 if (run->random)
			 run->next->random = run->random->next;
		 run = run->next->next;
	 }
	 /* Restore the original list and extract the duplicated nodes */
	 /*
	 origin		1 - 2 - 3 - NULL
	 after mod	1 - 2 - 3 - NULL

				1 - 2 - 3 - NULL
	 */
	 RandomListNode* new_head = new RandomListNode(0);
	 RandomListNode* new_run;
	 run = head;
	 new_head->next = head->next;
	 while (run) {
		 new_run = run->next;
		 run->next = new_run->next;
		 if (run->next)
			 new_run->next = new_run->next->next;
		 run = run->next;
	 }
	 return new_head->next;
 }

 //similiar to above, modify origin random instead
 RandomListNode *copyRandomList_2(RandomListNode *head) {
	 RandomListNode *newHead, *l1, *l2;
	 if (head == NULL) return NULL;
	 /*origin
	 l1			1 - 2 - 3   NULL
	 ramd p		  \ |
	 l1			1 - 2 - 3   NULL
	 */

	 /*connect
	 l2			1   2   3   NULL
	 next p		  \ |
	 l1			1 - 2 - 3   NULL
	 rand p		|	|	|
	 l2			1   2   3   NULL
	 */
	 for (l1 = head; l1 != NULL; l1 = l1->next) {
		 l2 = new RandomListNode(l1->label);
		 l2->next = l1->random;
		 l1->random = l2;
	 }

	 /* build rand p
	 l2			1   2   3   NULL
	 next p		  \ |
	 l1			1 - 2 - 3   NULL
	 rand p		|	|	|
	 l2			1   2   3   NULL
	 rand p		  \ |
	 l2			1   2   3   NULL
	 */
	 newHead = head->random;
	 for (l1 = head; l1 != NULL; l1 = l1->next) {
		 l2 = l1->random;
		 l2->random = l2->next ? l2->next->random : NULL;
	 }
	 /* restore	 */
	 for (l1 = head; l1 != NULL; l1 = l1->next) {
		 l2 = l1->random;
		 l1->random = l2->next;
		 l2->next = l1->next ? l1->next->random : NULL;
	 }

	 return newHead;
 }

 ///////////////////////////////////////////////////////////////////////////
 //109. Convert Sorted List to Binary Search Tree

  //Definition for a binary tree node.
  struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode(int x) : val(x), left(NULL), right(NULL) {}
  };

  /*
   The run time complexity is clearly O(N lg N), where N is the total number of elements in the list. 
   This is because each level of recursive call requires a total of N/2 traversal steps in the list, 
   and there are a total of lg N number of levels (ie, the height of the balanced tree).
  */
  TreeNode* sortedListToBST_0(ListNode* head) {
	  if (head == NULL)
		  return NULL;
	  if (head->next == NULL) {
		  TreeNode* tnod = new TreeNode(head->val);
		  return tnod;
	  }
	  //fast slow trick
	  ListNode* p1 = head;
	  ListNode* p2 = head->next->next;
	  while (p2 != NULL&&p2->next != NULL) {
		  p1 = p1->next;
		  p2 = p2->next->next;
	  }
	  p2 = p1->next;//middle
	  p1->next = NULL;//break left chain
	  p1 = p2->next;//right chain start
	  TreeNode* tnod = new TreeNode(p2->val);
	  //recur
	  tnod->left = sortedListToBST_0(head);
	  tnod->right = sortedListToBST_0(p1);

	  return tnod;
  }
 
  // create nodes bottom-up, and assign them to its parents. 
  //space complexity should be O(logn) 
  //(overhead used by the recursion, not including space used by the nodes) .
  //run time complexity is O(N)
  class Solution109 {
  public:
	  ListNode *list;
	  int count(ListNode *node) {
		  int size = 0;
		  while (node) {
			  ++size;
			  node = node->next;
		  }
		  return size;
	  }
	  //use subtree total nods number n as indicater
	  TreeNode *generate(int n) {
		  if (n == 0)//when divide to the leaf n==0
			  return NULL;
		  TreeNode *node = new TreeNode(0);
		  node->left = generate(n / 2);
		  node->val = list->val;
		  list = list->next;
		  node->right = generate(n - n / 2 - 1);
		  return node;
	  }

	  TreeNode *sortedListToBST(ListNode *head) {
		  this->list = head;
		  return generate(count(head));
	  }
  };


  //another idea
  /*
  We first create an empty tree with the correct structure, 
  then do an iterative in-order traversal of the tree 
  setting values into the nodes as we traverse the list. 
  The overall complexity is still O(N).
  */
  TreeNode* sortedListToBST_X(ListNode* head) {//divide upper generate function into two run
	  return NULL;
  }

  ///////////////////////////////////////////////////////////////////////////
  //92. Reverse Linked List II
  /* reverse everything after c to the front of shifting block
  ab|cde|f
  -----------------------
  ab|c e|f
     <d
  ab|dce|f
  ------------------------
  ab|dc |f
     <<e
  ab|edc|f
  */
  ListNode* reverseBetween(ListNode* head, int m, int n) {
	  ListNode* new_head = new ListNode(0);
	  new_head->next = head;
	  ListNode* pre = new_head;
	  for (int i = 0; i < m - 1; i++)
		  pre = pre->next; //nod (a) before shift block

	  ListNode* cur = pre->next;//nod (c) start to move (c)shift block <will finally go to the end of block>

	  for (int i = 0; i < n - m; i++) {
		  ListNode* move = cur->next; //moving nod(d) next to (c)
		  cur->next = move->next; //take out moving nod(d)
		  move->next = pre->next;//put moving nod(d) to the front of shift block
		  pre->next = move;//reconnect front side and shift block
	  }
	  return new_head->next;
  }


  ///////////////////////////////////////////////////////////////////////////
  // 86. Partition List
  ListNode* partition_0(ListNode* head, int x) {
	  ListNode smaller(0), bigger(0);
	  ListNode *stail = &smaller, *btail = &bigger;
	  //separete
	  while (head != NULL) {
		  if (head->val<x) {
			  stail = stail->next = head;//connect and shift            
		  }
		  else {
			  btail = btail->next = head;
		  }
		  head = head->next;
	  }
	  //connect
	  btail->next = NULL;
	  stail->next = bigger.next;
	  return smaller.next;
  }


  ///////////////////////////////////////////////////////////////////////////
  //82. Remove Duplicates from Sorted List II
  ListNode *deleteDuplicatesII_0(ListNode *head) {
	  ListNode fakeHead(0);
	  fakeHead.next = head;
	  ListNode* pre = &fakeHead;
	  ListNode* p = pre->next;
	  while (p) {
		  ListNode* pn = p->next;
		  if (pn && p->val == pn->val) {
			  // move pn to next different value
			  while (pn && p->val == pn->val) {
				  pn = pn->next;
			  }
			  p = pn;
			  pre->next = p;
		  }
		  else {
			  pre = p;
			  p = p->next;
		  }
	  }
	  return fakeHead.next;
  }

  //recur got easier code longer time
  ListNode* deleteDuplicatesII_1(ListNode* head) {
	  if (!head) return 0;
	  if (!head->next) return head;

	  int val = head->val;
	  ListNode* p = head->next;

	  if (p->val != val) {
		  head->next = deleteDuplicatesII_1(p);
		  return head;
	  }
	  else {
		  while (p && p->val == val) 
			  p = p->next;
		  return deleteDuplicatesII_1(p);
	  }
  }

  ///////////////////////////////////////////////////////////////////////////
  //328. Odd Even Linked List
  ListNode* oddEvenList_1(ListNode* head) {
	  if (head == NULL || head->next == NULL)
		  return head;

	  ListNode* Oed = head;
	  ListNode* Eed = head->next;
	  ListNode* cur = Eed->next;
	  bool odd = true;
	  while (cur) {
		  Eed->next = cur->next;
		  cur->next = Oed->next;
		  Oed = Oed->next = cur;
		  if (Eed->next)
			  Eed = Eed->next;
		  cur = Eed->next;
	  }
	  return head;
  }

  //easier to understand
  ListNode* oddEvenList_2(ListNode* head)
  {
	  if (!head) return head;
	  ListNode *odd = head, *evenhead = head->next, *even = evenhead;
	  while (even && even->next)
	  {
		  odd->next = odd->next->next;
		  even->next = even->next->next;
		  odd = odd->next;
		  even = even->next;
	  }
	  odd->next = evenhead;
	  return head;
  }

  ///////////////////////////////////////////////////////////////////////////
  //19. Remove Nth Node From End of List
  ListNode* removeNthFromEnd(ListNode* head, int n)
  {
	  ListNode** t1 = &head, *t2 = head;
	  for (int i = 1; i < n; ++i)//use two pointer to keep the gap
	  {
		  t2 = t2->next;
	  }
	  while (t2->next != NULL)
	  {
		  t1 = &((*t1)->next);
		  t2 = t2->next;
	  }
	  *t1 = (*t1)->next;//try use more pointer to pointer
	  return head;
  }


  ///////////////////////////////////////////////////////////////////////////
  //2. Add Two Numbers
  ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
	  int carry = 0;
	  ListNode dummy(0);
	  ListNode* tail = &dummy;
	  while (l1 || l2 || carry) {
		  int sum = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + carry;
		  carry = sum / 10;
		  tail = tail->next = new ListNode(sum % 10);//reduce copy can speed up
		  l1 = l1 ? l1->next : l1;
		  l2 = l2 ? l2->next : l2;
	  }
	  return dummy.next;
  }

  ///////////////////////////////////////////////////////////////////////////
  //24. Swap Nodes in Pairs
  ListNode* swapPairs_0(ListNode* head) {
		if (head == NULL || head->next == NULL)
			return head;

		ListNode* one = head;
		ListNode* two = one->next;

		one->next = two->next;
		two->next = one;		
		one->next = swapPairs_0(one->next);

		return two;
  }

  //pointer pointer 
  ListNode* swapPairs_1(ListNode* head) {
	  ListNode **pp = &head, *a, *b;
	  while ((a = *pp) && (b = a->next)) {
		  a->next = b->next;
		  b->next = a;
		  *pp = b;//replace what ever the origin pointer is
		  pp = &(a->next);//go to handle the next target pointer
	  }
	  return head;
  }

 void main() {
	 ListNode* nod=new ListNode(1);
	 ListNode* nod1=new ListNode(2);
	 ListNode* nod2=new ListNode(3);
	 ListNode* nod3 = new ListNode(4);
	 ListNode* nod4 = new ListNode(5);
	 ListNode* nod5 = new ListNode(6);
	 ListNode* nod6 = new ListNode(7);


	 nod->next = nod1;
	 nod1->next = nod2;
	 nod2->next = nod3;
	 //nod3->next = nod4;
	 nod4->next = nod5;
	 nod5->next = nod6;

	 swapPairs_1(nod);


	 delete nod;
	 delete nod1;
	 delete nod2;
	 delete nod3;
	 delete nod4;
	 delete nod5;

 }