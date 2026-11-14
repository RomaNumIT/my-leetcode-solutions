struct ListNode{
    int val;
    struct ListNode *next;
};

//Функция для проверки, есть ли цикл в связанном списке с помощью Алгоритма Флойда
bool hasCycle(struct ListNode *head) {
    struct ListNode *hare = head, *turtle = head;
    while (hare != NULL && hare->next != NULL){
        hare = hare->next->next; //Делаем два шага для одного указателя
        turtle = turtle->next; //Делаем один шаг для другого
        //Нашли совпадение указателей, значит есть цикл
        if (hare == turtle)
            return true;
    }
    return false;
}