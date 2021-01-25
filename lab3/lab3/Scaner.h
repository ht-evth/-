#pragma once

#include "defs.h"

class Scaner {
private:
	
	TypeMod t;							// ������ �������� (������ ���� ����� ������)
	int uk;								// ���������
	int num_of_str;						// ����� ������� ������

public:
	void GetData(char*);				// ������ ������ �� �����
	Scaner(char* FileName);				// ����������� ������
	~Scaner() {};						// ���������� ������
	void PutUK(int i);					// ����������� �������� ���������
	int GetUK(void);					// �������� �������� ���������
	void PrintError(char*, char*);		// ������� ������ ������
	int Scan(TypeLex l);				// ������� ������������ 
};

