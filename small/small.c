#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct
{
	long p;
	long q;
} rational;

bool fm_rat(size_t rows, size_t cols, rational a[rows][cols], rational c[rows]);

long gcd(long a, long b)
{
	long rem;

	if (a < b)
		return gcd(b,a);

	while ((rem = a % b) != 0)
	{
		a = b;
		b = rem;
	}

	return labs(b);
}

rational reduce(rational r)
{
	int g;
	if (r.p == 0)
		return r;
	while ((g = gcd(r.p, r.q)) != 1)
	{
		r.p /= g;
		r.q /= g;
	}
	return r;
}

rational new_rational(long p, long q)
{
	return (rational){.p = (q < 0 ? p * -1 : p), .q = (q < 0 ? q * -1 : q)};
}

rational addq(rational f1, rational f2)
{
	return reduce(new_rational(f1.p * f2.q + f2.p * f1.q, f1.q * f2.q));
}

rational mulq(rational f1, rational f2)
{
	return reduce(new_rational(f1.p * f2.p, f1.q * f2.q));
}

rational subq(rational f1, rational f2)
{
	f2.p = -f2.p;
	return addq(f1, f2);
}

rational divq(rational f1, rational f2)
{
	return mulq(f1, new_rational(f2.q, f2.p));
}

signed char sign(rational r)
{
	return (r.p < 0 ? -1 : r.p == 0 ? 0 : 1);
}

int compare(rational r1, rational r2)
{
	return sign(subq(r1, r2));
}

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
	rational new_a[rows][cols];
	rational new_c[rows];

	for (size_t j = 0; j < rows; j++)
	{
		for (size_t i = 0; i < cols; i++)
		{
			new_a[j][i] = new_rational(a[j][i], 1);
		}
		new_c[j] = new_rational(c[j], 1);
	}

	return fm_rat(rows, cols, new_a, new_c);
}

void swap_rows(size_t rows, size_t cols, rational a[rows][cols], int index1, int index2)
{
	rational tmp;
	for (size_t i = 0; i < cols; i++)
	{
		tmp = a[index1][i];
		a[index1][i] = a[index2][i];
		a[index2][i] = tmp;
	}
}

//void group_sort(size_t rows, size_t cols, rational a[rows][cols], rational c[rows])
//{
//	int pos = 0;
//	int zeros = rows - 1;
//	rational * temp;
//	rational c_temp;
//	for (size_t j = 0; j < rows; j++) {
//		signed char s = sign(a[j][cols - 1]);
//		if (s > 0) {
//			temp = a[pos];
//			a[pos] = ((rational *) (a + j));
//			*(a + j) = temp;
//			c_temp = c[pos];
//			c[pos++] = c[j];
//			c[j] = c_temp;
//		} else if (s == 0) {
//			temp = a[zeros];
//			a[zeros--] = a[j];
//			a[j] = temp;
//			c_temp = c[zeros];
//			c[zeros] = c[j];
//			c[j] = c_temp;
//			j--;
//		}
//}

void group_sort(size_t rows, size_t cols, rational a[rows][cols], rational c[rows], int *n1, int *n2)
{
	int pos = 0;
	int zeros = rows - 1;
	rational *temp;
	rational c_temp;
	for (size_t j = 0; j <= zeros; j++)
	{
		signed char s = sign(a[j][cols - 1]);
		if (s > 0)
		{
			(*n1)++;
			swap_rows(rows, cols, a, j, pos);
			swap_rows(rows, 1, c, j, pos++);
		}
		else if (s == 0)
		{
			(*n2)--;
			swap_rows(rows, cols, a, j, zeros);
			swap_rows(rows, 1, c, j, zeros--);
			j--;
		}
	}
}

bool fm_rat(size_t rows, size_t cols, rational a[rows][cols], rational c[rows])
{
	// step 1
	int r = cols;
	int s = rows;

	// step 2
	int n1 = 0;
	int n2 = rows;

	group_sort(rows, cols, a, c, &n1, &n2);

	// step 3
	for (int i = 0; i < n2; i++)
	{
		for (int j = 0; j < r - 1; j++)
		{
			a[i][j] = divq(a[i][j], a[i][r - 1]);
		}
		c[i] = divq(c[i], a[i][r - 1]);
	}

	// more variables needs to be eliminated
	if (r > 1)
	{
		// step 4
		// n2 > n1 exist negative coeffs

		// goto step 6 and 7
		int s_prim = s - n2 + n1 * (n2 - n1);
		if (s_prim == 0)
			return true;
		rational a_prim[s_prim][r - 1];
		rational c_prim[s_prim];
		int currentRow = 0;
		for (int i = 0; i < n1; i++)
		{
			for (int j = n1; j < n2; j++)
			{
				for (int l = 0; l < r - 1; l++)
				{
					rational rat = {.p = -1, .q = 1};
					a_prim[currentRow][l] = addq(mulq(a[j][l], rat), a[i][l]);
				}
				c_prim[currentRow] = subq(c[i], c[j]);
				currentRow++;
			}
		}

		for (int i = n2; i < s; i++)
		{
			for (int j = 0; j < r - 1; j++)
			{
				a_prim[currentRow][j] = a[i][j];
			}
			c_prim[currentRow] = c[i];
			currentRow++;
		}

		return fm_rat(s_prim, r - 1, a_prim, c_prim);
	}
	else
	{

		if ((s - n2 + n1 * (n2 - n1)) == 0)
			return true;
		//step 5
		rational max = c[n1];
		rational min = c[0];
		for (int i = 1; i < n1; i++)
		{
			if (compare(c[i], min) == -1)
			{
				min = c[i];
			}
		}

		for (int i = n1 + 1; i < n2; i++)
		{
			if (compare(c[i], max) == 1)
			{
				max = c[i];
			}
		}

		for (int i = n2; i < s; i++)
		{
			if (sign(c[i]) == -1)
				return false;
		}

		bool res = (compare(min, max) == 1);

		return res;
	}
}
