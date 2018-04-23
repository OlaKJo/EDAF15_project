#include <stdbool.h>
#include <stdio.h>

typedef struct
{
	int p;
	int q;
} rational;

void print_rational(rational);
int gcd(int, int);
void reduce(rational*);
rational addq(rational, rational);
rational subq(rational, rational);
rational mulq(rational, rational);
rational divq(rational, rational);

rational mulq(rational f1, rational f2)
{
	rational res;
	res.p = f1.p*f2.p;
	res.q = f1.q*f2.q;
	reduce(&res);
	return res;
}

rational subq(rational f1, rational f2)
{
	f2.p = -f2.p;
	return addq(f1,f2);
}

rational divq(rational f1, rational f2)
{
	int temp = f2.p;
	f2.p = f2.q;
	f2.q = temp;
	return mulq(f1,f2);
}

rational addq(rational f1, rational f2)
{
	rational res;
	res.p = f1.p*f2.q + f2.p*f1.q;
	res.q = f1.q*f2.q;
	reduce(&res);
	return res;
}

void print_rational(rational r)
{
	printf("%d/%d\n", r.p, r.q);
}

void reduce(rational* r)
{
	int g;
	while((g = gcd(r->p, r->q)) != 1) {
		r->p /= g;
		r->q /= g;
	}
}

int gcd(int a, int b)
{
	int tmp;
	int rem;

	if (a < b) {
		tmp = a;
		a = b;
		b = tmp;
	}

	rem = a % b;

	while (rem != 0) {
		a = b;
		b = rem;
		rem = a % b;
	}

	return b;
}

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
        signed char new_a[rows][cols];
        signed char new_c[rows];
        int order[3] = {1,-1,0};
        int n1, n2 = 0;

        int currentIndex = 0;
        for(int i = 0; i<3; i++) {
                for(int j = 0; j < rows; j++) {
                        if (order[i] * a[j][0] > 0) {
                                new_a[currentIndex][0] = a[j][0];
                                new_c[currentIndex++] = c[j];
                                if (order[i] == 1) 
                                        n1++;
                                else 
                                        n2++;

                                printf("%i\n", new_c[currentIndex-1]);
                        }
                        else if (a[j][0] == 0 && order[i] == 0) {
                                new_a[currentIndex][0] = a[j][0];
                                new_c[currentIndex++] = c[j];
                        }
                }
        }

        rational rat_c[rows];
        for(int i = 0; i<rows; i++) {
                rational rat = { .p = new_c[i], .q = new_a[i][0] };
                rat_c[i] = rat;
                print_rational(rat);
        }

	return 0;
}
