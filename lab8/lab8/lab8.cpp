#include <iostream>
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "Scaner.h"
#include "Syntax_analyzer.h"
#include "Tree.h"

int main()
 {
    setlocale(LC_ALL, "Russian");

    // инициализация объекта класса сканер 
    Scaner* scaner = new Scaner(const_cast<char*>("input10.txt"));

    // инициализация объекта клас а диаграмма
    SyntAnalyzer* synt_analyzer = new SyntAnalyzer(scaner);
    synt_analyzer->TProgram();

    synt_analyzer->TreePrint();
    

    return 0;
}
