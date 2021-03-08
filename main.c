#include <CONIO.H>
#include <STDLIB.H>
#include <STDIO.H>
#include <MATH.H>
#include <MALLOC.H>
#include <string.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

/****  The file header of bmp file 位图文件头*****/
#include <pshpack2.h> //This file turns 2 byte packing of structures on, then sizeof(BITMAPFILEHEADER)=14, otherwise sizeof(BITMAPFILEHEADER)=16
typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfoffBits;
} BITMAPFILEHEADER;
#include <poppack.h> //This file turns packing of structures off

/****  The information header of bmp file 位图信息头*****/
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    DWORD biWidth;
    DWORD biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompress;
    DWORD biSizeImage;
    DWORD biXPeIsPerMeter;
    DWORD biYPeIsPerMeter;
    DWORD biCIrUsed;
    DWORD biClrImprotant;
} BITMAPINFOHEADER;

/****  The RGB data of bmp file 图像RGB数据*****/
typedef struct tagRGBDATA{
    BYTE rgbBlue;
    BYTE rgbGreen;
	BYTE rgbRed;
} RGBDATA;

/**
    @return len The file length.
*/
long getFileLength(char *fileName)
{
    FILE *fp;
    char ch;
    fp = fopen(fileName,"r");
    int len = -1;
    while (ch !='\n'&&ch!=EOF) {
        ch = fgetc(fp);
        if (ch != ',') len++;
    }
    fclose(fp);
    return len;
}

/**
    @return len The file higt.
*/
long getFileHigt(char *fileName)
{
    FILE *fp;
    char ch;
    fp = fopen(fileName,"r");
    long len = 1;
    while ((ch) != EOF) {
        ch = fgetc(fp);
        if (ch == '\n') len++;
    }
    fclose(fp);
    return len;
}

/**
    @parme *fp The bmp file.
    @parme *fq The txt file.
    @parme fileDataToArray[][] Is a array,and save the txt data.
*/
int main(int argc, char *argv[])
{
	RGBDATA *bmpData=NULL;	//图像数据指针
	FILE *fp, *fq;				//BMP文件指针
	long i,j,k,len;
	long width=128;			//图像宽度
	long height=128;		//图像高度
	long dataSize=width*height;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;

    if(argc<3)
	{
		printf("\n    请指定您要生成的BMP文件名！\n");
		printf("\n    方法1：在cmd窗口输入： demo xxx.txt xxxx.bmp　后回车执行！\n");
		printf("\n    方法2：在VC的工程设置中添加参数： xxx.txt xxxx.bmp　后编译运行！\n");
		printf("\n    功能： 根据设定红绿蓝三色分量的值，控制显示不同的彩色条纹！\n\n");
		exit(0);
	}

    char *txtName = argv[1];
    char *bmpName = argv[2];

//    printf("%s\n", argv[1]);
    if((fp=fopen(argv[2], "wb+"))==NULL)
	{
		printf("Cannot open BMP file!");
		exit(0);
	}
	if ((fq=fopen(argv[1],"r")) == NULL) {
        printf("Cannot open TXT file!");
        exit(0);
	}

	long length = getFileLength(txtName);  //长度！！！！
    long higt = getFileHigt(txtName);     //高度！！！！！
    printf("higt = %ld length = %ld\n", higt,length);


    //把文件头数据写入bmp文件
	bmfHeader.bfType=0x4d42;
	bmfHeader.bfSize=14+40+width*height*3;
	bmfHeader.bfReserved1=0;
	bmfHeader.bfReserved2=0;
	bmfHeader.bfoffBits=0x36;
    fwrite(&bmfHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    //将信息头数据写入BMP文件
	bmiHeader.biSize=40;
	bmiHeader.biWidth=width;
	bmiHeader.biHeight=height;
	bmiHeader.biPlanes=1;
	bmiHeader.biBitCount=24;
	bmiHeader.biCompress=0;
	bmiHeader.biSizeImage=width*height*3;
	bmiHeader.biXPeIsPerMeter=0;
	bmiHeader.biYPeIsPerMeter=0;
	bmiHeader.biCIrUsed=0;
	bmiHeader.biClrImprotant=0;
    fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	//将文件数据转换到数组中
	long fileDataToArray[higt][length];

    i = 0;
    j = 0;
    char ch;
    long flag = 0;
    while (ch != EOF) {
        ch = fgetc(fq);
        if (ch !=',') {
            fileDataToArray[i][j] = ch=='_' ? 0:1;
            j++;
        }
        if (ch=='\n') {
            i++;
            flag = j;
            j = 0;
        }
    }

    //输出保存的信息
    for (i = 0; i < higt; i++) {
        for (j = 0; j < length; j++) {
            printf("%ld",fileDataToArray[i][j]);
        }
        printf("\n");
    }

    //根据数组保存的信息输出到bmp文件中
	if((bmpData=(RGBDATA*)malloc(width*height*3))==NULL)
	{
		printf("bmpData memory malloc error!");
	}
    	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			k=(height-i-1)*width + j;//计算bmp图片中第i行第j列图像数据在bmpData[]数组中的位置
            if (i<5|| height-i<5 || j < 5 || width-j < 5) {
                	bmpData[k].rgbBlue=0;
					bmpData[k].rgbGreen=0;
					bmpData[k].rgbRed=255;
            }
            else {
                    bmpData[k].rgbBlue=255;
					bmpData[k].rgbGreen=255;
					bmpData[k].rgbRed=255;
            }
            if (i >= 10 && i <= height-10 && j >=10 && j <= width-10) {
                if (fileDataToArray[i-10][j-10] == 1) {
                    bmpData[k].rgbBlue=0;
					bmpData[k].rgbGreen=0;
					bmpData[k].rgbRed=255;
                }
                else {
                    bmpData[k].rgbBlue=255;
					bmpData[k].rgbGreen=255;
					bmpData[k].rgbRed=255;
                }
            }
		}
	}

	fwrite(bmpData,sizeof(RGBDATA),dataSize,fp);//将bmpData所指内存中的RGB数据一次性写入BMP文件

	printf("\n    恭喜您！BMP文件已经成功生成！\n");
	printf("\n    请在当前目录下查看生成的BMP文件%s\n\n",argv[2]);

	free(bmpData);	//释放bmpData所指的内存空间
	bmpData=NULL;	//置bmpData为空指针
	fclose(fp);		//关闭fp所指文件
    fclose(fq);
    return 0;
}
