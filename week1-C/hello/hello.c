#include <cs50.h>
#include <stdio.h>

int main(void) {

    char user_name[20];
    printf("Enter your name: ");
    scanf("%s", user_name);
    printf("Hello, %s\n", user_name);
}