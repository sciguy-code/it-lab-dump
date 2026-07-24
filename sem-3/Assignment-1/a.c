/*
1. Write a program in C to find the third largest from the given array of integers without sorting.
*/

#include <stdio.h>
#include <limits.h>

void t_largest()
{
    int n;
    printf("Enter the size of array : ");

    if (scanf("%d", &n) != 1 || n < 3)
    {
        printf("Invalid input. Please enter at least 3 numbers.\n");
        return;
    }

    int f = INT_MIN;
    int s = INT_MIN;
    int t = INT_MIN;

    printf("Enter the array elements : \n");
    for (int i = 0; i < n; i++)
    {
        int curr;
        scanf("%d", &curr);

        if (curr > f)
        {
            t = s;
            s = f;
            f = curr;
        }
        else if (curr < f && curr > s)
        {
            t = s;
            s = curr;
        }
        else if (curr < s && curr > t)
        {
            t = curr;
        }
    }

    if (t == INT_MIN)
    {
        printf("There is no Third distinct largest number.\n");
    }
    else
    {
        printf("Third Largest number in the array : %d\n", t);
    }
}

int main(void)
{
    t_largest();
    return 0;
}