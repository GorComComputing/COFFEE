#include "Shell.hpp"


// Возвращает указатель на строку ввода
char *read_line(void){
  int bufsize = RL_BUFSIZE; // Размер буфера ввода
  char *buffer = (char*)malloc(sizeof(char) * bufsize); // Выделяем память под буфер ввода

  // Проверяет, выделилась ли память под буфер ввода
  if(!buffer){
    Serial2.println("Error allocation");
  }
  // Читаем по символу в буфер до конца строки или файла
  for(int position = 0; position < RL_BUFSIZE; position++)
  {
    while(Serial2.available()==0);  //Ожидаем ввода данных
    buffer[position]=Serial2.read();
    Serial2.flush();
    if(buffer[position]=='\n'){
      buffer[position]=0; 
      return buffer;
    }
  }
  return 0;  
}


// Извлекает аргументы
char **split_line(char *line){
  int bufsize = TOK_BUFSIZE;
  int position = 0;
  char **tokens = (char**)malloc(sizeof(char*) * bufsize);
  char *token;

  // Проверяет, выделилась ли память
  if(!tokens){
    Serial2.println("Error allocation");
  }

  // Получаем первый токен из буфера ввода
	token = strtok(line, TOK_DELIM);
	// Заполняем в цикле указатель на указатели на токены
	while (token != NULL) {
		tokens[position] = token;
		position++;
		
		// Получаем последующие токены (поэтому NULL)
		token = strtok(NULL, TOK_DELIM);
	}
	// Последний токен NULL
	tokens[position] = NULL;
	return tokens;
}


int num_builtins() {
		return sizeof(cmd_str) / sizeof(char *);
}


// ErrorMsg
void error_msg(){
  Serial2.println("Error!");
}


// Вычисляем выражение
int process_expr(){
  return 5;
}


// Выполнение встроенных команд
int execute(char **args, char *line){
  int i;

	// Проверяем, не путая ли строка введена
	if (args[0] == NULL) {
		return 1;
	}
	
  // Проверяем, не команда ли  
	for (i = 0; i < num_builtins(); i++) {
		if (strcmp(args[0], cmd_str[i]) == 0) {
Serial.println("HERE4");
			return (*cmd_func[i])(args);
		}
	}
	
  // Значит это переменная
  
  //vars[line[0] - 97] = line[0];
  
  // удаляем пробелы
  int position = 1;
  for(; position < MAX_LENGTH; position++){
    if(line[position] != 0 && line[position] != 32) { 
      //Serial.println(line[position], HEX); 
      break;
    }
  }
  //Serial.println(position);
  // проверяем на =
  if(line[position] == '='){
    // вычисляем выражение и присваиваем в адрес
    Serial2.println("Assume");
    // Вычисляем адрес по букве
    vars[line[0] - 97] = process_expr();

  // Служебный вывод
  for(int i = 0; i < 26; i++){
    Serial2.print(vars[i]);     
  } 
  Serial2.println(""); 
      
    return 1;    
  }
  else {
    // иначе вычисляем выражение 
    Serial2.println(process_expr());
    return 1;
  }
  
  // Если совсем ничего ErrorMsg
  error_msg();
  
  
  
  
  
  //return launch(args);
}


// Буфер исходника программы, заполняем символом 0Dh (Enter)
void start_bas(){
  for(int i = 0; i < MAX_SIZE; i++){
    program[i] = 0x0D;
    //Serial.print(i);
    //Serial.print(" ");
    //Serial.println(program[i], HEX);
  }  
}


// Вынимаем номер из строки
int dec_str_to_number(char *line){
  unsigned int N = 0;
  int digit;
  for(int i = 0; i < RL_BUFSIZE; i++){
    if((digit = line[i] - '0') >= 0 && digit <= 9){
      N = N*10 + digit;
    }
    else {
      return N;
    }
  }
  return N;
}


// Shell exec
void shell(){
  running = 0;                // Обнуляем счетчик команд
  if(Serial2.available() > 0){
    do_beep();
    line = read_line();       // Получаем указатель на строку ввода
    
    //Serial2.println(line);    // Выводим введенную строчку на экран
    //Serial.println(line);

    int number = dec_str_to_number(line);   
    if(number){               // Если строчка начинается с числа, то сохраняем строку в программе
    //Serial.println(number);
      char isStr = 0;
      char count = 0;
      for(int i = 0; i < MAX_LENGTH; i++){
        if(!line[i]) {  
          //Serial.print("break"); 
          break;
        }
        //Serial.print("#");
        if((line[i] == 0x20 || (line[i] > 0x2F && line[i] < 0x3A)) && isStr == 0){  
          //Serial.print(line[i], HEX);
          count++;
          continue;
        } else {
          isStr = 1;
          program[(number-1)*MAX_LENGTH + i - count] = line[i];
          //Serial.print("-");
        }
      }
    } else { // Иначе выполняем без сохранения
      // execute str
      args = split_line(line);  // Извлекает аргументы
      //Serial.println(args[0]);
      //Serial.println(args[1]);
      //Serial.println(args[2]);
      execute(args, line);            // Исполняем команду
    }
    // Освобождает память
    free(line);
    free(args);
    //Serial2.print(">");
  }
}
