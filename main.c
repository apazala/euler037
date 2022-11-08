#include <inttypes.h>
#include <math.h>
#include <stdio.h>

#define baisset(ba, i) ((ba)[(i)>>6] & (1ull << ((i) & 63))) != 0
#define baisclear(ba, i) ((ba)[(i)>>6] & (1ull << ((i) & 63))) == 0
#define baset(ba, i) (ba)[(i)>>6] |= (1ull << ((i) & 63))

void fillprimes(int upperBound);
uint64_t iscompound[16000];
uint64_t istruncl[16000];
uint64_t istruncr[16000];
uint64_t checkedr[16000];
uint64_t checkedl[16000];

int primesList[80000];
int numprimes;

int mult10[10];

int is_truncr(int n) {

	if (baisset(checkedr, n)) {
		return  baisset(istruncr, n);
	}

	int	istrr = baisclear(iscompound, n) && is_truncr(n/=10);

	baset(checkedr, n);

	if(istrr)
		baset(istruncr, n);

	return istrr;
}


int is_truncl(int n, int ndig) {

	if (baisset(checkedl, n)) {
		return  baisset(istruncl, n);
	}

	ndig--;
	int	istrl = baisclear(iscompound, n) && is_truncl(n % mult10[ndig], ndig);

	baset(checkedl, n);

	if (istrl)
		baset(istruncl, n);

	return istrl;
}

#define LIMIT 1000000
void init() {
	fillprimes(LIMIT);

	mult10[0] = 1;
	baset(checkedr, 0);
	baset(checkedl, 0);
	for (int i = 1; i < 10; i++) {
		mult10[i] = 10 * mult10[i - 1];
		baset(checkedr, i);
		baset(checkedl, i);
	}

	for (int i = 0; primesList[i] < 10; i++) {
		int p = primesList[i];
		baset(istruncr, p);
		baset(istruncl, p);
	}

}

int main()
{
	init();

	int count = 0;
	int sum = 0;
	int ndig = 0;
	for (int i = 4; primesList[i] < LIMIT; i++) {
		int p = primesList[i];
		while (mult10[ndig] < p)
			ndig++;

		if (is_truncl(p, ndig) && is_truncr(p)) {
			count++;
			printf("Found %d: %d\n", count, p);
			sum += p;
		}
	}

	printf("%d\n", sum);

    return 0;
}


void fillprimes(int upperBound) {
	int i, j;

	baset(iscompound, 0);
	baset(iscompound, 1);
	int sqrtUb = sqrt(upperBound);
	for (i = 2; i <= sqrtUb; i++) {
		if (baisclear(iscompound, i)) {
			primesList[numprimes++] = i;
			for (j = i*i; j <= upperBound; j += i) {
				baset(iscompound, j);
			}
		}
	}

	for (i = sqrtUb + 1; i <= upperBound; i++) {
		if (baisclear(iscompound, i)) {
			primesList[numprimes++] = i;
		}
	}

}