#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Константы. */

#define MAX_SIZE 20 // макс. длина обычного массива
#define ZERO_ASCII 48 // индекс '0' в ASCII
#define SQRT_ASCII 251 // индекс символа sqrt в ASCII

/* Объекты. */

/* Структура для хранения данных и состояний input. */

struct Input
{
	char opr;
	double new_num;
	double old_num;
	double radicand;

	double tmp;

	char type;

	unsigned int has_percent : 1;
	unsigned int has_point : 1;
	unsigned int has_radical : 1;
	unsigned int has_sign : 1;
	unsigned int is_done : 1;
	unsigned int is_initialized : 1;
	unsigned int is_num : 1; // для identify_input
	unsigned int is_operator : 1; // для identify_input
	unsigned int is_percent : 1; // для identify_input
	unsigned int is_root : 1; // для identify_input
	unsigned int was_num : 1;
	unsigned int was_operator : 1;
	unsigned int was_percent : 1;
	unsigned int was_root : 1;
};


/* Функции "общего назначения". */

/* 
	int is_digit(char elem);

	Функция is_digit.
		Если (char elem) - цифра, возвращает 1.
		В противном случае возвращает 0.

		Самопальный вариант встроенной функции isdigit. 
*/
int is_digit(char elem)
{
	register int i = 0;

	for (i = 48; i <= 57; i++) // индексы ASCII
	{
		if (i == (int) elem)
			return 1;
	}

	return 0;
}


/* 
	int is_sign(char elem);

	Стек:
		main / get_num / is_sign
		main / identify_input / check_if_is_number / is_sign

	Функция is_sign.
		Если (char elem) - знак (символ '+' или '-'), возвращает 1. 
		В противном случае возвращает 0. 
*/
int is_sign(char elem)
{
	if (elem == '+' || elem == '-')
		return 1;
	else
		return 0;
}


/* 
	int is_point(char elem);

	Стек:
		main / get_num / is_point
		main / identify_input / check_if_is_number / is_point

	Функция is_point.
		Если (char elem) - точка (символ '.'), возвращает 1. 
		В противном случае возвращает 0. 
*/
int is_point(char elem)
{
	if (elem == '.')
		return 1;
	else
		return 0;
}



int is_percent(char elem)
{
	if (elem == '%')
		return 1;
	else
		return 0;
}

int is_radical(char elem)
{
	if (elem == 'r')
		return 1;
	else
		return 0;
}


/* 
	double alter_num_sign(double num);

	Стек:
		main / get_num / alter_num_sign

	Функция alter_num_sign.
		Принимает (double num) и возвращает данное число с противоположным знаком.
*/
double alter_num_sign(double num)
{
	return num - (num * 2);
}


/* 
	int get_str_length(char *str);

	Стек:
		main / identify_input / check_if_is_operator / get_str_length

	Функция init_mem.
		Возвращает длину строки (char *str). Самопальный вариант встроенной функции strlen.
*/
int get_str_length(char *str)
{
	int count = 0;

	while (str[count] != '\0')
		count++;

	return count;
}


/* 
	void init_mem(char **pt, int mem_amount);

	Стек:
		main / identify_input / check_if_is_operator / init_mem

	Функция init_mem.
		Выделяет указателю (char **pt) память для символов в нужном объеме (int mem_amount).
*/
void init_mem(char **pt, int mem_amount)
{
	*pt = (char*) malloc(mem_amount * sizeof(char)); 
	/* Прим. Символ char, в отличие от других типов данных, всегда занимает 1 байт в памяти. Будем считать это
		лишней предосторожностью. */

	if (*pt == NULL) // если выделить память не получилось
	{
		perror("Memory hasn't been allocated");
		exit(1);
	}
	
	return;
}


/*
	void fill_list_pt(char **pt, char *str);

	Стек:
		main / identify_input / check_if_is_operator / fill_list_pt

	Функция fill_list_pt.
		Берет строку (char *str) и присваивает ее символы указателю (char **pt) с выделенной в нужном 
		размере памятью.
*/
void fill_list_pt(char **pt, char *str)
{
	/* 1. Объекты. */
	register int i;
	register char *start = *pt; // запомнили положение "внутреннего" указателя до сдвига

	/* 2. Присваивание символов строки указателю: */
	for (i = 0; str[i] != '\0'; i++)
	{
		**pt = str[i];

		(*pt)++; // сдвигаем "внутренний" указатель
	}

	**pt = '\0'; // завершаем новую "строку" нулевым указателем

	*pt = start; // сбрасываем "внутренний" указатель: смещаем с конца на первую позицию (его состояние до цикла)

	return;
}


/* 
	double pow(double num, double exp);

	Функция pow.
		Рекурсивно возводит число num в степень exp.
*/
double pow(double num, double exp)
{
	if (exp == 1)
		return num;

	else
		return num * pow(num, exp - 1);
}


/* 
	double get_percentage(double num, double prc);

	Стек:
		main / get_num / get_percentage

	Функция get_percentage.
		Возвращает указанный процент от числа num.
*/
double get_percentage(double num, double prc)
{
	prc /= 100.0; // 75% -> 0.75
	return num * prc;
}