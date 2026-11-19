#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//Структура для координаты ячейки в таблице
typedef struct{
    int row;
    int col;
} Point;

// Структура для стека, хранящего точки
struct Stack {
  Point point;
  struct Stack *next;
};

// Алиас для struct Stack
typedef struct Stack Stack;

// Результаты работы функций, работающий со стеком
typedef enum {
  OK = 0,
  ERR_MEM = 1,
  ERR_INPUT = 2,
  ERR_OUTPUT = 3,
  ERR_NULL = 4,
  ERR_OVERFLOW = 5,
  ERR_EMPTY = 6,
} Status;

// Инициализация стека
Stack *init_stack(void) { return NULL; }

// Функция для очищения памяти стека
void free_stack(Stack *p) {
  // Очищаем весь стек вправо от этой ноды
  while (p != NULL) {
    Stack *temp = p;
    p = p->next;
    free(temp);
  }
}

// Добавление элемента в стек
Status push_front_stack(Stack **p, Point point) {
    //Пустой указатель
    if (p == NULL)
        return ERR_NULL;

    //Создаём новую ноду, добавляем в неё нашу точку
    Stack *node = malloc(sizeof(*node));
    if (node == NULL)
        return ERR_MEM;
    node->point = point;
    node->next = *p;
    *p = node;
    return OK;
}

// Проверка, что стек не пустой
bool is_empty_stack(const Stack *const p) { return p == NULL; }

// Вытаскивание одного элемента из стека
Status pop_front_stack(Stack **p, Point *point) {
  if (is_empty_stack(*p))
    return ERR_EMPTY;
  *point = (*p)->point;
  Stack *temp = *p;
  *p = (*p)->next;
  free(temp);
  return OK;
}

//Структура для таблицы
typedef struct {
    char **data; //Указатель на саму таблицу
    size_t rows; //Кол-во строк
    size_t cols; //Кол-во столбцов
} Field;

//Инициализация таблицы
Status init_field(Field *field, const size_t M, const size_t N){
    //Битый указатель
    if (field == NULL)
        return ERR_NULL;
    
    //Для защиты, если не выделиться память, то чтобы не использовали эту таблицу
    field->data = NULL;
    field->rows = 0;
    field->cols = 0;

    //Выдыляем память под указатель на массив M указателей на массивы N char
    char **data = calloc(M, sizeof(*data));
    if (data == NULL)
        return ERR_MEM;

    //Выделяем память под массивы на char
    for (size_t i = 0; i < M; i++){
        data[i] = calloc(N, sizeof(**data));
        //Если не удалось выделить память, очищаем всё, что выделилось
        if (data[i] == NULL){
            for (size_t j = i; j-- > 0;)
                free(data[j]);
            free(data);
            return ERR_MEM;
        }
    }

    //Если всё удачно выделилось, то обновляем поля field
    field->data = data;
    field->rows = M;
    field->cols = N;
    return OK;
}

//Заполнение из in.txt таблицы
Status read_field(Field *field, FILE *fin){
    //Пустой указатель
    if (field == NULL || fin == NULL)
        return ERR_NULL;
    //Читаем значение таблицы из in.txt
    for (size_t i = 0; i < field->rows; i++)
        for (size_t j = 0; j < field->cols; j++)
            if (fscanf(fin, " %c", &(field->data[i][j])) != 1)
                return ERR_INPUT;
    return OK;
}

//Очищение таблицы
void free_field(Field *field){
    //Пустой указатель - нечего очищать
    if (field == NULL)
        return;
    
    //Очищаем данные в таблице
    if (field->data != NULL){
        for (size_t i = 0; i < field->rows; i++)
            if (field->data[i] != NULL)
                free(field->data[i]);
        free(field->data);
    }

    //Устанавливаем поля для защиты
    field->data = NULL;
    field->rows = 0;
    field->cols = 0;
}

//Функции для определения точки слева
Point left_point(Point p){
    p.col--;
    return p;
}

//Функции для определения точки спрва
Point right_point(Point p){
    p.col++;
    return p;
}

//Функции для определения точки сверху
Point top_point(Point p){
    p.row--;
    return p;
}

//Функции для определения точки снизу
Point bottom_point(Point p){
    p.row++;
    return p;
}

//Добавляем в стек всех соседей текущей точки
Status push_neighbors(Stack **stack, Point p){
    Status status = push_front_stack(stack, left_point(p));
    if (status != OK)
        return status;
    status = push_front_stack(stack, right_point(p));
    if (status != OK)
        return status;
    status = push_front_stack(stack, top_point(p));
    if (status != OK)
        return status;
    status = push_front_stack(stack, bottom_point(p));
    if (status != OK)
        return status;
    return OK;
}

//Проверка, что точка внутри таблицы
bool is_inside_cell(Point point, const Field *field){
    return point.col >= 0 && point.row >= 0 && (size_t)point.row < field->rows && (size_t)point.col < field->cols;
}

//Проверяем дошли ли до конца
bool is_found(Point point, const Field *field){
    //Проверяем, что точка не вышла за таблицу
    if (!is_inside_cell(point, field))
        return false;
    return field->data[point.row][point.col] == 'e';
}

//Проверяем, что ячейка не валидная
bool is_bad_cell(Point point, const Field *field, Field *visited){
    if (!is_inside_cell(point, field))
        return true;
    return visited->data[point.row][point.col] || field->data[point.row][point.col] == 'w';
}

//Покрашиваем текущую ячейку
void paint_cell(Point point, Field *visited){
    //Точка за таблицей
    if (!is_inside_cell(point, visited))
        return;
    visited->data[point.row][point.col] = 1;
}

//Функция для покраски всех стен в таблице
Status mark_walls(Field *field, Field *visited){
    //Если указатели битые
    if (field == NULL || visited == NULL)
        return ERR_NULL;
    //Проверяем, что таблица такого же размера, что и таблица для отметки
    if (field->rows != visited->rows || field->cols != visited->cols)
        return ERR_INPUT;
    //Обходим всю таблицу и ищим стены, подкрашивая их
    for (size_t i = 0; i < field->rows; i++)
        for (size_t j = 0; j < field->cols; j++)
            if (field->data[i][j] == 'w')
                visited->data[i][j] = 1;
    return OK;
}

//Функция, строящая путь из s в e
Status seek_path(Point *p, Field *field, bool *out){
    //Поданы пустые указатели
    if (p == NULL || field == NULL || out == NULL)  
        return ERR_NULL;

    //Проверяем, что данные корректные
    if (field->data == NULL || field->rows == 0 || field->cols == 0)
        return ERR_INPUT;

    //Инициализируем стек
    Stack *stack = init_stack();    

    //Инициализируем таблицу для отметки, где уже были
    Field visited;
    Status status = init_field(&visited, field->rows, field->cols);
    if (status != OK){
        free_stack(stack);
        return status;
    }

    //Сразу отметим все стены
    status = mark_walls(field, &visited);
    if (status != OK){
        free_field(&visited);
        free_stack(stack);
        return status;
    }

    //Добавляем текущую точку сразу в стек
    push_front_stack(&stack, *p); 

    //Пока стек не пустой, производим поиск в глубину
    while (!is_empty_stack(stack)){
        //Достаём элемент из стека
        Point curr;
        pop_front_stack(&stack, &curr);

        //Если ячейка не валидная, то продолжаем
        if (is_bad_cell(curr, field, &visited))
            continue;

        //Проверяем, что ячейка искомая
        if (is_found(curr, field)){
            free_field(&visited);
            free_stack(stack);
            *out = true;
            return OK;
        }
        
        //Подкрашиваем, что уже в ней были
        paint_cell(curr, &visited);
        
        //Добавляем в стек всех её соседей
        status = push_neighbors(&stack, curr);
        if (status != OK){
            free_field(&visited);
            free_stack(stack);
            return status;
        }
    }

    //Не нашли путь. Очищаем память
    free_stack(stack);
    free_field(&visited);
    *out = false;
    return OK;
}

//Функция, ищущая начало
Status seek_start(Field *field, Point *start){
    //Пустой указатель
    if (field == NULL || start == NULL)
        return ERR_NULL;

    //Перебираем все элементы в таблице и ищим "s":
    for (size_t i = 0; i < field->rows; i++)
        for (size_t j = 0; j < field->cols; j++)
            if (field->data[i][j] == 's'){
                start->row = i;
                start->col = j;
                return OK;
            }

    return ERR_EMPTY; //Не нашли старт
}

//Функция для проверки того, есть ли путь из s в e
Status path_exists(Field *field, bool *out){
    //Поданы пустые указатели
    if (field == NULL || out == NULL)
        return ERR_NULL;

    //Находим точку старта
    Point start;
    Status status = seek_start(field, &start);
    if (status != OK)
        return status;

    //Пытаемся построить маршрут до end
    return seek_path(&start, field, out);
}

//Очищение всей памяти, выделенной в main
void cleanup(FILE *fin, FILE *fout, Field *field){
    if (field != NULL)
        free_field(field);
    if (fin != NULL)
        fclose(fin);
    if (fout != NULL)
        fclose(fout);
}

//Ввод данных. Ожидается: сначала M N, на следующих строках MxN таблица
//Основная функция для получения таблицы MxN и поиск пути из s (start) в e (end), w (wall), n (null).
int main(void){
    //Открываем файл с вводными данными
    FILE *fin = fopen("in.txt", "r");
    if (fin == NULL) {
        perror("Не удалось открыть in.txt");
        return EXIT_FAILURE;
    }

    //Открываем файл для вывода ответа
    FILE *fout = fopen("out.txt", "w");
    if (fout == NULL) {
        perror("Не удалось открыть out.txt");
        fclose(fin);
        return EXIT_FAILURE;
    }

    //Получаем длину и ширину таблицы из in.txt
    long long tmpM, tmpN;
    if (fscanf(fin, "%lld %lld", &tmpM, &tmpN) != 2 || tmpM < 1 || tmpN < 1){
        fclose(fin);
        fprintf(fout,"bad input\n");
        fclose(fout);
        return EXIT_FAILURE;
    }
    size_t M = (size_t)tmpM, N = (size_t)tmpN;

    //Создаём таблицу
    Field field;
    if (init_field(&field, M, N) != OK){
        fclose(fin);
        fprintf(fout,"bad input\n");
        fclose(fout);
        return EXIT_FAILURE;
    }

    //Заполняем её из in.txt
    if (read_field(&field, fin) != OK){
        fprintf(fout,"bad input\n");
        cleanup (fin, fout, &field);
        return EXIT_FAILURE;
    }

    //Проверяем, можно ли построить путь из s в e
    bool result = false;
    if (path_exists(&field, &result) != OK){
        fprintf(fout,"error in path_exists\n");
        cleanup (fin, fout, &field);
        return EXIT_FAILURE;
    } 

    //Выводим на экран, результат проверки маршрута
    fprintf(fout,"%s\n", result? "true":"false");

    //Очищаем память и успешно завершаем программу
    cleanup (fin, fout, &field);
    return EXIT_SUCCESS;
}