#include <stddef.h> //NULL
#include <stack>
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

 void main() {
	 ListNode* nod=new ListNode(0);
	 ListNode* nod1=new ListNode(1);
	 ListNode* nod2=new ListNode(2);


	 ListNode nod0(1);
	 ListNode nod02(1);


	 reverseList(nod);
	 delete nod;
	 delete nod1;
	 delete nod2;

 }