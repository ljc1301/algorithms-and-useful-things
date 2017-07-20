#include <cstdio>
#include <windows.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;
typedef struct
{
    BYTE b;
    BYTE g;
    BYTE r;
}RGB;
const int r=5,c=3;
int height,width,cnt[r*c],n,num[100000];
RGB img[1500][1500];
double sum[100000][r*c],ans[10][r*c],t,d;
unsigned char unuse;
long long lt,ld;
char name[100];
BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER infoHeader;
FILE *pic,*data;
double sigma(double x) { if(exp(-x)==1/1.0) return 0; return 1/(1+exp(-x)); }
double cost()
{
	int i,j,k;
	double ret=0,add,psum,p[10];
	for(i=0;i<n;i++)
	{
		psum=0;
		for(j=0;j<10;j++)
		{
			add=0;
			for(k=0;k<r*c;k++)
				add+=ans[j][k]*sum[i][k];
			psum+=(p[j]=sigma(add));
		}
		for(j=0;j<10;j++) p[j]/=psum;
		for(j=0;j<10;j++)
			if(j==num[i]) ret+=1-p[j];
			else ret+=p[j];
	}
	return ret/n;
}
double dcost(int a,int b)
{
	double ret;
	ans[a][b]+=d/2;
	ret=cost();
	ans[a][b]-=d;
	ret-=cost();
	ans[a][b]+=d/2;
	return ret/d;
}
int main()
{
	int i,j,a,k,l;
	double temp;
	srand(time(NULL));
	n=0;
	printf("Please input compute time(sec) and delta: ");
	scanf("%lf %lf",&t,&d);
	printf("Reading...\n");
	for(k=0;k<10;k++)
		for(l=0;;l++)
		{
			name[0]='\0'; sprintf(name,"bitmaps\\%d\\%d.bmp",k,l);
			pic=fopen(name,"rb");
			if(pic==NULL) break;
			fread(&fileHeader,sizeof(BITMAPFILEHEADER),1,pic);
			fread(&infoHeader,sizeof(BITMAPINFOHEADER),1,pic);
			if(infoHeader.biBitCount!=24) { fclose(pic); break; }
			num[n]=k;
	        height=infoHeader.biHeight;
	        width=infoHeader.biWidth;
	        if(width%4!=0) a=4-(width*3)%4;
	        else a=0;
	        for(i=0;i<height;i++)
	        {
	            fread(img[i],sizeof(RGB),width,pic);
	            for(j=a;j>0;j--)
	                fread(&unuse,1,1,pic);
	        }
	        for(i=0;i<r*c;i++) sum[n][i]=cnt[i]=0;
			for(i=0;i<height;i++)
				for(j=0;j<width;j++)
				{
					a=(int)(floor(i*1.0/height*r)*c+floor(j*1.0/width*c));
					sum[n][a]+=1-(img[i][j].r+img[i][j].g+img[i][j].b)/765.0;
					cnt[a]++;
				}
			for(i=0;i<r*c;i++) sum[n][i]/=cnt[i];
	    	fclose(pic);
			n++;
		}
	printf("Computing...\n");
	ld=lt=clock();
	for(i=0;i<10;i++) for(j=0;j<r*c;j++) ans[i][j]=rand()/10000.0;
	a=0;
	while(clock()-lt<CLOCKS_PER_SEC*t)
	{
		i=rand()%10; j=rand()%(r*c);
		//for(i=0;i<10;i++)
			//for(j=0;j<r*c;j++)
			//{
				temp=dcost(i,j);
				if(abs(temp)>1e-8)
					ans[i][j]-=temp/d;
			//}
	}
	printf("cost: %lf\nWriting...\n",cost());
	data=fopen("data","w");
	for(i=0;i<10;i++)
	{
		if(i) fprintf(data,"\n");
		for(j=0;j<r*c;j++)
			if((j+1)%c) fprintf(data,"%lf ",ans[i][j]);
			else fprintf(data,"%lf\n",ans[i][j]);
	}
	fclose(data);
	printf("Done.\n");
    return 0;
}
