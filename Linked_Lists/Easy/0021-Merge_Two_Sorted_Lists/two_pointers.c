//Структура одной ноды
struct ListNode {
    int val;
    struct ListNode *next;
};

#include <stdio.h>
#include <stdlib.h>

//Очищаем память, выделенную под данный связанный лист
void free_list(struct ListNode *head){
    while(head != NULL){
        struct ListNode *tmp = head;
        head = head->next;
        free(tmp);
    }
}

//Функция для смены местами двух указателей
void swap(struct ListNode **l1, struct ListNode **l2){
    struct ListNode *tmp = *l1;
    *l1 = *l2;
    *l2 = tmp;
} 

//Функция для склеивания двух листов в один отсортированный
struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
    struct ListNode *head = NULL, *tail = NULL;
    while (list1 != NULL || list2 != NULL){
        //Меняем местами указатели на ноды, чтобы всегда работать с list1
        if (list2 != NULL && list1 != NULL){
            if (list2->val < list1->val)
                swap(&list1, &list2);
        }

        //Если первый лист кончился, то меняем со вторым
        if (list1 == NULL && list2 != NULL)
            swap(&list1, &list2);

        struct ListNode *tmp = list1; //Сохраняем текущую ноду
        list1 = list1->next; //Сдвигаем указатель на новую ноду
        //Инициализируем эту ноду
        tmp->next = NULL;

        //Если head пустой
        if (head == NULL)
            head = tmp;
        //Иначе прикрепляем к хвосту
        else
            tail->next = tmp;
        tail = tmp; //Сдвигаемся на новую ноду
    }
    return head;
}

//Функция для чтения чисел с stdin с записью в связанный список
struct ListNode *read_list(size_t len){
    struct ListNode *head = NULL, *tail = NULL;
    for (size_t i = 0; i < len; i++){
        //Получем число с stdin
        int value;
        if (scanf("%d", &value) != 1){
            free_list(head);
            return NULL;
        }

        //Выделяем память под новую ноду
        struct ListNode *node = malloc(sizeof(*node));
        if (node == NULL){
            free_list(head);
            return NULL;
        }

        //Инициализируем её
        node->val = value;
        node->next = NULL;

        //Если голова пуста, то добавляем в неё
        if (head == NULL)
            head = node;
        //Иначе добавляем к хвосту
        else
            tail->next = node;
        tail = node; //Сдвигаем на эту ноду
    }
    return head;
}

int main(void){
    //Получаем длины двух связанных списков
    size_t len1, len2;
    if (scanf("%zu %zu", &len1, &len2) != 2){
        printf("bad input\n");
        return EXIT_FAILURE;
    }

    //Получаем первый связанный список
    struct ListNode *l1 = read_list(len1);
    if (len1 > 0 && l1 == NULL){
        printf("bad input\n");
        return EXIT_FAILURE;
    }

    //Получаем второй связанный список
    struct ListNode *l2 = read_list(len2);
    if (len2 > 0 && l2 == NULL){
        printf("bad input\n");
        return EXIT_FAILURE;
    }

    //Группируем эти два списка в один отсортированный список
    struct ListNode *temp1 = l1, *temp2 = l2;
    struct ListNode *result = mergeTwoLists(temp1, temp2);

    if (result == NULL){
        free_list(l1); free_list(l2);
        printf("malloc failed");
        return EXIT_FAILURE;
    }
    //Выводим его на экран
    struct ListNode *temp = result;
    printf("result: ");
    while (temp != NULL){
        printf(" %d", temp->val);
        temp = temp->next;
    }

    //Освобождение памяти
    free_list(l1); free_list(l2);
    //Успешное завершение программы
    return EXIT_SUCCESS;
}