#include <stdbool.h>
#include <limits.h>

typedef struct
{
	long p;
	long q;
} rational;

long gcd(unsigned long u, unsigned long v)
{
	int shift;
	shift = __builtin_ctz(u | v);
	u >>= __builtin_ctz(u);
	do
	{
		v >>= __builtin_ctz(v);
		if (u > v)
		{
			unsigned long t = v;
			v = u;
			u = t;
		}
		v = v - u;
	} while (v != 0);
	return u << shift;
}

rational reduce(rational r)
{
	long lim = 600000;
	if (labs(r.p) < lim && labs(r.q) < lim)
	{
		return r;
	}

	int g;
	if (r.p == 0)
		return r;
	while ((g = gcd(labs(r.p), r.q)) != 1)
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
	//return reduce(new_rational(f1.p * f2.q + f2.p * f1.q, f1.q * f2.q));
	return new_rational(f1.p * f2.q + f2.p * f1.q, f1.q * f2.q);
}

rational mulq(rational f1, rational f2)
{
	//rational r1 = reduce(new_rational(f1.p, f2.q));
	//rational r2 = reduce(new_rational(f2.p, f1.q));
	//return new_rational(r1.p * r2.p, r1.q * r2.q);
	return reduce(new_rational(f1.p * f2.p, f1.q * f2.q));
}

rational divq(rational f1, rational f2)
{
	//return mulq(f1, new_rational(f2.q, f2.p));
	return reduce(new_rational(f1.p * f2.q, f1.q * f2.p));
}

signed char sign(rational r)
{
	return (r.p < 0 ? -1 : r.p == 0 ? 0 : 1);
}

int compare(rational r1, rational r2)
{
	r2.p = -r2.p;
	return sign(addq(r1, r2));
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
			swap_rows(rows, cols, m, j, pos++);
		}
		else if (s == 0)
		{
			(*n2)--;
			swap_rows(rows, cols, m, j, zeros--);
			j--;
		}
	}
}

bool fm_rat(size_t rows, size_t cols, rational m[rows][cols])
{
	// step 1
	int r = cols - 1;
	int r_prim = r - 1;
	int s = rows;

	// step 2
	int n1 = 0;
	int n2 = rows;

	group_sort(rows, cols, m, &n1, &n2);

	// step 3
	// POTENTIALLY MAKE THIS SHORTER, REMOVE OUTER LOOP
	for (int i = 0; i < n2; i++)
	{
		for (int j = 0; j < r_prim; j++)
			m[i][j] = divq(m[i][j], m[i][r_prim]);
		m[i][r] = divq(m[i][r], m[i][r_prim]);
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
		rational rat = {.p = -1, .q = 1};
		for (int i = 0; i < n1; i++)
			for (int j = n1; j < n2; j++)
			{
				for (int l = 0; l < r_prim; l++)
				{
					m_prim[currentRow][l] = addq(mulq(m[j][l], rat), m[i][l]);
				}
				m_prim[currentRow][r_prim] = addq(m[i][r], mulq(rat, m[j][r]));
				currentRow++;
			}

		for (int i = n2; i < s; i++)
		{
			for (int j = 0; j < r_prim; j++)
				m_prim[currentRow][j] = m[i][j];

			m_prim[currentRow][r_prim] = m[i][r];
			currentRow++;
		}

		return fm_rat(s_prim, r, m_prim);
	}
	else
	{

		if ((s - n2 + n1 * (n2 - n1)) == 0)
			return true;
		//step 5
		rational max = m[n1][r];
		rational min = m[0][r];
		for (int i = 1; i < n1; i++)
			if (compare(m[i][r], min) == -1)
				min = m[i][r];

		for (int i = n1 + 1; i < n2; i++)
			if (compare(m[i][r], max) == 1)
				max = m[i][r];

		for (int i = n2; i < s; i++)
			if (sign(m[i][r]) == -1)
				return false;

		return compare(min, max) == 1;
	}
}

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
	rational m[rows][cols + 1];

	for (size_t j = 0; j < rows; j++)
	{
		for (size_t i = 0; i < cols; i++)
			m[j][i] = new_rational(a[j][i], 1);
		m[j][cols] = new_rational(c[j], 1);
	}

	return fm_rat(rows, cols + 1, m);
}