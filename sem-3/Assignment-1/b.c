/*
2. Given an array of n integers, write C program for reversing the contents of the array
without using another array.
*/

#include <stdio.h>
#include <limits.h>

void reverse_array()
{
    int n;
    printf("Enter the size of array : ");
    scanf("%d", &n);

    printf("Enter the array elements : ");
    int arr[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    printf("Reversed array : ");
    for (int i = n - 1; i >= 0; i--)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");
}

int main(void)
{
    reverse_array();
    return 0;
}