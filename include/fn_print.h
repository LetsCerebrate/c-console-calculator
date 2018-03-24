/* Функции для вывода данных. */

/* 
  void print_subtotal(double subtotal, struct Input input);

  Стек:
    main / print_subtotal

  Функция print_subtotal.
    Выводит результаты вычислений, а также выражения, которые привели к означенным результатам.
*/
void print_subtotal(double subtotal, struct Input input, struct Memory memory)
{
  /* 1. Объекты. */
  int exp_tmp = 0; // показатель степени для операции '^'
  register int i;

  /* Если всюду нули, то и показывать нечего. */
  /* Однако это не распространяется на команду "c" (clear). */
  if (!subtotal && !input.tmp && !input.new_num && !input.keyword_code == 3)
    return;

  for (i = 7; i <= 9; i++) // [7; 9]
    if (input.keyword_code == i)
    {
      printf("  Memory cell #%d has: %f\n", memory.index, memory.cells[memory.index - 1]);
      return;
    }

  /* 2. Вывод на экран введенных выражений. */
  /* 2.1. Вывод выражений, приведших к NaN или Infinity. */
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

  /* 2.2. Вывод выражений с указанным оператором. */
  else if (input.opr)
  {
    /* Операции с квадратным корнем. */
    if (input.opr == 'r')
      printf("  [%c%f = %f]\n", ASCII_IND_SQRT, subtotal, input.tmp);

    /* Операция возведение в степень. */
    else if (input.opr == '^')
    {
      exp_tmp = input.new_num; // 3.75 -> 3
      printf("  [%f %c %d = %f]\n", subtotal, input.opr, exp_tmp, input.tmp);
    }

    /* Прочие операции с бинарными операторами. */
    else
      printf("  [%f %c %f = %f]\n", subtotal, input.opr, input.new_num, input.tmp);
  }

  /* 3. Отдельный вывод промежуточного итога и значения в 1-й ячейке памяти, если таковое отлично от 0. */
  if (memory.cells[0])
  {
    if (!is_bad_num(input.tmp))
      printf("  Subtotal is: %f / Memory cell #1 has: %f\n", input.tmp, memory.cells[0]);
    else
      printf("  Subtotal is: %f / Memory cell #1 has: %f\n", subtotal, memory.cells[0]);
  }
  else
  {
    if (!is_bad_num(input.tmp))
      printf("  Subtotal is: %f\n", input.tmp);
    else
      printf("  Subtotal is: %f\n", subtotal);
  }

  return;
}


/* 
  void print_help();

  Стек:
    main / print_help

  Функция print_help.
    Читает файл справки и выводит на экран его содержимое.
*/
void print_help(char filename[])
{
  /* 1. Объекты. */
  char buff[BUFF_SIZE]; // буфер для приема содержимого файла filename
  FILE *fp = NULL;
  register int count;

  /* 2. Попытаться открыть файл. */
  fp = fopen(filename, "r");

  /* 2.1. Есть такой файл? Тогда прочитать его и вывести текст на экран. */
  count = 0;

  if (fp)
  {
    /* Левая часть условия для того, чтобы не схватить segmentation fault, если букв в файле многовато. */
    while ( (count < sizeof(buff)) && (buff[count] = fgetc(fp)) != EOF )
      count++;

    buff[count] = '\0';
    printf("%s\n", buff);
    fclose(fp);
  }
  
  /* 2.2. Файла нет? */
  else
  {
    perror("Missed file");
    exit(1);
  }

  return;
}


/* 
  void print_error(char **input_pt, struct Input input);

  Стек:
    main / print_error

  Функция print_error.
    Выводит сообщение о некорректном input.
*/
void print_error(char **input_pt, struct Input input, struct Memory memory, int error_code)
{
  /* 1. Объекты. */
  char input_str[MAX_SIZE];

  register char *start = *input_pt;
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

  /* Если у input неопределенный тип. */
  if (error_code == 1)
    printf("  Invalid input: \"%s\". Please enter something meaningful.\n", input_str);

  /* 3.1. Вычисления, приведшие к NaN или Infinity. */
  else if (error_code == 2)
  {
    if (type_is_root(input.type))
      printf("  Invalid input: \"%s\". Input's been withdrawn. You may enter operator to continue "
      "your calculations with subtotal shown above.\n", input_str);

    else
      printf("  Invalid input: \"%s\". Input's been withdrawn. Please: 1) enter different number to finish "
      "this expression, or 2) enter operator to make different expression with subtotal shown above.\n", input_str);
  }

  /* Если после ввода 'r' вводится число. */
  else if (error_code == 3)
    printf("  Invalid input: \"%s\". Please enter operator.\n", input_str);

  /* 3.2. Если ввод начинается с оператора или символа 'r'. */
  else if (error_code == 4)
    printf("  Invalid input: \"%s\". Please enter number.\n", input_str);

  /* Работа с памятью калькулятора. Попытка получить доступ к элементу массива за границами последнего. */
  else if (error_code == 5)
    printf("  There's no such cell. Overall number of available cells is %d.\n", MAX_SIZE);

  /* Остальное. */
  else
    printf("  Invalid input: \"%s\".\n", input_str);

  return;
}


/* 
  void print_all_memcells();

  Стек:
    main / print_all_memcells

  Функция print_all_memcells.
    Выводит на экран содержимое всех занятных ячеек памяти.
*/
void print_all_memcells(struct Memory *memory)
{
  struct Memory cell;
  cell.has_value = 0;

  register int i;

  for (i = 0; i < MAX_SIZE; i++)
    if ( memory -> cells[i] && (cell.has_value = 1) )
      printf("  Memory cell #%d has: %f\n", i + 1, memory -> cells[i]);

  if (!cell.has_value)
    printf("  All memory cells have 0.\n");

  return;
}