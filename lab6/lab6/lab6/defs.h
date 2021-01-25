#pragma once
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT 10000
#define MAX_LEX 100
#define MAX_KEYW 8

typedef char TypeLex[MAX_LEX];
typedef char TypeMod[MAX_TEXT];

// идентификатор
#define TYPE_IDENT 1
// ключевые слова
#define TYPE_FLOAT 10
#define TYPE_DOUBLE 11
#define TYPE_INT 12
#define TYPE_MAIN 13
#define TYPE_DO 14
#define TYPE_WHILE 15
#define TYPE_RETURN 16

// специальные знаки
#define TYPE_OPENED_PARANTHESES 20
#define TYPE_CLOSED_PARANTHESES 21
#define TYPE_OPENED_BRACE 22
#define TYPE_CLOSED_BRACE 23
#define TYPE_OPENED_BRACKET 24
#define TYPE_CLOSED_BRACKET 25
#define TYPE_SEMICOLON 26
#define TYPE_COMMA 27

// константы
#define TYPE_CONST_INT 30
#define TYPE_CONST_CHAR 31

// знаки операций
#define TYPE_LESS_THAN 40
#define TYPE_GREATER_THAN 41
#define TYPE_LESS_EQUAL 42
#define TYPE_GREATER_EQUAL 43
#define TYPE_NOT_EQUAL 44
#define TYPE_EQUAL_TO 45
#define TYPE_ADD 46
#define TYPE_SUB 47
#define TYPE_MULT 48
#define TYPE_DIV 49
#define TYPE_MODULO 50
#define TYPE_INCREMENT 51
#define TYPE_DECREMENT 52
#define TYPE_ASSIGNMENT 53
#define TYPE_ADD_ASSIGN 54
#define TYPE_SUB_ASSIGN 55
#define TYPE_MULT_ASSIGN 56
#define TYPE_DIV_ASSIGN 57
#define TYPE_MODULO_ASSIGN 58

// дополнительно
#define TYPE_ERROR 999
#define TYPE_END 1000
