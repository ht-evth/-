#pragma once

// коды ошибок

#define NO_ERRORS 0
#define ERROR_FILE_NOT_EXIST 1
#define ERROR_FILE_TOO_LONG 2
#define ERROR_MULTY_COMMENT 3
#define ERROR_INT_CONST_TOO_LONG 4
#define ERROR_IDENT_TOO_LONG 5
#define ERROR_CHAR_CONST_NOT_CLOSED 6
#define ERROR_MSG_SYMBOL_NOT_ASSING 7
#define ERROR_UNKNOWN_SYMBOL 8
#define ERROR_INT_FOR_MAIN 9
#define ERROR_MAIN_FOR_MAIN 10
#define ERROR_MAIN_IS_EXIST 11
#define ERROR_OPENED_PARANTHESES 12
#define ERROR_CLOSED_PARANTHESES 13
#define ERROR_VAR_OR_FUNC 14
#define ERROR_RETURN 15
#define ERROR_SEMICOLON 16
#define ERROR_STATEMENT 17
#define ERROR_OPENED_BRACE 18
#define ERROR_CLOSED_BRACE 19
#define ERROR_DO 20
#define ERROR_WHILE 21
#define ERROR_OPENED_BRACKET 22
#define ERROR_CLOSED_BRACKET 23
#define ERROR_TElExpression 24
#define ERROR_IDENT 25
#define ERROR_TYPE 26
#define ERROR_ASSIGMENT 27
#define ERROR_ANY_ASSIGMENT 28
#define ERROR_UNWANTED_TEXT_END 29


// класс для печати ошибок
class ErrorPrinter
{

private:
	static int amount_errors;		// общий счетчик всех ошибок


public:
	~ErrorPrinter() {}
	ErrorPrinter() {}

	void print(int num_of_err, char* symbol, int num_of_str);	// функция печати ошибки
	void print(int num_of_err);									// печать ошибки без номера строки и символа
	int getAmountOfErrors();									// получить число найденых ошибок

};

