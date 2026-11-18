#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//Структура бинарного дерева
struct TreeNode{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// Структура для стека
struct Stack {
  struct TreeNode *node;
  struct Stack *next;
};

// Алиас для struct Stack
typedef struct Stack Stack;

// Результаты работы функций, работающий со стеком
typedef enum {
  OK = 1,
  STACK_IS_EMPTY = 0,
  OUT_OF_MEMORY = 2,
  VECTOR_IS_EMPTY = 3,
  NOT_EXIST = 4,
} Status;

// Инициализация стека
Stack *init_stack(void) { return NULL; }

// Функция для очищения памяти стека
void free_stack(Stack *p) {
  // Очищаем весь лист вправо от этой ноды
  while (p != NULL) {
    Stack *temp = p;
    p = p->next;
    free(temp);
  }
}

// Добавление элемента в стек
Status push_front_stack(Stack **p, struct TreeNode *node) {
  Stack *new_node = malloc(sizeof(*new_node));
  if (new_node == NULL)
    return OUT_OF_MEMORY;
  new_node->node = node;
  new_node->next = *p;
  *p = new_node;
  return OK;
}

// Проверка, что стек не пустой
bool is_empty_stack(const Stack *const p) { return p == NULL; }

// Вытаскивание одного элемента из стека
Status pop_front_stack(Stack **p, struct TreeNode **out) {
  if (is_empty_stack(*p))
    return STACK_IS_EMPTY;
  *out = (*p)->node;
  Stack *temp = *p;
  *p = (*p)->next;
  free(temp);
  return OK;
}

//Структура вектора
struct Vector{
    int *data;
    size_t size;
    size_t count;
};

//Инициализация вектора
Status init_vector(struct Vector *v){
    if (v == NULL)
        return NOT_EXIST;
    int *data = malloc(0);
    v->count = 0;
    v->size = 0;
    v->data = data;
    return OK;
}

//Очистка памяти вектора
void free_vector(struct Vector *v){
    if (v == NULL)
        return;
    free(v->data);
    v->data = NULL;
    v->count = 0;
    v->size = 0;
}

//Расширение вектора, если необходимо
Status expansion_vector_if_necessary(struct Vector *v){
    //Указатель не указывает на вектор
    if (v == NULL)
        return NOT_EXIST;
    //Памяти достаточно для нового элемента
    if (v->count < v->size)
        return OK;
    //Проверяем, что он не нулевой длины
    if (v->size == 0)
        v->size = 1;
    //Расширяем память
    int *data = realloc(v->data, v->size * 2 * sizeof(int));
    if (data == NULL)
        return OUT_OF_MEMORY;
    v->data = data;
    v->size = v->size * 2;
    return OK;
}

//Добавление элемента в вектор
Status push_back_vector(struct Vector *v, int val){
    //Проверяем необходимость увеличения вектора и его корректность
    Status status = expansion_vector_if_necessary(v);
    if (status != OK)
        return status;
    //Добавляем элемент на нужную позицию
    v->data[v->count++] = val;
    return OK;
}

//Функция для возврата значений бинарного дерева
int* inorderTraversal(struct TreeNode* root, int* returnSize) {
    //Пустой указатель на кол-во возвращённых элементов
    if (returnSize == NULL)
        return NULL;

    //Дерево пустое
    if (root == NULL){
        *returnSize = 0;
        return NULL;
    }

    //Инициализируем стек структур TreeNode, и указатель на текущую ноду, указатель на вектор результата
    Stack *stack = init_stack();
    struct TreeNode *curr = root;
    struct Vector result;
    if (init_vector(&result) != OK){
        *returnSize = 0;
        free_stack(stack);
        return NULL;
    }

    //Пока есть узлы для обработки в curr или в стеке
    while (curr != NULL || !is_empty_stack(stack)) {
        // Спускаемся до самого левого узла
        while (curr != NULL) {
            if (push_front_stack(&stack, curr) != OK) {
                free_stack(stack);
                free_vector(&result);
                return NULL;
            }
            curr = curr->left;
        }

        //Извлекаем узел из стека
        if (pop_front_stack(&stack, &curr) != OK) {
            break; // Стек пуст (не должно случиться из-за условия цикла)
        }

        //Добавляем его значение в результат
        if (push_back_vector(&result, curr->val) != OK) {
            free_stack(stack);
            free_vector(&result);
            return NULL;
        }

        // Идем в правую ветку
        curr = curr->right;
    }

    //Сохраняем кол-во заполненных значений, очищаем стек, выводим результат
    *returnSize = result.count;
    free_stack(stack);
    return result.data;
}

//Прототип
Status create_binary_tree(struct TreeNode *curr, int *arr, size_t count, size_t i);

//Функция для добавляния ребёнка к текущей ноде
Status create_new_tree_node(struct TreeNode *curr, int *arr, size_t count, size_t j, bool is_right){
    //Нет дерева или массива
    if (curr == NULL || arr == NULL)
        return NOT_EXIST;

    //Если элемент за массивом или со значение 0, значит эта пустая ветка (для удобства создания бинарного дерева)
    if (j >= count || arr[j] == 0){
        if (is_right)
            curr->right = NULL;
        else
            curr->left = NULL;
        return OK;
    }

    //Иначе создаём ноду
    struct TreeNode *node = malloc(sizeof(*node));
    if (node == NULL)
        return OUT_OF_MEMORY;
    node->left = NULL;
    node->right = NULL;
    node->val = arr[j];

    //Прикрепляем к текущей
    if (is_right)
        curr->right = node;
    else
        curr->left = node;

    //Заполняем всё дерево от добавленной ноды
    Status status = create_binary_tree(node, arr, count, j);
    if (status != OK)
        return status;
    return OK;
}

//Функция для создания бинарного дерева из массива int. i - индекс элемента в массиве
Status create_binary_tree(struct TreeNode *curr, int *arr, size_t count, size_t i){
    //Пустая нода, нечего к ней прикреплять
    if (curr == NULL)
        return NOT_EXIST;
    //Массива нет
    if (arr == NULL)
        return NOT_EXIST;

    //Разбираемся с левой веткой
    size_t l = 2 * i + 1;
    Status status = create_new_tree_node(curr, arr, count, l, false);
    if (status != OK)
        return status;

    //Разбираемся с правой веткой
    size_t r = 2 * i + 2;
    status = create_new_tree_node(curr, arr, count, r, true);
    if (status != OK)
        return status;
    return OK;
}

//Функция для вывода всех нод на определённой глубины
void print_one_tree_level(struct TreeNode *curr, size_t curr_depth, size_t max_depth){
    //Дошли до конца ветки
    if (curr == NULL){
        //Значит это второстепенная ветка, выводим 0, чтобы массив был корректный
        if (curr_depth <= max_depth)
            printf(" 0");
        return;
    }

    //Если добрались до нужной глубины, то выводим на экран весь уровень
    if (curr_depth == max_depth){
        printf(" %d", curr->val);
        return;
    }

    //Ищим дальше слева и справа ноды на нужном уровне
    print_one_tree_level(curr->left, curr_depth + 1, max_depth);
    print_one_tree_level(curr->right, curr_depth + 1, max_depth);
}

//Функция для определения глубины дерева
size_t len_tree(struct TreeNode *curr){
    //Если дошли до дна дерева, то возвращаем 0
    if (curr == NULL) return 0;
    //Находим глубину провой и нижней ветки от текущей ноды
    size_t l = len_tree(curr->left);
    size_t r = len_tree(curr->right);
    //Возвращаем максимальную глубину
    return 1 + ((l >= r)? l:r);
}

//Функция для вывода на экран дерева
void print_tree(struct TreeNode *head){
    size_t max_depth = len_tree(head);
    for (size_t depth = 0; depth < max_depth; depth++)
        print_one_tree_level(head, 0, depth);
}

//Очищение бинарного дерева
void free_tree(struct TreeNode *head){
    //Если нода пуста, значит ниже дерево не существует
    if (head == NULL)
        return;
    //Запускаем очистку левой и правой ветки от этой ноды
    free_tree(head->left);
    free_tree(head->right);

    //Очищаем текущую ноду
    free(head);
}

//Основная функция для ввода данных
//Вводить не 1 null 2 3, а 1 0 2 0 0 3
int main(void){
    //Получаем кол-во элементов в дереве
    size_t count;
    if (scanf("%zu", &count) != 1){
        printf("bad input\n");
        return EXIT_FAILURE;
    }

    //Нет элементов в дереве, нечего выводить
    if (count == 0){
        printf("bad input\n");
        return EXIT_FAILURE;
    }

    //Создаём массив для элементов дерева
    int *arr = malloc(sizeof(*arr) * count);
    if (arr == NULL){
        printf("malloc failed\n");
        return EXIT_FAILURE;
    }
    
    //Получаем данные с stdin
    for (size_t i = 0; i < count; i++){
        if (scanf("%d", &arr[i]) != 1){
            free(arr);
            printf("bad input\n");
            return EXIT_FAILURE;
        }
    }

    //Создаём указатель на первую ноду в дереве
    struct TreeNode *head = malloc(sizeof(*head));
    if (head == NULL){
        free(arr);
        printf("bad input\n");
        return EXIT_FAILURE;
    }
    //Заполняем значение первой ноды
    head->left = NULL;
    head->right = NULL;
    head->val = arr[0];

    //Создаём дерево
    if (create_binary_tree(head, arr, count, 0) != OK){
        free(arr);
        free_tree(head);
        printf("bad input\n");
        return EXIT_FAILURE;
    }

    //Выводим дерево на экран:
    printf("result:");
    print_tree(head);
    printf("\n");

    //Получаем массив обхода бинарного дерева
    int returnSize = 0;
    int* result = inorderTraversal(head, &returnSize);
    //Если вывод не пустой, то выводим обход на экран
    if (result != NULL){
        printf("inorder traversal:");
        if (returnSize > 0)
            for (size_t i = 0; i < returnSize; i++)
                printf(" %d", result[i]);
        free(result);
    }
    
    //Очищаем память дерева и вектора
    free_tree(head);
}