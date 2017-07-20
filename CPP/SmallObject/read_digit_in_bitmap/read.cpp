#include <cstdio>
#include <windows.h>
#include <cmath>
#include <algorithm>
using namespace std;
typedef struct
{
    BYTE b;
    BYTE g;
    BYTE r;
}RGB;
const int max_lable_num=100;
struct number
{
	char lable[100];
	double p;
	bool operator <(const number& n) const { return p>n.p; }
}numbers[max_lable_num];
const int r=5,c=3;
int height,width,i,j,a,cnt[r*c],lnum;
RGB img[1500][1500];
double sum[r*c],t,add,psum;
char name[100];
unsigned char unuse;
BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER infoHeader;
FILE *pic,*data;
double sigma(double x) { if(exp(-x)==1/1.0) return 0; return 1/(1+exp(-x)); }
int main()
{
	//system("get_data");
	printf("Please input bitmap path(Ctrl+Z to end): ");
	while(scanf("%s",name)==1)
	{
	    pic=fopen(name,"rb");
	    if(pic!=NULL)
	    {
		    fread(&fileHeader,sizeof(BITMAPFILEHEADER),1,pic);
		    fread(&infoHeader,sizeof(BITMAPINFOHEADER),1,pic);
		    if(infoHeader.biBitCount==24)
		    {
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
		        for(i=0;i<r*c;i++) sum[i]=cnt[i]=0;
				for(i=0;i<height;i++)
					for(j=0;j<width;j++)
					{
						a=(int)(floor(i*1.0/height*r)*c+floor(j*1.0/width*c));
						sum[a]+=1-(img[i][j].r+img[i][j].g+img[i][j].b)/765.0;
						cnt[a]++;
					}
				for(i=0;i<r*c;i++) sum[i]/=cnt[i];
	    		data=fopen("data","r");
	    		lnum=0; psum=0;
				for(i=0;fscanf(data,"%s",numbers[i].lable)==1;i++,lnum++)
				{
					add=0;
					for(j=0;j<r*c;j++)
						fscanf(data,"%lf",&t),add+=t*sum[j];
					numbers[i].p=sigma(add);
					psum+=sigma(add);
				}
				for(i=0;i<lnum;i++) numbers[i].p/=psum;
				sort(numbers,numbers+lnum);
				for(i=0;i<lnum;i++)
					printf("%10s: %.1lf%%\n",numbers[i].lable,100*numbers[i].p);
				fclose(data);
		    }
		    fclose(pic);
		}
		printf("Please input bitmap path(Ctrl+Z to end): ");
	}
    return 0;
}
