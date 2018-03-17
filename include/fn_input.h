/* Функции для работы с input. */

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
    Если (char input_type) - символ 'x', возвращает 1.
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
  double do_math(double subtotal, struct Input input);

  Стек:
    main / do_math

  Функция do_math.
    Возвращает результат вычислений.
*/
double do_math(double subtotal, struct Input input)
{
  /* 1. Объекты. */
  int exp_tmp = 0; // для case '^'

  /* 2. Вычисления. */
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
      /* Конвертировать показатель степени в целое число. */
      /* И если принятый показатель отличается от преобразованного, предупредить об этом. */
      exp_tmp = input.new_num;
      if ((double) exp_tmp != input.new_num)
        printf("  Please note that exponent's been converted to %d.\n", exp_tmp);

      /* Число в степени 0 равно 1. */
      /* 0 в степени 0 - это, по сути, неопределенность, но по примеру того же стандартного калькулятора Windows
      0 в степени 0 сделаем 1. */
      if (!input.new_num)
        return 1.0;

      /* Если показатель степени - отриц. число, воспользоваться формулой (a / b) ^ -n == (b / a) ^ n
      Число a нашем случае можно автоматом принять за 1. */
      else if (input.new_num < 0)
        return 1 / (pow(subtotal, alter_num_sign(exp_tmp)));

      else
        return pow(subtotal, exp_tmp);

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
  double get_num(char **input_pt);

  Стек:
    main / get_num

  Функция get_num.
    (char **input_pt) уже считается числом.
    Принимает означенную строку и "сооружает" из нее значение типа double.
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
  /* 1. Объекты. */
  char input_str[MAX_SIZE];
  char sample_str[] = "quit";

  register char count;

  /* 2. Перебор. */

  /* 2.1. Если строка имеет вид: {'=', '\0'} */
  if (***input_pt == '=' && !(*((**input_pt) + 1))) // такой синтаксис не смещает pt, в отличие от ++/--
    return 1;

  /* 2.2. Если строка - это фраза "quit". */
  else if (***input_pt == 'q') // если начинается не с 'q', то дальше и проверять не стоит
  {
    register char *start = **input_pt;
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

  /* 2.3. Прочие случаи. */
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

    (*) Числом может также считаться input вида "50%" - на этапе get_num будет преобразован в нужный вид.
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
    (**input_pt)++; // если есть знак ('-' или '+'), перешагнуть его - пока никак не обрабатывать

  while ( (elem = ***input_pt) != '\0')
  {
    /* Если в input более одной точки '.', значит, input - не число. Вернуть 0. */
    /* Тем не менее input вроде ".2" или "2." допустим (такие числа будут интерпретироваться как 
    0.2 или 2.0 соответственно, как у JS). */
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
  free(operators_list); // высвободить память после init_mem

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
  register char *start = **input_pt;

  if ( (***input_pt == 'r') && !(*((**input_pt) + 1)) )
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
    Если (char **input_pt) - "=" или "quit" возвращает 'x' (exit).
    Если (char **input_pt) - число, возвращает 'n' (number).
    Если (char **input_pt) - мат. оператор ('+', '*', ...), возвращает 'o' (operator).
    Если (char **input_pt) - корень (квадратный) 'r', возвращает 'r' (root).
    Если (char **input_pt) - некорректный ввод, возвращает '\0' (false).
*/
char identify_input(char **input_pt, double subtotal, struct Input input)
{
  /* 1. Объекты. Состояния. */
  struct Input input_state;
  input_state.is_quit_query = 0;
  input_state.is_num = 0;
  input_state.is_operator = 0;
  input_state.is_root = 0;

  /* 2. Результат. Определение типа input ('x', 'n', 'o', 'r' или '\0'). */
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