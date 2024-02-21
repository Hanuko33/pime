#include <cstring>
#include <cstdlib>

char vowels[]="euyioa";
char consonant[]="qwrtpsdfghjklzxcvbnm";

char * get_name(int s)
{
    int v = strlen(vowels);
    int c = strlen(consonant);

    char *n= new char[1+2*s];
    for (int i=0; i< s; i++)
    {
        n[2*i]=consonant[rand() % c];
        n[2*i+1]=vowels[rand() % v];
    }
    n[2*s]=0;
    return n;
}

