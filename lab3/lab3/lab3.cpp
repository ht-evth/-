#include <iostream>
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "Scaner.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    int type;
    TypeLex l;

    // инициализация объекта класса сканер 
    Scaner* scaner = new Scaner(const_cast<char*>("input2.txt"));

    // читаем пока не дойдем до конца модуля
    do
    {
        type = scaner->Scan(l);
        std::cout << "\t" << l << "\t\t- тип " << type << "\n";
    } while (type != TYPE_END);

    return 0;
}

