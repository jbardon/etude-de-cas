#include <stdio.h>
#include <ctype.h>
 
void afficher(int etat[], char *t[])
{
  int i;
  for (i = 0; i < 5; i++)
    if (etat[i])
      printf("%s ", t[i]);
  puts("");
}
 
void partie(int h, int etat[], char *t[])
{
  enum { ABSENT, PRESENT };
  if (h < 0)
    afficher(etat, t);
  else
    {
      etat[h] = ABSENT;
      partie(h - 1, etat, t);
      etat[h] = PRESENT;
      partie(h - 1, etat, t);
    }
}
 
int main(void)
{
  char *t[5] = { "A", "B", "C", "D", "E" };
  int etat[5];
  partie(5 - 1, etat, t);
  return 0;
}
