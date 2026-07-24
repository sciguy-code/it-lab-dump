/*
4. Write a C program to find the frequency of a character present in the text given in a file.
*/

#include <stdio.h>

int main(void)
{

    FILE *f = fopen("file.txt", "r");

    char ch, x;
    int n = 0;

    printf("Enter the Character : ");
    scanf("%c", &ch);

    while ((x = fgetc(f)) != EOF)
    {
        if (x == ch) n++;
    }

    printf("Number of occurence of character `%c` in file.txt is %d.\n", ch, n);

    return 0;
}