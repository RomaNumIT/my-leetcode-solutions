#include <stdbool.h>
#include <stdlib.h>

// Структура для стека
struct Stack {
  unsigned char val;
  struct Stack *next;
};

// Алиас для struct Stack
typedef struct Stack Stack;

// Результаты работы функций, работающий со стеком
typedef enum {
  OK = 1,
  STACK_IS_EMPTY = 0,
  OUT_OF_MEMORY = 2,
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
Status push_front(Stack **p, const unsigned char val) {
  Stack *node = malloc(sizeof(*node));
  if (node == NULL)
    return OUT_OF_MEMORY;
  node->val = val;
  node->next = *p;
  *p = node;
  return OK;
}

// Проверка, что стек не пустой
bool is_empty_stack(const Stack *const p) { return p == NULL; }

// Вытаскивание одного элемента из стека
Status pop_front(Stack **p, unsigned char *out) {
  if (is_empty_stack(*p))
    return STACK_IS_EMPTY;
  *out = (*p)->val;
  Stack *temp = *p;
  *p = (*p)->next;
  free(temp);
  return OK;
}

// Проверка является ли символ открывающейся скобкой
bool is_open_bracket(const unsigned char c) { return c == '(' || c == '{' || c == '['; }

// Проверка, что скобка закрывающаяся
bool is_close_bracket(const unsigned char c) { return c == ')' || c == '}' || c == ']'; }

// Функция, возвращаюся для открывающейся скобки её тип закрывающейся
unsigned char matching_close(const unsigned char c) {
  switch (c) {
  case '(':
    return ')';
  case '{':
    return '}';
  case '[':
    return ']';
  default:
    return '\0'; // Не закрывающаяся скобка
  }
}

// Функция, проверяющая корректность скобочной последовательности
bool isValid(char *s) {
  // Если скобок нет, значит уже скобочная последовательность корректная
  if (s == NULL)
    return true;

  // Инициализируем стек для хранения скобок
  Stack *stack = init_stack();

  // Пробегаем по всей строке и проверяем корректность
  for (size_t i = 0; s[i] != '\0'; i++) {
    // Если скобка, открывающаяся, то добавляем в стек
    if (is_open_bracket(s[i])) {
      if (push_front(&stack, s[i]) != OK) {
        free_stack(stack);
        return false;
      }
      continue;
    }

    // Если закрывающаяся скобка, то проверяем корректность закрытия скобкок с последней в стеке
    if (is_close_bracket(s[i])) {
      // Достаём последнюю скобку из стека, если её нет, значит неверная скобочная последовательность
      unsigned char last_bracket;
      if (pop_front(&stack, &last_bracket) != OK) {
        free_stack(stack);
        return false;
      }
      // Если открывающаяся не того же типа, что и закрывающаяся, значит скобочная последовательность неверная
      if (matching_close(last_bracket) != s[i]) {
        free_stack(stack);
        return false;
      }
    }
  }
  // Проверяем, что в стеке нет не закрытых открывающихся скобок
  if (is_empty_stack(stack)) {
    free_stack(stack);
    return true;
  }

  // Есть не закрытая открывающаяся скобка, очищаем стек и false
  free_stack(stack);
  return false;
}