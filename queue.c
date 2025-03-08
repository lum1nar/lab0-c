#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));

    INIT_LIST_HEAD(head);

    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    element_t *entry = NULL, *safe = NULL;

    list_for_each_entry_safe (entry, safe, head, list) {
        free(entry->value);
        list_del_init(&entry->list);
        free(entry);
    }

    free(head);
    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *ele = (element_t *) malloc(sizeof(element_t));

    ele->value = malloc(strlen(s) + 1);
    memcpy(ele->value, s, strlen(s) + 1);

    list_add(&ele->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *ele = (element_t *) malloc(sizeof(element_t));

    ele->value = malloc(strlen(s) + 1);
    memcpy(ele->value, s, strlen(s) + 1);

    list_add_tail(&ele->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    element_t *ele = list_entry(head->next, element_t, list);

    if (sp && ele->value)
        memcpy(sp, ele->value, bufsize);

    list_del_init(&ele->list);

    return ele;
}
/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    element_t *ele = list_entry(head->prev, element_t, list);

    if (sp && ele->value)
        memcpy(sp, ele->value, bufsize);

    list_del_init(&ele->list);

    return ele;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *fast = head->next, *slow;

    list_for_each (slow, head) {
        if (fast->next == head || fast == head)
            break;
        fast = fast->next->next;
    }

    list_del_init(slow);

    element_t *ele = list_entry(slow, element_t, list);

    if (ele->value)
        free(ele->value);

    free(ele);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *slow, *fast;

    for (slow = head->next, fast = slow->next; slow != head;
         slow = fast, fast = slow->next) {
        fast = slow->next;

        while ((fast) != head &&
               !strcmp(list_entry(fast, element_t, list)->value,
                       list_entry(slow, element_t, list)->value))
            fast = fast->next;

        if (slow->next == fast)
            continue;


        for (struct list_head *tmp = slow, *next = slow->next; tmp != fast;
             tmp = next, next = tmp->next) {
            element_t *ele = list_entry(tmp, element_t, list);

            list_del_init(&ele->list);

            if (ele->value)
                free(ele->value);

            free(ele);
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *li = head->next;

    while (li != head && li->next != head) {
        struct list_head *next = li->next;
        li->next = next->next;
        li->next->prev = li;
        next->next = li;
        next->prev = li->prev;
        li->prev->next = next;
        li->prev = next;

        li = li->next;
    }

    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *cur = head->next, *next = NULL;

    while (cur != head) {
        next = cur->next;
        cur->next = cur->prev;
        cur->prev = next;
        cur = next;
    }

    next = head->next;
    head->next = head->prev;
    head->prev = next;

    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
