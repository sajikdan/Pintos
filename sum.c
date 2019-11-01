#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int main(int argc, char **argv)
{
    int a, b, c, d, fib, four;

    a = atoi(argv[1]);
    b = atoi(argv[2]);
    c = atoi(argv[3]);
    d = atoi(argv[4]);
	
	fib = fibonacci(a);
	four = sum_of_four_int(a,b,c,d);
    
	printf("%d %d\n", fib, four);
    /*int sum = 0;
    int num;
    int i, j;
    int f1 = 1;
    int f2 = 1;
    int f3 = 1;

    for (i = 0; i < argc; i++){
        num = atoi(argv[i]);
        if (i == 1){
            for (j = 2; j < num; j++){
                f3 = f1 + f2;
                f1 = f2;
                f2 = f3;
            }
        }
        sum += num;
    }
    */
    return EXIT_SUCCESS;
}
