#include <iostream>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;
int n; // tedad khane haye khali // 5 default
void showSudoku();
bool help();
bool inEmpty(int num);
string positionString(int position);
int issetInBlock(int inputNum, int position);
int issetInColumn(int inputNum, int position);
int issetInRows(int inputNum, int position);
bool checkWin();
void createEmptyInSudoku();
bool curserInEmpty();
int myCin(char mode, int min, int max);
void generateSudoku(int numCell = 0);

HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
int Sudoku[81] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0
};
int *empty; //negahadary shomare cellol haye khali dar empty 
int curser; // location curser daron puzzle 
int curserP = 0; // chandomin sellol khali az empty array
string alert; // baraye namayesh khata
int main()
{
	time_t t = time(NULL);
	srand(t);
	generateSudoku(); // create sudoku puzzle
	cout << "number of free cells : \n";
	n = myCin('i', 1, 81);
	empty = new int[n]; // sellol haye khali 
	bool systemCls = true;//reset kardane safhe
	bool ok = true;
	createEmptyInSudoku(); // khali kardan n khane az puzzle
	curser = empty[0]; // avalin mogheiate curser
	char input;
	while (ok)
	{
		if (systemCls)
		{
			system("cls");
			showSudoku();
			systemCls = false;
			if (curser == -1) // -1 = win
			{
				SetConsoleTextAttribute(hstdout, 02);
				cout << "WIN";
				SetConsoleTextAttribute(hstdout, 15);
				break;
			}
		}
		input = _getch();
		if (input == 77 && curserP < n - 1) // input = right
		{
			curserP++;
			curser = empty[curserP]; // tagir mogheiate curser
			systemCls = true;
		}
		else if (input == 75 && curserP > 0) //input = left
		{
			curserP--;
			curser = empty[curserP];
			systemCls = true;
		}
		else if (input == 72) // up 
		{
			int b = curser % 9;
			for (int c = curser - b - 1; c >= 0; c--)
			{
				for (int c2 = n; c2 >= 0; c2--)
				{
					if (empty[c2] == c)
					{
						curserP = c2;
						curser = c;
						break;
					}
				}
				if (curser == c) break;
			}
			systemCls = true;
		}
		else if (input == 80) //down
		{
			int b = curser % 9;
			for (int c = curser - b + 9; c < 81; c++)
			{
				for (int c2 = 0; c2 < n; c2++)
				{
					if (empty[c2] == c)
					{
						curserP = c2;
						curser = c;
						break;
					}
				}
				if (curser == c) break;
			}
			systemCls = true;
		}
		else if (input > 48 && input < 58)
		{
			char YN;
			if (curserInEmpty()) //agar curser daron selloli empty ke karbar por karde bashad
			{
				cout << "are you sure to change this value ? Y/N \n";
				YN = getchar();
				if (YN == 'Y' || YN == 'y')
				{
					Sudoku[curser] = 0;
				}
				else
				{
					if (curserP < n - 1)
					{
						curserP++;
						curser = empty[curserP];
					}
					systemCls = true;
					continue;
				}
			}

			int setBlock = issetInBlock(input - 48, curser); // check kardan ghanon sudoku
			int setColumn = issetInColumn(input - 48, curser);
			int setRows = issetInRows(input - 48, curser);

			if (setBlock == -1 && setColumn == -1 && setRows == -1) //hame ghavanin dorst bod
			{
				Sudoku[curser] = input - 48;
				if (curserP < n - 1)
				{
					curserP++;
					curser = empty[curserP];
				}
				alert = "";
				systemCls = true;
			}
			else // adade vard shode khatata bod
			{
				alert = "ERROR : Number ";
				alert += input;
				alert += " is already written in the same column ";
				if (setBlock != -1)
				{
					alert += " (" + positionString(setBlock) + ')';
				}
				if (setColumn != -1 && setColumn != setBlock)
				{
					alert += " (" + positionString(setColumn) + ')';
				}
				if (setRows != -1 && setRows != setBlock && setRows != setColumn)
				{
					alert += " (" + positionString(setRows) + ')';
				}
				systemCls = true;
			}
		}
		else if (input == 'h' || input == 'H') // help
		{
			bool helpResult = help();
			cout << helpResult;
			if (!helpResult)
			{
				alert = "ERROR : you can't get help! ";
			}
			systemCls = true;
		}
		if (checkWin()) // ckeck win
		{
			alert = "";
			curser = -1;
			alert = "";
			systemCls = true;
		}
	}
	cout << endl;
	system("pause");
}
int issetInBlock(int inputNum, int position)
{
	int temp = position % 27; //mohasebat baraye peida kardan avalin khane az block
	int temp2 = temp % 3;
	int temp3 = temp / 9;

	temp = position - ((temp3)* 9) - temp2;

	for (int i = temp; i < temp + 9 * 3; i++)
	{
		if (inputNum == Sudoku[i]) return i; // seloli ke khata midahad
		if (((i + 1) % 3) == 0) i += 6; // raftan line baad
	}
	return -1; // bedone khata dar block
}

void generateSudoku(int numCell) // numCell default 0
{
	int random, errLog = 0;
	int i = numCell;
	bool ok = false;
	if (numCell <= 80) ok = true;
	for (int free = numCell; free < 81; free++) Sudoku[free] = 0;
	while (ok)
	{
		random = rand() % 9 + 1;
		if (issetInBlock(random, i) == -1 && issetInColumn(random, i) == -1 && issetInRows(random, i) == -1)
		{
			Sudoku[i] = random;
			i++;
		}
		else
		{
			errLog++;
			if (errLog > 8) // agar bad az 9 bar adade random khata dad
			{
				errLog = 0;
				for (int j = 0; j < 10; j++) // check kardane dobare az 1 ta 9
				{
					if (issetInBlock(j, i) == -1 && issetInColumn(j, i) == -1 && issetInRows(j, i) == -1)
					{
						Sudoku[i] = j;
						i++;
					}
					else
					{
						errLog++;
						if (errLog > 8) //agar az 1 ta 9 ham javab nadad
						{
							int temp = i % 9;
							generateSudoku(i - temp); //az aval line dobare por mikonim
							ok = false;
							break;
						}
					}//end else
				}//end for j
			}//end if errlog > 8
		}
	}
}
bool help()
{
	int numAnswer = 0;
	int answer = 0;
	for (int i = 0; i < n; i++)
	{
		numAnswer = 0;
		for (int j = 1; j < 10; j++)
		{
			int setBlock = issetInBlock(j, empty[i]);
			int setColumn = issetInColumn(j, empty[i]);
			int setRows = issetInRows(j, empty[i]);
			if (setBlock + setColumn + setRows == -3)
			{
				numAnswer++;
				answer = j;
			}
		}
		if (numAnswer == 1) // agar cellol faghat yek javab dasht
		{
			Sudoku[empty[i]] = answer;
			return true;
		}
	}
	return false;
}
void createEmptyInSudoku()
{
	int random = 0;
	for (int i = 0; i < n; i++)
	{
		int r = rand() % (81);
		for (int j = 0; j < i; j++) // jelogiri az ijad khaneye khali tekrari
		{
			while (empty[j] == r)
			{
				r = rand() % (81);
				j = 0;
			}
		}
		empty[i] = r;
	}
	sort(empty, empty + n);
	for (int i = 0; i < n; i++)
	{
		Sudoku[empty[i]] = 0; //khali kardan sudoku
	}
}
bool curserInEmpty()  //check curser daron khanei khali hast ya na // baraye showSudoku()
{
	if (Sudoku[curser] == 0) return false;
	for (int i = 0; i < n; i++)
	{
		if (curser == empty[i]) return true;
	}
	return false;
}
int issetInColumn(int inputNum, int position) // check kardane ghanon sudoku
{
	int r = 0;
	int temp = position / 9;
	int temp2 = position - temp * 9;
	int temp3 = position % 9;
	for (int i = 0; i < 9; i++)
	{
		r = i * 9 + temp3;
		if (inputNum == Sudoku[r]) return r;
	}
	return -1;
}
int issetInRows(int inputNum, int position)// check kardane ghanon sudoku
{
	int temp = position % 9;
	for (int i = position - temp; i < position - temp + 9; i++)
	{
		if (inputNum == Sudoku[i]) return i;
	}
	return -1;
}
string positionString(int position) //tabdil shomare cellol be string
{
	string result = "";
	char ch = 'a';
	ch += position / 9;
	char col = (char)((position % 9) + 1 + 48);
	result += ch;
	result += col;
	return result;
}

void showSudoku()
{
	char column = 'a';
	SetConsoleTextAttribute(hstdout, 3);
	cout << "    1  2  3    4  5  6    7  8  9 \n" << endl << column << "  ";
	column++;

	for (int i = 0; i < 81; i++)
	{
		SetConsoleTextAttribute(hstdout, 15);
		cout << ' ';
		if (i == curser)
		{
			SetConsoleTextAttribute(hstdout, 33);
			if (Sudoku[i] != 0) cout << Sudoku[i];
			else cout << ' ';
		}
		else if (Sudoku[i] == 0)
		{
			cout << ' ';
		}
		else if (inEmpty(i))
		{
			SetConsoleTextAttribute(hstdout, 2);
			cout << Sudoku[i];
		}
		else
		{
			SetConsoleTextAttribute(hstdout, 15);
			cout << Sudoku[i];
		}
		SetConsoleTextAttribute(hstdout, 15);
		cout << ' ';
		if (((i + 1) % 27) == 0)
		{
			cout << endl << "   ";
			SetConsoleTextAttribute(hstdout, 06);
			if (i < 54)
			for (int j = 0; j < 30; j++)
			{
				if (((j + 1) % 11) == 0) cout << '+';
				else cout << '-';
			}

			SetConsoleTextAttribute(hstdout, 3);
			if (column < 'j'){
				cout << endl << column << "  ";
				column++;
			}
			else cout << endl;
		}

		else if (((i + 1) % 9) == 0)
		{
			SetConsoleTextAttribute(hstdout, 3);
			cout << endl << column << "  ";
			column++;
		}
		else if (((i + 1) % 3) == 0)
		{
			cout << ' ';
			SetConsoleTextAttribute(hstdout, 06);
			cout << '|';
			SetConsoleTextAttribute(hstdout, 0);
		}
	}
	cout << "Press h to get help !" << endl;
	SetConsoleTextAttribute(hstdout, 4);
	cout << alert << endl;
	SetConsoleTextAttribute(hstdout, 15);
}
bool inEmpty(int num)
{
	for (int i = 0; i < n; i++)
	{
		if (num == empty[i]) return true;
	}
	return false;
}
bool checkWin()
{
	for (int i = 0; i < 81; i++)
	{
		if (Sudoku[i] == 0) return false;
	}
	return true;
}
int myCin(char mode, int min, int max) // for validation
{
	/*  i --> get a integer
	s --> get a string
	// max =  -1 --> no limit
	*/
	switch (mode)
	{
	case 'i':
		int integer = 0;
		bool ok = false;
		while (!ok)
		{
			cin >> integer;
			if (!cin.fail() && ((integer <= max || max == -1) && integer >= min) && (cin.peek() == EOF
				|| cin.peek() == '\n'))
			{
				ok = true;
			}
			else
			{
				cin.clear();
				cin.ignore(256, '\n');
				cout << "Error, Enter a number";
				if (max == -1)
				{
					cout << " bigger than " << min << endl;
				}
				else
				{
					cout << " between " << min << " and " << max << endl;
				}
			}
		}
		return integer;
		break;

		//case 's':
		//break;
	}
	return 1;
}