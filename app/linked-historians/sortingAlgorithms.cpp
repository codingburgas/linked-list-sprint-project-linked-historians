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

static EVENT* mergeSortedListsDescending(EVENT* l1, EVENT* l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    if (l1->title >= l2->title) { 
        l1->next = mergeSortedListsDescending(l1->next, l2);
        return l1;
    }
    else {
        l2->next = mergeSortedListsDescending(l1, l2->next);
        return l2;
    }
}

EVENT* sortEventsByTitleDescending(EVENT* head) {
    if (!head || !head->next) return head;

    EVENT* a;
    EVENT* b;
    splitList(head, &a, &b);
    a = sortEventsByTitleDescending(a);
    b = sortEventsByTitleDescending(b);
    return mergeSortedListsDescending(a, b);
}

static bool compareByDate(EVENT* e1, EVENT* e2) {
    if (e1->dateYear != e2->dateYear)
        return e1->dateYear < e2->dateYear;
    if (e1->dateMonth != e2->dateMonth)
        return e1->dateMonth < e2->dateMonth;
    return e1->dateDay < e2->dateDay;
}

static EVENT* mergeSortedListsByDate(EVENT* l1, EVENT* l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    if (compareByDate(l1, l2)) {
        l1->next = mergeSortedListsByDate(l1->next, l2);
        return l1;
    }
    else {
        l2->next = mergeSortedListsByDate(l1, l2->next);
        return l2;
    }
}



static EVENT* mergeSortByDate(EVENT* head) {
    if (!head || !head->next) {
        return head;
    }

    EVENT* a;
    EVENT* b;
    splitList(head, &a, &b);

    a = mergeSortByDate(a);
    b = mergeSortByDate(b);

    return mergeSortedListsByDate(a, b);
}

EVENT* sortEventsByDate(EVENT* head) {
    return mergeSortByDate(head);
}

static bool compareByDateDescending(EVENT* e1, EVENT* e2) {
    if (e1->dateYear != e2->dateYear)
        return e1->dateYear > e2->dateYear;
    if (e1->dateMonth != e2->dateMonth)
        return e1->dateMonth > e2->dateMonth;
    return e1->dateDay > e2->dateDay;
}

static EVENT* mergeSortedListsByDateDescending(EVENT* l1, EVENT* l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    if (compareByDateDescending(l1, l2)) {
        l1->next = mergeSortedListsByDateDescending(l1->next, l2);
        return l1;
    }
    else {
        l2->next = mergeSortedListsByDateDescending(l1, l2->next);
        return l2;
    }
}

EVENT* sortEventsByDateDescending(EVENT* head) {
    if (!head || !head->next) return head;

    EVENT* a;
    EVENT* b;
    splitList(head, &a, &b);
    a = sortEventsByDateDescending(a);
    b = sortEventsByDateDescending(b);
    return mergeSortedListsByDateDescending(a, b);
}
