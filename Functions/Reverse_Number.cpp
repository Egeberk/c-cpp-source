
 
void reverse (int a)
{
   int  reverse = 0;
 

 
   while (a!= 0)
   {
      reverse = reverse * 10;
      reverse = reverse + a%10;
      a = a/10;
   }
 
   printf("Reverse of entered number is = %d\n", reverse);
 
   
}
