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
    /* Левая часть условия для того, чтобы не получить segmentation fault, если букв в файле многовато. */
    while ( (count < sizeof(buff)) && (buff[count] = fgetc(fp)) != EOF )
      count++;

    buff[count] = '\0';

    printf("%s\n", buff);
    fclose(fp);
  }

  /* 2.2. Файла нет? */
  else
    printf("The needed file \"%s\" hasn't been found! You may read README.md file to get "
    "the necessary information.\n", 
    filename);

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

  /* 3.1. Вычисления, приведшие к NaN или Infinity. */
  if (is_bad_num(input.tmp))
  {
    if (type_is_root(input.type))
      printf("  Invalid input: \"%s\". Input's been withdrawn. You may enter operator to continue "
      "your calculations with subtotal shown above.\n", input_str);

    else
      printf("  Invalid input: \"%s\". Input's been withdrawn. Please: 1) enter different number to finish "
      "this expression, or 2) enter operator to make different expression with subtotal shown above.\n", input_str);
  }

  /* 3.2. Если ввод начинается с оператора или символа 'r'. */
  else if (input.type == 'o' || input.type == 'r')
    printf("  Invalid input: \"%s\". Please enter number.\n", input_str);

  /* Если у input неопределенный тип. */
  else if (!input.type)
    printf("  Invalid input: \"%s\". Please enter something meaningful.\n", input_str);

  /* Если после ввода 'r' вводится число. */
  else if (type_is_num && input.opr == 'r')
    printf("  Invalid input: \"%s\". Please enter operator.\n", input_str);

  /* Гипотетические сценарии. */
  else
    printf("  Invalid input: \"%s\".\n", input_str);

  return;
}


/* 
  void print_subtotal(double subtotal, struct Input input);

  Стек:
    main / print_subtotal

  Функция print_subtotal.
    Выводит результаты вычислений, а также выражения, которые привели к означенным результатам.
*/
void print_subtotal(double subtotal, struct Input input)
{
  /* 1. Объекты. */
  int exp_tmp = 0; // показатель степени для операции '^'

  /* Если всюду нули, то и показывать нечего. */
  if (!subtotal && !input.tmp && !input.new_num)
    return;

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

  /* 3. Отдельный вывод промежуточного итога. */
  if (!is_bad_num(input.tmp))
    printf("  Subtotal is: %f\n", input.tmp);
  else
    printf("  Subtotal is: %f\n", subtotal);

  return;
}