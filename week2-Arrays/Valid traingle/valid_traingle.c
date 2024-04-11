#include <cs50.h>
#include <stdio.h>

bool valid_triangle(int, int, int);

int main(void)


   {
int a = 0, b = 0, c = 0;

bool z = valid_triangle(a, b, c);
// printf("%i", z);
if(z){
     printf("True\n");
     return 0;
}
else
{
    printf("False\n");
    return 12;
}
return 1;
   }

   bool valid_triangle(int a, int b, int c)
   {
   a = get_int("Enter first side: ");
   b = get_int("Enter the seconde side: ");
   c = get_int("Enter the last side: ");
   if ((a > 0 && b > 0 && c > 0) && (a + b > c && a + c > b && c + b > a))
   {
    printf("VALID TRAINGLE\n");
    return true;
   }
   else
   {
    printf("UNVALID\n");
   }
   return false;
    }
