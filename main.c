/*
  Простой калькулятор для оболочки.
  By Goatish Priest, 2018. All right reversed.

  Порядок работы цикла программы.

  1. Итеративно запрашивается input (*).
  2. input обрабатывается программой.
    2.1. input'у определяется тип (**).
    2.2. Проверяется, корректный ли input. 
      - Если некорректный, итерация сбрасывается (continue), и данный пункт становится последним.
      - Если корректный, программа переходит к следующему пункту.
  3. Из input извлекаются данные. Программа получает операнды и операторы для вычислений.
  4. Производятся вычисления. Выводится промежуточный итог.
  5. Следующей итерации передаются необходимые данные о текущей.
  6. Цикл завершается. Выводится итоговый результат.

  (*)
  Ввод "=" или "quit" обрабатывается особо - такой input означает, что цикл нужно завершить.

  (**)
  Типы input:
    'x' (exit / запрос на выход), строка. Это input "=" или "quit"
    'n' (number / число), double. Пример input'а: 10.0
    'o' (operator / мат. бинарный оператор), char. Пример input'а: '+'
    'r' (root (sqrt) / квадратный корень), char. Это input 'r'
    '\0' (input неопределенного типа). Ввод, не имеющий в данном контексте смысла. 

  Справка.
  - Функция main может принимать первый аргумент "h" (help). Если запустить программу с данным аргументом, на 
  экран будет  выведена краткая справка.

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
  /* 0. Вывод справки. */
  if ( (argc > 1) && (*(argv[1]) == 'h') )
  {
    print_help();
    return 0; // если вызвана справка, калькуляцию не предлагать
  }

  /* 1. Объекты. */
  char *input_pt; // input хранится в статической переменной
  double subtotal = 0.0; // промежуточный итог и итоговый результат

  /* input - структура для данных об input. */
  struct Input input;
  input.opr = '\0'; // текущий введенный оператор
  input.new_num = 0.0; // последнее введенное число
  input.tmp = 0.0; // "временный" subtotal, "подменяет" последний в рамках одного выражения
  input.type = '\0'; // тип значения текущего input: 'x', 'n', 'o', 'r' или '\0'
  input.radicand = 0.0; // подкоренное выражение; для корректного вывода вычислений с участием корня

  /* last_input - структура для состояний о предыдущем input. */
  /* Состояния необходимы для определения некорректного input. */
  struct Input last_input;
  last_input.is_num = 0;
  last_input.is_operator = 0;
  last_input.is_root = 0;

  /* Introduction. */
  printf("Please enter what you want to calculate. Enter \"=\" to get subtotal and quit. If you wish to see\
  brief help section, you may launch program with \"h\" argument, like so: \"./calc h\".\n***\n");

  /* 2. Основная часть. */
  while (1)
  {
    /* 1. Запрос input. */
    /* А также определение типа input. */
    input_pt = query_input();
    input.type = identify_input(&input_pt, subtotal, input); // получаем 'x', 'n', 'o', 'r' или '\0'

    /* 2. Обработка некорректного input. */
    if ( 
    /* Ветка срабатывает, если тип input неизвестен, т.е. '\0' */
    !input.type ||

    /* ИЛИ: если в ходе вычислений был получен NaN или Infinity. */
    ( type_is_operator(input.type) && is_bad_num(input.tmp) ) ||

    /* ИЛИ: если был введен оператор 'r', а потом сразу число. */
    ( type_is_num(input.type) && input.opr == 'r' ) ||

    /* ИЛИ: если ввод начинается *не* с числа. И: */
    /* разрешается ввести другой оператор после ввода оператора. */
    ( !type_is_num(input.type) && !(last_input.is_num || last_input.is_root) &&
      !(type_is_operator(input.type) && last_input.is_operator) ))
    {
      /* Был ли запрос на выход? Если да, прервать цикл. */
      if (type_is_quit_query(input.type))
        break;

      print_error(&input_pt, input);
      continue;
      /* Если input некорректный, не извлекать данные из него: прервать итерацию. */
    }

    /* 3. Извлечение данных из корректного input. */

    /* 3.1. Если тип input - 'n', извлечь число из input. */
    /* Для вычислений используется 3 числа: new_num, subtotal и tmp. Здесь мы обозначаем new_num. */
    if (input.type && type_is_num(input.type))
      input.new_num = get_num(&input_pt, subtotal); // ввод числа

    /* 3.2. Если тип input - 'o' или 'r', извлечь оператор из input. */
    else
      input.opr = get_operator(&input_pt); // получаем оператор

    /* 4. Обработка извлеченных данных. */

    /* 4.1 Если input имеет тип 'n' или 'r'. */
    if (input.type && (type_is_num(input.type) || type_is_root(input.type)))
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
        if (is_bad_num(input.tmp))
        {
          print_subtotal(subtotal, input);
          print_error(&input_pt, input);

          input.tmp = subtotal;
          continue;
        }
      }

      /* 3. Вывести на экран вычисления. */
      if (type_is_num(input.type))
        print_subtotal(subtotal, input);
      else if (type_is_root(input.type))
        print_subtotal(input.radicand, input); // sqrt: время для radicand
    }

    /* 4.2. Если input имеет тип 'o'. */
    else if (input.type && type_is_operator(input.type))
      subtotal = input.tmp;

    /* 5. Данные о текущем input для обработки следующего. */
    /* При след. вводе покажут, какого типа данный input. */
    last_input.is_operator = type_is_operator(input.type);
    last_input.is_num = type_is_num(input.type);
    last_input.is_root = type_is_root(input.type);

    /* Был ли запрос на выход? Если да, прервать цикл. */
    if (type_is_quit_query(input.type))
      break;
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