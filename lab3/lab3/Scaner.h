#pragma once

#include "defs.h"

class Scaner {
private:
	
	TypeMod t;							// массив символов (хранит весь текст модуля)
	int uk;								// указатель
	int num_of_str;						// номер текущей строки

public:
	void GetData(char*);				// чтение данных из файла
	Scaner(char* FileName);				// конструктор класса
	~Scaner() {};						// деструктор класса
	void PutUK(int i);					// установтить значение указателя
	int GetUK(void);					// получить значение указателя
	void PrintError(char*, char*);		// функция печати ошибки
	int Scan(TypeLex l);				// функция сканирования 
};

