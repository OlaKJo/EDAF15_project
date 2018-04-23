#include <stdbool.h>

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
        signed char new_a[rows][cols];
        int order[3] = {1,-1,0};

        int currentIndex = 0;
        for(int i = 0; i<3; i++) {
                for(int j = 0; j < rows; j++) {
                        if (order[i] * a[j][0] > 0) {
                                new_a[currentIndex++][0] = a[j][0];
                                printf("%i\n", new_a[currentIndex-1][0]);
                        }
                        else if (a[j][0] == 0 && order[i] == 0)
                                new_a[currentIndex++][0] = a[j][0];
                }
        }

	return 0;
}
