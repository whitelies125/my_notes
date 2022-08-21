# 2. Add_Two_Numbers

```c
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){
    int sum = 0;
    struct ListNode* l3 = NULL;
    struct ListNode* q = NULL;
    while( l1 || l2 || sum )
    {
        if(l1)
        {
            sum += (l1->val);
            l1 = l1->next;
        }
        if(l2)
        {
            sum += (l2->val);
            l2 = l2->next;
        }
        struct ListNode* p = (struct ListNode*)malloc(sizeof(struct ListNode));
        if(!l3)
        {
            l3 = p;
            q = p;
        }
        p->val = sum % 10;
        sum /= 10;
        q->next = p;
        p->next = NULL;
        q = p;
    }
    return l3;
}
```

