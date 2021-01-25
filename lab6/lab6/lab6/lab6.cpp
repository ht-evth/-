#include <iostream>
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "Scaner.h"
#include "Syntax_analyzer.h"


int main()
{
    setlocale(LC_ALL, "Russian");

    // инициализация объекта класса сканер 
    Scaner* scaner = new Scaner(const_cast<char*>("input.txt"));

    // инициализация объекта клас а диаграмма
    SyntAnalyzer* synt_analyzer = new SyntAnalyzer(scaner);

    synt_analyzer->TProgram();
    

    return 0;
}
