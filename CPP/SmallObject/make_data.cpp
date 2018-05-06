#include <bits/stdc++.h>
using namespace std;
int bigrand() { return rand()|(rand()<<15); }
long long hugerand() { return bigrand()|((bigrand()+0ll)<<30); }
void make(int p)
{
	//Input the code
}
int main()
{
	char str[100];
	int i;
	long long lt;
	srand(time(NULL));
	for(i=0;i<10;i++)
	{
		str[0]='\0'; sprintf(str,"data\\data%d.in",i);
		freopen(str,"w",stdout);
		lt=clock();
		make(i);
		fprintf(stderr,"data%d.in  made, used %.3lf sec.\n",i,(clock()-lt)*1.0/CLOCKS_PER_SEC);
		fclose(stdout);
	}
	for(i=0;i<10;i++)
	{
		str[0]='\0';
		sprintf(str,"answer.exe < data\\data%d.in > data\\data%d.ans",i,i);
		lt=clock();
		system(str);
		fprintf(stderr,"data%d.ans made, used %.3lf sec.\n",i,(clock()-lt)*1.0/CLOCKS_PER_SEC);
	}
	return 0;
}
