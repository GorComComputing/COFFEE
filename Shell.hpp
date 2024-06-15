#ifndef SHELL_HPP
#define SHELL_HPP

#define RL_BUFSIZE 20 // Размер буфера ввода
#define TOK_BUFSIZE 64  // Размер буфера с указателями на токены
#define TOK_DELIM " \t\r\n\a" // Разделители для токенизации

#define MAX_LINE 10  // Максимальное количество строчек в программе
#define MAX_LENGTH 20 // Максимальная длина строчки программы
#define MAX_SIZE MAX_LINE * MAX_LENGTH

int cmd_state(char **args);
int cmd_relay(char **args);
int cmd_led_red(char **args);
int cmd_fun(char **args);
int cmd_track(char **args);
int cmd_pump(char **args);
int cmd_help(char **args);
int cmd_wait(char **args);
int cmd_list(char **args);
int cmd_print(char **args);
int cmd_new(char **args);
int cmd_input(char **args);
int cmd_run(char **args);
int cmd_servoX(char **args);
int cmd_servoY(char **args);

void shell();

// Массив имен встроенных команд
char *cmd_str[] = {
  "state",         // Запрос состояния всех датчиков
  "relay",         // Включение реле
  "red",           // Включение светодиода
  "fun",           // Включение вентилятора
  "track",         // Включение конвейера
  "pump",          // Включение качалки
	"help",			     // Справка
  "wait", 		     // Ожидание
	"list",          // Вывод на экран программы
	"print",         // PRINT
	"new",           // Очищает программу
	"input",         // Ввести переменную  
	"run",           // Запуск выполнения программы
  "servox",        // Задать угол серво X
  "servoy"         // Задать угол серво Y
};

// Массив указателей на функции встроенных команд
int (*cmd_func[]) (char **) = {
  cmd_state,
  cmd_relay,
  cmd_led_red,
  cmd_fun,
  cmd_track,
  cmd_pump,
	cmd_help,
  cmd_wait,
	cmd_list,
	cmd_print,
	cmd_new,
	cmd_input,
	cmd_run, 
  cmd_servoX,
  cmd_servoY   
};

#endif // SHELL_HPP