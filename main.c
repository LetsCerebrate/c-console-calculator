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

	/* 1. Объекты. */

	char *input_pt; // input хранится в статической переменной
	double subtotal = 0.0; // промежуточный итог

	/* input - структура для данных об input. */

	struct Input input;

	input.is_done = 0; // введено ли '='
	input.subtotal_is_initialized = 0; // присвоено ли переменной subtotal первое число, введенное пользователем

	input.current_operator = '\0'; // последний введенный оператор
	input.prev_operator = '\0'; // оператор, введенный в предыдущем input - используется для расчетов в get_subtotal
	input.new_num = 0.0; // последнее введенное число
	input.old_num = 0.0; // "старое" число; в вычислениях не участвует, требуется для корректной работы print_subtotal
	input.type = '\0'; // тип значения текущего input

	/* last_input - структура для состояний о последнем input. */

	struct Input last_input;

	last_input.was_num = 0; // число, т.е. операнд
	last_input.was_operator = 0; // бинарный оператор: '+', '-', ... , '^'
	last_input.was_percent = 0; // процент: '%'
	last_input.was_root = 0; // корень (квадратный): 'r'

	/* Начало. */

	printf("Please enter what you want to calculate. Enter \"=\" to get subtotal and quit.\n");

	/* 2. Основная часть. */

	while (1) // цикл прервется, если будет введено '='
	{
		/* 1. Запрос input. */
		/* А также определение типа input. */
		input_pt = query_input();
		input.type = identify_input(&input_pt, subtotal, input); // получаем 'o', 'n', 'p', 'r' или '\0'

		/* Знак '=' необходимо обработать особо. */
		/* Если введен '=', при проверке input.is_done программа будет завершена. */
		if (is_operator(input.type) && (get_operator(&input_pt) == '='))
			input.is_done = 1;


		// printf("| | 1. subtotal? %f\n", subtotal);
		// printf("| | 2. new_num? %f\n", input.new_num);
		// printf("| | 3. current_operator? %c\n", input.current_operator);
		// printf("| | 4. prev_operator? %c\n", input.prev_operator);
		// printf("| | 5. type? %c\n\n", input.type);	

		// printf("| | 1. was_operator? %d\n", last_input.was_operator);
		// printf("| | 2. was_num? %d\n", last_input.was_num);
		// printf("| | 3. was_percent? %d\n\n", last_input.was_percent);	

		/* 2. Обработка некорректного input. */

		/* 2.1. Собственно ошибка. */
		if ( !input.type || 
			( (!is_num(input.type) && (!last_input.was_num && !last_input.was_percent && !last_input.was_root)) ) ||
			( is_operator(input.type) && last_input.was_operator ) ||
			( is_num(input.type) && last_input.was_num ) ||
			( is_percent(input.type) && (last_input.was_percent || last_input.was_root) ) )
		/* Ветка срабатывает, если тип input неизвестен, т.е. '\0' */
		/* Или: если ввод начинается не с числа. */
		/* Или: если после ввода, например, числа вводится не оператор, а опять число - т.е. input того же типа. Однако
		на извлечение корня данное правило не распространяется. */
		{
			/* Введено ли '='? Если да, прервать цикл. */
			if (input.is_done)
				break;

			print_error(&input_pt, input);

			continue;
			/* Если input некорректный, не извлекать данные из него: прервать итерацию. */
		}

		/* 2.2. Состояния, необходимые для идентификации некорректного input. */
		/* Показывают, какого типа был предыдущий input. */

		last_input.was_operator = is_operator(input.type);
		last_input.was_num = is_num(input.type);
		last_input.was_percent = is_percent(input.type);
		last_input.was_root = is_root(input.type);
	
		/* 3. Извлечение данных из корректного input. */

		/* 3.1. Если тип input - 'n', извлечь число из input. */
		/* Для вычислений используется 2 числа: new_num и subtotal. Здесь мы обозначаем new_num. */
		if (input.type && is_num(input.type))
			input.new_num = get_num(&input_pt);

		/* 3.2. Если тип input - 'o', 'p' или 'r', извлечь оператор из input. */
		else
			input.current_operator = get_operator(&input_pt); // получаем оператор

		/* 4. Обработка извлеченных данных. */

		/* Вычисления производятся только если текущий input имеет тип 'o', 'p' или 'r'. */
		/* Однако типы 'p' и 'r' участвуют не во всех нижеприведенных операциях. */
		if (input.type && !is_num(input.type))
		{
			/* 4.1. "Инициализация" промежуточного итога subtotal. */
			/* Производится только 1 раз за цикл (очевидно), когда поступает 1-й new_num. */
			/* В дальнейшем subtotal изменяется уже на основании вычислений. */
			if (!input.subtotal_is_initialized && !input.prev_operator)
			{
				subtotal = input.new_num;
				input.subtotal_is_initialized = 1;
			}

			/* 4.2. Произвести вычисления. */
			/* Только если subtotal уже "инициализирован". */
			else
			{
				input.old_num = subtotal; // как указывалось выше, old_num предназначен для print_subtotal
				subtotal = get_subtotal(subtotal, input); // получить результат вычислений

				/* Вывести на экран промежуточный итог subtotal. */
				/* Но только если данный input имеет тип 'o' или 'r' (иначе при работе с процентами 'p' будет выводиться 
				"лишний" subtotal). */
				if (is_operator(input.type) || is_root(input.type))
					print_subtotal(subtotal, input);
				
				/* Если тип данного input - 'p', нужно "сбросить" значение new_num, чтобы корректно завершить вычисления, т.к.
				new_num - процент. */
				if (is_percent(input.type))
					input.new_num = reset_new_num(input); // в зависимости от prev_operator присвоить 1.0 или 0.0
			}
		}

		/* 5. Данные о текущем input для обработки следующего. */
		/* '%' не сохранять! В функции get_subtotal случай с оператором '%' будет обрабатываться особо. */
		if (input.current_operator && (input.current_operator != '%'))
			input.prev_operator = input.current_operator;

		/* Введено ли '='? Если да, прервать цикл. */
		if (input.is_done)
			break;
	}

  /* 6. Вывести итоговый результат. */
  /* Не распространяется на случаи с некорректным вводом: в таких случая происходит exit(1) раньше. */
	printf("Result is: %f\n", subtotal);

	return 0;
}