/* Объекты. */

/* Константы. */
#define BUFF_SIZE 5000 // макс. длина массива-буфера (приемника для file pointer)
#define MAX_SIZE 20 // макс. длина обычного массива
#define SAMPLE_ARR_SIZE 9 // длина массива sample_strings (проверка ключевых слов)
#define ASCII_IND_ZERO 48 // индекс символа '0' в ASCII
#define ASCII_IND_SQRT 251 // индекс символа sqrt в ASCII

/* Структура для хранения данных и состояний input. */
struct Input
{
  char opr;
  char type;
  double new_num;
  double radicand;
  double tmp;
  int keyword_code;
  unsigned int are_identical : 1; // ?
  unsigned int has_percent : 1;
  unsigned int has_point : 1;
  unsigned int has_radical : 1;
  unsigned int has_sign : 1;
  unsigned int is_ready_to_die : 1;
  unsigned int is_keyword : 1;
  unsigned int is_num : 1;
  unsigned int is_operator : 1;
  unsigned int is_root : 1;
};

/* Memory - структура для хранения данных о работе с памятью калькулятора. */
struct Memory
{
  double cells[MAX_SIZE]; // массив ячеек для double, до MAX_SIZE шт.
  int index; // cells[index]
  unsigned int has_value : 1;
};