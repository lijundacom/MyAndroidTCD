/*
 * MultiDeepModeTCDThread.CPP
 *
 *  Created on: Apr 21, 2016
 *      Author: ljd
 */

#include "../../../include/image/mode/MultiDeepModeTCDThread.h"
#include "../../../include/image/utility/ThreadManage.h"

#include "../../../include/image/buffer/CycleBuffer.h"
#include "../../../include/image/utility/SynSemaphore.h"
#include "../../../include/image/signalprocess/MultiDeepModeTCDSignalManage.h"

#include "../../../include/image/utility/TimerTest.h"

#include "../../../include/image/utility/ImageDef.h"
#include "../../../include/image/utility/FrameStructure.h"
#include "../../../include/image/utility/ReadFile.h"
#include <sys/prctl.h>
#include <list>

//MultiDeepModeTCD Mode Signal Process class obj
static SignalProcess::MultiDeepModeTCDSignalManage objMultiDeepModeTCDSignalManage;
//MultiDeepModeTCD模式的数据帧，解析模块和信号处理模块之间的缓冲，这里仅仅用与采用CSimDataThread仿真Color数据时使用
//DEEP_POINTS应该是动态变换的，从数据头文件解析得到
static PingPangBuffer<INT16> ParsedMultiDeepModeTCDBuffer(sizeof(INT16) * MAX_MD_ENSEMBLE * DEEP_POINTS*2);

//MultiDeepModeTCD模式的数据帧，图像处理模块和显示模块之间的缓冲
static Buffer::CycleBuffer<int> MultiDeepModeTCDFrameBuffer(sizeof(int) * DEEP_POINTS);

// ------------------------------------------------------------
// Description	:获取解析之后的MultiDeep缓冲区数据
// Parameter	:void
// Retrun Value	:ParsedCBuffer的引用
// ------------------------------------------------------------
PingPangBuffer<INT16> * GetParsedMultiDeepModeTCDBuffer(void)
{
	return &ParsedMultiDeepModeTCDBuffer;
}
// ------------------------------------------------------------
// Description	:MultiDeepModeTCD模式数据解析获取线程，从socket获取，或文件读取
// Parameter	:void
// Retrun Value	:void
// ------------------------------------------------------------
void * MultiDeepModeTCDGetDataThread(void *)
{
	LOGI( "MultiDeepModeTCD Get Data thread Begin!");
	//printf( "MultiDeepModeTCD Get Data thread Begin!\n");
	//定义了两个数组，其实就是地址偏移量
	signed short int * pParsedMultiDeepModeTCDFrame = NULL;
	signed short int * IQData = NULL;
	char* IQFileName = "/sdcard/Download/data/IQdata4.dat";
	int k=0;
	while (1)
	{
	//for(int k =0;k<10;k++){
		SynSem::GetPSSem()->m_Sem_MultiDeepModeTCD.ProducterWait();
		//一个pingpangbuffer,获得写指针，接下来向里面写
		ParsedMultiDeepModeTCDBuffer.GetWritePointer(&pParsedMultiDeepModeTCDFrame);
		if (NULL == pParsedMultiDeepModeTCDFrame)
		{
			LOGE("Get write pointer failed in the MultiDeepModeTCD mode sim data thread");
			//printf("Get write pointer failed in the c mode sim data thread\n");
		}

		//读取IQ数据
		IQData = ReadFile(IQFileName,DEEP_POINTS,MAX_MD_ENSEMBLE,k);
		//将IQ数据放到乒乓缓存中
		//LOGI( "haha1");
		for (int i=0;i<MAX_MD_ENSEMBLE;i++){
			for (int j =0;j<DEEP_POINTS;j++){
				pParsedMultiDeepModeTCDFrame[i*DEEP_POINTS*2+j*2]=IQData[i*DEEP_POINTS*2+j*2];
				pParsedMultiDeepModeTCDFrame[i*DEEP_POINTS*2+j*2+1]=IQData[i*DEEP_POINTS*2+j*2+1];
			}

		}
		//LOGI( "haha2");
		//std::cout<<"生产"<<std::endl;
		//打印乒乓缓存中存储的IQ数据
		//PrintArray(DEEP_POINTS,MAX_MD_ENSEMBLE,pParsedMultiDeepModeTCDFrame);
		//usleep(1000 * 500);
		//LOGI( "haha3");
		SynSem::GetPSSem()->m_Sem_MultiDeepModeTCD.ProducterDone();
		k++;
		LOGI( "MultiDeepModeTCD Get Data thread is Done!,%d",k);

	}
	//}
}

// ------------------------------------------------------------
// Description	:C数据信号处理线程
// Parameter	:void
// Retrun Value	:void
// ------------------------------------------------------------
void * MultiDeepModeTCDSignalProcessThread(void*)
{
	LOGI( "MultiDeepModeTCD mode signal thread is start here");
	//printf( "MultiDeepModeTCD mode signal thread is start here\n");

	prctl(PR_SET_NAME, (unsigned long) "Signal_MultiDeepModeTCD_Mode");

	// MultiDeepModeTCD 从乒乓缓存中读取IQ数据的指针
	signed short int * pParsedMultiDeepModeTCDData = NULL;
	//IQ数据处理后生成的速度矩阵
	int* pParsedMultiDeepModeTCDVelocity = (int*)malloc(DEEP_POINTS*sizeof(int));

	//用于显示的ARGB数据
	int * pParsedMultiDeepModeTCDFrame= NULL;
    int nCount = 0;
	//MultiDeepModeTCDSignalManage
	while (1)
	{
		ThreadSpace::GetMultiDeepModeTCDSignalThread()->SuspendCheck();
		SynSem::GetPSSem()->m_Sem_MultiDeepModeTCD.ConsumerWait();
		//获取乒乓缓存中的数据指针
		if (false == ParsedMultiDeepModeTCDBuffer.GetReadPointer(&pParsedMultiDeepModeTCDData))
		{
			//获取数据出错
			LOGE( "GetParsedCFrame Failed, in the c mode signal thread\n");
			//printf( "GetParsedCFrame Failed, in the c mode signal thread\n");
		}
		//printf("消费\n");
		//PrintArray(DEEP_POINTS,MAXC_ENSEMBLE,pParsedMultiDeepModeTCDData);

		static TimerTest objCTime(100, "MultiDeepModeTCDSignal");
		//objCTime.CheckFreq();
		objCTime.Start();
		//数据处理(传入待处理数据DeepPoint*8，传出处理后数据DeepPoint*1)
		objMultiDeepModeTCDSignalManage.SignalProcess(pParsedMultiDeepModeTCDData, pParsedMultiDeepModeTCDVelocity);
		objCTime.End();
		SynSem::GetPSSem()->m_Sem_MultiDeepModeTCD.ConsumerDone();
		SynSem::GetIDSem()->m_Sem_MultiDeepModeTCD.ProducterWait();
		MultiDeepModeTCDFrameBuffer.GetWritePointer(&pParsedMultiDeepModeTCDFrame);

		if (NULL == pParsedMultiDeepModeTCDFrame)
		{
			LOGE("Get write pointer failed in the pParsedMultiDeepModeTCDVelocityn");
			//printf("Get write pointer failed in the pParsedMultiDeepModeTCDVelocityn\n");
		}
		//把数据处理记过保存到显示缓存区中
		//velocity 到RGB的映射函数
		//////
		for(int i = 0;i<DEEP_POINTS;i++){
			pParsedMultiDeepModeTCDFrame[i]=pParsedMultiDeepModeTCDVelocity[i];
		}
		SynSem::GetIDSem()->m_Sem_MultiDeepModeTCD.ProducterDone();
		///////
		//printf("显示\n");

		//PrintArray(DEEP_POINTS,pParsedMultiDeepModeTCDFrame);
        nCount++;
		LOGI( "MultiDeepModeTCD mode signal thread is Done!,%d",nCount);

	} //while(1)

}
// ------------------------------------------------------------
// Description	:图像处理C模式线程
// Parameter	:void
// Retrun Value	:void
// ------------------------------------------------------------
void * MultiDeepModeTCDImageProcessThread(void*)
{
	LOGI( "MultiDeepModeTCD mode image thread is start here");
	//printf( "MultiDeepModeTCD mode image thread is start here\n");
	int nCnt = 0;

	//Color Frame
	int *pMultiDeepModeTCDFrameDisp = NULL;


	while (1)
	{
		//suspend check
		ThreadSpace::GetMultiDeepModeTCDImageThread()->SuspendCheck();

		//wait m_Sem_MultiDeepModeTCD
		SynSem::GetIDSem()->m_Sem_MultiDeepModeTCD.ConsumerWait();
		//get the cycle buffer
		int nRet = MultiDeepModeTCDFrameBuffer.GetReadPointer(&pMultiDeepModeTCDFrameDisp);
		if (0 == nRet)
		{
			LOGE("GetReadPointer failed in the GetMultiDeepModeTCD mode image thread!");
			//printf("GetReadPointer failed in the GetMultiDeepModeTCD mode image thread!\n");
		}
		//printf("显示\n");

		PrintArray(DEEP_POINTS,pMultiDeepModeTCDFrameDisp);

		SynSem::GetIDSem()->m_Sem_MultiDeepModeTCD.ConsumerDone();
        nCnt++;
		LOGI( "MultiDeepModeTCD mode image thread is done,%d",nCnt);

	}

}
// --------------------------------------------------------------
// Description	:获取最新数据接口，供显示模块调用以获取Image处理后的数据
// Parameters	:
//		pMultiDeepModeTCDFrameDisp-ARGB颜色数组指针
// Return Value	:
//		0-获取数据失败
//		1-获取数据成功
// --------------------------------------------------------------
int* GetMultiDeepModeTCDFrameBuffer(int* pMultiDeepModeTCDFrameDisp)
{
	int nCnt = 0;
	//Color Frame
	ThreadSpace::GetMultiDeepModeTCDImageThread()->SuspendCheck();
	LOGI( "MultiDeepModeTCD mode image thread is start,%d",nCnt);
	SynSem::GetIDSem()->m_Sem_MultiDeepModeTCD.ConsumerWait();
	//get the cycle buffer
	int nRet = MultiDeepModeTCDFrameBuffer.GetReadPointer(&pMultiDeepModeTCDFrameDisp);
	if (0 == nRet)
	{
		LOGE("GetReadPointer failed in the GetMultiDeepModeTCD mode image thread!");
		//printf("GetReadPointer failed in the GetMultiDeepModeTCD mode image thread!\n");
	}
		//printf("显示\n");

	//PrintArray(DEEP_POINTS,pMultiDeepModeTCDFrameDisp);

	SynSem::GetIDSem()->m_Sem_MultiDeepModeTCD.ConsumerDone();
	LOGI( "MultiDeepModeTCD mode image thread is done,%d",nCnt);
	return pMultiDeepModeTCDFrameDisp;
}