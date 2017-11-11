#include <string.h>
#include <stdio.h>

void reverse(
    char* str
) {
    if (str == NULL) {
        return;
    }
    size_t right = strlen(str);
    if (right < 2) {
        return;
    }
    --right;
    size_t left = 0;
    while (left < right) {
        char ch = str[left];
        str[left] = str[right];
        str[right] = ch;
        ++left;
        --right;
    }
}

int main() {
    {
        char str0[] = "a";
        printf("0 before: '%s'\n", str0);
        reverse(str0);
        printf("0 after: '%s'\n", str0);
    }
    {
        char str[] = "ab";
        printf("1 before: '%s'\n", str);
        reverse(str);
        printf("1 after: '%s'\n", str);
    }
    {
        char str[] = "abc";
        printf("2 before: '%s'\n", str);
        reverse(str);
        printf("2 after: '%s'\n", str);
    }
    {
        char str[] = "abcd";
        printf("3 before: '%s'\n", str);
        reverse(str);
        printf("3 after: '%s'\n", str);
    }
    {
        char str[] = "";
        printf("4 before: '%s'\n", str);
        reverse(str);
        printf("4 after: '%s'\n", str);
    }
    {
        char* str = NULL;
        reverse(str);
    }
    return 0;
}
