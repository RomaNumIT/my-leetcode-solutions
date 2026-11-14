struct ListNode {
    int val;
    struct ListNode *next;
};

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//Функция для нахождения первого коммита пересечения двух связных списков
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) {
    //Если одновременно дошли до конца списков, значит пересечений нет
    struct ListNode *pointerA = headA, *pointerB = headB; 
    while (pointerA != NULL || pointerB != NULL){
        //Если дошли до конца по одному из списков, то переходим в начало другого
        if (pointerA == NULL) pointerA = headB;
        if (pointerB == NULL) pointerB = headA;

        //Если указатели совпали - значит это и есть нода, от которой дальше два списка указывают на одни ноды
        if (pointerA == pointerB)
            return pointerA;

        //Если нет, то сдвигаемся
        pointerA = pointerA->next;
        pointerB = pointerB->next;
    }
    //Два списка не пересекаются
    return NULL;
}