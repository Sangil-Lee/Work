#include <stdio.h>
main()
{
  FILE *file;
  char scurr[16];
  float curr;

  file = popen("caget SR:C03-BI{DCCT:1}I:Total-I","r");
  fgets(scurr, 16, file);
  scanf(scurr,"%f",&curr);
  printf("%f\n",curr);
  pclose(file);
}

