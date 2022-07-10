# Remove Nth Node From End of List

~~~C++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        int count = 1;
        ListNode *p = head;
        while(p->next != NULL){
            p = p->next;
            count++;
        }
        if(count == 1) return NULL;
        if(count == n) return head->next;
        ListNode *q = head;
        p = head->next;
        while(count - n - 1 >0){ 
            q = p;
            p = p->next;
            count--;
        }
        q->next = p->next;
        return head;
    }
};
~~~

