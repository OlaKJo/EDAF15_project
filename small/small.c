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

long abs_long(long l)
{
	if (l < 0) 
		return l * (-1);
	return l;
}
long gcd(long a, long b)
{
	long tmp;
	long rem;

	if (a < b)
	{
		tmp = a;
		a = b;
		b = tmp;
	}

	rem = a % b;

	while (rem != 0)
	{
		a = b;
		b = rem;
		rem = a % b;
	}

	return abs_long(b);
}

rational reduce(rational r)
{
	if (r.p == 0)
		return r;
	int g;
	while ((g = gcd(r.p, r.q)) != 1)
	{	
		r.p /= g;
		r.q /= g;
	}
	return r;
}

rational new_rational(long p, long q)
{
 	return (rational) {.p = (q < 0 ? p * -1 : p), .q = (q < 0 ? q* -1 : q)};
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

int sign(rational r)
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

void copyRow(size_t rows, size_t cols, rational a[rows][cols], int src_pos, rational b[rows][cols], int dest_pos)
{
	for (size_t i = 0; i < cols; i++)
	{
		b[dest_pos][i] = a[src_pos][i];
	}
}

bool fm_rat(size_t rows, size_t cols, rational a[rows][cols], rational c[rows])
{
	rational new_a[rows][cols];
	rational new_c[rows];

	int order[3] = {1, -1, 0};

	// step 1
	int r = cols;
	int s = rows;

	// step 2
	int n1 = 0;
	int n2 = 0;
	int currentIndex = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < s; j++)
		{
			if (order[i] * sign(a[j][r - 1]) > 0)
			{
				copyRow(rows, cols, a, j, new_a, currentIndex);

				new_c[currentIndex++] = c[j];
				if (order[i] == 1)
					n1++;
				else
					n2++;
								

				//printf("%i\n", new_c[currentIndex - 1]);
			}
			else if (a[j][r - 1].p == 0 && order[i] == 0)
			{
				copyRow(rows, cols, a, j, new_a, currentIndex);
				new_c[currentIndex++] = c[j];
			}
		}
	}

	n2 += n1;

	// step 3
	for (int i = 0; i < n2; i++)
	{
		for (int j = 0; j < r - 1; j++)
		{
			new_a[i][j] = divq(new_a[i][j], new_a[i][r - 1]);
		}
		new_c[i] = divq(new_c[i], new_a[i][r - 1]);
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
					a_prim[currentRow][l] = addq(mulq(new_a[j][l], rat), new_a[i][l]);

				}
				c_prim[currentRow] = subq(new_c[i], new_c[j]);
				currentRow++;
			}
		}

		for (int i = n2; i < s; i++)
		{
			for (int j = 0; j < r - 1; j++)
			{
				a_prim[currentRow][j] = new_a[i][j];
			}
			c_prim[currentRow] = new_c[i];
			currentRow++;
		}

		return fm_rat(s_prim, r - 1, a_prim, c_prim);
	}
	else
	{

		if ((s - n2 + n1 * (n2 - n1)) == 0)
			return true;
		//step 5
		rational max = new_c[n1];
		rational min = new_c[0];
		for (int i = 1; i < n1; i++)
		{
			if (compare(new_c[i], min) == -1)
			{
				min = new_c[i];
			}
		}

		for (int i = n1 + 1; i < n2; i++)
		{
			if (compare(new_c[i], max) == 1)
			{
				max = new_c[i];
			}
		}

		for (int i = n2; i < s; i++)
		{
			if (sign(new_c[i]) == -1)
				return false;
		}

		bool res = (compare(min, max) == 1);

		return res;
	}
}
