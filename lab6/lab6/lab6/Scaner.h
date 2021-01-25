#pragma once

#include "defs.h"
#include "Error.h"

class Scaner 
{
private:
	
	TypeMod t;							// ������ �������� (������ ���� ����� ������)
	int uk;								// ���������
	int num_of_str;						// ����� ������� ������
	int old_num_of_str;					// ��� �������������� ������ ������, ��� ����������� ���������

public:
	ErrorPrinter error;					// ����������� ������


public:

	void GetData(char*);				// ������ ������ �� �����
	Scaner(char* FileName);				// ����������� ������
	~Scaner() {};						// ���������� ������
	void PutUK(int i);					// ����������� �������� ���������
	int GetUK(void);					// �������� �������� ���������
	int GetNumOfLine();					// �������� ����� ������� ������
	int Scan(TypeLex l);				// ������� ������������ 

};

