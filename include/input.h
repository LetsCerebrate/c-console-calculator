/* Объекты. */

/* Структура для хранения данных и состояний input. */
struct Input
{
	char opr;
	char type;
	double new_num;
	double radicand;
	double tmp;
	unsigned int has_percent : 1;
	unsigned int has_point : 1;
	unsigned int has_radical : 1;
	unsigned int has_sign : 1;
	unsigned int is_initialized : 1;
	unsigned int is_quit_query : 1;
	unsigned int is_num : 1;
	unsigned int is_operator : 1;
	unsigned int is_root : 1;
};


/* Функции для работы с input. */

/* Функции для определения типа input. */

/* 	
	int type_is_num(char input_type);

	Стек:
		main / type_is_num

	Функция type_is_num.
		Если (char input_type) - символ 'n', возвращает 1.
		В противном случае возвращает 0.
*/
int type_is_num(char input_type)
{
	if (input_type == 'n')
		return 1;
	else
		return 0;
}


/* 
	int type_is_quit_query(char input_type);

	Стек:
		main / type_is_quit_query

	Функция type_is_quit_query.
		Если (char input_type) - символ 'o', возвращает 1.
		В противном случае возвращает 0.
*/
int type_is_quit_query(char input_type)
{
	if (input_type == 'x')
		return 1;
	else
		return 0;
}


/* 
	int type_is_operator(char input_type);

	Стек:
		main / type_is_operator

	Функция type_is_operator.
		Если (char input_type) - символ 'o', возвращает 1.
		В противном случае возвращает 0.
*/
int type_is_operator(char input_type)
{
	if (input_type == 'o')
		return 1;
	else
		return 0;
}


/* 	
	int type_is_root(char input_type);

	Стек:
		main / type_is_root

	Функция type_is_root.
		Если (char input_type) - символ 'r', возвращает 1.
		В противном случае возвращает 0.
*/
int type_is_root(char input_type)
{
	if (input_type == 'r')
		return 1;
	else
		return 0;
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
	scanf("%s", input);

	return input;
}


/* 
	int get_correct_exp(double exp);

	Стек:
		main / do_math / get_correct_exp

	Функция get_correct_exp.
		Принимает число (показатель степени), приводит его к нужному виду, если он не соответствовал требованиям.
		Возвращает измененный, корректный показатель степени.
		Пример: -2.75 -> 2
*/
int get_correct_exp(double exp)
{
	int result = exp; // int значит, что дробная часть double будет отброшена автоматически

	if (exp < 0)
		result = alter_num_sign(exp);

	if ((double) result != exp)
		printf("  Please note that exponent's been converted into %d.\n", result);

	return result;
}


/*
	double do_math(double subtotal, struct Input input);

	Стек:
		main / do_math

	Функция do_math.
		Возвращает результат вычислений для промежуточного итога subtotal.
*/
double do_math(double subtotal, struct Input input)
{
	/* 1. Вычисления. */
	switch (input.opr)
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
			/* 0 в степени 0 пусть будет... 0 (хотя, в сущности, это некорректная операция). */
			if (!input.new_num && !subtotal)
				return 0.0;

			/* num (если num != 0) в степени 0 равно 1. */
			else if (!input.new_num)
				return 1.0;

			else
			{
				input.new_num = get_correct_exp(input.new_num); // "подкорректировать" показатель степени
				return pow(subtotal, input.new_num);
			}

		// case '%':
		// 	/* Узнать, сколько будет new_num процентов от subtotal. */
		// 	input.new_num = get_percentage(subtotal, input.new_num);

		// 	/* После этого очистить current_operator от '%', чтобы не оплошать на следующей инструкции, с рекурсией. */
		// 	input.current_operator = '\0';

		// 	 Повторно вызвать do_math, но уже не с оператором '%'; будет использоваться предыдущий оператор:
		// 	'+', '-', etc. 
		// 	subtotal_initial = do_math(subtotal, input);

		// 	/* Вывести данные о вычислениях с процентами. */
		// 	if (!input.current_operator)
		// 		print_subtotal(subtotal_initial, input);

		// 	return subtotal_initial;

		case 'r':
			return sqrt(input.tmp);
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
	return **input_pt; // получить 1-й элемент строки
}


/* 
	char reset_new_num(struct Input input);

	Стек:
		main / reset_new_num

	Функция reset_new_num.
		"Затирает" new_num после работы с процентами.
		Если в вычислениях участвуют '+' или '-', возвращает 0.0.
		Если в вычислениях участвуют '*' или '/', возвращает 1.0. (*)

		(*) 
		11.0 / 1.0
		11.0 - 0.0
*/
double reset_new_num(struct Input input)
{
	if (input.opr == '+' || input.opr == '-')
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
double get_num(char **input_pt, double subtotal)
{
	/* 1. Объекты. */
	/* Состояния для корректного представления результата. */
	struct Input input;

	input.has_percent = 0;
	input.has_point = 0;
	
	char elem = '\0'; // псевдоним для **input_pt внутри цикла

	char sign = '\0'; // знак числа (если указан)
	double result = 0.0, // итоговое число
		multiplier = 1.0, // множитель
		remainder = 1.0, // остаток для десятичной дроби
		tmp = 0.0; // "промежуточный результат" для result

	/* 2. Определение знака числа. */
	register char *start = *input_pt;

	if (is_sign(**input_pt)) // есть знак?
	{
		sign = **input_pt; // если указан знак, запомнить его
		(*input_pt)++; // и перешагнуть данный элемент
	}

	/* 3. Перебор input, "создание" числа. */
	while ( (elem = **input_pt) != '\0' )
	{
		/* Если натыкаемся на точку, фиксируем это и пропускаем ее (ее наличие отразится в дальнейшем 
		на переменной remainder). */
		/* Если натыкаемся на символ '%', также фиксируем это и пропускаем данный элемент. */
		if ( ( !input.has_point && (input.has_point = is_point(elem)) ) ||
		( !input.has_percent && (input.has_percent = is_percent(elem)) ))
		{
			(*input_pt)++;
			continue;
		}

		tmp = (double) (elem - (double) ASCII_IND_ZERO) / multiplier;
		result += tmp;

		/* Если есть точка (т.е. input - десятичная дробь). */
		if (input.has_point) // начать отсчет remainder начиная с элемента '.' для того
			remainder *= 10.0; // чтобы получить остаток

		multiplier *= 10; 
		
		(*input_pt)++;
	}

	*input_pt = start;

	/* 3. "Дошлифовка" результата. */
	result *= (multiplier / 10); // компенсировать лишнюю операцию multiplier *= 10

	if (input.has_point) // если число - десятичная дробь
		result /= remainder;

	if (sign == '-') // прописать нужный знак числу
		result = alter_num_sign(result);

	/* 4. Результат. */
	if (input.has_percent)
	{
		if (!subtotal)
			printf("  Please note that percantage of nothing is 0.");

		return get_percentage(subtotal, result);
	}

	else
		return result;
}

/* 
	int input_is_quit_query(char ***input_pt);

	Стек:
		main / identify_input / input_is_quit_query

	Функция input_is_quit_query.
		Если (char ***input_pt) указывает на строку вида "=" или "quit", возвращает 1. 
		В противном случае возвращает 0. 
*/
int input_is_quit_query(char ***input_pt)
{
	char input_str[MAX_SIZE];
	char sample_str[] = "quit";

	char count;

	/* Если строка имеет вид: {'=', '\0'} */
	if (***input_pt == '=' && !(*((**input_pt) + 1))) // такой синтаксис не смещает pt, в отличие от ++/--
		return 1;

	/* Если строка - это фраза "quit". */
	else if (***input_pt == 'q') // если начинается не с 'q', то дальше и проверять не стоит
	{
		char *start = **input_pt;
		count = 0;
	
		while (***input_pt != '\0')
		{
			input_str[count] = ***input_pt;
	
			count++;
			(**input_pt)++;
		}

		input_str[count] = '\0';
		**input_pt = start;

		if (!strcmp(input_str, sample_str)) // если строки input и sample идентичны
			return 1;
		else
			return 0;
	}

	else
		return 0;
}	


/* 
	int input_is_number(char ***input_pt);

	Стек:
		main / identify_input / input_is_number 

	Функция input_is_number.
		Если (char ***input_pt) - число, возвращает 1. (*)
		В противном случае возвращает 0. 

		(*) Числом может также считаться input вида "50%" или "81r" - на этапе get_num будет преобразован 
		в 0.5 или 9 соотв.
*/
int input_is_number(char ***input_pt)
{
	/* 1. Объекты. */
	struct Input input;
	input.has_percent = 0;
	input.has_point = 0;

	char elem = '\0'; // псевдоним для ***input_pt внутри цикла

	/* 2. Перебор input. */
	register char *start = **input_pt;

	if (is_percent(***input_pt))
		return 0; // число не может начинаться с символа '%'

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

		/* Элемент - '%' или цифра? Если ни одно из двух, вернуть 0. */
		else if ( !( 
			!input.has_percent && (input.has_percent = is_percent(elem)) || // (1)
			is_digit(elem) // (2)
		))
		/* 1) Если элемент - '%', ok, зафиксировать это. Символ '%' должен быть в ед. ч. */
		/* 2) Или: элемент - цифра? */

		{
			**input_pt = start;
			return 0;
		}

		(**input_pt)++;
	}

	/* Символ '%' должен быть последним значащим элементом массива, например: {'5', '%', '/0'} */
	if ( input.has_percent && !is_percent( *((**input_pt) - 1) ))
		return 0;

	**input_pt = start;

	return 1;
}


/* 
	int input_is_operator(char ***input_pt);

	Стек:
		main / identify_input / input_is_operator

	Функция input_is_operator.
		Если (char ***input_pt) - оператор, возвращает 1. 
		В противном случае возвращает 0. 
*/
int input_is_operator(char ***input_pt)
{
	/* 1. Объекты. */
	char operators_list_src[MAX_SIZE] = {'=', '+', '-', '*', '/', '^', '\0'};
	char *operators_list; // будем пользоваться данным указателем вместо строки src
	// for no particular reason, разминки ради

	/* 2. Привести operators_list в соответствие с src. */
	init_mem(&operators_list, get_str_length(operators_list_src) + 1); // плюс 1 байт под нулевой терминатор
	fill_list_pt(&operators_list, operators_list_src);

	/* 3. Перебор символов input, перебор символов operators_list: */
	register char *start_input = **input_pt;
	register char *start_operators_list = operators_list;	

	while (*operators_list != '\0')
	{
		/* Проверить, является ли 1-й элемент input оператором. */
		if ( *operators_list == ***input_pt )
		{
			/* Если является - сдвинуть указатель на input на 1 вправо, ко 2-му элементу, и прервать цикл. */
			(**input_pt)++;
			break;
		}

		operators_list++;
	}

	operators_list = start_operators_list;
	free(operators_list); // очистить память после malloc

	/* 4. Результат. */
	/* 4.1. Если 2-й элемент input - нулевой терминатор, значит, это оператор. */
	if (***input_pt == '\0')
	/* Прим. input вроде " + -  *" будет считаться верным, поскольку scanf останавливается на первом же пробеле после 
	значащего элемента, и таким образом получает следующую строку: {'+', '\0'}, что есть '+' */
	{
		**input_pt = start_input;
		return 1;
	}

	/* 4.2. Если 2-й элемент input - не '\0', значит, это не оператор. Вернуть 0. */
	else
	{
		**input_pt = start_input;
		return 0;
	}
}


/* 
	int input_is_root(char ***input_pt);

	Стек:
		main / identify_input / input_is_root

	Функция input_is_root.
		Если (char ***input_pt) - корень 'r', возвращает 1. 
		В противном случае возвращает 0. 
*/
int input_is_root(char ***input_pt)
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
	char identify_input(char **input_pt, double subtotal, struct Input input);

	Стек:
		main / identify_input

	Функция identify_input.
		Если (char **input_pt) - число, возвращает 'n' (number).
		Если (char **input_pt) - мат. оператор ('+', '*', ...), возвращает 'o' (operator). 
		Если (char **input_pt) - процент '%', возвращает 'p' (percent).
		Если (char **input_pt) - корень (квадратный) 'r', возвращает 'r' (root).
		Если (char **input_pt) - некорректное значение, возвращает '\0' (false).
*/
char identify_input(char **input_pt, double subtotal, struct Input input)
{
	/* 1. Объекты. Состояния. */
	struct Input input_state;
	input_state.is_quit_query = 0;
	input_state.is_num = 0;
	input_state.is_operator = 0;
	input_state.is_root = 0;

	/* 2. Результат. Определение типа input ('x', 'n', 'o' или 'r'). */
	if (input_state.is_quit_query = input_is_quit_query(&input_pt))
		return 'x';

	else if (input_state.is_root = input_is_root(&input_pt))
		return 'r';

	else if (input_state.is_operator = input_is_operator(&input_pt))
		return 'o';

	else if (input_state.is_num = input_is_number(&input_pt))
		return 'n';
		
	/* Если input некорректный (не соотв. ни одному типу). */
	else if ( (!input_state.is_quit_query && 
		!input_state.is_root && 
			!input_state.is_operator && 
				!input_state.is_num) )
		return '\0';
}



/* Функции для вывода данных. */

/* 
	void print_help();

	Стек:
		main / print_help

	Функция print_help.
		Выводит на экран содержимое файла справки.
*/
void print_help()
{
	/* 1. Объекты. */
	char filename[] = "doc/help",
		buff[5000]; // буфер для приема содержимого файла filename
	FILE *fp = NULL;

	register int count;

	/* 2. Попытаться открыть файл. */
	fp = fopen(filename, "r");

	/* 2.1. Есть такой файл? Тогда прочитать и вывести его содержимое на экран. */
	count = 0;

	if (fp)
	{
		/* Левая часть условия для того, чтобы не получить segmentation fault в случае чего. */
		while ( (count < sizeof(buff)) && (buff[count] = fgetc(fp)) != EOF )
			count++;

		buff[count] = '\0';

		printf("%s\n", buff);
		fclose(fp);
	}

	/* 2.2. Файл пропал? */
	else
		printf("The needed file \"%s\" hasn't been found! You may read README.md file to get the necessary information.\n", filename);

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

	/* 3. Результат. */

	/* Вычисления, приведшие к NaN или Infinity. */
	if (isnan(input.tmp) || isinf(input.tmp))
	{
		if (type_is_root(input.type))
			printf("  Invalid input: \"%s\". Input's been withdrawn. You may enter operator to continue your calculations with subtotal shown above.\n", 
			input_str);

		else
			printf("  Invalid input: \"%s\". Input's been withdrawn. Please: 1) enter different number to finish this expression, or 2) enter operator to make different expression with subtotal shown above.\n", 
			input_str);
	}

	/* Если ввод начинается с оператора или символа 'r'. */
	else if (input.type == 'o' || input.type == 'r')
		printf("  Invalid input: \"%s\". Please enter number.\n", input_str);

	/* Если у input неопределенный тип. */
	else if (!input.type)
		printf("  Invalid input: \"%s\". Please enter something meaningful.\n", input_str);

	/* Если после ввода 'r' вводится число. */
	else if (type_is_num && input.opr == 'r')
		printf("  Invalid input: \"%s\". Please enter operator.\n", input_str);

	/* Остальные гипотетические сценарии. */
	else
		printf("  Invalid input: \"%s\".\n", input_str);

	return;
}


/* 
	void print_subtotal(double subtotal, struct Input input);

	Стек:
		main / print_subtotal

	Функция print_subtotal.
		Выводит результаты вычислений, а также выражения.
*/
void print_subtotal(double subtotal, struct Input input)
{
	/* 1. Объекты. */
	int exp_tmp = 0; // показатель степени

	/* Если всюду нули, то и показывать нечего. */
	if (subtotal == 0 && input.tmp == 0 && input.new_num == 0)
		return;

	/* 2. Отдельный вывод промежуточного итога. */
	if (!is_bad_num(input.tmp))
		printf("  Subtotal is: %f\n", input.tmp);
	else
		printf("  Subtotal is: %f\n", subtotal);

	/* 3. Вывод на экран введенных выражений. */
	/* 3.1. Вывод выражений, приведших к NaN или Infinity. */
	if (is_bad_num(input.tmp))
	{
		/* Попытка извлечь корень из отриц. числа. */
		if (type_is_root(input.type))
			printf("  [%c%f] Invalid operation!\n", ASCII_IND_SQRT, input.radicand);

		/* Остальное (деление на 0, в частности). */
		else
		{
			if (input.opr)
				printf("  [%f %c %f] Invalid operation!\n", subtotal, input.opr, input.new_num);
			else
				printf("  Invalid operation!\n");
		}
	}

	/* 3.2. Вывод выражений с указанным оператором. */
	else if (input.opr)
	{
		/* Корень. */
		if (input.opr == 'r')
			printf("  [%c%f = %f]\n", ASCII_IND_SQRT, subtotal, input.tmp);

		/* Возведение в степень. */
		/* Приводит показатель степени к удобоваримому виду: -3.75 -> 3 */
		else if (input.opr == '^')
		{
			exp_tmp = input.new_num;
			if (exp_tmp < 0)
				exp_tmp = (int) alter_num_sign((double) exp_tmp);

			printf("  [%f %c %d = %f]\n", subtotal, input.opr, exp_tmp, input.tmp);
		}

		/* Прочие бинарные операторы. */
		else
			printf("  [%f %c %f = %f]\n", subtotal, input.opr, input.new_num, input.tmp);
	}

	return;
}