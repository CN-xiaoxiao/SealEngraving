#include <CONIO.H>
#include <STDLIB.H>
#include <STDIO.H>
#include <MATH.H>
#include <MALLOC.H>
#include <string.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

/****  The file header of bmp file λͼ�ļ�ͷ*****/
#include <pshpack2.h> //This file turns 2 byte packing of structures on, then sizeof(BITMAPFILEHEADER)=14, otherwise sizeof(BITMAPFILEHEADER)=16
typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfoffBits;
} BITMAPFILEHEADER;
#include <poppack.h> //This file turns packing of structures off

/****  The information header of bmp file λͼ��Ϣͷ*****/
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

/****  The RGB data of bmp file ͼ��RGB����*****/
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
	RGBDATA *bmpData=NULL;	//ͼ������ָ��
	FILE *fp, *fq;				//BMP�ļ�ָ��
	long i,j,k,len;
	long width=128;			//ͼ����
	long height=128;		//ͼ��߶�
	long dataSize=width*height;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;

    if(argc<3)
	{
		printf("\n    ��ָ����Ҫ���ɵ�BMP�ļ�����\n");
		printf("\n    ����1����cmd�������룺 demo xxx.txt xxxx.bmp����س�ִ�У�\n");
		printf("\n    ����2����VC�Ĺ�����������Ӳ����� xxx.txt xxxx.bmp����������У�\n");
		printf("\n    ���ܣ� �����趨��������ɫ������ֵ��������ʾ��ͬ�Ĳ�ɫ���ƣ�\n\n");
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

	long length = getFileLength(txtName);  //���ȣ�������
    long higt = getFileHigt(txtName);     //�߶ȣ���������
    printf("higt = %ld length = %ld\n", higt,length);


    //���ļ�ͷ����д��bmp�ļ�
	bmfHeader.bfType=0x4d42;
	bmfHeader.bfSize=14+40+width*height*3;
	bmfHeader.bfReserved1=0;
	bmfHeader.bfReserved2=0;
	bmfHeader.bfoffBits=0x36;
    fwrite(&bmfHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    //����Ϣͷ����д��BMP�ļ�
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

	//���ļ�����ת����������
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

    //����������Ϣ
    for (i = 0; i < higt; i++) {
        for (j = 0; j < length; j++) {
            printf("%ld",fileDataToArray[i][j]);
        }
        printf("\n");
    }

    //�������鱣�����Ϣ�����bmp�ļ���
	if((bmpData=(RGBDATA*)malloc(width*height*3))==NULL)
	{
		printf("bmpData memory malloc error!");
	}
    	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			k=(height-i-1)*width + j;//����bmpͼƬ�е�i�е�j��ͼ��������bmpData[]�����е�λ��
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

	fwrite(bmpData,sizeof(RGBDATA),dataSize,fp);//��bmpData��ָ�ڴ��е�RGB����һ����д��BMP�ļ�

	printf("\n    ��ϲ����BMP�ļ��Ѿ��ɹ����ɣ�\n");
	printf("\n    ���ڵ�ǰĿ¼�²鿴���ɵ�BMP�ļ�%s\n\n",argv[2]);

	free(bmpData);	//�ͷ�bmpData��ָ���ڴ�ռ�
	bmpData=NULL;	//��bmpDataΪ��ָ��
	fclose(fp);		//�ر�fp��ָ�ļ�
    fclose(fq);
    return 0;
}
