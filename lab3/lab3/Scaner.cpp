#include <string.h>
#include <iostream>
#include "defs.h"
#include "Scaner.h"

// список ключевых слов и их индексы
TypeLex Keyword[MAX_KEYW] = { "float", "double", "int",  "main", "do", "while", "return" };
int IndexKeyword[MAX_KEYW] = { TYPE_FLOAT, TYPE_DOUBLE, TYPE_INT, TYPE_MAIN, TYPE_DO, TYPE_WHILE, TYPE_RETURN };


void Scaner::GetData(char* FileName) 
{
	// функция чтения данных из файла

	char aa;

	FILE* in;
	errno_t error = fopen_s(&in, const_cast<char*>(FileName), "r");

	if (error != 0)
	{
		PrintError(const_cast<char*>("Отсутствует входной файл"), const_cast<char*>("\0"));
		exit(1);
	}

	int i = 0;
	while (!feof(in))
	{
		fscanf_s(in, "%c", &aa);

		if (!feof(in)) t[i++] = aa;

		if (i >= MAX_TEXT - 1)
		{
			PrintError(const_cast<char*>("Слишком большой размер исходного модуля"), const_cast<char*>(""));
			break;
		}
	}

	// добавляем знак конца модуля
	t[i] = '\0';
	fclose(in);
}

// конструктор класса - открывает файл и устанавливает указатель = 0
Scaner::Scaner(char* FileName)
{
	GetData(FileName);
	PutUK(0);
	this->num_of_str = 1;
}



// восстановить указатель
void Scaner::PutUK(int i)
{
	uk = i;
}

// получить указатель
int Scaner::GetUK()
{
	return uk;
}


// функция для печати ошибки
void Scaner::PrintError(char* error, char* a)
{

	if (a[0] == '\0')
		std::cout << "Ошибка: " << error << "  В строке: " << num_of_str << std::endl;
	else
		std::cout << "Ошибка: " << error << ". Неверный символ: " << a << "  В строке: " << num_of_str << std::endl;

}

int Scaner::Scan(TypeLex l)
{
	// функция сканирования

	// очищаем поле лексемы
	int i;
	for (i = 0; i < MAX_LEX; i++) l[i] = 0; 
	i = 0; 

	// цикл для проверки символов, которые игнорируем
	while (true) 
	{
		
		// пока есть пробелы, табуляции и переходы
		// пропускаем их
		while ((t[uk] == ' ') || (t[uk] == '\n') || (t[uk] == '\t'))
		{
			if (t[uk] == '\n')
				num_of_str++;
			uk++;

		}

		// если встретились ('/' и '/') или ('/' и '*')
		// проверяем однострочный или многострочный комментарий
		if ((t[uk] == '/' && t[uk + 1] == '/') || (t[uk] == '/' && t[uk + 1] == '*')) 
		{
			uk++;

			// если однострочный - ищем конец строки (комментария)
			if (t[uk] == '/') 
			{
				uk++;
				while ((t[uk] != '\n') && (t[uk] != '\0')) uk++;
				continue;
			}

			// если многострочный, в бесконечном цикле ищем конец комментария
			else if (t[uk] == '*') 
			{
				uk++;

				while (true) 
				{
					// пропускаем символы, которые лежат внутри комментария
					while ((t[uk] != '*') && (t[uk] != '\0'))
					{
						if (t[uk] == '\n')
							num_of_str++;

						uk++;
					}
					

					

					// дошли до конца - коментарий не был закрыт - возвращаем ошибку и прерываем цикл
					if (t[uk] == '\0') 
					{ 
						PrintError(const_cast<char*>("Многострочный комментарий не закрыт"), l);
						uk--; 
						break; 
					}
					
					uk++;
					// если комментарий не окончен - пропускаем все символы
					if (t[uk] != '/') 
					{ 
						uk++;  
						continue; 
					}
					
					break;
				}
				uk++;
				continue;
			}
		}
		// выходим из цикла
		break;
	}
	
	// если конец модуля
	if (t[uk] == '\0') 
	{ 
		l[0] = '\0'; 
		return TYPE_END; 
	}

	// иначе, если не дошли до конца файла
	else 
	{

		// если встретили цифру, проверяем длину числа
		if ((t[uk] >= '0') && (t[uk] <= '9')) 
		{
			bool is_long = false; // флаг для проверки

			l[i++] = t[uk++];
			while ((t[uk] >= '0') && (t[uk] <= '9'))
				if (i < MAX_LEX - 1)
					l[i++] = t[uk++]; 
				else 
				{ 
					uk++; 
					is_long = true; // устанавливаем флаг в true - число большое
				}

			// если число большое - печатаем ошибку и возвращаем ошибку
			if (is_long) 
			{
				PrintError(const_cast<char*>("целая константа слишком длинная"), const_cast<char*>("\0"));
				return TYPE_ERROR;
			}

			return TYPE_CONST_INT;
		}

		// проверка на буквы обоих регистров и спец символ нижнего подчеркиваня
		else if (((t[uk] >= 'a') && (t[uk] <= 'z')) || ((t[uk] >= 'A') && (t[uk] <= 'Z')) || (t[uk] == '_')) 
		{
			bool is_long = false;

			l[i++] = t[uk++];
			// пропускаем символы
			while (((t[uk] >= '0') && (t[uk] <= '9')) ||
				((t[uk] >= 'a') && (t[uk] <= 'z')) ||
				((t[uk] >= 'A') && (t[uk] <= 'Z')) ||
				(t[uk] == '_'))
				if (i < MAX_LEX - 1) l[i++] = t[uk++]; else { uk++;  is_long = true; }

			// если мы обрезаем идентификатор
			// и в программе два идентификатора, длина которых > MAX_LEX, 
			// и они различаются, например, только последним символом
			// тогда обрезав их, получим, что идентификатор один
			if (is_long) 
			{
				PrintError(const_cast<char*>("идентификатор слишком длинный"), const_cast<char*>("\0"));
				return TYPE_ERROR;
			}

			// проверка на ключевое слово
			for (int j = 0; j < MAX_KEYW; j++)
				// если есть в списке ключевых слов - вернуть это слово
				if (strcmp(l, Keyword[j]) == 0) 
					return IndexKeyword[j];

			// если не ключевое слово - возвращаем тип "идентификатор"
			return TYPE_IDENT;
		}
		

		else if (t[uk] == '\'') {

			// не будем включать апостроф в константу
			uk++;

			// если не дошли до конца модуля или строки
			if ((t[uk] != '\0') && (t[uk] != '\n'))
				// считываем любой символ
				l[i++] = t[uk++];


			if (t[uk] != '\'') 
			{
				PrintError(const_cast<char*>("cимвольная константа, отсутствует апостроф"), const_cast<char*>("\0"));
				return TYPE_ERROR;
			}

			uk++;
			return TYPE_CONST_CHAR;
		}


		// проверка на символ '<' или комбинацию '<='
		else if (t[uk] == '<') 
		{
			l[i++] = t[uk++];

			if (t[uk] == '=') 
			{
				l[i++] = t[uk++];
				return TYPE_LESS_EQUAL;
			}
			else return TYPE_LESS_THAN;
		}

		// проверка на символ '>' или комбинацию '>='
		else if (t[uk] == '>') 
		{
			l[i++] = t[uk++];

			if (t[uk] == '=') 
			{
				l[i++] = t[uk++];
				return TYPE_GREATER_EQUAL;
			}
			else return TYPE_GREATER_THAN;
		}

		// проверка на комбинацию символов '!='
		else if (t[uk] == '!') 
		{

			l[i++] = t[uk++];

			if (t[uk] == '=') 
			{
				l[i++] = t[uk++];
				return TYPE_NOT_EQUAL;
			}
			else 
			{
				PrintError(const_cast<char*>("Знак неравно !="), l);
				return TYPE_ERROR;
			}
		}

		// проверка на комбинацию символов '+=' или '++', или просто '+'
		else if (t[uk] == '+') 
		{
			l[i++] = t[uk++];

			if (t[uk] == '=') 
			{
				l[i++] = t[uk++];
				return TYPE_ADD_ASSIGN;
			}
			else if (t[uk] == '+') 
			{
				l[i++] = t[uk++];
				return TYPE_INCREMENT;
			}
			else return TYPE_ADD;
		}

		// проверка на комбинацию символов '-=' или '--', или просто '-'
		else if (t[uk] == '-') 
		{
			l[i++] = t[uk++];

			if (t[uk] == '=') 
			{
				l[i++] = t[uk++];
				return TYPE_SUB_ASSIGN;
			}
			else if (t[uk] == '-') 
			{
				l[i++] = t[uk++];
				return TYPE_DECREMENT;
			}
			else return TYPE_SUB;
		}

		// проверка на комбинацию символов *= или просто *
		else if (t[uk] == '*')
		{
			l[i++] = t[uk++];

			if (t[uk] == '=') 
			{
				l[i++] = t[uk++];
				return TYPE_MULT_ASSIGN;
			}
			else return TYPE_MULT;
		}

		// проверка на комбинацию символов /= просто /
		else if (t[uk] == '/') 
		{
			l[i++] = t[uk++];

			if (t[uk] == '=') 
			{
				l[i++] = t[uk++];
				return TYPE_DIV_ASSIGN;
			}
			else return TYPE_DIV;
		}

		// проверка на комбинацию символов '%=' или просто '%' 
		else if (t[uk] == '%') 
		{
			l[i++] = t[uk++];

			if (t[uk] == '=') 
			{
				l[i++] = t[uk++];
				return TYPE_MODULO_ASSIGN;
			}
			else return TYPE_MODULO;
		}

		// проверка на комбинацию символов '==' или просто '='
		else if (t[uk] == '=') 
		{
			l[i++] = t[uk++];

			if (t[uk] == '=') 
			{
				l[i++] = t[uk++];
				return TYPE_EQUAL_TO;
			}
			else return TYPE_ASSIGNMENT;
		}


		// второстепенные проверки на символы скобок, точки с запятой, запятой
		else if (t[uk] == '(') { l[i++] = t[uk++]; return TYPE_OPENED_PARANTHESES; }
		else if (t[uk] == ')') { l[i++] = t[uk++]; return TYPE_CLOSED_PARANTHESES; }
		else if (t[uk] == '{') { l[i++] = t[uk++]; return TYPE_OPENED_BRACE; }
		else if (t[uk] == '}') { l[i++] = t[uk++]; return TYPE_CLOSED_BRACE; }
		else if (t[uk] == '[') { l[i++] = t[uk++]; return TYPE_OPENED_BRACKET; }
		else if (t[uk] == ']') { l[i++] = t[uk++]; return TYPE_CLOSED_BRACKET; }
		else if (t[uk] == ';') { l[i++] = t[uk++]; return TYPE_SEMICOLON; }
		else if (t[uk] == ',') { l[i++] = t[uk++]; return TYPE_COMMA; }


		// если не обнаружили символ среди заданных - печатаем ошибку
		else 
		{
			l[i++] = t[uk++];
			PrintError(const_cast<char*>("неизвестный символ"), l);
			return TYPE_ERROR;
		}
	}
}



