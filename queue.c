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
    return q_reverseK(head, 2);
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

    LIST_HEAD(rev_list);
    LIST_HEAD(new_head);
    struct list_head *cut;
    int group = q_size(head) / k;

    while (group--) {
        int count = 0;
        list_for_each(cut, head) {
            count++;
            if (count == k)
                break;
        }
        list_cut_position(&rev_list, head, cut);
        q_reverse(&rev_list);
        list_splice_tail_init(&rev_list, &new_head);
    }
    list_splice_init(&new_head, head);

    return;
}


struct list_head *q_mergelists(struct list_head *list1, struct list_head *list2)
{
    struct list_head *new_head = NULL, **indirect = &new_head;

    while (true) {
        if (!list1) {
            *indirect = list2;
            break;
        }
        if (!list2) {
            *indirect = list1;
            break;
        }
        const char *str1 = list_entry(list1, element_t, list)->value,
                   *str2 = list_entry(list2, element_t, list)->value;
        if (strcmp(str1, str2) < 0) {
            *indirect = list1;
            list1 = list1->next;
        } else {
            *indirect = list2;
            list2 = list2->next;
        }
        indirect = &(*indirect)->next;
    }
    return new_head;
}



/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *stack[32], *node, *safe;
    unsigned int size[32];

    int it = 0;

    list_for_each_safe(node, safe, head) {
        node->next = NULL;
        stack[it++] = node;
        size[it - 1] = 1;
        while ((it > 1) && (size[it - 1] == size[it - 2])) {
            stack[it - 2] = q_mergelists(stack[it - 1], stack[it - 2]);
            size[it - 2] *= 2;
            it--;
        }
    }

    it--;
    while (it >= 1) {
        stack[it - 1] = q_mergelists(stack[it], stack[it - 1]);
        it--;
    }

    INIT_LIST_HEAD(head);
    node = stack[0];
    while (node) {
        safe = node->next;
        list_add_tail(node, head);
        node = safe;
    }

    if (descend) {
        q_reverse(head);
    }

    return;
}


/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    if (list_is_singular(head))
        return 1;

    struct list_head *R = head->prev, *L = head->prev->prev;
    element_t *delete;

    while (L != head) {
        if (strcmp(list_entry(R, element_t, list)->value,
                   list_entry(L, element_t, list)->value) > 0) {
            L = L->prev;
            R = R->prev;
        } else {
            delete = list_entry(L, element_t, list);
            list_del_init(L);
            free(delete->value);
            free(delete);
            L = R->prev;
        }
    }

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    if (list_is_singular(head))
        return 1;

    struct list_head *R = head->prev, *L = head->prev->prev;
    element_t *delete;

    while (L != head) {
        if (strcmp(list_entry(R, element_t, list)->value,
                   list_entry(L, element_t, list)->value) < 0) {
            L = L->prev;
            R = R->prev;
        } else {
            delete = list_entry(L, element_t, list);
            list_del_init(L);
            free(delete->value);
            free(delete);
            L = R->prev;
        }
    }

    return q_size(head);
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
