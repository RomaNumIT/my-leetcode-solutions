struct ListNode{
    int val;
    struct ListNode *next;
};

#include <stdint.h>
#include <stdlib.h>

//Функция удаляющая промежуточную ноду
void removeListNode(struct ListNode *pointer){
    if (pointer == NULL || pointer->next == NULL)
        return;
    struct ListNode *temp = pointer->next;
    pointer->next = pointer->next->next;
    free(temp);
}

//Функция удаляющая из связанного списка все ноды, с определённым значением
struct ListNode* removeElements(struct ListNode* head, int val) {
    //Если уже список пустой, то нечего проверять
    if (head == NULL)
        return head;
    //Сдвигаем указатель на начало списка, пока элементы в начале совпадают с val
    while (head != NULL && head->val == val )
        head = head->next;
    //После пробегаем оставщуюся часть списка удаляя ноды с искомым значением
    struct ListNode *pointer = head;
    while (pointer != NULL && pointer->next != NULL){
        if (pointer->next->val != val){
            pointer = pointer->next;
            continue;
        }
        removeListNode(pointer);
    }
    return head; //Возвращаем указатель на изменённый список
}