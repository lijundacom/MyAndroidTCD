#include "../../../include/image/utility/ReadFile.h"
#include "../../../include/image/utility/DebugPrint.h"
/*
 * fileName：	读取文件的路径名
 * deepPoint：	一个深度上的点数
 * ensemble：	多项式滤波器阶数
 * IQOffset：	从第几组IQ数据开始读，内部IQOffset*ensemble*2*sizeof(signed short int)
 */
signed short int* ReadFile(char* fileName,int deepPoint,int ensemble,long IQOffset){
	FILE* fp = NULL;
	bool bRet = true;
	int head = 1;
	int deepth=deepPoint/2;
	int width = deepPoint/8;
	signed short int I;
	signed short int Q;
	if (fileName == NULL){
		//printf("fileName == NULL\n");
		LOGE("fileName == NULL");
	}
    //char cpath[50];
    //sprintf(cpath, "/sdcard/Download/data/IQdata4.dat");
	fp = fopen(fileName,"rb");
	if (fp == NULL){
		bRet == false;
		//printf("Read File Error\n");
		LOGE("ReadFile :Read File Error");
		exit(1);
	}
	signed short int* IQArray = (signed short int*)malloc((deepPoint*ensemble)*sizeof(signed short int)*2);
	memset(IQArray,0,(deepPoint*ensemble)*sizeof(signed short int)*2);
	if(fseek(fp,IQOffset*ensemble*2*sizeof(signed short int),SEEK_SET)!=0){
		LOGE("fseek error: maybe over the limite");
		exit(1);
	}
	for(int i=0;i<ensemble;i++){
		
			fread(&I,sizeof(signed short int),1,fp);
			fread(&Q,sizeof(signed short int),1,fp);
			for(int j=deepth-width;j<deepth+width;j++){
				IQArray[i*deepPoint*2+j*2]=I;
				IQArray[i*deepPoint*2+j*2+1]=Q;
			}
		
	}
	fclose(fp);
	fp=NULL;
	return IQArray;
}
void PrintArray(int deepPoint,int  ensemble,signed short int* IQArray){
    MyStreamBuf g_MyStreamBuf;
    std::cout.rdbuf(&g_MyStreamBuf);
    for (int i=0;i<deepPoint;i++){
			
			for (int j = 0;j<ensemble;j++){
				std::cout<<" ("<<IQArray[j*deepPoint*2+i*2]<<","<<IQArray[j*deepPoint*2+i*2+1]<<") ";
			}
		std::cout<<std::endl;
		}
}
void PrintArray(int deepPoint,int  ensemble,int* IQArray){
	MyStreamBuf g_MyStreamBuf;
	std::cout.rdbuf(&g_MyStreamBuf);
	for (int i=0;i<deepPoint;i++){

		for (int j = 0;j<ensemble;j++){
			std::cout<<" ("<<IQArray[j*deepPoint*2+i*2]<<","<<IQArray[j*deepPoint*2+i*2+1]<<") ";
		}
		std::cout<<std::endl;
	}
}
void PrintArray(int deepPoint,int  ensemble,float* IQArray){
    MyStreamBuf g_MyStreamBuf;
    std::cout.rdbuf(&g_MyStreamBuf);
	for (int i=0;i<deepPoint;i++){

			for (int j = 0;j<ensemble;j++){
				std::cout<<" ("<<IQArray[j*deepPoint*2+i*2]<<","<<IQArray[j*deepPoint*2+i*2+1]<<") ";
			}
		std::cout<<std::endl;
		}
}
void PrintArray(int deepPoint,signed short int* Array){
    MyStreamBuf g_MyStreamBuf;
    std::cout.rdbuf(&g_MyStreamBuf);
	for (int i=0;i<deepPoint;i++){
				std::cout<<" ("<<Array[i]<<") ";
		std::cout<<std::endl;
		}
}
void PrintArray(int deepPoint,int* Array){
	MyStreamBuf g_MyStreamBuf;
	std::cout.rdbuf(&g_MyStreamBuf);
	for (int i=0;i<deepPoint;i++){
		std::cout<<" ("<<Array[i]<<") ";
		std::cout<<std::endl;
	}
}
void PrintArray(int deepPoint,float* velocityArray){
    MyStreamBuf g_MyStreamBuf;
    std::cout.rdbuf(&g_MyStreamBuf);
	for (int i=0;i<deepPoint;i++){
			std::cout<<" "<<velocityArray[i]<<" ";
		}
	std::cout<<std::endl;
}
