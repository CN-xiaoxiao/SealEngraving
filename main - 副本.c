#include <CONIO.H>
#include <STDLIB.H>
#include <STDIO.H>
#include <MATH.H>
#include <MALLOC.H>
#include <string.h>

/****  �ļ���Ϣͷ�ṹ�� *****/

typedef struct tagBITMAPFILEHEADER {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfoffBits;
} BITMAPFILEHEADER;


/****  ͼ����Ϣͷ�ṹ�� *****/
typedef struct tagBITMAPINFOHEADER
{
    unsigned int    biSize;          // �˽ṹ��Ĵ�С (14-17�ֽ�)
    long            biWidth;         // ͼ��Ŀ�  (18-21�ֽ�)
    long            biHeight;        // ͼ��ĸ�  (22-25�ֽ�)
    unsigned short  biPlanes;        // ��ʾbmpͼƬ��ƽ��������Ȼ��ʾ��ֻ��һ��ƽ�棬���Ժ����1 (26-27�ֽ�)
    unsigned short  biBitCount;      // һ������ռ��λ����һ��Ϊ24   (28-29�ֽ�)
    unsigned int    biCompression;   // ˵��ͼ������ѹ�������ͣ�0Ϊ��ѹ���� (30-33�ֽ�)
    unsigned int    biSizeImage;     // ����������ռ��С, ���ֵӦ�õ��������ļ�ͷ�ṹ��bfSize-bfOffBits (34-37�ֽ�)
    long            biXPelsPerMeter; // ˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ��һ��Ϊ0 (38-41�ֽ�)
    long            biYPelsPerMeter; // ˵����ֱ�ֱ��ʣ�������/�ױ�ʾ��һ��Ϊ0 (42-45�ֽ�)
    unsigned int    biClrUsed;       // ˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ����� (46-49�ֽ�)
    unsigned int    biClrImportant;  // ˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��(50-53�ֽ�)
} BITMAPINFOHEADER;

/**   ��ɫ�� */
typedef struct _PixelInfo {
    unsigned char rgbBlue;   //����ɫ����ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbGreen;  //����ɫ����ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbRed;    //����ɫ�ĺ�ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbReserved;// ����������Ϊ0
} PixelInfo;

/****  ͼ����Ϣͷ�ṹ�� *****/
typedef struct tagRGBDATA{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
	unsigned char rgbRed;
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
    FILE *fp, *fq;           //BMP�ļ�,TXT�ļ�ָ��
    RGBDATA *bmpData=NULL;	//ͼ������ָ��

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
//    printf("higt = %ld length = %ld\n", higt,length);

    //����
    long width=128;			//ͼ����
	long height=128;		//ͼ��߶�
    long dataSize=width*height;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;

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
	bmiHeader.biCompression=0;
	bmiHeader.biSizeImage=width*height*3;
	bmiHeader.biXPelsPerMeter=0;
	bmiHeader.biYPelsPerMeter=0;
	bmiHeader.biClrUsed=0;
	bmiHeader.biClrImportant=0;
    fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	//���ļ�����ת����������
	long fileDataToArray[higt][length];
    long i,j;
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
    int k;
    if((bmpData=(RGBDATA*)malloc(width*height*3))==NULL)
	{
		printf("bmpData memory malloc error!");
	}

	//����bmpData��ָ�ڴ�����ͼ��RGB���ݣ�Ȼ����������д��BMP�ļ�
	for(i=0;i<higt;i++)
	{
		for(j=0;j<width;j++)
		{
			k=(height-i-1)*width + j;//����bmpͼƬ�е�i�е�j��ͼ��������bmpData[]�����е�λ��
				bmpData[k].rgbBlue=255;
				bmpData[k].rgbGreen=255;
				bmpData[k].rgbRed=255;
		}

	}

	fwrite(bmpData,sizeof(RGBDATA),dataSize,fp);//��bmpData��ָ�ڴ��е�RGB����һ����д��BMP�ļ�

	printf("\n    ��ϲ����BMP�ļ��Ѿ��ɹ����ɣ�\n");
	printf("\n    ���ڵ�ǰĿ¼�²鿴���ɵ�BMP�ļ�%s\n\n",argv[2]);

	free(bmpData);	//�ͷ�bmpData��ָ���ڴ�ռ�
	bmpData=NULL;

    fclose(fp);
    fclose(fq);
    return 0;
}
