#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct
{
	long p;
	long q;
} rational;

bool fm_rat(size_t rows, size_t cols, rational m[rows][cols]);

long gcd(long a, long b)
{
	long rem;

	if (a < b)
		return gcd(b, a);

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

void group_sort(size_t rows, size_t cols, rational m[rows][cols], int *n1, int *n2)
{
	int pos = 0;
	int zeros = rows - 1;
	for (size_t j = 0; j <= zeros; j++)
	{
		signed char s = sign(m[j][cols - 2]);
		if (s > 0)
		{
			(*n1)++;
			swap_rows(rows, cols, m, j, pos);
		}
		else if (s == 0)
		{
			(*n2)--;
			swap_rows(rows, cols, m, j, zeros);
			j--;
		}
	}
}

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
	rational m[rows][cols + 1];

	for (size_t j = 0; j < rows; j++)
	{
		for (size_t i = 0; i < cols; i++)
		{
			m[j][i] = new_rational(a[j][i], 1);
		}
		m[j][cols] = new_rational(c[j], 1);
	}

	return fm_rat(rows, cols + 1, m);
}

bool fm_rat(size_t rows, size_t cols, rational m[rows][cols])
{
	// step 1
	int r = cols - 1;
	int s = rows;

	// step 2
	int n1 = 0;
	int n2 = rows;

	group_sort(rows, cols, m, &n1, &n2);

	// step 3
	// POTENTIALLY MAKE THIS SHORTER, REMOVE OUTER LOOP
	for (int i = 0; i < n2; i++)
	{
		for (int j = 0; j < r; j++)
		{
			m[i][j] = divq(m[i][j], m[i][r - 2]);
		}
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
		rational m_prim[s_prim][r];
		int currentRow = 0;
		for (int i = 0; i < n1; i++)
		{
			for (int j = n1; j < n2; j++)
			{
				for (int l = 0; l < r - 1; l++)
				{
					rational rat = {.p = -1, .q = 1};
					m_prim[currentRow][l] = addq(mulq(m[j][l], rat), m[i][l]);
				}
				m_prim[currentRow][r-1] = subq(m[i][r], m[j][r]);
				currentRow++;
			}
		}

		for (int i = n2; i < s; i++)
		{
			for (int j = 0; j < r - 1; j++)
			{
				m_prim[currentRow][j] = m[i][j];
			}
			m_prim[currentRow][r - 1] = m[i][r];
			currentRow++;
		}

		return fm_rat(s_prim, r - 1, m_prim);
	}
	else
	{

		if ((s - n2 + n1 * (n2 - n1)) == 0)
			return true;
		//step 5
		rational max = m[n1][r];
		rational min = m[0][r];
		for (int i = 1; i < n1; i++)
		{
			if (compare(m[i][r], min) == -1)
			{
				min = m[i][r];
			}
		}

		for (int i = n1 + 1; i < n2; i++)
		{
			if (compare(m[i][r], max) == 1)
			{
				max = m[i][r];
			}
		}

		for (int i = n2; i < s; i++)
		{
			if (sign(m[i][r]) == -1)
				return false;
		}

		bool res = (compare(min, max) == 1);

		return res;
	}
}
