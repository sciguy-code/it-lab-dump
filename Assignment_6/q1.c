#include <stdio.h>
#include <stdarg.h>

int find(int key, int n, ...)
{
    va_list ap;
    va_start(ap, n);
    for(int i = 0; i < n; i++)
    {
        int x = va_arg(ap, int);
        if(x == key)
        {
            va_end(ap);
            return 1;
        }
    }
    va_end(ap);
    return 0;
}

int main()
{
    int res = find(5, 6, 1, 3, 5, 7, 9, 11);
    if(res) printf("found\n");
    else printf("not found\n");
    return 0;
}
