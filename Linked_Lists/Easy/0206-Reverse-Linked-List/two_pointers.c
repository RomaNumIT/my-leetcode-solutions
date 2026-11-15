struct ListNode{
    int val;
    struct ListNode *next;
};

#include <stdlib.h>

//Функция для разворота связанного списка
struct ListNode* reverseList(struct ListNode* head) {
    //Либо нет Нод, либо одна, значит нечего разворачивать
    if (head == NULL || head->next == NULL)
        return head;
    
    //Два указателя на предыдущий и текущий
    struct ListNode *previous = head, *curr = head->next;
    previous->next = NULL; //Теперь он является последней нодой
    while (curr != NULL){
        struct ListNode *temp = curr->next;
        curr->next = previous;
        previous = curr;
        curr = temp;
    }
    return previous;
}