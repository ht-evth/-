#include "Syntax_analyzer.h"
#include "defs.h"
#include "Error.h"
#include <iostream>

// �������� ���������
void SyntAnalyzer::TProgram()
{

	TypeLex l;
	int type_data, ident, uk1, skip_uk;

	uk1 = sc->GetUK();
	type_data = ident = sc->Scan(l);				// ��� ������
	sc->PutUK(uk1);


	while (type_data != TYPE_END)
	{

		uk1 = sc->GetUK();
		type_data = sc->Scan(l);				// ��� ������
		ident = sc->Scan(l);					// �������������
		skip_uk = sc->GetUK();

			


		// ��� ���������� �������, ��� � ���������� ���������� �� ���������� ���������
		if (type_data == TYPE_INT || type_data == TYPE_FLOAT || type_data == TYPE_DOUBLE)
		{

			// ��������� ������������� ��� �������� ����� main 
			if (ident != TYPE_IDENT && ident != TYPE_MAIN)
				this->error->print(ERROR_VAR_OR_FUNC, l, sc->GetNumOfLine());
		

			// ���� ������������� ��� main
			if (ident == TYPE_IDENT || ident == TYPE_MAIN)
			{
				// ���� ����� ���� ������ ��� ������������ ��������������� ��� ������������ - ��� ���������� ����������
				type_data = sc->Scan(l);
				if (type_data == TYPE_COMMA || type_data == TYPE_ASSIGNMENT)
				{
					sc->PutUK(uk1);
					TVarsDeclar();
				}

				// ����� - ��������� ����������/�������� �������
				// ���� �������� ������ (
				else if (type_data == TYPE_OPENED_PARANTHESES)
				{
					// ��������� �������� ������ )
					type_data = sc->Scan(l);
					if (type_data != TYPE_CLOSED_PARANTHESES)
						this->error->print(ERROR_CLOSED_PARANTHESES, l, sc->GetNumOfLine());

					uk1 = sc->GetUK();
					type_data = sc->Scan(l);

					// ���� ������ ; �� ���������� �������
					// ������� � ��������� ��������
					if (type_data == TYPE_SEMICOLON)
						continue;
				

					//  �����, ���� �������� �������� ������ - ����������� �������
					else if (type_data == TYPE_OPENED_BRACE)
					{
						sc->PutUK(uk1);
						TCompoundStatement();
					}

					// ���� ������ �� ������� - ������ - ��������� ���������� ���������� ��� �������
					else
						this->error->print(ERROR_VAR_OR_FUNC, l, sc->GetNumOfLine());


				}

			}

		}

		// ���� ����� �� ����� �����
		else if (type_data == TYPE_END)
		{
			std::cout << "\n������ ��������� ��������\n";
			 
			if (ident != TYPE_END)
				this->error->print(ERROR_UNWANTED_TEXT_END, l, sc->GetNumOfLine());
			

			
			break;
		}

		// ������ ����� � ����� ���������
		else
		{
			this->error->print(ERROR_UNWANTED_TEXT_END, l, sc->GetNumOfLine());

			std::cout << "\n������ �� ��� ��������� �������� ��-�� �������������� ������\n";
			break;
		}
			
	}

	
	std::cout << "\n����� ������ �������: " << this->getNumberOfErrors() << std::endl;
			

}



// ��������
void SyntAnalyzer::TStatement()
{

	TypeLex l;
	int t, uk1;

	uk1 = sc->GetUK();
	t = sc->Scan(l);

	// ���� �������� return
	if (t == TYPE_RETURN)
	{
		sc->PutUK(uk1);


		// �������� return
		t = sc->Scan(l);
		if (t != TYPE_RETURN)
			this->error->print(ERROR_RETURN, l, sc->GetNumOfLine());

		uk1 = sc->GetUK();
		t = sc->Scan(l);


		// �.�. �� ������� ������� void �� �� �����, 
		// ����� return ���� ��������� � ������ ;

		sc->PutUK(uk1);
		TExpression();

		// �������� ;
		t = sc->Scan(l);
		if (t != TYPE_SEMICOLON)
			this->error->print(ERROR_SEMICOLON, l, sc->GetNumOfLine());

	}

	// ���� �������� ;
	else if (t == TYPE_SEMICOLON)
		return;


	// ���� �������� do, �� ��������� do-while
	else if (t == TYPE_DO)
	{
		sc->PutUK(uk1);
		TDoWhile();
	}

	// ���� �������� {
	else if (t == TYPE_OPENED_BRACE)
	{
		sc->PutUK(uk1);
		TCompoundStatement();
	}



	// ���� ��������� �������������, �� ��������� 
	// ��� ����� ������� (TFuncCall) - ����� ������� �������� (
	// ��� ������������ (TAssignment) - ����� �������� ��� [] � ����������, ��� ������ ����������
	else if (t == TYPE_IDENT)
	{
		t = sc->Scan(l);
		sc->PutUK(uk1);

		// ��������� ������ "(" - ����� �������
		if (t == TYPE_OPENED_PARANTHESES)
			TFuncCall();


		// ����� ��������� ������������
		else
		{
			TAssignment();

			// �������� ����� � �������
			t = sc->Scan(l);
			if (t != TYPE_SEMICOLON)
				this->error->print(ERROR_SEMICOLON, l, sc->GetNumOfLine());
		}
	}

	else
		this->error->print(ERROR_STATEMENT, l, sc->GetNumOfLine());
}


// ��������� �������� {}
void SyntAnalyzer::TCompoundStatement()
{
	TypeLex l;
	int t, uk1;

	// ��������� �������� "{"
	t = sc->Scan(l);
	if (t != TYPE_OPENED_BRACE)
		this->error->print(ERROR_OPENED_BRACE, l, sc->GetNumOfLine());

	uk1 = sc->GetUK();
	t = sc->Scan(l);
	sc->PutUK(uk1);

	while (t != TYPE_CLOSED_BRACE)
	{

		// ���� ��� ������ - ��� ���������� ����������
		if (t == TYPE_FLOAT || t == TYPE_DOUBLE || t == TYPE_INT)
			TVarsDeclar();

		// � ���� ������ - ��������
		else if (t == TYPE_OPENED_BRACE || t == TYPE_IDENT || t == TYPE_DO || t == TYPE_RETURN || t == TYPE_SEMICOLON)
			TStatement();

		// �������� �� ����� �����
		else if (t == TYPE_END)
			break;

		uk1 = sc->GetUK();
		t = sc->Scan(l);
		sc->PutUK(uk1);
	}

	// ��������� �������� "}"
	t = sc->Scan(l);
	if (t != TYPE_CLOSED_BRACE)
		this->error->print(ERROR_CLOSED_BRACE, l, sc->GetNumOfLine());
}

// ���������
void SyntAnalyzer::TExpression()
{
	TypeLex l;
	int t, uk1;

	// ���������
	TSummand();

	// ���� ���� ����������� ������ ���������
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

// ���� "do-while"
void SyntAnalyzer::TDoWhile()
{

	TypeLex l;
	int t, uk1;

	// �������� "do"
	t = sc->Scan(l);
	if (t != TYPE_DO)
		this->error->print(ERROR_DO, l, sc->GetNumOfLine());

	// ��������
	TStatement();

	// �������� "while"
	t = sc->Scan(l);
	if (t != TYPE_WHILE)
		this->error->print(ERROR_WHILE, l, sc->GetNumOfLine());

	// �������� "("
	t = sc->Scan(l);
	if (t != TYPE_OPENED_PARANTHESES)
		this->error->print(ERROR_OPENED_PARANTHESES, l, sc->GetNumOfLine());

	// ���������
	TExpression();

	// �������� )
	t = sc->Scan(l);
	if (t != TYPE_CLOSED_PARANTHESES)
		this->error->print(ERROR_CLOSED_PARANTHESES, l, sc->GetNumOfLine());

	// �������� ;
	t = sc->Scan(l);
	if (t != TYPE_SEMICOLON)
		this->error->print(ERROR_SEMICOLON, l, sc->GetNumOfLine());
}

// ���������
void SyntAnalyzer::TSummand()
{
	TypeLex l;
	int t, uk1;

	// ���������
	TFactor();

	// ���� ���� ����������� + -
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

// ���������
void SyntAnalyzer::TFactor()
{
	TypeLex l;
	int t, uk1;

	// ����. ��
	TPrefUn();


	uk1 = sc->GetUK();
	t = sc->Scan(l);
	// ���� ���� ����������� ���������, ������� ��� ������ ������
	while (t == TYPE_MULT || t == TYPE_DIV || t == TYPE_MODULO)
	{
		TPrefUn();

		uk1 = sc->GetUK();
		t = sc->Scan(l);
	}

	sc->PutUK(uk1);
}

// ����. ��.
void SyntAnalyzer::TPrefUn()
{
	TypeLex l;
	int t, uk1;

	uk1 = sc->GetUK();
	t = sc->Scan(l);

	// ���� �� ���� +, - ��� ++, --
	// ��������� ���������� �� ��� �����
	if (t != TYPE_ADD && t != TYPE_SUB && t != TYPE_INCREMENT && t != TYPE_DECREMENT)
		sc->PutUK(uk1);

	// ����. ��.
	TPostUn();
}

// ��. ���������
void SyntAnalyzer::TElExpression()
{
	TypeLex l;
	int t, uk1;

	t = sc->Scan(l);

	// ���� ���������, �� �������� ���������
	if (t == TYPE_CONST_CHAR || t == TYPE_CONST_INT)
		return;

	// ���� ��������� �������������,
	// ������� "[" ��� ������
	else if (t == TYPE_IDENT)
	{
		uk1 = sc->GetUK();
		t = sc->Scan(l);

		//  ���� "["
		if (t == TYPE_OPENED_BRACKET)
		{
			// ���������
			TExpression();

			// �������� "]"
			t = sc->Scan(l);
			if (t != TYPE_CLOSED_BRACKET)
				this->error->print(ERROR_CLOSED_BRACKET, l, sc->GetNumOfLine());
		}
		else sc->PutUK(uk1);
	}

	// ����� ���� �������� ������� ������
	else if (t == TYPE_OPENED_PARANTHESES)
	{
		// ���������
		TExpression();

		// �������� )
		t = sc->Scan(l);
		if (t != TYPE_CLOSED_PARANTHESES)
			this->error->print(ERROR_CLOSED_PARANTHESES, l, sc->GetNumOfLine());
	}

	// ����� ������ ������, �.�. �� ����� ��������� ����� ���� ����� ������ - � ��� ������
	else
		this->error->print(ERROR_TElExpression, l, sc->GetNumOfLine());

}

// ����. ��.
void SyntAnalyzer::TPostUn()
{
	TypeLex l;
	int t, uk1;

	// ��.���.
	TElExpression();

	uk1 = sc->GetUK();
	t = sc->Scan(l);

	// ���� �� ���� ++ --
	// ���������� ��������� �� ��� �����
	if (t != TYPE_INCREMENT && t != TYPE_DECREMENT)
		sc->PutUK(uk1);
}


void SyntAnalyzer::TFuncCall()
{
	TypeLex l;
	int t, uk1;

	// ��������� �������������
	t = sc->Scan(l);
	if (t != TYPE_IDENT)
		this->error->print(ERROR_IDENT, l, sc->GetNumOfLine());

	// �������� "("
	t = sc->Scan(l);
	if (t != TYPE_OPENED_PARANTHESES)
		this->error->print(ERROR_OPENED_PARANTHESES, l, sc->GetNumOfLine());

	// �������� ")"
	t = sc->Scan(l);
	if (t != TYPE_CLOSED_PARANTHESES)
		this->error->print(ERROR_CLOSED_PARANTHESES, l, sc->GetNumOfLine());

	// �������� ;
	t = sc->Scan(l);
	if (t != TYPE_SEMICOLON)
		this->error->print(ERROR_SEMICOLON, l, sc->GetNumOfLine());
}


// ���������� ����������
void SyntAnalyzer::TVarsDeclar()
{
	TypeLex l;
	int t, uk1;

	// ������� ������� "��� ������"
	t = sc->Scan(l);
	if (t != TYPE_FLOAT && t != TYPE_DOUBLE && t != TYPE_INT)
		this->error->print(ERROR_TYPE, l, sc->GetNumOfLine());


	int uk2;
	// ������ ����������
	do
	{
		
		// ��������� �������������
		t = sc->Scan(l);
		if (t != TYPE_IDENT)
			this->error->print(ERROR_IDENT, l, sc->GetNumOfLine());

		uk2= sc->GetUK();
		t = sc->Scan(l);

		// ���� ������ �������������
		if (t != TYPE_ASSIGNMENT && t != TYPE_OPENED_BRACKET)
		{
			sc->PutUK(uk2);
		}

		// ������������� � ���������� - �������� ���������
		else if (t == TYPE_ASSIGNMENT)
			TExpression();


		// ����� ������������� � "["
		else if (t == TYPE_OPENED_BRACKET)
		{

			uk2 = sc->GetUK();
			t = sc->Scan(l);

			// ���� �������� "�������������[]", �� ��������� "= { <���������� ����������> }"
			if (t == TYPE_CLOSED_BRACKET)
			{

				// �������� "="
				t = sc->Scan(l);
				if (t != TYPE_ASSIGNMENT)
					this->error->print(ERROR_ASSIGMENT, l, sc->GetNumOfLine());

				// �������� "{"
				t = sc->Scan(l);
				if (t != TYPE_OPENED_BRACE)
					this->error->print(ERROR_OPENED_BRACE, l, sc->GetNumOfLine());

				// ���������� ����������
				do
				{
					TExpression();

					uk2 = sc->GetUK();
					t = sc->Scan(l);

				} while (t == TYPE_COMMA);
				sc->PutUK(uk2);

				// �������� }
				t = sc->Scan(l);
				if (t != TYPE_CLOSED_BRACE)
					this->error->print(ERROR_CLOSED_BRACE, l, sc->GetNumOfLine());
			}

			// ���� � ���������� ������� ���������� ��������� 
			else
			{
				sc->PutUK(uk2);
				TExpression();

				// �������� ]
				t = sc->Scan(l);
				if (t != TYPE_CLOSED_BRACKET)
					this->error->print(ERROR_CLOSED_BRACKET, l, sc->GetNumOfLine());

				uk2 = sc->GetUK();
				t = sc->Scan(l);

				// ���� ���� "="
				// �� ���������� ����������
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

				// ����� 
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

	// �������� ;
	t = sc->Scan(l);
	if (t != TYPE_SEMICOLON)
		this->error->print(ERROR_SEMICOLON, l, sc->GetNumOfLine());
}


// ������������
void SyntAnalyzer::TAssignment()
{
	TypeLex l;
	int t, uk1;

	t = sc->Scan(l);
	if (t != TYPE_IDENT)
		this->error->print(ERROR_IDENT, l, sc->GetNumOfLine());

	uk1 = sc->GetUK();
	t = sc->Scan(l);

	// ���������� ������
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
