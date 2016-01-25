#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>		// exit

// незаполненные ячейки равны нулю

typedef struct state_t
{
	int d[81];
} state_t;

bool solved = false;

void input(state_t *pst)
{
	char str[11];	// 9 символов, новая строка и нуль-терминатор

	printf("Заполните начальные условия (используйте пробел для пустых ячеек, в конце ряда можно не вводить).\n");
	for (int i = 0; i < 9; i++)
	{
		printf("Введите ряд №%d: ", i+1);
		if (!fgets(str, 11, stdin))
			exit(EXIT_FAILURE);
		// printf("(%zi) %s\n", strlen(str), str);
		int len = strlen(str);
		str[len-1] = 0;		// затираем перенос строки, если есть
		len = strlen(str);
		if (len < 9)
		{
			for (int k = len; k < 9; k++)
				str[k] = ' ';
		}
		// printf("(%zi) %s\n", strlen(str), str);
		for (int j = 0; j < 9; j++)
		{
			if (str[j] == ' ')
				pst->d[i*9+j] = 0;
			else if ((str[j] >= '0') && (str[j] <= '9'))
				pst->d[i*9+j] = (int)str[j] - 0x30;
			else
			{
				printf("Недопустимый ввод!\n");
				i--;		// повтор
				break;
			}
		}
	}
}

void print_sudoku(const state_t *pst)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			printf("%d ", pst->d[i*9+j]);
		printf("\n");
	}
}

// возвращает индекс первой незаполненной ячейки (0..80) или -1, если все заняты
int find_free(const state_t *pst)
{
	for (int i = 0; i < 81; i++)
	{
		if (pst->d[i] == 0)
			return i;
	}
	return -1;
}

// проверка на допустимость цифры в ряду
// возвращает false, если не допустима
// index - 0..80
bool check_row(const state_t *pst, int index, int value)
{
	int row = index / 9;
	int col = index % 9;

	for (int i = 0; i < 9; i++)
	{
		if (i == col)
			continue;
		if (pst->d[row*9+i] == 0)
			continue;		// ячейка не заполнена
		if (pst->d[row*9+i] == value)
			return false;	// такая цифра уже есть
	}
	return true;
}

// проверка на допустимость цифры в колонке
// возвращает false, если не допустима
// index - 0..80
bool check_column(const state_t *pst, int index, int value)
{
	int row = index / 9;
	int col = index % 9;

	for (int i = 0; i < 9; i++)
	{
		if (i == row)
			continue;
		if (pst->d[i*9+col] == 0)
			continue;		// ячейка не заполнена
		if (pst->d[i*9+col] == value)
			return false;	// такая цифра уже есть
	}
	return true;
}

// проверка на допустимость цифры в квадрате
// возвращает false, если не допустима
// index - 0..80
bool check_quad(const state_t *pst, int index, int value)
{
	int row = index / 9;
	int col = index % 9;
	int r0 = (row / 3) * 3;	// первый ряд квадрата
	int c0 = (col / 3) * 3;	// первая колонка квадрата

	for (int i = r0; i < r0+3; i++)
	{
		for (int j = c0; j < c0+3; j++)
		{
			if ((i == row) && (j == col))
				continue;
			if (pst->d[i*9+j] == 0)
				continue;		// ячейка не заполнена
			if (pst->d[i*9+j] == value)
				return false;	// такая цифра уже есть
		}
	}
	return true;
}

void visualize(const state_t *pst)
{
	char s[82] = {};
	for (int i = 0; i < 81; i++)
	{
		if (pst->d[i] == 0)
			s[i] = ' ';
		else
			s[i] = (char)(pst->d[i] + 0x30);
	}
	s[81] = 0;
	fprintf(stderr, "%s\r", s);
}

// алгоритм решения
// состояние передаётся копированием
void solve(state_t st)
{
	int i_free = find_free(&st);
	if (i_free == -1)
	{
		solved = true;
		printf("Решение:\n");
		print_sudoku(&st);
		return;
	}
	visualize(&st);

	// подбираем подходящую цифру на свободное место
	for (int n = 1; n <= 9; n++)
	{
		if (check_row(&st, i_free, n) == false)
			continue;
		if (check_column(&st, i_free, n) == false)
			continue;
		if (check_quad(&st, i_free, n) == false)
			continue;
		// цифра подходит, углубляемся
		st.d[i_free] = n;
		solve(st);
		if (solved)
			break;	// дальше не продолжать
	}
}

int main()
{
	state_t s = {};
	input(&s);
	// print_sudoku(&s);
	solve(s);
	if (!solved)
		printf("Нет решения!\n");
}
