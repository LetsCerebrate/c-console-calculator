// Тренировочная задачка. Местами усложнено на пустом месте - понятно, что это плохая практика 
// для реального проекта ("write dumb code", как говорят), но набить руку на фичах C (указатели,
// работа с памятью) необходимо


/* Preprocessor directives */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "include/objects.h"
 
/* Objects */

extern char *math_operators_pt;
extern char math_operators_arr[MAX_SIZE];

struct Input;

// extern char operator; 

/* Functions prototypes */

void print_help();
void print_error(char **input_pt, struct Input input);
void print_subtotal(double subtotal, struct Input input);

char get_operator(char **input_pt);
double get_num(char **input_pt);
char identify_input(char **input_pt, double subtotal, struct Input input);
double get_subtotal(double subtotal, struct Input input);
double reset_new_num(struct Input input);

int is_operator(char input_type);
int is_num(char input_type);
int is_percent(char input_type);
int is_root(char input_type);

// int is_any_opr(char type);

/* Main */

int main(int argc, char *argv[])
{

	// printf("ARGS: %s, %d\n", argv[1], argc);
	// if (argv[1] == "help" || argv[1] == "h") // why does it not work?
	// {
	// 	print_help();
	// 	return 0;
	// }

	// printf("TEST: %d\n", "a" == "ab");


	struct Input input;

	input.is_done = 0;
	input.subtotal_is_initialized = 0; // присвоено ли переменной subtotal первое число, введенное пользователем
	input.prev_operator = '\0'; // оператор предыдущего input - используется для расчетов в get_subtotal
	input.current_operator = '\0'; // последний введенный оператор
	

	input.type = '\0'; // тип значения текущего input

	input.new_num = 0.0; // последнее введенное число
	input.old_num = 0.0; // "старое" число; в вычислениях не участвует, необходимо для корректной работы print_subtotal (и только)

	char *input_pt; // input хранится в статической переменной
	double subtotal = 0.0; // промежуточный итог


	printf("Please consistently enter what you want to calculate. Enter \"=\" to get subtotal and quit.\n");

	/* Структура для состояний о последнем input. */
	struct Input last_input;
	last_input.was_num = 0; // число, т.е. операнд
	last_input.was_operator = 0; // бинарный оператор: '+', '-', ...
	last_input.was_percent = 0; // постфиксный оператор: '%', 'r'
	last_input.was_root = 0;

	while (1)
	{


		/* 1. Запрос input, определение типа input (оператор 'o', число 'n', постфиксный оператор 'p' (например, процент
		и sqrt) или unknown '\0'). */
		input_pt = query_input();
		input.type = identify_input(&input_pt, subtotal, input); // получаем 'o', 'n', 'p' или '\0'


		if (is_operator(input.type) && (get_operator(&input_pt) == '='))
			input.is_done = 1;

		/* Если пользователь вводит '=', цикл прерывается: */
		// if (is_operator(input.type) && (get_operator(&input_pt) == '='))
		// {
		// 	print_subtotal(subtotal, input);
		// 	break;
		// }
			// if (*input_pt == 'q') break;

		// printf("| | 1. subtotal? %f\n", subtotal);
		// printf("| | 2. new_num? %f\n", input.new_num);
		// printf("| | 3. current_operator? %c\n", input.current_operator);
		// printf("| | 4. prev_operator? %c\n", input.prev_operator);
		// printf("| | 5. type? %c\n\n", input.type);	

		// printf("| | 1. was_operator? %d\n", last_input.was_operator);
		// printf("| | 2. was_num? %d\n", last_input.was_num);
		// printf("| | 3. was_percent? %d\n\n", last_input.was_percent);	

		/* 2. Обработка некорректного input. */

		/* 2.1. Выдать ошибку. */
		if ( !input.type || 
			( (!is_num(input.type) && (!last_input.was_num && !last_input.was_percent && !last_input.was_root)) ) ||
			( is_operator(input.type) && last_input.was_operator ) ||
			( is_num(input.type) && last_input.was_num ) ||
			( is_percent(input.type) && (last_input.was_percent || last_input.was_root) ) )
		/* Ветка срабатывает, если тип input неизвестен, т.е. '\0' */
		/* Или: если ввод начинается не с числа. */
		/* Или: если после ввода, например, числа вводится не оператор, а опять число - т.е. input того же типа. Однако
		на извлечение корня данное правило не распространяется: (81 r r == 3) */
		{		
			/* Знак '=' необходимо обработать особо. */
			/* Если введен '=', необходимо завершить работу программы. */
			if (input.is_done)
				break;

			print_error(&input_pt, input);

			continue;
			/* Если input некорректный, не извлекать данные из него: прервать итерацию. */
		}


		/* 2.2. Данные, необходимые для идентификации некорректного input. */
		/* Это состояния, показывающие, какого типа был предыдущий input. */

		last_input.was_operator = is_operator(input.type);
		last_input.was_num = is_num(input.type);
		last_input.was_percent = is_percent(input.type);
		last_input.was_root = is_root(input.type);
	

		/* 3. Извлечение данных из корректного input. */

		/* 3.1. Если input является 'o', 'p' или 'r', извлечь оператор из него. */
		// if (is_any_opr(input.type)) // м.б., лучше !is_num ? 
		// 	input.current_operator = get_operator(&input_pt); // получаем оператор


		/* 3.2. Если input является 'n', извлечь число из него. */
		/* Для вычислений использвуется 2 числа: new_num и subtotal. Здесь мы обозначаем new_num. */
		if (input.type && is_num(input.type))
			input.new_num = get_num(&input_pt);

		else
			input.current_operator = get_operator(&input_pt); // получаем оператор


		/* 4. Обработка извлеченных данных. */

		/* 4.1. Вычисления производятся только если текущий input имеет тип 'o', 'p' или 'r'. */
		/* Однако типы 'p' и 'r' участвуют не во всех нижеприведенных операциях. */
		// if (is_any_opr(input.type))
		if (input.type && !is_num(input.type))
		{ //printf("  pre  !!! subtotal: %f; prev_opr: %c; type: %c\n", subtotal, input.prev_operator, input.type);
			/* 4.2. "Инициализация" промежуточного итога subtotal. */
			/* Производится только 1 раз за цикл (очевидно), когда поступает 1-й input.new_num. */
			if (!input.subtotal_is_initialized && !input.prev_operator)
			{
				// 
				subtotal = input.new_num;
				input.subtotal_is_initialized = 1;
				//printf("    !!! is_init: %d, subtotal: %f; prev_opr: %c; type: %c\n", input.subtotal_is_initialized, subtotal, input.prev_operator, input.type);
			}

			/* 4.3. Произвести вычисления. */
			/* Только если subtotal уже "инициализирован". */
			else
			{
				input.old_num = subtotal;
				subtotal = get_subtotal(subtotal, input);

				/* Вывести на экран промежуточный итог subtotal. */
				/* Но только если данный input имеет тип 'o' или 'r' (иначе при работе с процентами 'p' будет выводиться 
				"лишний" subtotal). */
				if (is_operator(input.type) || is_root(input.type))
					print_subtotal(subtotal, input);
				
				/* Чтобы корректно завершить вычисления, нужно "сбросить" значение input.new_num, поскольку оно является
				процентом: */
				if (is_percent(input.type))
					input.new_num = reset_new_num(input); // в зависимости от input.prev_operator присвоить 1.0 или 0.0
			}
		}

		// if (is_operator(input.type))
			


		/* Данные о текущем input для обработки следующего. */
		if (input.current_operator && (input.current_operator != '%')) /* '%' не записывать! В функции get_subtotal случай
		с оператором '%' будет обрабатываться особо. */
		{
			input.prev_operator = input.current_operator;
		}
// printf("!!!! %c\n", input.current_operator);

		/* Знак '=' необходимо обработать особо. */
		/* Если введен '=', необходимо завершить работу программы. */
		if (input.is_done)
			break;
	}

  // if there weren't errors there
	printf("Result is: %f\n", subtotal); // хотя происходит exit(1) при ошибке раньше

			


	return 0;
}