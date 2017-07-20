#include <cstdio>
#include <windows.h>
#include <algorithm>
using namespace std;
typedef struct
{
    BYTE b;
    BYTE g;
    BYTE r;
}RGB;
int height,width,r,c,i,j,k,l,a;
RGB img[1500][1500],ret;
long long sumr[1500][1500],sumg[1500][1500],sumb[1500][1500];
unsigned char unuse;
BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER infoHeader;
FILE *pic,*out;
void s(int x,int y)
{
	ret.r=(BYTE)((sumr[x+r-1][y+c-1]-sumr[x-1][y+c-1]-sumr[x+r-1][y-1]+sumr[x-1][y-1])/(r*c));
	ret.g=(BYTE)((sumg[x+r-1][y+c-1]-sumg[x-1][y+c-1]-sumg[x+r-1][y-1]+sumg[x-1][y-1])/(r*c));
	ret.b=(BYTE)((sumb[x+r-1][y+c-1]-sumb[x-1][y+c-1]-sumb[x+r-1][y-1]+sumb[x-1][y-1])/(r*c));
}
int main()
{
    pic=fopen("test.bmp","rb");
    if(pic!=NULL)
    {
	    fread(&fileHeader,sizeof(BITMAPFILEHEADER),1,pic);
	    fread(&infoHeader,sizeof(BITMAPINFOHEADER),1,pic);
	    if(infoHeader.biBitCount==24)
	    {
	        out=fopen("ans.bmp","wb");
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
	        scanf("%d %d",&r,&c);
			for(i=1;i<=height;i++)
				for(j=1;j<=width;j++)
				{
					sumr[i][j]=img[i-1][j-1].r;
					sumg[i][j]=img[i-1][j-1].g;
					sumb[i][j]=img[i-1][j-1].b;
				}
			for(i=1;i<=height;i++)
				for(j=1;j<=width;j++)
				{
					sumr[i][j]+=sumr[i][j-1];
					sumg[i][j]+=sumg[i][j-1];
					sumb[i][j]+=sumb[i][j-1];
				}
			for(i=1;i<=height;i++)
				for(j=1;j<=width;j++)
				{
					sumr[i][j]+=sumr[i-1][j];
					sumg[i][j]+=sumg[i-1][j];
					sumb[i][j]+=sumb[i-1][j];
				}
			infoHeader.biHeight=height-r+1; infoHeader.biWidth=width-c+1;
	        fwrite(&fileHeader,sizeof(BITMAPFILEHEADER),1,out);
	        fwrite(&infoHeader,sizeof(BITMAPINFOHEADER),1,out);
	        height=infoHeader.biHeight;
	        width=infoHeader.biWidth;
	        if(width%4!=0) a=4-(width*3)%4;
	        else a=0;
			for(i=1;i<=height;i++)
			{
				for(j=1;j<=width;j++)
	            {
	            	s(i,j);
	                fwrite(&ret,sizeof(RGB),1,out);
	            }
	            for(j=a;j>0;j--)
	                fwrite(&unuse,1,1,out);
	        }
	        fclose(out);
	        system("start mspaint.exe ans.bmp");
	    }
	    fclose(pic);
	}
    return 0;
}
