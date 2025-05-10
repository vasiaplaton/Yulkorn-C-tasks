## 📁 Структура проекта

```
.
├── Makefile
├── include
│   ├── cli.h
│   ├── udt_list.h
│   ├── io_file.h
│   └── sort_variant.h
├── src
│   ├── main.c
│   ├── cli.c
│   ├── udt_list.c
│   ├── io_file.c
│   └── sort_variant.c
├── test_list.txt
└── bin
    ├── *.o
    └── lab3
```

* **`include/*.h`** — заголовки, описывают интерфейсы модулей.
* **`src/*.c`** — реализация модулей и точка входа.
* **`bin/`** — каталоги для `.o` и исполняемого файла `lab3`.
* **`test_list.txt`** — пример входных данных для команды `load`.

---

## 🔧 Модуль `udt_list` (ADT списка)

### Файл `include/udt_list.h`

* **Полное определение** узла:

  ```c
  struct list_node {
      int value;
      struct list_node *next;
  };
  ```

* `typedef struct list_node* UDTList;`
  Вместо оперирования `struct list_node*` мы используем `UDTList` — синоним указателя, скрывающий детали структуры (инкапсуляция).

* **Прототипы функций:**

  ```c
  void   UDTList_create(UDTList *lst_ptr);
  void   UDTList_destroy(UDTList *lst_ptr);
  bool   UDTList_is_empty(const UDTList lst);
  size_t UDTList_size(const UDTList lst);
  void   UDTList_push_back(UDTList lst, int value);
  int    UDTList_pop_front(UDTList lst);
  void   UDTList_print(const UDTList lst);
  void   UDTList_erase(UDTList lst, int value);
  void   UDTList_copy(const UDTList src, UDTList dst);
  ```

### Файл `src/udt_list.c`

1. **Создание списка**

   ```c
   void UDTList_create(UDTList *lst_ptr) {
       *lst_ptr = malloc(sizeof(struct list_node));
       (*lst_ptr)->next = NULL;
   }
   ```

   * Выделяем «головной» фиктивный узел (без данных), чтобы упростить операции вставки/удаления.

2. **Удаление списка**

   ```c
   void UDTList_destroy(UDTList *lst_ptr) {
       struct list_node *curr = *lst_ptr;
       while (curr) { … free(curr); … }
       *lst_ptr = NULL;
   }
   ```

   * Проходим по всем узлам и освобождаем память, затем обнуляем указатель.

3. **Проверка на пустоту**

   ```c
   bool UDTList_is_empty(const UDTList lst) {
       return lst->next == NULL;
   }
   ```

   * Смотрим, есть ли узел после фиктивного «head».

4. **Добавление в конец**

   ```c
   void UDTList_push_back(UDTList lst, int value) { … }
   ```

   * Проходим до последнего узла (`while (p->next)`), создаём новый, вставляем в `p->next`.

5. **Удаление с начала**

   ```c
   int UDTList_pop_front(UDTList lst) { … }
   ```

   * Если пусто — `exit(EXIT_FAILURE)`.
   * Снимаем первый реальный узел, возвращаем его `value`.

6. **Печать / стирание по значению**

   * Пробегаем и выводим все `value`.
   * Для удаления ищем первый узел с заданным `value` и связываем «предыдущий» с `next` этого узла.

7. **Копирование**

   ```c
   void UDTList_copy(const UDTList src, UDTList dst) {
       for (… p = src->next; p; p = p->next)
           UDTList_push_back(dst, p->value);
   }
   ```

   * Важно: `dst` уже создан (`UDTList_create`), мы лишь «наращиваем» к нему все элементы из `src`.

---

## 💾 Модуль `io_file` (I/O)

### Файл `include/io_file.h`

```c
bool UDTList_read_from_file(const char *filename, UDTList lst);
void UDTList_write_to_file(const char *filename, const UDTList lst);
```

### Файл `src/io_file.c`

* **Чтение**:

  1. `fopen` в режиме `"r"`;
  2. `while (fscanf(f, "%d", &v)==1) UDTList_push_back(lst, v);`
  3. Проверяем `ferror` и закрываем файл.

* **Запись**:

  1. `fopen` в режиме `"w"`;
  2. Проходим по списку и `fprintf(f, "%d\n", p->value);`
  3. `fclose`.

> **Зачем?**
> Ассоциативная логика (чтение/запись) вынесена в отдельный модуль, чтобы не засорять основной код списков.

---

## 🔀 Модуль `sort_variant` (вставочная сортировка)

### Файл `include/sort_variant.h`

```c
bool   UDTList_sort_insert_variant(UDTList *lst_ptr);
void   UDTList_sort(UDTList *lst_ptr);
```

* `sort_insert_variant` делает **один шаг**:
  ищет первый узел, который «нарушает» порядок (меньше предыдущего), и вставляет его в правильное место.

* Возвращает `true`, если **переместил** узел, `false` — если список уже полностью упорядочен.

* `UDTList_sort` просто **циклично** вызывает шаги до тех пор, пока они возвращают `true`.
  В вашей версии ещё выводит промежуточные состояния:

  ```c
  while (UDTList_sort_insert_variant(lst_ptr)) {
      printf("Текущий список: ");
      UDTList_print(*lst_ptr);
  }
  ```

> **Почему так?**
> Метод «постепенной вставки» удобен для демонстрации работы алгоритма и соответствует заданию:
> повторять вспомогательную процедуру, не пересоздавая весь алгоритм.

---

## 🖥 CLI-модуль

### Файл `include/cli.h`

```c
#pragma once
void user_client(void);
```

### Файл `src/cli.c`

* Цикл `while(1)` читает команды через `fgets` + `strtok`.
* На каждую команду (`create`, `push_back <v>`, `load <file>`, `sort`, `exit` и пр.) вызывается своя функция из модулей.
* **Важно:** в `main.c` вообще нет работы со структурой — только вызов `user_client()`.
  Все операции проходят через **функциональную спецификацию**.

---

## 🚀 Точка входа

### Файл `src/main.c`

```c
#include "include/cli.h"

int main(void) {
    user_client();
    return 0;
}
```

* **Никакой «дырявости»**: в `main` нет ни одного знания о внутренностях списка.

---

## 🛠 Makefile

```makefile
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -Iinclude

SRC     = src/main.c src/cli.c src/udt_list.c src/io_file.c src/sort_variant.c
BINDIR  = bin
OBJS    = $(patsubst src/%.c,$(BINDIR)/%.o,$(SRC))
EXEC    = lab3
BINARY  = $(BINDIR)/$(EXEC)

.PHONY: all clean run

all: $(BINARY)

# Линковка: создаём bin/, собираем .o → lab3
$(BINARY): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

# Компиляция: каждый src/%.c → bin/%.o
$(BINDIR)/%.o: src/%.c
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(BINARY)
	-rmdir --ignore-fail-on-non-empty $(BINDIR)

run: $(BINARY)
	@./$(BINARY)
```

* **Переменные**:

  * `CC`, `CFLAGS` — компилятор и опции.
  * `SRC` — список исходников.
  * `BINDIR` — куда складываем `.o` и исполняемый файл.
* **Шаблонные правила**:

  * `$(BINDIR)/%.o: src/%.c` — для каждого `foo.c` создаём `bin/foo.o`.
* **`all`** — по умолчанию собирает `bin/lab3`.
* **`clean`** — удаляет `.o`, бинарник и старается удалить пустой каталог `bin`.
* **`run`** — выполняет `./bin/lab3`.

> **Зачем так?**
>
> * Раздельная компиляция ускоряет сборку при изменении одного модуля.
> * Вынос в `bin/` — чисто структурный приём, чтобы корень не захламлять.

---

**Итог**:

* Абстракция (ADT) через `UDTList`.
* Чистая архитектура модулей (списки, I/O, сортировка, CLI).
* Пошаговая сортировка переиспользует вспомогательную функцию.
* Makefile организует сборку в отдельный каталог.

**Refining lesson plan**

The user is studying C language through this lab and wants the lesson plan adjusted with a focus on C language features. Two lessons were already given, but they might want a different approach, possibly refining or changing the structure. They’ve confirmed the timeframe of two 2-hour lessons, so I’ll need to adapt the plan to ensure it emphasizes key elements of C programming. I'll make sure to provide a refined plan based on their request.


Thought for 7 seconds


Ниже — скорректированный план двух 2-часовых занятий, где на каждом шаге мы не просто «делаем» фичи лабораторной, а тщательно разбираем ключевые конструкции Си, иллюстрируя их на вашем коде.

---

## Урок 1 (120 мин)

### Тема: Динамическая память и односвязный список

|    Время    | Содержание                                                                                                                                                                                   |
| :---------: | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|   0–10 мин  | **Вступление**: цели курса, структура проекта, зачем модульность в Си.                                                                                                                       |
|  10–30 мин  | **`struct` и `typedef`**<br>– Объявление `struct list_node`<br>– Почему `typedef struct list_node *UDTList` удобно скрывает детали реализации.                                               |
|  30–60 мин  | **`malloc`/`free`**<br>– Разбор `UDTList_create` (голова-фиктивный узел) и `UDTList_destroy`.<br>– Демонстрация утечки памяти (Valgrind).                                                    |
|  60–80 мин  | **Указатели и передача по ссылке**<br>– Почему `UDTList *lst_ptr` вместо `UDTList lst` в `create` и `destroy`.<br>– Практика: переписать функцию, принимая `UDTList lst` — что получится?    |
|  80–100 мин | **Вставка/удаление узлов**<br>– `push_back` и `pop_front`: как сдвигаются указатели, где проверка на NULL.<br>– Обработка ошибок: `exit(EXIT_FAILURE)` vs. возвращаемый код.                 |
| 100–120 мин | **Практика и Q\&A**<br>– Написать мини-тест: создаём список, добавляем 3 элемента, удаляем первый, выводим. <br>– Обсуждаем: стек вызовов, адреса в памяти, почему размер списка в `size_t`. |

**Домашнее задание:**

1. Сборка и запуск примеров из урока.
2. Написать функцию `UDTList_push_front` (вставка в начало) и протестировать.
3. Подготовить 3 вопроса по управлению памятью в Си.

---

## Урок 2 (120 мин)

### Тема: Модульный дизайн, I/O и сортировка вставками

|    Время    | Содержание                                                                                                                                                                                            |
| :---------: | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|   0–10 мин  | **Повторение**: коротко отвечаем на вопросы по динамике и спискам.                                                                                                                                    |
|  10–30 мин  | **Препроцессор и include guards**<br>– Разбираем `#pragma once` и альтернативу через `#ifndef … #define … #endif`.<br>– Как правильно включать заголовки (`"udt_list.h"` vs `<…>`).                   |
|  30–60 мин  | **Файловый ввод-вывод**<br>– `fopen`, режимы `"r"`/`"w"`, `fscanf`/`fprintf`.<br>– Обработка ошибок: `ferror`, `clearerr`.<br>– Практика: загрузить `test_list.txt`, вывести все элементы.            |
|  60–85 мин  | **Алгоритм вставочной сортировки**<br>– Логика `UDTList_sort_insert_variant`: поиск «плохого» узла, удаление и вставка.<br>– Разбираем условие `while (curr->value >= prev->value)`.                  |
|  85–105 мин | **Полная сортировка**<br>– Цикл `while (UDTList_sort_insert_variant(...))` и вывод промежуточных шагов.<br>– Обсуждаем сложность O(n²), когда это плохо, и почему для списков нет случайного доступа. |
| 105–120 мин | **Makefile и CLI**<br>– Разбор переменных (`CC`, `CFLAGS`, `SRC`, `OBJS`, `BINDIR`).<br>– Шаблон `$(BINDIR)/%.o: src/%.c` и что даёт `-Iinclude`.<br>– Запускаем `make run`, правим цель `clean`.     |

**Домашнее задание:**

1. В CLI-модуле добавить команду `reverse` (обратный порядок списка) и реализовать её через новые функции в `udt_list.c`.
2. Расширить Makefile: добавить цель `check`, которая компилирует и сразу запускает серию команд (через shell-скрипт).
3. Писменная мини-рефлексия (1 страница) о том, как модули на Си помогают управлять сложностью больших проектов.
