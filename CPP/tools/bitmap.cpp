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
int height,width,size,i,j,k,l,a,cnt;
RGB img[640][640];
unsigned char unuse;
BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER infoHeader;
FILE *pic,*out;
const RGB black={0,0,0};
const RGB white={255,255,255};
bool equ(RGB a,RGB b)
{
    return ((int)a.r-b.r)*((int)a.r-b.r)+((int)a.g-b.g)*((int)a.g-b.g)
             +((int)a.b-b.b)*((int)a.b-b.b)<200;
}
int main()
{
    pic=fopen("test.bmp","rb");
    fread(&fileHeader,sizeof(BITMAPFILEHEADER),1,pic);
    fread(&infoHeader,sizeof(BITMAPINFOHEADER),1,pic);
    if(infoHeader.biBitCount==24)
    {
        out=fopen("ans.bmp","wb");
        fwrite(&fileHeader,sizeof(BITMAPFILEHEADER),1,out);
        fwrite(&infoHeader,sizeof(BITMAPINFOHEADER),1,out);
        height=infoHeader.biHeight;
        width=infoHeader.biWidth;
        size=width*height;
        if(width%4!=0) a=4-(width*3)%4;
        else a=0;
        for(i=0;i<height;i++)
        {
            fread(img[i],sizeof(RGB),width,pic);
            for(j=a;j>0;j--)
                fread(&unuse,1,1,pic);
        }
        for(i=0;i<height;i++)
        {
            for(j=0;j<width;j++)
            { 
                cnt=0;
                for(k=max(0,i-1);k<=min(height,i+1);k++)
                for(l=max(0,j-1);l<=min(width,j+1);l++)
                if(k!=i || l!=j)
                if(!equ(img[i][j],img[k][l]))
                cnt++;
                if(cnt>(min(height,i+1)-max(0,i-1)+1)*(min(width,j+1)-max(0,j-1)+1)/2)
                    fwrite(&black,sizeof(RGB),1,out);
                else
                    fwrite(&white,sizeof(RGB),1,out);
            }
            for(j=a;j>0;j--)
                fwrite(&unuse,1,1,out);
        }
        fclose(out);
        system("mspaint.exe ans.bmp");
    }
    fclose(pic);
    return 0;
}
