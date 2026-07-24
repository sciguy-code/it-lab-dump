#include <stdio.h>
#include <string.h>

int isPalindrome(char *left, char *right) {
    if (left >= right) {
        return 1;
    }
    if (*left != *right) {
        return 0;
    }
    return isPalindrome(left + 1, right - 1);
}

int main() {
    char str[100];
    
    printf("Enter a string: ");
    scanf("%s", str);
    
    int len = strlen(str);
    
    if (len == 0 || isPalindrome(str, str + len - 1)) {
        printf("The string is a palindrome.\n");
    } else {
        printf("The string is not a palindrome.\n");
    }
    
    return 0;
}

/*
Observations --->

string as a continuous block of memory, and you place two pointers at opposite ends one at the start and one at the end. At each step, you compare the characters they point to. If they don’t match, you immediately conclude it’s not a palindrome. If they do match, you move both pointers inward (one step from each side) and repeat. With every recursive call, the problem size shrinks.
The process stops when the pointers meet or cross, meaning all symmetric pairs are checked. This works directly on the original string without extra copies. The time complexity is O(n) since each character is checked once, and the space complexity is O(n) due to recursion stack usage (no extra heap memory is used).






*/
