#include <bits/stdc++.h>
using namespace std;
int bigrand() { return rand()|(rand()<<15); }
long long hugerand() { return bigrand()|((bigrand()+0ll)<<30); }
inline long long randint(long long l,long long r) { return hugerand()%(r-l+1)+l; }
FILE *f;
#define printf(format,args...) fprintf(f,format,##args)
void make()
{
    //Input the code
}
#undef printf
int main()
{
    srand(time(NULL));
    do
    {
        f=fopen("test.in","w");
        make();
        fclose(f);
        system("test < test.in > test.out");
        system("std < test.in > test.ans");
    }while(!system("fc test.out test.ans"));
    return 0;
}
