#include "Shell.hpp"


int cmd_state(char **args) {
/*  char JSONMessage[] ="{\"SensorType\":\"Temperature\", \"Value\": 10}";
  StaticJsonDocument<300> parsed;   //Пул памяти
  // Десериализация документа JSON
  DeserializationError error = deserializeJson(parsed, JSONMessage);
  // Проверьте, удастся ли выполнить синтаксический анализ.
  if (error) {
    Serial2.println("failed!");
    return 0;
  } else {   //Вывести если ошибок нет
    const char* sensorType = parsed["SensorType"];  //Получить значение типа датчика
    int value = parsed["Value"];                    //Получить значение измерения датчика

    Serial2.print(sensorType);
    Serial2.print(": ");
    Serial2.println(value);
  }*/


  JsonDocument doc;
  String input = "{\"Red\":\"\","  
                  "\"Pump\":\"\","  
                  "\"Track\":\"\","
                  "\"Fun\":\"\","
                  "\"Relay\":\"\","
                  "\"ServoX\":\"\","
                  "\"ServoY\":\"\""
                  "}";
  deserializeJson(doc, input);

  doc["ServoX"] = String(position_X);
  doc["ServoY"] = String(position_Y);

  if(is_TRAFF_RED) doc["Red"] = "ON";
  else doc["Red"] = "OFF";

  if(is_PUMP_IN) doc["Pump"] = "ON";
  else doc["Pump"] = "OFF";

  if(is_TRACK_IN) doc["Track"] = "ON";
  else doc["Track"] = "OFF";

  if(is_FUN_IN) doc["Fun"] = "ON";
  else doc["Fun"] = "OFF";

  if(is_RELAY) doc["Relay"] = "ON";
  else doc["Relay"] = "OFF";
    
  String output;
  serializeJson(doc, output);
  Serial2.println(output);
	return 0;
}


int cmd_servoX(char **args) {
  // Проверяем, есть ли второй аргумент
	if (args[1] == NULL) {
		// Сообщение об ошибке
    Serial2.println("Expected argument (num) to \"servox\"");
	} else {
    //String str = args[1];
    position_X = dec_str_to_number(args[1]);
    if(position_X < 0) position_X = 0;
    if(position_X > 180) position_X = 180;
    char msg[20];
    sprintf(msg, "Servo: %d", position_X); 
    Serial2.print(msg);
    servo_X.write(position_X);
	}
	return 0;
}


int cmd_servoY(char **args) {
  // Проверяем, есть ли второй аргумент
	if (args[1] == NULL) {
		// Сообщение об ошибке
    Serial2.println("Expected argument (num) to \"servoy\"");
	} else {
    //String str = args[1];
    position_Y = dec_str_to_number(args[1]);
    if(position_Y < 107) position_Y = 107;
    if(position_Y > 180) position_Y = 180;
    char msg[20];
    sprintf(msg, "Servo: %d", position_Y); 
    Serial2.print(msg);
    servo_Y.write(position_Y);
	}
	return 0;
}


int cmd_wait(char **args) {
  // Проверяем, есть ли второй аргумент
	if (args[1] == NULL) {
		// Сообщение об ошибке
    Serial2.println("Expected argument (1000) to \"wait\"");
	} else {   
    Serial2.print("Waiting: ");
    Serial2.print(args[1]);
    delay(dec_str_to_number(args[1]));
    Serial2.println(" DONE");
	}
	return 0;
}


int cmd_relay(char **args) {
	// Проверяем, есть ли второй аргумент
	if (args[1] == NULL) {
		// Сообщение об ошибке
    Serial2.println("Expected argument (on/off) to \"relay\"");
	} else {
    String str = args[1];    
    if (str == "on") {
      is_RELAY = true;  				
      digitalWrite(RELAY, HIGH);
      Serial2.println("Relay ON");
	  }
    if (str == "off") {
      is_RELAY = false;  				
      digitalWrite(RELAY, LOW);
      Serial2.println("Relay OFF");      
	  }
	}
	return 1;
}


int cmd_led_red(char **args) {
	// Проверяем, есть ли второй аргумент
	if (args[1] == NULL) {
		// Сообщение об ошибке
    Serial2.println("Expected argument (on/off) to \"red\"");
	} else {
    String str = args[1];    
    if (str == "on") {
      is_TRAFF_RED = true;  				
      digitalWrite(TRAFF_RED, HIGH);
      Serial2.println("Red ON");
	  }
    if (str == "off") {
      is_TRAFF_RED = false;  				
      digitalWrite(TRAFF_RED, LOW);
      Serial2.println("Red OFF");      
	  }
	}
	return 1;
}


int cmd_fun(char **args) {
	// Проверяем, есть ли второй аргумент
	if (args[1] == NULL) {
		// Сообщение об ошибке
    Serial2.println("Expected argument (on/off) to \"fun\"");
	} else {
    String str = args[1];    
    if (str == "on") {
      digitalWrite(FUN_IN, HIGH);
      digitalWrite(TRAFF_YELLOW, HIGH);
      is_FUN_IN = !is_FUN_IN;
      Serial2.println("Fun ON");
	  }
    if (str == "off") {
      digitalWrite(FUN_IN, LOW);
      digitalWrite(TRAFF_YELLOW, LOW);
      is_FUN_IN = !is_FUN_IN; 
      Serial2.println("Fun OFF");      
	  }
	}
	return 1;
}


int cmd_track(char **args) {
	// Проверяем, есть ли второй аргумент
	if (args[1] == NULL) {
		// Сообщение об ошибке
    Serial2.println("Expected argument (on/off) to \"track\"");
	} else {
    String str = args[1];    
    if (str == "on") {
      digitalWrite(TRACK_IN, HIGH);
      digitalWrite(TRACK_LED, HIGH);
      is_TRACK_IN = !is_TRACK_IN;
      Serial2.println("Track ON");
	  }
    if (str == "off") {
      digitalWrite(TRACK_IN, LOW);
      digitalWrite(TRACK_LED, LOW);
      is_TRACK_IN = !is_TRACK_IN;
      Serial2.println("Track OFF");      
	  }
	}
	return 1;
}


int cmd_pump(char **args) {
	// Проверяем, есть ли второй аргумент
	if (args[1] == NULL) {
		// Сообщение об ошибке
    Serial2.println("Expected argument (on/off) to \"pump\"");
	} else {
    String str = args[1];    
    if (str == "on") {
      digitalWrite(PUMP_IN, HIGH);
      digitalWrite(TRAFF_GREEN, HIGH);
      is_PUMP_IN = !is_PUMP_IN;
      Serial2.println("Pump ON");
	  }
    if (str == "off") {
      digitalWrite(PUMP_IN, LOW);
      digitalWrite(TRAFF_GREEN, LOW);
      is_PUMP_IN = !is_PUMP_IN;
      Serial2.println("Pump OFF");      
	  }
	}
	return 1;
}


// Команда help
int cmd_help(char **args) {
	int i;
  Serial2.println("Coffee v.1.0 (c) 2024 Gor.Com");
	Serial2.println("The following command are built in:");
   
	// Выводим массив имен встроенных команд
	for (i = 0; i < num_builtins(); i++) {
		Serial2.println(cmd_str[i]);    
	}
	return 1;
}


// Команда вывода на экран программы
int cmd_list(char **args) {
    // Служебный вывод программы
    for(int i = 0; i < MAX_LINE; i++){
      if(program[i*20] != 0x0D){ 
        Serial2.print(i+1);
        Serial2.print(" ");       
      }

      for(int j = 0; j < MAX_LENGTH; j++){
        if(program[i*20 + j] != 0x0D){        
          Serial2.print(program[i*20 + j]);
        }
      }
      if(program[i*20] != 0x0D){ 
        Serial2.println("");
      }        
    }
	return 0;
}


// Команда выполнения программы
int cmd_run(char **args) {
    for(int i = 0; i < MAX_LINE; i++){
      line[0] = 0;
      for(int j = 0; j < MAX_LENGTH; j++){
        if(program[i*20 + j] != 0x0D){
          line[j] = program[i*20 + j];       // Копируем строку из программы для выполнения      
        }
      }
      // execute str
      if(line[0]){
        args = split_line(line);  // Извлекает аргументы
        execute(args, line);      // Исполняем команду  
      }    
        
    }
	return 0;
}


// Команда PRINT
int cmd_print(char **args) {
  if(args[1] == NULL){
    return 0;
  }
  if(args[1][0] == 34){
    for(int i = 1; i < 20; i++){
      if(args[1][i] == 34 || args[1][i] == 0x0D || args[1][i] == 0x20 || args[1][i] == 0){
        if(args[1][i] == 34 && args[1][i+1] != 0x3B){    
          Serial2.println(""); 
        }
        break;
      } 
      Serial2.print(args[1][i]);  
    }
    
    return 0;
  }

  Serial2.println(process_expr());
	return 0;
}


// Команда очистки программы
int cmd_new(char **args) {
  running = 0;
  start_bas();
	return 0;
}


// Команда ввода переменной
int cmd_input(char **args) {
  Serial2.print("?");
  while(Serial2.available()==0);  //Ожидаем ввода данных
  // Вычисляем адрес по букве
  vars[args[1][0] - 97] = process_expr();//Serial.read();

  // Служебный вывод
  for(int i = 0; i < 26; i++){
    Serial2.print(vars[i]);   
  } 
  Serial2.println("");  
  
	return 0;
}