#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    element_t *entry, *safe;
    list_for_each_entry_safe(entry, safe, head, list) {
        free(entry->value);
        free(entry);
    }
    free(head);

    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *ele = malloc(sizeof(element_t));
    if (!ele)
        return false;

    ele->value = strdup(s);
    if (!ele->value) {
        free(ele);
        return false;
    }

    list_add(&ele->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    return q_insert_head(head->prev, s);
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *ele = list_entry(head->next, element_t, list);

    if (sp) {
        size_t n = strlen(ele->value);
        if (bufsize - 1 < n)
            n = bufsize - 1;
        strncpy(sp, ele->value, n);
        sp[n] = '\0';
    }

    list_del_init(&ele->list);

    return ele;
}
/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;

    return q_remove_head(head->prev->prev, sp, bufsize);
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each(li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *L = head->next, *R = head->prev;
    while (L != R && L->next != R) {
        L = L->next;
        R = R->prev;
    }
    element_t *mid = list_entry(L, element_t, list);

    list_del_init(&mid->list);
    free(mid->value);
    free(mid);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    element_t *L, *R;
    bool has_dup = false;

    list_for_each_entry_safe(L, R, head, list) {
        if (&R->list != head && !strcmp(L->value, R->value)) {
            has_dup = true;
            list_del_init(&L->list);
            free(L->value);
            free(L);
        } else if (has_dup) {
            has_dup = false;
            list_del_init(&L->list);
            free(L->value);
            free(L);
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *L = head->next, *R;

    while (L != head && L->next != head) {
        R = L->next;
        L->next = R->next;
        R->next = L;
        R->prev = L->prev;
        L->prev = R;
        L->next->prev = L;
        R->prev->next = R;
        L = L->next;
    }

    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *L, *R;

    list_for_each_safe(L, R, head) {
        L->next = L->prev;
        L->prev = R;
    }

    head->next = head->prev;
    head->prev = R;

    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head) || list_is_singular(head) || k == 1)
        return;

    struct list_head *cur = head->next, *ghead = NULL, *gtail = NULL;
    struct list_head *ptail = head;

    while (cur != head) {
        struct list_head *begin = cur;
        int count = 0;

        while (cur != head) {
            count++;
            cur = cur->next;
            if (count == k)
                break;
        }

        if (count != k)
            break;

        ghead = begin;
        gtail = cur->prev;

        struct list_head *temp, *it = ghead;

        while (it != cur) {
            temp = it->next;
            it->next = it->prev;
            it->prev = temp;
            it = temp;
        }

        ptail->next = gtail;
        gtail->prev = ptail;

        ghead->next = cur;
        cur->prev = ghead;

        ptail = ghead;
    }


    return;
}


struct list_head *q_mergelists(struct list_head *list1, struct list_head *list2)
{
    if (!list1)
        return list2;

    if (!list2)
        return list1;

    const element_t *l = list_entry(list1, element_t, list);
    const element_t *r = list_entry(list2, element_t, list);

    struct list_head *sorted = NULL;

    if (strcmp(l->value, r->value) < 0) {
        sorted = list1;
        sorted->prev = NULL;
        sorted->next = q_mergelists(list1->next, list2);
        if (sorted->next)
            sorted->next->prev = sorted;
    } else {
        sorted = list2;
        sorted->prev = NULL;
        sorted->next = q_mergelists(list1, list2->next);
        if (sorted->next)
            sorted->next->prev = sorted;
    }
    return sorted;
}


struct list_head *q_mergesort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return head;

    struct list_head *slow = head, *fast = head;

    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    struct list_head *list2 = slow->next;

    slow->next = NULL;
    list2->prev = NULL;

    struct list_head *left = q_mergesort(head), *right = q_mergesort(list2);

    return q_mergelists(left, right);
}


/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *listh = head->next, *listt = head->prev;

    listh->prev = NULL;
    listt->next = NULL;

    listh = q_mergesort(listh);
    listt = listh;

    while (listt->next) {
        listt = listt->next;
    }

    head->next = listh;
    head->prev = listt;
    listh->prev = head;
    listt->next = head;

    if (descend)
        q_reverse(head);
}


/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    if (list_is_singular(head))
        return 1;

    /* cppcheck-suppress constVariablePointer */
    struct list_head *tail = head->prev, *li = head->prev;

    int count = 0;

    while (li != head) {
        struct list_head *prev = li->prev;
        if (strcmp(list_entry(li, element_t, list)->value,
                   list_entry(tail, element_t, list)->value) > 0) {
            list_del_init(li);
            element_t *ele = list_entry(li, element_t, list);
            if (ele->value)
                free(ele->value);
            free(li);
            //           free(li);
        } else {
            tail = li;
            count++;
        }
        li = prev;
    }

    return count;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    if (list_is_singular(head))
        return 1;

    /* cppcheck-suppress constVariablePointer */
    struct list_head *tail = head->prev, *li = head->prev;

    int count = 0;

    while (li != head) {
        struct list_head *prev = li->prev;
        if (strcmp(list_entry(li, element_t, list)->value,
                   list_entry(tail, element_t, list)->value) < 0) {
            list_del_init(li);
        } else {
            tail = li;
            count++;
        }
        li = prev;
    }

    //   q_reverse(head);
    return count;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return 0;

    queue_contex_t *merged = list_entry(head->next, queue_contex_t, chain);
    struct list_head *merged_queue = merged->q;

    if (list_is_singular(head))
        return merged->size;

    struct list_head *cur_chain = head->next->next;

    while (cur_chain != head) {
        queue_contex_t *cur_ctx = list_entry(cur_chain, queue_contex_t, chain);
        struct list_head *cur_queue = cur_ctx->q;
        if (!list_empty(cur_queue)) {
            list_splice_tail(cur_queue, merged_queue);
            merged->size += cur_ctx->size;
            cur_ctx->size = 0;
            INIT_LIST_HEAD(cur_queue);
        }
        cur_chain = cur_chain->next;
    }

    q_sort(merged_queue, descend);

    return merged->size;



    return 0;
}
