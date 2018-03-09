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

struct State;
struct Input;

// extern char operator; 

/* Functions prototypes */

void print_help();
void print_error(char **input_pt, struct Input input);
void print_result(double subtotal);

char get_operator(char **input_pt);
double get_num(char **input_pt);
char identify_input(char **input_pt, double subtotal, struct Input input);
double get_subtotal(double subtotal, struct Input input);
double reset_new_num(struct Input input);

int is_operator(char input_type);
int is_num(char input_type);
int is_percent(char input_type);

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

	input.subtotal_is_initialized = 0; // присвоено ли переменной subtotal первое число, введенное пользователем
	input.prev_operator = '\0'; // оператор предыдущего input - используется для расчетов в get_subtotal
	input.prev_operator_backup = '\0'; // чтобы предыдущий input не затерялся в случае некорректного input
	// вроде и не нужно

	input.current_operator = '\0'; // текущий оператор
	

	input.type = '\0'; // тип значения input

	input.new_num; // последнее введенное число

	char *input_pt; // input хранится в статической переменной
	double subtotal = 0.0; // промежуточный итог










	printf("Enter:\n");

	struct State last_input;
	last_input.was_num = 0;
	last_input.was_operator = 0;
	last_input.was_percent = 0;



	while (*input_pt != '=')
	// while (1)
	{
		/* 1. Запрос input, определение типа input (оператор 'o', число 'n', процент 'p' или unknown '\0'). */
		input_pt = query_input();
		input.type = identify_input(&input_pt, subtotal, input); // получаем 'o', 'n', 'p' или '\0'

		printf("| | 1. subtotal? %f\n", subtotal);
		printf("| | 2. new_num? %f\n", input.new_num);
		printf("| | 3. current_operator? %c\n", input.current_operator);
		printf("| | 4. prev_operator? %c\n", input.prev_operator);
		printf("| | 5. type? %c\n\n", input.type);	


		/* 2. Обработка некорректного input. */

		/* 2.1. Выдать ошибку. */
		if ( !input.type || 
			( is_operator(input.type) && (!last_input.was_num && !last_input.was_percent) ) ||
			( is_operator(input.type) && last_input.was_operator ) ||
			( is_num(input.type) && last_input.was_num ) ||
			( is_percent(input.type) && last_input.was_percent ) )
		/* Ветка срабатывает, если тип input неизвестен, т.е. '\0' */
		/* Или: если ввод начинается не с числа, а с оператора. */
		/* Или: если после ввода, например, числа вводится не оператор, а опять число - т.е. input того же типа. */
		{
			print_error(&input_pt, input);
			continue;
		} // плюс в cond запихать функцию проверки на значения pow и sqrt 
		/* Если input некорректный, не извлекать данные из него: прервать итерацию. */

		/* 2.2. Данные, необходимые для идентификации некорректного input. */
		/* Это состояния, показывающие, какого типа был предыдущий input. */
		last_input.was_operator = is_operator(input.type);	
		last_input.was_num = is_num(input.type);
		last_input.was_percent = is_percent(input.type);
	

		/* 3. Извлечение данных из корректного input. */

		/* 3.1. Если input является 'o' или 'p', извлечь оператор или символ '%' из него. */
		if (is_operator(input.type) || is_percent(input.type))
			input.current_operator = get_operator(&input_pt); // получаем, м.б., '+', или '%'...

		/* 3.2. Если input является 'n', извлечь число из него. */
		/* Для вычислений использвуется 2 числа: new_num и subtotal. Здесь мы обозначаем new_num. */
		else if (is_num(input.type))
			input.new_num = get_num(&input_pt);


		/* 4. Обработка извлеченных данных. */
		/* Вычисления производятся только если текущий input - оператор. */
		if (is_operator(input.type) || is_percent(input.type))
		{
			if (!input.subtotal_is_initialized && !input.prev_operator)
			/* "Инициализация" промежуточного итога subtotal: */
			{
				subtotal = input.new_num;
				input.subtotal_is_initialized = 1;
			}

			else
			/* Вычисления: */
			{
				subtotal = get_subtotal(subtotal, input);

				/* Чтобы корректно завершить вычисления, нужно "сбросить" значение input.new_num, поскольку оно является
				процентом: */
				if (is_percent(input.type))
					input.new_num = reset_new_num(input); // в зависимости от input.prev_operator присвоить 1 или 0



			}
		}



		/* Данные о текущем input для обработки следующего. */
		if (input.current_operator && (input.current_operator != '%')) /* '%' не записывать! В функции get_subtotal случай
		с оператором '%' будет обрабатываться особо. */
		{
			input.prev_operator = input.current_operator;
			// input.prev_operator_backup = input.current_operator;
		}


		/* Если пользователь вводит '=', цикл прерывается: */
		// if (input.current_operator == '=')
		// {
		// 	break;
		// }

		// input.count++;



	}

	/* Получить результат вычислений: */
		if (isnan(subtotal) || isinf(subtotal))
		{
			printf("Error in calculations.\n");
			// print_error(&input_pt, input);
		}
		else
			print_result(subtotal);


	return 0;
}