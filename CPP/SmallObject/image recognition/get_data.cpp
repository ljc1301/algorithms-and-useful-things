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
const int r=5,c=3,max_lable_num=1000,max_bitmap_num=100000;
int height,width,cnt[r*c],n,num[max_bitmap_num],lnum;
RGB img[1500][1500];
double sum[max_bitmap_num][r*c],ans[max_bitmap_num][r*c],t,d;
unsigned char unuse;
long long lt;
char name[100],lables[max_lable_num][100];
BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER infoHeader;
FILE *pic,*data;
double sigma(double x) { if(exp(-x)==1/1.0) return 0; return 1/(1+exp(-x)); }
double cost()
{
	int i,j,k;
	double ret=0,add,psum,p[max_lable_num];
	for(i=0;i<n;i++)
	{
		psum=0;
		for(j=0;j<lnum;j++)
		{
			add=0;
			for(k=0;k<r*c;k++)
				add+=ans[j][k]*sum[i][k];
			psum+=(p[j]=sigma(add));
		}
		for(j=0;j<lnum;j++) p[j]/=psum;
		for(j=0;j<lnum;j++)
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
	int i,j,a,k;
	double temp;
	srand(time(NULL));
	lnum=n=0;
	printf("Please input compute time(sec) and delta: ");
	scanf("%lf %lf",&t,&d);
	printf("Reading...\nDirectory name(Ctrl+Z to end): bitmaps\\");
	while(scanf("%s",lables[lnum])==1)
	{
		for(k=0;;k++)
		{
			name[0]='\0'; sprintf(name,"bitmaps\\%s\\%d.bmp",lables[lnum],k);
			pic=fopen(name,"rb");
			if(pic==NULL) break;
			fread(&fileHeader,sizeof(BITMAPFILEHEADER),1,pic);
			fread(&infoHeader,sizeof(BITMAPINFOHEADER),1,pic);
			if(infoHeader.biBitCount!=24) { fclose(pic); break; }
			num[n]=lnum;
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
		lnum++;
		printf("Directory name(Ctrl+Z to end): bitmaps\\");
	}
	printf("Computing...\n");
	lt=clock();
	for(i=0;i<lnum;i++) for(j=0;j<r*c;j++) ans[i][j]=rand()/10000.0;
	while(clock()-lt<CLOCKS_PER_SEC*t)
	{
		i=rand()%lnum; j=rand()%(r*c);
		//for(i=0;i<lnum;i++)
		//	for(j=0;j<r*c;j++)
		//	{
				temp=dcost(i,j);
				if(abs(temp)>1e-8)
					ans[i][j]-=temp/d;
		//	}
	}
	printf("cost: %lf\nWriting...\n",cost());
	data=fopen("bitmaps\\data","w");
	for(i=0;i<lnum;i++)
	{
		if(i) fprintf(data,"\n");
		fprintf(data,"%s\n",lables[i]);
		for(j=0;j<r*c;j++)
			if((j+1)%c) fprintf(data,"%lf ",ans[i][j]);
			else fprintf(data,"%lf\n",ans[i][j]);
	}
	fclose(data);
	printf("Done.\n");
    return 0;
}
