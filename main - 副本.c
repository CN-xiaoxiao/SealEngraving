#include <CONIO.H>
#include <STDLIB.H>
#include <STDIO.H>
#include <MATH.H>
#include <MALLOC.H>
#include <string.h>

/****  文件信息头结构体 *****/

typedef struct tagBITMAPFILEHEADER {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfoffBits;
} BITMAPFILEHEADER;


/****  图像信息头结构体 *****/
typedef struct tagBITMAPINFOHEADER
{
    unsigned int    biSize;          // 此结构体的大小 (14-17字节)
    long            biWidth;         // 图像的宽  (18-21字节)
    long            biHeight;        // 图像的高  (22-25字节)
    unsigned short  biPlanes;        // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27字节)
    unsigned short  biBitCount;      // 一像素所占的位数，一般为24   (28-29字节)
    unsigned int    biCompression;   // 说明图象数据压缩的类型，0为不压缩。 (30-33字节)
    unsigned int    biSizeImage;     // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits (34-37字节)
    long            biXPelsPerMeter; // 说明水平分辨率，用象素/米表示。一般为0 (38-41字节)
    long            biYPelsPerMeter; // 说明垂直分辨率，用象素/米表示。一般为0 (42-45字节)
    unsigned int    biClrUsed;       // 说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）。 (46-49字节)
    unsigned int    biClrImportant;  // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53字节)
} BITMAPINFOHEADER;

/**   调色板 */
typedef struct _PixelInfo {
    unsigned char rgbBlue;   //该颜色的蓝色分量  (值范围为0-255)
    unsigned char rgbGreen;  //该颜色的绿色分量  (值范围为0-255)
    unsigned char rgbRed;    //该颜色的红色分量  (值范围为0-255)
    unsigned char rgbReserved;// 保留，必须为0
} PixelInfo;

/****  图像信息头结构体 *****/
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
    FILE *fp, *fq;           //BMP文件,TXT文件指针
    RGBDATA *bmpData=NULL;	//图像数据指针

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
//    printf("higt = %ld length = %ld\n", higt,length);

    //创建
    long width=128;			//图像宽度
	long height=128;		//图像高度
    long dataSize=width*height;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;

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
	bmiHeader.biCompression=0;
	bmiHeader.biSizeImage=width*height*3;
	bmiHeader.biXPelsPerMeter=0;
	bmiHeader.biYPelsPerMeter=0;
	bmiHeader.biClrUsed=0;
	bmiHeader.biClrImportant=0;
    fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	//将文件数据转换到数组中
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

    //输出保存的信息
    for (i = 0; i < higt; i++) {
        for (j = 0; j < length; j++) {
            printf("%ld",fileDataToArray[i][j]);
        }
        printf("\n");
    }

    //根据数组保存的信息输出到bmp文件中
    int k;
    if((bmpData=(RGBDATA*)malloc(width*height*3))==NULL)
	{
		printf("bmpData memory malloc error!");
	}

	//先在bmpData所指内存中置图像RGB数据，然后将所有数据写入BMP文件
	for(i=0;i<higt;i++)
	{
		for(j=0;j<width;j++)
		{
			k=(height-i-1)*width + j;//计算bmp图片中第i行第j列图像数据在bmpData[]数组中的位置
				bmpData[k].rgbBlue=255;
				bmpData[k].rgbGreen=255;
				bmpData[k].rgbRed=255;
		}

	}

	fwrite(bmpData,sizeof(RGBDATA),dataSize,fp);//将bmpData所指内存中的RGB数据一次性写入BMP文件

	printf("\n    恭喜您！BMP文件已经成功生成！\n");
	printf("\n    请在当前目录下查看生成的BMP文件%s\n\n",argv[2]);

	free(bmpData);	//释放bmpData所指的内存空间
	bmpData=NULL;

    fclose(fp);
    fclose(fq);
    return 0;
}
