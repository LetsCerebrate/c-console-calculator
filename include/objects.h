#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// #ifndef MAX_SIZE
	#define MAX_SIZE 20
// #endif
// #ifndef ZERO_ASCII
	#define ZERO_ASCII 48 // 0 has index 48 in ASCII
// #endif

#define SQRT_ASCII 251

/* Объекты */


struct Input // структура для хранения данных об input
{
	char current_operator;
	char prev_operator;
	double new_num;
	double old_num;
	char type;

	unsigned int has_point : 1;
	unsigned int has_sign : 1;
	unsigned int is_done : 1;
	unsigned int is_num : 1; // for identify_input
	unsigned int is_operator : 1; // for identify_input
	unsigned int is_percent : 1; // for identify_input
	unsigned int is_root : 1; // for identify_input
	unsigned int subtotal_is_initialized : 1;
	unsigned int was_num : 1;
	unsigned int was_operator : 1;
	unsigned int was_percent : 1;
	unsigned int was_root : 1; // not needed?


};



/* 
	int is_operator(char input_type);

	Стек:
		main / is_operator

	Функция is_operator.
		Если (char input_type) - символ 'o', возвращает 1.
		В противном случае возвращает 0.
*/
int is_operator(char input_type)
{
	if (input_type == 'o')
		return 1;
	else
		return 0;
}


/* 	
	int is_num(char input_type);

	Стек:
		main / is_num

	Функция is_num.
		Если (char input_type) - символ 'n', возвращает 1.
		В противном случае возвращает 0.
*/
int is_num(char input_type)
{
	if (input_type == 'n')
		return 1;
	else
		return 0;
}


/* 	
	int is_percent(char input_type);

	Стек:
		main / is_percent

	Функция is_percent.
		Если (char input_type) - символ 'p', возвращает 1.
		В противном случае возвращает 0.
*/
int is_percent(char input_type)
{
	if (input_type == 'p')
		return 1;
	else
		return 0;
}


/* 	
	int is_root(char input_type);

	Стек:
		main / is_root

	Функция is_root.
		Если (char input_type) - символ 'r', возвращает 1.
		В противном случае возвращает 0.
*/
int is_root(char input_type)
{
	if (input_type == 'r')
		return 1;
	else
		return 0;
}



/* Функции для вывода данных. */

/*
	void print_help();

	...

*/
void print_help()
{
	printf("Acceptable operators are follows:\n  +\n  -\n  *\n  /\n  ^\n  %\n  r\n  =\n");
	return;
}


/* 
	void print_error(char **input_pt, struct Input input);

	Стек:
		main / print_error

	Функция print_error.
		Выводит сообщение о некорректном input.
*/
void print_error(char **input_pt, struct Input input)
{
	/* 1. Объекты. */
	char input_str[MAX_SIZE];

	char *start = *input_pt;
	register int i;

	/* 2. Определение input. */
	/* Данный input будет выведен в сообщении об ошибке. */
	for (i = 0; **input_pt != '\0'; i++)
	{
		input_str[i] = **input_pt;

		(*input_pt)++;
	}

	input_str[i] = '\0';

	*input_pt = start;

	/* 3. Результат: */
	if (input.type == 'n')
		printf("Invalid input: \"%s\". Please enter operator.\n", input_str);
	else if (input.type == 'o')
		printf("Invalid input: \"%s\". Please enter number.\n", input_str);
	else if (input.type == 'p')
		printf("Invalid input: \"%s\". Please enter operator.\n", input_str);
	else if (!input.type)
		printf("Invalid input: \"%s\". Please enter something meaningful.\n", input_str);

	return;
}


/* 
	void print_subtotal(double subtotal, struct Input input);

	Стек:
		main / print_subtotal
		main / get_subtotal / print_subtotal

	Функция print_subtotal.
		Выводит результаты вычислений.
*/
void print_subtotal(double subtotal, struct Input input)
{
	/* 1. Ошибка в вычислениях (недопустимая операция). */
	if ( (isnan(subtotal) || isinf(subtotal)) )
	{
		/* 1.1. Недопустимая операция с процентами. */
		/* Если input начинается со ввода процентов. */
		if (input.old_num == input.new_num)
			printf("[? and %f%%] Invalid operation!\n", input.old_num);

		/* 1.2. Прочие недопустимые операции. */
		else
		{
			printf("[%f %c %f] Invalid operation!\n", input.old_num, input.prev_operator, input.new_num);
			printf("Subtotal was before last operation occured: %f\n", input.old_num);
		}

		/* 1.3. Закрыть программу. */
		exit(1);
	}

	/* 2. Корректные вычисления. */

	/* 2.1. Вычисления с квадратным корнем. */
	else if ( input.prev_operator == 'r' )
		printf("[%c%f = %f]\n", SQRT_ASCII, input.old_num, subtotal);

	/* 2.2. Все прочие вычисления. */
	else if ( (input.old_num != subtotal) ) // отфильтровать лишние вызовы printf при работе с процентами
		printf("[%f %c %f = %f]\n", input.old_num, input.prev_operator, input.new_num, subtotal);

	return;
}


/* Функции для выделения памяти указателю. */

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


/* Функции общего назначения. */

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

double get_percentage(double subtotal, double new_num) // (2, 50)
{
	// subtotal - число
	// input.new_num - процент от числа
	new_num /= 100.0; // 50 -> 0.5
	return subtotal * new_num;
}


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


/* Функции для обработки input. */

/* 
	char * query_input();

	Стек:
		main / query_input

	Функция query_input.
		Производит запрос на ввод input и возвращает указатель на строку (т.е. указатель на данный input).
*/
char * query_input()
{
	static char input[MAX_SIZE];

	// Enter ... any number or a mathematical operator. To get help, run the program with 'h' or 'help' argument.\n
	scanf("%s", input);

	return input;
}



int get_correct_exp(double exp)
{
	int result = exp; // int значит, что дробная часть double будет отброшена автоматически

	if (exp < 0)
		result = alter_num_sign(exp);

	if ((double) result != exp)
		printf("Please note that exponent's been converted into %d.\n", result);

	return result;
}

/*
	double get_subtotal(double subtotal, struct Input input);

	Стек:
		main / get_subtotal

	Функция get_subtotal.
		Возвращает результат вычислений (double subtotal) и (input.new_num).
*/
double get_subtotal(double subtotal, struct Input input)
{
	char opr = '\0';
	double subtotal_initial = subtotal;

	if (input.current_operator == '%') // '%' не записывается в input.prev_operator, поэтому его нужно обработать отдельно
		opr = '%';
	else
		opr = input.prev_operator;

	switch (opr)
	{
		case '+':
			return subtotal + input.new_num;

		case '-':
			return subtotal - input.new_num;

		case '*':
			return subtotal * input.new_num;

		case '/':
			return subtotal / input.new_num;

		case '^':
			if (!input.new_num)
				return 1.0; // num (если num != 0) в степени 0 равно 1

			else if (!input.new_num && !subtotal)
				return 0.0; // 0 в степени 0 пусть будет... 0 (хотя, в сущности, это некорректная операция)

			else
			{
				input.new_num = get_correct_exp(input.new_num);
				return pow(subtotal, input.new_num);
			}

		case '%':
			/* Узнать, сколько будет input.new_num процентов от subtotal: */
			input.new_num = get_percentage(subtotal, input.new_num);

			/* После этого очистить current_operator от '%', чтобы не оплошать на следующей инструкции, с рекурсией: */
			input.current_operator = '\0';

			/* Повторно вызвать данную функцию, но уже не с оператором '%' (будет использоваться предыдущий оператор:
			'+', '-', etc): */
			subtotal_initial = get_subtotal(subtotal, input);

			if (!input.current_operator)
				print_subtotal(subtotal_initial, input);

			return subtotal_initial;

		case 'r':
			return sqrt(subtotal);
	}
}


/* 
	char get_operator(char **input_pt);

	Стек:
		main / get_operator

	Функция get_operator.
		(char **input_pt) уже считается оператором.
		Принимает означенную строку и возвращает оператор.
*/
char get_operator(char **input_pt)
{
	return **input_pt; // получить 1-й элемент
}


/*

..

*/
double reset_new_num(struct Input input)
{
	if (input.prev_operator == '+' || input.prev_operator == '-')
		return 0.0;
	else
		return 1.0;
}


/* 
	double get_num(char **input_pt);

	Стек:
		main / get_num

	Функция get_num.
		(char **input_pt) уже считается числом.
		Принимает означенную строку и "сооружает" из нее значение double.
*/
double get_num(char **input_pt)
{
	/* 1. Объекты. */
	struct Input input;
	input.has_point = 0;
	
	char elem = '\0'; // псевдоним для **input_pt внутри цикла

	char sign = '\0'; // знак числа (если указан)
	double result = 0.0, // итоговое число
		multiplier = 1.0, // множитель
		remainder = 1.0, // остаток для десятичной дроби
		tmp = 0.0; // "промежуточный результат" для result

	/* 2. Определение знака числа: */
	register char *start = *input_pt;

	if (is_sign(**input_pt)) // есть знак?
	{
		sign = **input_pt; // если указан знак, запомнить его
		(*input_pt)++; // и перешагнуть данный элемент
	}

	/* 3. Перебор input, "создание" числа: */
	while ( (elem = **input_pt) != '\0' )
	{
		/* если натыкаемся на точку, фиксируем это и пропускаем ее (ее наличие отразится в дальнейшем 
			на переменной remainder). */
		if ( !input.has_point && (input.has_point = is_point(elem)) )
		{
			(*input_pt)++;
			continue;
		}

		tmp = (double) (elem - (double) ZERO_ASCII) / multiplier;
		result += tmp;

		/* Если есть точка (т.е. input - десятичная дробь): */
		if (input.has_point) // начать отсчет remainder начиная с элемента '.'
			remainder *= 10.0; // чтобы получить остаток

		multiplier *= 10; 
		
		(*input_pt)++;
	}

	*input_pt = start;

	/* 3. "Дошлифовка": */
	result *= (multiplier / 10); // компенсировать лишнюю операцию multiplier *= 10

	if (input.has_point) // если число - десятичная дробь
		result /= remainder;

	/* 4. Результат: */
	if (sign == '-')
		return alter_num_sign(result);
	else
		return result;
}



/* 
	int check_if_is_operator(char ***input_pt);

	Стек:
		main / identify_input / check_if_is_operator

	Функция check_if_is_operator.
		Если (char ***input_pt) - оператор, возвращает 1. 
		В противном случае возвращает 0. 
*/
int check_if_is_operator(char ***input_pt)
{
	/* 1. Объекты. */
	char operators_list_src[MAX_SIZE] = {'=', '+', '-', '*', '/', '^', '%', '\0'}; // м.б., '=' вынести отсюда?
	char *operators_list; // будем пользоваться данным указателем вместо строки src
	// for no particular reason, разминки ради

	init_mem(&operators_list, get_str_length(operators_list_src) + 1); // (n + 1) - плюс место под нулевой терминатор
	fill_list_pt(&operators_list, operators_list_src);

	/* 2. Перебор символов input, перебор символов math_operators: */
	register char *start_input = **input_pt;
	register char *start_operators_list = operators_list;	

	while (*operators_list != '\0')
	{
		if ( *operators_list == ***input_pt ) // проверить, является ли 1-й элемент input оператором
		{
			(**input_pt)++; // если является - сдвинуть указатель на input на 1 вправо, ко 2-му элементу, и прервать цикл
			break;
		}

		operators_list++;
	}

	operators_list = start_operators_list;
	free(operators_list); // очистить память после malloc

	/* 3. Результат: */
	if (***input_pt == '\0') // если 2-й элемент input - нулевой терминатор, значит, это оператор.
	/* Прим. input вроде " + -  *" будет считаться верным, поскольку scanf останавливается на первом же пробеле после 
		значащего элемента, и таким образом получает следующую строку: {'+', '\0'}. */
	{
		**input_pt = start_input;
		return 1;
	}

	else // если 2-й элемент input - не '\0', значит, это не оператор. Вернуть 0.
	{
		**input_pt = start_input;
		return 0;
	}
}




int check_if_is_root(char ***input_pt)
{
	char *start = **input_pt;

	if ( (***input_pt == 'r') && ( *((**input_pt) + 1) == '\0' ) )
	{
		**input_pt = start;
		return 1;
	}

	**input_pt = start;
	return 0;
}



/*

*/
int check_if_is_percent(char ***input_pt)
{
	char *start = **input_pt;

	if ( (***input_pt == '%') && ( *((**input_pt) + 1) == '\0' ) )
	{
		**input_pt = start;
		return 1;
	}

	**input_pt = start;
	return 0;
}


/* 
	int check_if_is_number(char ***input_pt);

	Стек:
		main / identify_input / check_if_is_number 

	Функция check_if_is_number.
		Если (char ***input_pt) - число, возвращает 1. 
		В противном случае возвращает 0. 
*/
int check_if_is_number(char ***input_pt)
{
	/* 1. Объекты. */
	struct Input input;
	input.has_point = 0;

	char elem = '\0'; // псевдоним для ***input_pt внутри цикла

	/* 2. Перебор input: */
	register char *start = **input_pt;

	if (is_sign(***input_pt))
		(**input_pt)++; // если есть знак, перешагнуть его - пока никак не обрабатывать

	while ( (elem = ***input_pt) != '\0')
	{
		/* Если в input более одной точки '.', значит, input - не число. Вернуть 0. */
		/* Тем не менее input вроде ".2" или "2." допустим (такие числа будут интерпретироваться как 
			0.2 или 2.0 соответственно - как в JS). */
		if (is_point(elem) && input.has_point)
		{
			**input_pt = start;
			return 0;
		}	

		/* Элемент - точка? Определяется 1-я (и единственная допустимая) точка. */
		if ( !input.has_point && (input.has_point = is_point(elem)) )
		{
			(**input_pt)++;
			continue; 
		}

		/* Если элемент - не точка и не цифра, значит, input - не число. Вернуть 0. */
		if (!is_digit(elem))
		{
			**input_pt = start;
			return 0;
		}

		(**input_pt)++;
	}

	**input_pt = start;

	return 1;
}


/*
...
*/
// int is_any_opr(char type)
// {
// 	if (type == 'o' || type == 'p' || type == 'r')
// 		return 1;
// 	else
// 		return 0;
// }


/* 
	char identify_input(char **input_pt, double subtotal, struct Input input);

	Стек:
		main / identify_input

	Функция identify_input.
		Если (char **input_pt) - оператор ('+', '*', ...), возвращает 'o' (operator). 
		Если (char **input_pt) - число, возвращает 'n' (number).
		Если (char **input_pt) - корень или процент, возвращает 'p' (postfix operator * ).
		Если (char **input_pt) - некорректное значение, возвращает '\0' (false).

		* Квадратный корень в данной реализации калькулятора считается постфиксным оператором.
		Например, (4r == 2)
*/
char identify_input(char **input_pt, double subtotal, struct Input input)
{
	/* 1. Объекты. */
	struct Input input_state;
	input_state.is_num = 0;
	input_state.is_operator = 0;
	input_state.is_percent = 0;
	input_state.is_root = 0;

	/* 2. Результат. Определение типа input ('o', 'n' или 'p'). */
	if (input_state.is_percent = check_if_is_root(&input_pt))
		return 'r';

	if (input_state.is_percent = check_if_is_percent(&input_pt))
		return 'p';

	/* Если operator 'o': */
	if (input_state.is_operator = check_if_is_operator(&input_pt))
		return 'o';

	/* Если number 'n': */
	else if (input_state.is_num = check_if_is_number(&input_pt))
		return 'n';
		
	/* Если input некорректный (ни 'o', ни 'n'): */
	else if ( (!input_state.is_num && !input_state.is_operator && !input_state.is_percent) )
		return '\0';
}