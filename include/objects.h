#include <stdio.h>
#include <stdlib.h>

#ifndef MAX_SIZE
	#define MAX_SIZE 20
#endif
#ifndef ZERO_ASCII
	#define ZERO_ASCII 48.0 // 0 has index 48 in ASCII
#endif

/* Объекты */

struct State // структура для хранения состояний (1 или 0)
{
	unsigned int has_point : 1;
	unsigned int has_sign : 1;
	// unsigned int has_error : 1;
	// unsigned int is_initialized : 1;
	unsigned int is_num : 1;
	unsigned int is_operator : 1;
	unsigned int was_num : 1;
	unsigned int was_operator : 1;
};

struct Input // структура для хранения данных об input
{
	int subtotal_is_initialized;
	char prev_operator;
	char prev_operator_backup;
	char current_operator;
	char type;
	double new_num;
};


/* Функции для вывода данных. */

/*
	void print_help();

	...

*/
void print_help()
{
	printf("Acceptable operators are follows:\n  +\n  -\n  *\n  /\n  ^\n  =\n");
	return;
}


/* 
	void print_error(char **input_pt, struct Input input);

	Стек:
		main / print_error

	Функция print_error.
		Вывести сообщение о некорректном input.
*/
void print_error(char **input_pt, struct Input input)
{
	char input_str[MAX_SIZE];

	char *start = *input_pt;
	register int i;

	for (i = 0; **input_pt != '\0'; i++)
	{
		input_str[i] = **input_pt;

		(*input_pt)++;
	}

	input_pt[i] = '\0';

	*input_pt = start;

	if (input.type == 'n')
		printf("Invalid input: \"%s\". Please enter operator.\n", input_str);
	else if (input.type == 'o')
		printf("Invalid input: \"%s\". Please enter number.\n", input_str);

	return;
}


/* 
	void print_result(double subtotal);

	Стек:
		main / print_result

	Функция print_result.
		Вывести значение (double subtotal).
*/
void print_result(double subtotal)
{
	printf("Result is: %f\n", subtotal);
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
	/* 1. Хранилище значений: */
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


/*
	double get_subtotal(double subtotal, struct Input input);

	Стек:
		main / get_subtotal

	Функция get_subtotal.
		Возвращает результат вычислений (double subtotal) и (input.new_num).
*/
double get_subtotal(double subtotal, struct Input input)
{
	char opr = input.prev_operator ; // input.prev_operator_backup;


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
	}
}


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
	double get_num(char **input_pt);

	Стек:
		main / get_num

	Функция get_num.
		(char **input_pt) уже считается числом.
		Принимает означенную строку и "сооружает" из нее значение double.
*/
double get_num(char **input_pt)
{
	/* 1. Хранилище значений: */
	struct State input;
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

		tmp = (double) (elem - ZERO_ASCII) / multiplier;
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
	/* 1. Хранилище значений: */
	char operators_list_src[MAX_SIZE] = {'=', '+', '-', '*', '/', '^', '\0'}; // м.б., '=' вынести отсюда?
	char *operators_list; // будем пользоваться данным указателем вместо строки src
	// for no particular reason, просто для разминки

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
	/* 1. Хранилище значений: */
	struct State input;
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
	char identify_input(char **input_pt, double subtotal, struct Input input);

	Стек:
		main / identify_input

	Функция identify_input.
		Если (char **input_pt) - оператор ('+', '*', ...), возвращает 'o'. 
		Если (char **input_pt) - число, возвращает 'n'.
		Если (char **input_pt) - некорректное значение, возвращает '\0' (false).
*/
char identify_input(char **input_pt, double subtotal, struct Input input)
{
	/* 1. Хранилище значений: */
	struct State input_state;
	input_state.is_num = 0;
	input_state.is_operator = 0;
	// input.is_mem = 0;

	/* 2. Результат. Определение типа input ('o' operator или 'n' number). */

	/* Если operator 'o': */
	if (input_state.is_operator = check_if_is_operator(&input_pt))
		return 'o';

	/* Если number 'n': */
	else if (input_state.is_num = check_if_is_number(&input_pt))
		return 'n';
		
	/* Если input некорректный (ни 'o', ни 'n'): */
	else if ( (!input_state.is_num && !input_state.is_operator) )
		return '\0';
}