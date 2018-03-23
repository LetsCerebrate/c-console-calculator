/*
  Простой калькулятор для оболочки.
  By Goatish Priest, 2018. All rights reversed.

  Порядок работы цикла программы.

  1. Итеративно запрашивается input.
  2. input обрабатывается программой.
    2.1. input'у определяется тип (*).
    2.2. Проверяется, корректный ли input. 
      - Если некорректный, итерация сбрасывается (continue), и данный пункт становится последним.
      - Если корректный, программа переходит к следующему пункту.
  3. Из input извлекаются данные. Программа получает операнды и операторы для вычислений или команду.
  4. Производятся вычисления. Выводится промежуточный итог.
  5. Совершаются необходимые приготовления к приему следующего input'а. 
  6. Цикл завершается. Выводится итоговый результат.

  (*)
  Типы input:
    'k' (keyword / ключевое слово), строка. Это input "=", "quit", "c", а также команды для работы с памятью.
    'n' (number / число), double. Пример input'а: 10.0
    'o' (operator / мат. бинарный оператор), char. Пример input'а: '+'
    'r' (root (sqrt) / квадратный корень), char. Это input 'r'
    '\0' (input неопределенного типа). Ввод, не имеющий в данном контексте смысла. 

  Справка.
  - Функция main может принимать первый аргумент "h" (help). Если запустить программу с данным аргументом, на 
  экран будет выведена краткая справка.

  Подробнее о программе - в файле README.md.
*/

/* Директивы для препроцессора. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "include/objects.h"
#include "include/fn_general.h"
#include "include/fn_input.h"
#include "include/fn_print.h"

/* Main. */

int main(int argc, char *argv[])
{
  /* Вывод справки. */
  if ( (argc > 1) && (*(argv[1]) == 'h') )
  {
    print_help();
    return 0; // если вызвана справка, калькуляцию не предлагать
  }

  /* Объекты. */
  char *input_pt = NULL, // input хранится в статической переменной
    *shifted_input_pt = NULL;
  double subtotal = 0.0; // промежуточный итог и итоговый результат

  int error_code = 0,
    query_print_code = 0;

  /* input - данные об input. */
  struct Input input;
  input.is_ready_to_die = 0;
  input.keyword_code = 0;
  input.opr = '\0'; // текущий введенный оператор
  input.new_num = 0.0; // последнее введенное число
  input.tmp = 0.0; // "временный" subtotal, "подменяет" последний в рамках одного выражения
  input.type = '\0'; // тип значения текущего input: 'x', 'n', 'o', 'r' или '\0'
  input.radicand = 0.0; // подкоренное выражение; для корректного вывода вычислений с участием корня

  /* last_input - состояния о предыдущем input. */
  /* Необходимы для определения некорректного input. */
  struct Input last_input;
  last_input.is_keyword = 0;
  last_input.is_num = 0;
  last_input.is_operator = 0;
  last_input.is_root = 0;

  /* Memory - данные о работе с памятью калькулятора. */
  struct Memory memory;
  memory.cells;
  memory.index = 0;

  /* Introduction. */
  printf("Welcome to C Console Calculator! Please enter what you want to calculate. Type \"quit\" "
  "to get subtotal and quit. If you wish to see brief Help, you may launch program with \"h\" argument, like so: "
  "\"./calc h\".\n***\n"); // пробел между строками - конкатенация

  reset_all_memcells(&memory); // инициализировать ячейки памяти - обнулить (по умолчанию они хранят garbage values)

  /* Основная часть. */
  while (1)
  {
    /* 1. Запрос input. */
    /* А также определение типа input. */
    input_pt = query_input();
    input.type = identify_input(&input_pt, subtotal, input); // получаем 'k', 'n', 'o', 'r' или '\0'

    /* 2. Обработка некорректного input. */
    if ( 
    /* Ветка срабатывает, если тип input неизвестен, т.е. '\0' */
    (!input.type && 
      (error_code = 1)) ||

    /* ИЛИ: если в ходе вычислений был получен NaN или Infinity. */
    ( (type_is_operator(input.type) && is_bad_num(input.tmp)) && 
      (error_code = 2) ) ||

    /* ИЛИ: если был введен оператор 'r', а потом сразу число. */
    ( (type_is_num(input.type) && input.opr == 'r') && 
      (error_code = 3) ) ||

    /* ИЛИ: если ввод начинается *не* с числа (хотя может начинаться с ключевого слова). И: */
    /* разрешается ввести другой оператор после ввода оператора. */
    ( (!type_is_keyword(input.type) && 
      ( !type_is_num(input.type) && !(last_input.is_num || last_input.is_root || last_input.is_keyword) &&
        !(type_is_operator(input.type) && last_input.is_operator)) && 
          (error_code = 4) )))
    {
      /* Был ли запрос на выход? Если да, прервать цикл. */
      if (input.is_ready_to_die)
        break;

      print_error(&input_pt, input, memory, error_code);
      continue;
      /* Если input некорректный, не извлекать данные из него: прервать итерацию. */
    }

    /* 3. Извлечение данных из корректного input. */

    /* 3.1. Если тип input - 'k', извлечь ключевое слово из input. */
    if (input.type && type_is_keyword(input.type))
    {
      input.keyword_code = get_keyword_code(&input_pt); // получить код ключевого слова: [1; 9]

      if (input.keyword_code > 5) // команды для работы с памятью
      {
        /* Если указанный элемент - '\0', т.е. input имеет вид "m+". */
        if ( !(*(input_pt + 2)) && (memory.index = 1) ) {} // обработать как "m+1"

        /* Если input имеет вид "m+#": например, "m+10". */
        else
        {
          shifted_input_pt = (input_pt + 2); // получить указатель, смещенный к "10"
          memory.index = (int) get_num( &shifted_input_pt, subtotal );

          if ( (memory.index >= MAX_SIZE + 1) && 
            (error_code = 5)) // anti-segmentation fault
          {
            print_error(&input_pt, input, memory, error_code);
            continue;
          }
        }
      }

      switch (input.keyword_code)
      {
        case 1: // "quit"
          input.is_ready_to_die = 1;
          break;

        case 2: // "="
          input.new_num = 0.0;
          input.opr = '\0';
          print_subtotal(subtotal, input, memory);
          break;

        case 3: // "c"
          subtotal = input.tmp = input.new_num = input.radicand = 0.0;
          input.opr = '\0';
          print_subtotal(subtotal, input, memory);
          break;

        case 4: // "mca"
          reset_all_memcells(&memory);
          print_all_memcells(&memory);
          break;

        case 5: // "mpa"
          print_all_memcells(&memory);
          break;

        case 6: // "mr" или "mr#"
          input.new_num = memory.cells[memory.index - 1];
          input.type = 'n';
          break;

        case 7: // "mc" или "mc#"
          memory.cells[memory.index - 1] = 0.0;
          print_subtotal(subtotal, input, memory);
          break;

        case 8: // "m+" или "m+#"
          memory.cells[memory.index - 1] += input.tmp;
          print_subtotal(subtotal, input, memory);
          break;

        case 9: // "m-" или "m-#"
          memory.cells[memory.index - 1] -= input.tmp;
          print_subtotal(subtotal, input, memory);
      }
    }

    /* 3.2. Если тип input - 'n', извлечь число из input. */
    /* Для вычислений используется 3 числа: new_num, subtotal и tmp. Здесь мы обозначаем new_num. */
    else if (input.type && type_is_num(input.type))
      input.new_num = get_num(&input_pt, subtotal); // получаем число new_num

    /* 3.3. Если тип input - 'o' или 'r', извлечь оператор из input. */
    else
      input.opr = get_operator(&input_pt); // получаем оператор

    /* 4. Обработка извлеченных данных. */

    /* 4.1. Если input имеет тип 'n' или 'r'. */
    if ( input.type && (type_is_num(input.type) || type_is_root(input.type)) )
    {
      /* 1. "Инициализация" промежуточного итога. */
      /* Производится только 1 раз за цикл (очевидно): на этапе, когда поступает 1-й new_num. */
      if (!input.opr)
        input.tmp = input.new_num;

      /* 2. Произвести вычисления. */
      else
      {
        input.radicand = input.tmp; // опять же, radicand - только для корректных print'ов для выражений с sqrt
        
        /* Собственно вычисления. */
        if (input.opr)
          input.tmp = do_math(subtotal, input);

        /* Если в ходе вычислений получили NaN или Infinity. */
        if ( (is_bad_num(input.tmp)) && (error_code = 2) )
        {
          print_subtotal(subtotal, input, memory);
          print_error(&input_pt, input, memory, error_code);

          input.tmp = subtotal;
          continue;
        }
      }

      /* 3. Вывести на экран вычисления. */
      if ( type_is_num(input.type) )
        print_subtotal( subtotal, input, memory );
      else if ( type_is_root(input.type) )
        print_subtotal( input.radicand, input, memory ); // sqrt: время для radicand
    }

    /* 4.2. Если input имеет тип 'o'. */
    else if (input.type && type_is_operator(input.type))
      subtotal = input.tmp;

    /* Был ли запрос на выход? Если да, прервать цикл. */
    if (input.is_ready_to_die)
      break;

    /* 5. Приготовления к следующему input. */
    last_input.is_keyword = type_is_keyword(input.type);
    last_input.is_operator = type_is_operator(input.type);
    last_input.is_num = type_is_num(input.type);
    last_input.is_root = type_is_root(input.type);

    memory.index = input.keyword_code = error_code = query_print_code = 0;
  }

  /* 6. Вывести итоговый результат. */
  printf("***\nResult is: %f\n", input.tmp);

  return 0;
}

/* Output для дебага. */
// printf("| | 1. tmp? %f\n", input.tmp);
// printf("| | 2. subtotal? %f\n", subtotal);
// printf("| | 3. new_num? %f\n", input.new_num);
// printf("| | 4. opr? %c\n", input.opr);
// printf("| | 5. type? %c\n", input.type);  