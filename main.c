/*
	Простой калькулятор для оболочки.	
	By Goatish Priest, 2018. All right reversed.

	Порядок работы цикла программы.

	1. Итеративно запрашивается input (*). 
	2. input обрабатывается программой.
		2.1. input'у определяется тип (**).
		2.2. Проверяется, корректный ли input. 
			Если некорректный, итерация сбрасывается (continue), и данный пункт становится последним.
			Если корректный, программа переходит к следующему пункту.
	3. Из input извлекаются данные. Программа получает операнды и операторы для вычислений.
	4. Производятся вычисления. Выводится промежуточный итог.
	5. Следующей итерации передаются необходимые данные о текущей.
	6. Цикл завершается. 
		Если цикл завершается без ошибок (без получения NaN или Infinity), выводится итоговый результат вычислений.

	(*)
	Ввод '=' обрабатывается особо - такой input означает, что цикл нужно завершить.

	(**)
	Типы input:
		'n' (number / число), double. Пример input'а: 10.0
		'o' (operator / мат. бинарный оператор), char. Пример input'а: '+'
		'p' (percent / процент), char. Это input '%'
		'r' (root (sqrt) / квадратный корень), char. Это input 'r'

	Справка.
	- Функция main может принимать первый аргумент "h" (help). Если запустить программу с данным аргументом, на 
	экран будет	выведена краткая справка.

	Подробнее о программе - в файле README.md.
*/

/* Директивы для препроцессора. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "include/general.h"
#include "include/input.h"
 
/* Объявление объектов, прототипы функций. */

extern char *math_operators_pt;
extern char math_operators_arr[MAX_SIZE];
struct Input;
void print_help();
double get_num(char **input_pt, double subtotal);
char get_operator(char **input_pt);
double do_math(double subtotal, struct Input input);
char identify_input(char **input_pt, double subtotal, struct Input input);
void print_error(char **input_pt, struct Input input);
void print_subtotal(double subtotal, struct Input input);
double reset_new_num(struct Input input);
int type_is_operator(char input_type);
int type_is_num(char input_type);
int type_is_percent(char input_type);
int type_is_root(char input_type);

/* Main. */

int main(int argc, char *argv[])
{
	/* Справка. */
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

	input.is_done = 0; // введено ли '='
	input.is_initialized = 0; // присвоено ли переменной subtotal первое число, введенное пользователем

	input.opr = '\0'; // текущий введенный оператор
	input.new_num = 0.0; // последнее введенное число

	input.tmp = 0.0; // "подменяет" subtotal
	input.radicand = 0.0; // подкоренное выражение; для корректного вывода вычислений с участием корня

	input.type = '\0'; // тип значения текущего input: 'n', 'o' или 'r'

	/* last_input - структура для состояний о последнем input. */
	/* Необходима для обработки некорректного input. */
	struct Input last_input;
	last_input.was_num = 0;
	last_input.was_operator = 0;
	last_input.was_root = 0;

	/* Introduction. */
	printf("Please enter what you want to calculate. Enter \"=\" to get subtotal and quit. If you wish to see\
	brief help section, you may launch program with \"h\" argument, like so: \"./calc h\".\n\n");

	/* 2. Основная часть. */
	while (1) // цикл прервется, если будет введено '='
	{
		/* 1. Запрос input. */
		/* А также определение типа input. */
		input_pt = query_input();
		input.type = identify_input(&input_pt, subtotal, input); // получаем 'o', 'n', 'p', 'r' или '\0'

		/* Знак '=' необходимо обработать особо. */
		/* Если введен '=', при проверке input.is_done программа будет завершена. */
		if (type_is_operator(input.type) && (get_operator(&input_pt) == '='))
			input.is_done = 1;


// printf("| | 1. tmp? %f\n", input.tmp);
// printf("| | 2. subtotal? %f\n", subtotal);
// printf("| | 3. new_num? %f\n", input.new_num);
// printf("| | 5. opr? %c\n", input.opr);
// printf("| | 6. type? %c\n", input.type);		


		/* 2. Обработка некорректного input. */
		if ( 
		/* Ветка срабатывает, если тип input неизвестен, т.е. '\0' */
		!input.type ||

		/* Или: если в ходе вычислений был получен NaN или Infinity. */
		( type_is_operator(input.type) && is_bad_num(input.tmp) ) ||

		/* Или: если был введен оператор 'r', а потом сразу число. */
		( type_is_num(input.type) && input.opr == 'r' ) ||

		/* Или: если ввод начинается *не* с числа. */
		/* Однако разрешается ввести другой оператор после ввода оператора. */
		( !type_is_num(input.type) && !(last_input.was_num || last_input.was_root) &&
			!(type_is_operator(input.type) && last_input.was_operator) ))
		{
			/* Был ли запрос на выход? Если да, прервать цикл. */
			if (input.is_done)
				break;

			print_error(&input_pt, input);

			continue;
			/* Если input некорректный, не извлекать данные из него: прервать итерацию. */
		}


	
		/* 3. Извлечение данных из корректного input. */

		/* 3.1. Если тип input - 'n', извлечь число из input. */
		/* Для вычислений используется 2 числа: new_num и subtotal. Здесь мы обозначаем new_num. */
		if (input.type && type_is_num(input.type))
		{
			// if (get_num(&input_pt, subtotal))
			input.new_num = get_num(&input_pt, subtotal);

			// if (isnan(input.tmp) || isinf(input.tmp))

		}

		/* 3.2. Если тип input - 'o', 'p' или 'r', извлечь оператор из input. */
		else
			input.opr = get_operator(&input_pt); // получаем оператор

// printf("| | 1. tmp? %f\n", input.tmp);
// printf("| | 2. subtotal? %f\n", subtotal);
// printf("| | 3. new_num? %f\n", input.new_num);
// printf("| | 5. opr? %c\n", input.opr);
// printf("| | 6. type? %c\n", input.type);	

		/* 4. Обработка извлеченных данных. */






		/* Если input имеет тип 'n' или 'r'. */
		if (input.type && (type_is_num(input.type) || type_is_root(input.type)))
		{
			/* 4.1. "Инициализация" промежуточного итога subtotal. */
			/* Производится только 1 раз за цикл (очевидно), когда поступает 1-й new_num. */
			if (/*!input.is_initialized && */!input.opr)
			{
				input.tmp = input.new_num;
				// input.is_initialized = 1;
			}

			/* 4.2. Произвести вычисления. */
			/* Только если subtotal уже "инициализирован". */
			else
			{
				input.radicand = input.tmp;
				// input.tmp_backup = input.tmp;
				if (input.opr)
					input.tmp = do_math(subtotal, input);

				if (is_bad_num(input.tmp))
				{
					print_subtotal(subtotal, input);
					print_error(&input_pt, input);

					input.tmp = subtotal;
					continue;
				}
			}



			/* Вывести на экран вычисления и input. */
			if (type_is_num(input.type))
				print_subtotal(subtotal, input);

			else if (type_is_root(input.type))
				print_subtotal(input.radicand, input);
		}

		/* Если input имеет тип 'o'. */
		else if (input.type && type_is_operator(input.type)) // elif для лучшей читаемости
			subtotal = input.tmp;



		/* 5. Данные о текущем input для обработки следующего. */




		/* 2.2. Состояния, необходимые для идентификации некорректного input. */
		/* Показывают, какого типа был предыдущий input. */

		last_input.was_operator = type_is_operator(input.type);
		last_input.was_num = type_is_num(input.type);
		last_input.was_root = type_is_root(input.type);


		/* Был ли запрос на выход? Если да, прервать цикл. */
		if (input.is_done)
			break;
	}

	/* 6. Вывести итоговый результат. */
	printf("Result is: %f\n", subtotal);

	return 0;
}

/* Output для дебага. */
// printf("| | 1. tmp? %f\n", input.tmp);
// printf("| | 2. subtotal? %f\n", subtotal);
// printf("| | 3. new_num? %f\n", input.new_num);
// printf("| | 5. opr? %c\n", input.opr);
// printf("| | 6. type? %c\n", input.type);	