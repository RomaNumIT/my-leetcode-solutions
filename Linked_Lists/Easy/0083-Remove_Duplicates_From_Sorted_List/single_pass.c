struct ListNode{
    int val;
    struct ListNode *next;
};

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//Функция для удаления дубликатов
struct ListNode* deleteDuplicates(struct ListNode* head) {
    //Если на входе пустой связанный лист
    if (head == NULL)
        return NULL;

    struct ListNode *curr = head;
    while (curr->next != NULL){
        //Если дубликат, то его очищаем и сдвигаем указатель на следующий элемент
        if (curr->val == curr->next->val){
            struct ListNode *tmp = curr->next;
            curr->next = curr->next->next;
            free(tmp);
        }
        //Если не дубликат, то просто сдвигаем указатель
        else
            curr = curr->next;
    }
    return head;
}

//Очистка связанного списка
void free_list(struct ListNode *list){
    while (list != NULL){
        struct ListNode *tmp = list;
        list = list->next;
        free(tmp);
    }
}

//Функция для чтения и заполнения связанного листа
struct ListNode *read_list(size_t len){
    struct ListNode *head = NULL, *tail = NULL;
    for (size_t i = 0; i < len; i++){
        //Получаем значение
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

        //Инициализируем ноду
        node->val = value;
        node->next = NULL;

        //Прикрепляем её к голове, если она пуста, иначе к хвосту
        if (head == NULL)
            head = node;
        else
            tail->next = node;
        tail = node; //Сдвигаемся к этой ноде
    }
    return head;
}

int main(void){
    //Получаем кол-во элементов в листе
    size_t len;
    if (scanf("%zu", &len) != 1){
        printf("bad input\n");
        return EXIT_FAILURE;
    }

    //Получаем связанный лист
    struct ListNode *list = read_list(len);
    if (len > 0 && list == NULL){
        printf("malloc failed\n");
        return EXIT_FAILURE;
    }

    //Убираем дубликаты из связанного лист
    struct ListNode *head = list;
    struct ListNode *result = deleteDuplicates(head);

    //Проверяем, что память правильно выделялась
    if (len > 0 && result == NULL){
        printf("malloc failed\n");
        return EXIT_FAILURE;
    }

    //Выводим на экран связанный лист
    head = result;
    printf("result: ");
    while(head != NULL){
        printf(" %d", head->val);
        head = head->next;
    }

    //Очищаем память
    free_list(result);

    //Успешно завершаем программу
    return EXIT_SUCCESS;
}