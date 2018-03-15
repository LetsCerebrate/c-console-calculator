/* Объекты. */

/* Константы. */
#define MAX_SIZE 20 // макс. длина обычного массива
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