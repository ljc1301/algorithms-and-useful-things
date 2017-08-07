#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <windows.h>
const int MAX_LEN=150; const int MAX_N=100;
char pro[MAX_N][MAX_LEN],che[MAX_N][MAX_LEN];
char in[MAX_N][MAX_LEN],ans[MAX_N][MAX_LEN],temp[MAX_LEN];
long long ti[MAX_N],lt,t;
int i,n,ret,j,scr[MAX_N],score,tot;
int main()
{
	while(true)
	{
		system("cls");
		n=score=tot=0;
		printf("Enter 0 0 0 0 0 0 to end, Enter -1 -1 -1 -1 -1 -1 to exit\n");
		printf("#  program      time(ms) checker      input        answer      score\n");
		do
			printf("%02d ",++n);
		while(scanf("%s %lld %s %s %s %d",pro[n],&ti[n],che[n],in[n],ans[n],&scr[n])==6
		          && pro[n][0]!='0' && pro[n][0]!='-');
		if(pro[n][0]=='-') return 0;
		system("cls");
		printf("#  program      input        answer       time(ms) score result\n");
		for(i=1;i<n;i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
			printf("%02d %-12s %-12s %-12s -------- ----- Judging",i,pro[i],in[i],ans[i]);
			temp[0]='\0'; sprintf(temp,"start /min timer.exe %lld %s",ti[i],pro[i]);
			system(temp);
			temp[0]='\0'; sprintf(temp,"%s < %s > temp.out 2> temp.err",pro[i],in[i]);
			lt=clock();
			ret=system(temp);
			t=(clock()-lt)*CLOCKS_PER_SEC/1000;
			system("start /min taskkill /im timer.exe /f > nul");
			if(t>ti[i])
			{
				for(j=0;j<64;j++)
					printf("\b \b");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
				printf("%02d %-12s %-12s %-12s --------     0 Time Limit Exceeded",i,pro[i],in[i],ans[i]);
			}
			else
			{
				FILE * err=fopen("temp.err","r");
				if(ret!=0 || fgetc(err)!=EOF)
				{
					for(j=0;j<64;j++)
						printf("\b \b");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
					printf("%02d %-12s %-12s %-12s %8lld     0 Runtime Error",i,pro[i],in[i],ans[i],t);
				}
				else
				{
					temp[0]='\0'; sprintf(temp,"%s temp.out %s > nul",che[i],ans[i]);
					if(system(temp))
					{
						for(j=0;j<64;j++)
							printf("\b \b");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
						printf("%02d %-12s %-12s %-12s %8lld     0 Wrong Answer",i,pro[i],in[i],ans[i],t);
					}
					else
					{
						for(j=0;j<64;j++)
							printf("\b \b");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
						score+=scr[i];
						printf("%02d %-12s %-12s %-12s %8lld %5d Accepted",i,pro[i],in[i],ans[i],t,scr[i]);
					}
				}
				fclose(err);
			}
	        printf("\n");
	        tot+=scr[i];
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
		printf("score: ");
		if(score==tot && tot)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
		else if(score==0 && tot)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
		else
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
		printf("%d",score);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
		printf(" / %d",tot);
		system("del temp.err > nul");
		system("del temp.out > nul");
		system("pause > nul");
	}
	return 0;
}
