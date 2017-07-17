#include <cstdio>
#include <ctime>
#include <cstdlib>
int main(int args,char *argv[])
{
	long long lt,ms;
	char str[100];
	sscanf(argv[1],"%lld",&ms);
	str[0]='\0'; sprintf(str,"taskkill -im %s -f",argv[2]);
	lt=clock()+(ms+15)*CLOCKS_PER_SEC/1000;
	while(clock()<=lt);
	if(system(str)==0) while(1);
	return 0;
}
