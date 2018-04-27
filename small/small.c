#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct
{
	long p;
	long q;
} rational;

void print_rational(rational);
long gcd(long, long);
void reduce(rational *);
rational addq(rational, rational);
rational subq(rational, rational);
rational mulq(rational, rational);
rational divq(rational, rational);
int sign(rational r);
bool fm_rat(size_t rows, size_t cols, rational a[rows][cols], rational c[rows]);
void print_matrix(size_t rows, size_t cols, rational a[rows][cols]);

rational mulq(rational f1, rational f2)
{
	rational res;
	res.p = f1.p * f2.p;
	res.q = f1.q * f2.q;
	reduce(&res);
	return res;
}

rational subq(rational f1, rational f2)
{
	f2.p = -f2.p;
	return addq(f1, f2);
}

rational divq(rational f1, rational f2)
{
	if(f2.p == 0) 
		printf("division by zero\n");
	if(f1.p == 0)
		printf("division with result zero\n");
	long temp = f2.p;
	f2.p = f2.q;
	f2.q = temp;
	return mulq(f1, f2);
}

rational addq(rational f1, rational f2)
{
	rational res;
	res.p = f1.p * f2.q + f2.p * f1.q;
	res.q = f1.q * f2.q;
	reduce(&res);
	return res;
}
int compare(rational r1, rational r2)
{
	return sign(subq(r1, r2));
}

void print_rational(rational r)
{
	printf("%d/%d", r.p, r.q);
}

void reduce(rational *r)
{
	if (r->p == 0)
		return;
	r->p = sign(*r) * abs(r->p);
	r->q = abs(r->q);
	int g;
	while ((g = gcd(r->p, r->q)) != 1)
	{
		r->p /= g;
		r->q /= g;
	}
}

int sign(rational r)
{
	if (r.p == 0)
		return 0;
	if (r.p < 0 && r.q < 0)
	{
		return 1;
	}
	else if (r.p < 0)
	{
		return -1;
	}
	else if (r.q < 0)
	{
		return -1;
	}
	return 1;
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

	return abs(b);
}

void print_matrix(size_t rows, size_t cols, rational a[rows][cols])
{
	for(size_t i = 0; i < rows; i++)  {
		for(size_t j = 0; j < cols; j++) {
			print_rational(a[i][j]);
			printf(" ");
		}
		printf("\n");
	}
	printf("\n\n");		
}


bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{

	rational new_a[rows][cols];
	rational new_c[rows];

	for (size_t j = 0; j < rows; j++)
	{
		for (size_t i = 0; i < cols; i++)
		{
			rational rat = {.p = a[j][i], .q = 1};
			new_a[j][i] = rat;
		}
		rational rat = {.p = c[j], .q = 1};
		new_c[j] = rat;
	}
	//debugging, remove later
	printf("The original matrix: \n");
	print_matrix(rows, cols, new_a);

	bool res = fm_rat(rows, cols, new_a, new_c);
	return res;
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

	//debug code, remove later
	printf("the reordered matrix is: \n");
	print_matrix(rows, cols, new_a);
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
	
	//debug code, remove later
	printf("After division the matrix is: \n");
	print_matrix(rows, cols, new_a);

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

					/*
					 *print_rational(new_a[j][l]);
					 *printf("\nand\n");
					 *print_rational(new_a[i][l]);
					 *printf("\ngive: \n");
					 *print_rational(a_prim[currentRow][l]);
					 *printf("\n\n");
					 */
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
