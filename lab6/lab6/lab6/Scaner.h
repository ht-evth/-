#pragma once

#include "defs.h"
#include "Error.h"

class Scaner 
{
private:
	
	TypeMod t;							// массив символов (хранит весь текст модуля)
	int uk;								// указатель
	int num_of_str;						// номер текущей строки
	int old_num_of_str;					// для восстановления номера строки, при перемещении указателя

public:
	ErrorPrinter error;					// печатальщик ошибок


public:

	void GetData(char*);				// чтение данных из файла
	Scaner(char* FileName);				// конструктор класса
	~Scaner() {};						// деструктор класса
	void PutUK(int i);					// установтить значение указателя
	int GetUK(void);					// получить значение указателя
	int GetNumOfLine();					// получить номер текущей строки
	int Scan(TypeLex l);				// функция сканирования 

};

