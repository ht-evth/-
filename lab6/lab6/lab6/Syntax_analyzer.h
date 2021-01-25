#pragma once

#include "Scaner.h"
#include "Error.h"

class SyntAnalyzer
{

private:
	Scaner* sc;
	ErrorPrinter* error;

	bool is_errors_exist = false;

	void TStatement();
	void TCompoundStatement();
	void TDoWhile();
	void TExpression();
	void TSummand();
	void TFactor();
	void TPrefUn();
	void TPostUn();
	void TElExpression();
	void TFuncCall();
	void TVarsDeclar();
	void TAssignment();


public:
	~SyntAnalyzer() {}

	SyntAnalyzer(Scaner* s)
	{
		sc = s;
		error = new ErrorPrinter();
	}

	void TProgram();
	int getNumberOfErrors() { return this->error->getAmountOfErrors(); }

};

