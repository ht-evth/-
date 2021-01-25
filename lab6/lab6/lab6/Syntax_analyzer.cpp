#include "Syntax_analyzer.h"
#include "defs.h"
#include "Error.h"
#include <iostream>

// основная программа
void SyntAnalyzer::TProgram()
{

	TypeLex l;
	int type_data, ident, uk1, skip_uk;

	uk1 = sc->GetUK();
	type_data = ident = sc->Scan(l);				// тип данных
	sc->PutUK(uk1);


	while (type_data != TYPE_END)
	{

		uk1 = sc->GetUK();
		type_data = sc->Scan(l);				// тип данных
		ident = sc->Scan(l);					// идентификатор
		skip_uk = sc->GetUK();

			


		// как объявление функций, так и переменных начинается на одинаковые терминалы
		if (type_data == TYPE_INT || type_data == TYPE_FLOAT || type_data == TYPE_DOUBLE)
		{

			// ожидается идентификатор или ключевое слово main 
			if (ident != TYPE_IDENT && ident != TYPE_MAIN)
				this->error->print(ERROR_VAR_OR_FUNC, l, sc->GetNumOfLine());
		

			// если индетификатор или main
			if (ident == TYPE_IDENT || ident == TYPE_MAIN)
			{
				// Если после типа данных идёт перечисление идентификаторов или присваивание - это объявление переменной
				type_data = sc->Scan(l);
				if (type_data == TYPE_COMMA || type_data == TYPE_ASSIGNMENT)
				{
					sc->PutUK(uk1);
					TVarsDeclar();
				}

				// иначе - ожидается объявление/описание функции
				// если открытая скобка (
				else if (type_data == TYPE_OPENED_PARANTHESES)
				{
					// ожидается закрытая скобка )
					type_data = sc->Scan(l);
					if (type_data != TYPE_CLOSED_PARANTHESES)
						this->error->print(ERROR_CLOSED_PARANTHESES, l, sc->GetNumOfLine());

					uk1 = sc->GetUK();
					type_data = sc->Scan(l);

					// если символ ; то объявление функции
					// переход к следующей итерации
					if (type_data == TYPE_SEMICOLON)
						continue;
				

					//  иначе, если открытая фигурная скобка - определение функции
					else if (type_data == TYPE_OPENED_BRACE)
					{
						sc->PutUK(uk1);
						TCompoundStatement();
					}

					// если ничего не подошло - ошибка - ожидалось объявление переменных или функции
					else
						this->error->print(ERROR_VAR_OR_FUNC, l, sc->GetNumOfLine());


				}

			}

		}

		// если дошли до конца файла
		else if (type_data == TYPE_END)
		{
			std::cout << "\nМодуль полностью проверен\n";
			 
			if (ident != TYPE_END)
				this->error->print(ERROR_UNWANTED_TEXT_END, l, sc->GetNumOfLine());
			

			
			break;
		}

		// лишний текст в конце программы
		else
		{
			this->error->print(ERROR_UNWANTED_TEXT_END, l, sc->GetNumOfLine());

			std::cout << "\nМодуль не был полностью проверен из-за синтаксической ошибки\n";
			break;
		}
			
	}

	
	std::cout << "\nВсего ошибок найдено: " << this->getNumberOfErrors() << std::endl;
			

}



// оператор
void SyntAnalyzer::TStatement()
{

	TypeLex l;
	int t, uk1;

	uk1 = sc->GetUK();
	t = sc->Scan(l);

	// если терминал return
	if (t == TYPE_RETURN)
	{
		sc->PutUK(uk1);


		// терминал return
		t = sc->Scan(l);
		if (t != TYPE_RETURN)
			this->error->print(ERROR_RETURN, l, sc->GetNumOfLine());

		uk1 = sc->GetUK();
		t = sc->Scan(l);


		// т.к. по заданию вернуть void мы не можем, 
		// после return идет выражение и символ ;

		sc->PutUK(uk1);
		TExpression();

		// терминал ;
		t = sc->Scan(l);
		if (t != TYPE_SEMICOLON)
			this->error->print(ERROR_SEMICOLON, l, sc->GetNumOfLine());

	}

	// если терминал ;
	else if (t == TYPE_SEMICOLON)
		return;


	// если терминал do, то ожидается do-while
	else if (t == TYPE_DO)
	{
		sc->PutUK(uk1);
		TDoWhile();
	}

	// если терминал {
	else if (t == TYPE_OPENED_BRACE)
	{
		sc->PutUK(uk1);
		TCompoundStatement();
	}



	// Если встретили идентификатор, то ожидается 
	// или вызов функции (TFuncCall) - далее следует терминал (
	// или присваивание (TAssignment) - далее следуюет или [] и присвоение, или просто присвоение
	else if (t == TYPE_IDENT)
	{
		t = sc->Scan(l);
		sc->PutUK(uk1);

		// ожидается символ "(" - вызов функции
		if (t == TYPE_OPENED_PARANTHESES)
			TFuncCall();


		// иначе ожидается присваивание
		else
		{
			TAssignment();

			// проверка точки с запятой
			t = sc->Scan(l);
			if (t != TYPE_SEMICOLON)
				this->error->print(ERROR_SEMICOLON, l, sc->GetNumOfLine());
		}
	}

	else
		this->error->print(ERROR_STATEMENT, l, sc->GetNumOfLine());
}


// составной оператор {}
void SyntAnalyzer::TCompoundStatement()
{
	TypeLex l;
	int t, uk1;

	// ожидается терминал "{"
	t = sc->Scan(l);
	if (t != TYPE_OPENED_BRACE)
		this->error->print(ERROR_OPENED_BRACE, l, sc->GetNumOfLine());

	uk1 = sc->GetUK();
	t = sc->Scan(l);
	sc->PutUK(uk1);

	while (t != TYPE_CLOSED_BRACE)
	{

		// если тип данных - это объявление переменных
		if (t == TYPE_FLOAT || t == TYPE_DOUBLE || t == TYPE_INT)
			TVarsDeclar();

		// в ином случае - оператор
		else if (t == TYPE_OPENED_BRACE || t == TYPE_IDENT || t == TYPE_DO || t == TYPE_RETURN || t == TYPE_SEMICOLON)
			TStatement();

		// проверка на конец файла
		else if (t == TYPE_END)
			break;

		uk1 = sc->GetUK();
		t = sc->Scan(l);
		sc->PutUK(uk1);
	}

	// ожидается терминал "}"
	t = sc->Scan(l);
	if (t != TYPE_CLOSED_BRACE)
		this->error->print(ERROR_CLOSED_BRACE, l, sc->GetNumOfLine());
}

// выражение
void SyntAnalyzer::TExpression()
{
	TypeLex l;
	int t, uk1;

	// слагаемое
	TSummand();

	// цикл пока встречается символ сравнения
	uk1 = sc->GetUK();
	t = sc->Scan(l);

	while (t == TYPE_LESS_THAN || t == TYPE_LESS_EQUAL || t == TYPE_GREATER_THAN || t == TYPE_GREATER_EQUAL || t == TYPE_EQUAL_TO || t == TYPE_NOT_EQUAL)
	{
		TSummand();

		uk1 = sc->GetUK();
		t = sc->Scan(l);
	}

	sc->PutUK(uk1);
}

// цикл "do-while"
void SyntAnalyzer::TDoWhile()
{

	TypeLex l;
	int t, uk1;

	// терминал "do"
	t = sc->Scan(l);
	if (t != TYPE_DO)
		this->error->print(ERROR_DO, l, sc->GetNumOfLine());

	// оператор
	TStatement();

	// терминал "while"
	t = sc->Scan(l);
	if (t != TYPE_WHILE)
		this->error->print(ERROR_WHILE, l, sc->GetNumOfLine());

	// терминал "("
	t = sc->Scan(l);
	if (t != TYPE_OPENED_PARANTHESES)
		this->error->print(ERROR_OPENED_PARANTHESES, l, sc->GetNumOfLine());

	// выражение
	TExpression();

	// терминал )
	t = sc->Scan(l);
	if (t != TYPE_CLOSED_PARANTHESES)
		this->error->print(ERROR_CLOSED_PARANTHESES, l, sc->GetNumOfLine());

	// терминал ;
	t = sc->Scan(l);
	if (t != TYPE_SEMICOLON)
		this->error->print(ERROR_SEMICOLON, l, sc->GetNumOfLine());
}

// слагаемое
void SyntAnalyzer::TSummand()
{
	TypeLex l;
	int t, uk1;

	// множитель
	TFactor();

	// цикл пока встречается + -
	uk1 = sc->GetUK();
	t = sc->Scan(l);

	while (t == TYPE_ADD || t == TYPE_SUB)
	{
		TFactor();

		uk1 = sc->GetUK();
		t = sc->Scan(l);
	}

	sc->PutUK(uk1);

}

// множитель
void SyntAnalyzer::TFactor()
{
	TypeLex l;
	int t, uk1;

	// преф. ун
	TPrefUn();


	uk1 = sc->GetUK();
	t = sc->Scan(l);
	// цикл пока встречается умножение, деление или взятие модуля
	while (t == TYPE_MULT || t == TYPE_DIV || t == TYPE_MODULO)
	{
		TPrefUn();

		uk1 = sc->GetUK();
		t = sc->Scan(l);
	}

	sc->PutUK(uk1);
}

// преф. ун.
void SyntAnalyzer::TPrefUn()
{
	TypeLex l;
	int t, uk1;

	uk1 = sc->GetUK();
	t = sc->Scan(l);

	// если не знак +, - или ++, --
	// указатель возвращаем на шаг назад
	if (t != TYPE_ADD && t != TYPE_SUB && t != TYPE_INCREMENT && t != TYPE_DECREMENT)
		sc->PutUK(uk1);

	// пост. ун.
	TPostUn();
}

// эл. выражение
void SyntAnalyzer::TElExpression()
{
	TypeLex l;
	int t, uk1;

	t = sc->Scan(l);

	// если константа, то терминал достигнут
	if (t == TYPE_CONST_CHAR || t == TYPE_CONST_INT)
		return;

	// если встретили идентификатор,
	// ожидаем "[" или ничего
	else if (t == TYPE_IDENT)
	{
		uk1 = sc->GetUK();
		t = sc->Scan(l);

		//  Если "["
		if (t == TYPE_OPENED_BRACKET)
		{
			// выражение
			TExpression();

			// терминал "]"
			t = sc->Scan(l);
			if (t != TYPE_CLOSED_BRACKET)
				this->error->print(ERROR_CLOSED_BRACKET, l, sc->GetNumOfLine());
		}
		else sc->PutUK(uk1);
	}

	// иначе если открытая круглая скобка
	else if (t == TYPE_OPENED_PARANTHESES)
	{
		// выражение
		TExpression();

		// терминал )
		t = sc->Scan(l);
		if (t != TYPE_CLOSED_PARANTHESES)
			this->error->print(ERROR_CLOSED_PARANTHESES, l, sc->GetNumOfLine());
	}

	// иначе выдать ошибку, т.к. на месте выражения может быть любой символ - а это ошибка
	else
		this->error->print(ERROR_TElExpression, l, sc->GetNumOfLine());

}

// пост. ун.
void SyntAnalyzer::TPostUn()
{
	TypeLex l;
	int t, uk1;

	// эл.выр.
	TElExpression();

	uk1 = sc->GetUK();
	t = sc->Scan(l);

	// если не знак ++ --
	// возвращаем указатель на шаг назад
	if (t != TYPE_INCREMENT && t != TYPE_DECREMENT)
		sc->PutUK(uk1);
}


void SyntAnalyzer::TFuncCall()
{
	TypeLex l;
	int t, uk1;

	// ожидается идентификатор
	t = sc->Scan(l);
	if (t != TYPE_IDENT)
		this->error->print(ERROR_IDENT, l, sc->GetNumOfLine());

	// терминал "("
	t = sc->Scan(l);
	if (t != TYPE_OPENED_PARANTHESES)
		this->error->print(ERROR_OPENED_PARANTHESES, l, sc->GetNumOfLine());

	// терминал ")"
	t = sc->Scan(l);
	if (t != TYPE_CLOSED_PARANTHESES)
		this->error->print(ERROR_CLOSED_PARANTHESES, l, sc->GetNumOfLine());

	// терминал ;
	t = sc->Scan(l);
	if (t != TYPE_SEMICOLON)
		this->error->print(ERROR_SEMICOLON, l, sc->GetNumOfLine());
}


// объявление переменных
void SyntAnalyzer::TVarsDeclar()
{
	TypeLex l;
	int t, uk1;

	// ожидаем терминл "тип данных"
	t = sc->Scan(l);
	if (t != TYPE_FLOAT && t != TYPE_DOUBLE && t != TYPE_INT)
		this->error->print(ERROR_TYPE, l, sc->GetNumOfLine());


	int uk2;
	// список переменных
	do
	{
		
		// ожидается идентификатор
		t = sc->Scan(l);
		if (t != TYPE_IDENT)
			this->error->print(ERROR_IDENT, l, sc->GetNumOfLine());

		uk2= sc->GetUK();
		t = sc->Scan(l);

		// Если только идентификатор
		if (t != TYPE_ASSIGNMENT && t != TYPE_OPENED_BRACKET)
		{
			sc->PutUK(uk2);
		}

		// идентификатор и присвоение - ожидание выражения
		else if (t == TYPE_ASSIGNMENT)
			TExpression();


		// ветка идентификатор и "["
		else if (t == TYPE_OPENED_BRACKET)
		{

			uk2 = sc->GetUK();
			t = sc->Scan(l);

			// Если получаем "идентификатор[]", то ожидается "= { <объявление аргументов> }"
			if (t == TYPE_CLOSED_BRACKET)
			{

				// терминал "="
				t = sc->Scan(l);
				if (t != TYPE_ASSIGNMENT)
					this->error->print(ERROR_ASSIGMENT, l, sc->GetNumOfLine());

				// терминал "{"
				t = sc->Scan(l);
				if (t != TYPE_OPENED_BRACE)
					this->error->print(ERROR_OPENED_BRACE, l, sc->GetNumOfLine());

				// объявление аргументов
				do
				{
					TExpression();

					uk2 = sc->GetUK();
					t = sc->Scan(l);

				} while (t == TYPE_COMMA);
				sc->PutUK(uk2);

				// терминал }
				t = sc->Scan(l);
				if (t != TYPE_CLOSED_BRACE)
					this->error->print(ERROR_CLOSED_BRACE, l, sc->GetNumOfLine());
			}

			// если в квадратных скобках содержится выражение 
			else
			{
				sc->PutUK(uk2);
				TExpression();

				// терминал ]
				t = sc->Scan(l);
				if (t != TYPE_CLOSED_BRACKET)
					this->error->print(ERROR_CLOSED_BRACKET, l, sc->GetNumOfLine());

				uk2 = sc->GetUK();
				t = sc->Scan(l);

				// если знак "="
				// то объявление аргументов
				if (t == TYPE_ASSIGNMENT)
				{

					t = sc->Scan(l);
					if (t != TYPE_OPENED_BRACE)
						this->error->print(ERROR_OPENED_BRACE, l, sc->GetNumOfLine());

					do
					{
						TExpression();

						uk2 = sc->GetUK();
						t = sc->Scan(l);

					} while (t == TYPE_COMMA);
					sc->PutUK(uk2);

					t = sc->Scan(l);
					if (t != TYPE_CLOSED_BRACE)
						this->error->print(ERROR_CLOSED_BRACE, l, sc->GetNumOfLine());
				}

				// иначе 
				else
				{
					sc->PutUK(uk2);
				}
			}
		}


		uk1 = sc->GetUK();
		t = sc->Scan(l);
	} while (t == TYPE_COMMA);
	sc->PutUK(uk1);

	// терминал ;
	t = sc->Scan(l);
	if (t != TYPE_SEMICOLON)
		this->error->print(ERROR_SEMICOLON, l, sc->GetNumOfLine());
}


// присваивание
void SyntAnalyzer::TAssignment()
{
	TypeLex l;
	int t, uk1;

	t = sc->Scan(l);
	if (t != TYPE_IDENT)
		this->error->print(ERROR_IDENT, l, sc->GetNumOfLine());

	uk1 = sc->GetUK();
	t = sc->Scan(l);

	// квадратные скобки
	if (t == TYPE_OPENED_BRACKET)
	{
		TExpression();

		t = sc->Scan(l);
		if (t != TYPE_CLOSED_BRACKET)
			this->error->print(ERROR_CLOSED_BRACKET, l, sc->GetNumOfLine());

		uk1 = sc->GetUK();
	}

	sc->PutUK(uk1);

	t = sc->Scan(l);

	if (t != TYPE_ASSIGNMENT && t != TYPE_ADD_ASSIGN && t != TYPE_SUB_ASSIGN && t != TYPE_MULT_ASSIGN && t != TYPE_DIV_ASSIGN && t != TYPE_MODULO_ASSIGN)
		this->error->print(ERROR_ANY_ASSIGMENT, l, sc->GetNumOfLine());

	TExpression();
}
