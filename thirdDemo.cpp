﻿#include"stdafx.h"
#include<iostream>
using namespace std;
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main() {
	//输入图像
	GDALDataset* poSrcDS;
	GDALDataset* poSrcDS1;
	//输出图像
	GDALDataset* poDstDS;
	//图像高度和宽度
	int imgXlen, imgYlen;
	int tmpXlen, tmpYlen;
	//输入图像路径
	char* srcPath = "superman.jpg";
	char* srcPath1 = "space.jpg";
	//输出图像路径
	char* dstPath = "space_1.tif";
	//缓存
	GByte* buffTmp;
	GByte* buffTmp1;
	//图像波段数
	int i, j, k, bandNum;
	int RGB[3];

	//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	poSrcDS1 = (GDALDataset*)GDALOpenShared(srcPath1, GA_ReadOnly);

	//获取图像宽度，高度和波段数量
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();

	tmpXlen = poSrcDS1->GetRasterXSize();
	tmpYlen = poSrcDS1->GetRasterYSize();

	bandNum = poSrcDS->GetRasterCount();

	//根据图像的宽度和高度分配内存
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmp1 = (GByte*)CPLMalloc(tmpXlen*tmpYlen * sizeof(GByte));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//一个波段输入，一个波段输出 输出superman图像
	
	//读入写入超人整个图像
	for (i = 0;i < bandNum;i++) {
		poSrcDS->GetRasterBand(i+1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i+ 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	//判断超人的整个图像的像素点读到buffTmp1中，再把第二张图的背景填进去
	for (i = 0;i < tmpXlen;i++) {
		for (j = 0;j < tmpYlen;j++) {
			for (k = 0;k < bandNum;k++) {
				poSrcDS->GetRasterBand(k + 1)->RasterIO(GF_Read, i, j, 1, 1, buffTmp1, 1, 1, GDT_Byte, 0, 0);
				RGB[k] = buffTmp1[0];
			}
			if ((10 < RGB[0] && RGB[0] < 160) && (100 < RGB[1] && RGB[1] < 220) && (10 < RGB[2] && RGB[2] < 110)) {
				for (k = 0;k < bandNum;k++) {
					poSrcDS1->GetRasterBand(k + 1)->RasterIO(GF_Read, i, j, 1, 1, buffTmp1, 1, 1, GDT_Byte, 0, 0);
					poDstDS->GetRasterBand(k + 1)->RasterIO(GF_Write, i, j, 1, 1, buffTmp1, 1, 1, GDT_Byte, 0, 0);
				}
			}
		}
	}
		
		

	//清除内存
	CPLFree(buffTmp);
	CPLFree(buffTmp1);

	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);
	GDALClose(poSrcDS1);
	cout << "123456" << endl;
	system("PAUSE");
	return 0;
}

