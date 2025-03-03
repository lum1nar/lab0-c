#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));

    head->prev = head;
    head->next = head;

    return head;

    return NULL;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;



    if (head->next == head) {
        free(head);
        return;
    }

    element_t *entry = NULL, *safe = NULL;

    list_for_each_entry_safe (entry, safe, head, list) {
        if (entry->value)
            free(entry->value);

        struct list_head *next = (&entry->list)->next;
        struct list_head *prev = (&entry->list)->prev;

        next->prev = prev;
        prev->next = next;

        (&entry->list)->next = NULL;
        (&entry->list)->prev = NULL;

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

    struct list_head *next = head->next, *node = &ele->list;

    next->prev = node;
    node->next = next;
    node->prev = head;
    head->next = node;

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *ele = (element_t *) malloc(sizeof(element_t));


    ele->value = malloc(strlen(s) + 1);
    memcpy(ele->value, s, strlen(s) + 1);

    struct list_head *prev = head->prev, *node = &ele->list;

    prev->next = node;
    node->prev = prev;
    head->prev = node;
    node->next = head;

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
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
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

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
