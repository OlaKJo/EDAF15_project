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

int main(int argc, char* argv[])
{

	rational a, b;
	a.p = 4;
	a.q = 6;

	b.p = 5;
	b.q = 7;

	print_rational(addq(a,b));
	print_rational(subq(a,b));
	print_rational(mulq(a,b));
	print_rational(divq(a,b));

	return 0;
}

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
