#include "sortingAlgorithms.h"

static EVENT* mergeSortedLists(EVENT* l1, EVENT* l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    if (l1->title <= l2->title) {
        l1->next = mergeSortedLists(l1->next, l2);
        return l1;
    }
    else {
        l2->next = mergeSortedLists(l1, l2->next);
        return l2;
    }
}

static void splitList(EVENT* source, EVENT** front, EVENT** back) {
    EVENT* slow = source;
    EVENT* fast = source->next;
    while (fast != nullptr) {
        fast = fast->next;
        if (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *front = source;
    *back = slow->next;
    slow->next = nullptr;
}

static EVENT* mergeSort(EVENT* head) {
    if (!head || !head->next) {
        return head;
    }
    EVENT* a;
    EVENT* b;
    splitList(head, &a, &b);
    a = mergeSort(a);
    b = mergeSort(b);
    return mergeSortedLists(a, b);
}

EVENT* sortEventsByTitle(EVENT* head) {
    return mergeSort(head);
}