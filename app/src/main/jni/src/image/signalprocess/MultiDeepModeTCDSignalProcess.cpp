/*
 * MultiDeepModeTCDSignalProcess.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: ljd
 */
#include "../../../include/image/signalprocess/MultiDeepModeTCDSignalProcess.h"
#include "../../../include/image/utility/DebugPrint.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
int fprf = SAMPLE_FREQ;//采样频率
float cosTh=0.707;//cos(45),45为探头与血流角度
int transferFrequence = TRANSFER_FREQ;//探头频率
// ------------------------------------------------------------
// Description	:壁滤波
// Parameter	:
//		pMDWFCoef-滤波系数，大小为[nEnsemble*nEnsemble]
//		pSrc-输入的数据指针
//		pDest-返回计算结果的指针
//		nEnsemble-滤波因子维数
//		nPointAmount-点数
// Retrun Value	:
//		0-计算正确
//		-1-输入数据指针为空,有错误
// ------------------------------------------------------------
int MultiDeepModeTCDWallFilter(const float *pMDWFCoef, signed short int *pSrc, float *pDest, int nEnsemble,int nDeepPoint)
{
	if (NULL == pMDWFCoef || NULL == pSrc || NULL == pDest)
	{
		return -1;
	}
	for (int i = 0; i < nDeepPoint; i++)
	{

		for (int j = 0; j < nEnsemble; j++)
		{
			float fSumI = 0;
			float fSumQ = 0;
			for(int k = 0;k<nEnsemble;k++){
				fSumI += pSrc[k*nDeepPoint*2+i*2] * pMDWFCoef[j*nEnsemble+k];
				fSumQ += pSrc[k*nDeepPoint*2+i*2+1] * pMDWFCoef[j*nEnsemble+k];
			}
			pDest[j*nDeepPoint*2+i*2] = fSumI;
			pDest[j*nDeepPoint*2+i*2+1] = fSumQ;
		}//The end of j cycle


	} //The end of i cycle



	return 0;
}
//----------------------------------------
//将-π到+π映射到-127到+127之间
//--------------------------------------
static inline int GetVelocity(float velocity){
	int outdata_velocity = 0;
	velocity = velocity * 40.4459;
	if (velocity > 127)
	{
		outdata_velocity = 127;
	} else if (velocity < -127)
	{
		outdata_velocity = -127;
	} else
	{
		outdata_velocity = ((int) velocity);
	}
	return outdata_velocity;
}
static inline int Velocity2ARGB(float velocityPI){
	int velocityARGB = 0;
	velocityPI = velocityPI + 127;
	//velocityARGB = colorBar[velocityPI];
	return velocityARGB;
}
// ------------------------------------------------------------
// Description	:color数据自相关计算
// Parameter	:
//		m_fAfterCWF:壁滤波之后的矩阵[nPoints*nEnsemble]
//		m_nVelocityAfterAC:自相关之后的速度矩阵[nPoints*1]
//		nEnsemble:自相关阶数
//		nPoints:每个深度的点数
// Retrun Value	:
//		0-计算正确
//		-1-输入数据指针为空,有错误
// ------------------------------------------------------------
int MultiDeepModeTCDAutoCorrelation(float* m_fAfterCWF,int* m_nVelocityAfterAC,int nEnsemble,int nPoints)
{
	float myatan = 0.0;
	int velocityPI2INT = 0;
	int velocityARGB = 0;
	if(m_fAfterCWF==NULL||m_nVelocityAfterAC==NULL){
		return -1;
	}
	for (int i=0;i<nPoints;i++){
		float fsumI=0;
		float fsumQ=0;
		for(int j=0;j<nEnsemble-1;j++){
			//*(I2+iQ2)*(I1-iQ1)=(I1*I2+Q1*Q2)+i(I1*Q2-I2*Q1)
			//I1*I2+Q1*Q2
			fsumI+=m_fAfterCWF[j*nPoints*2+i*2]*m_fAfterCWF[(j+1)*nPoints*2+i*2]+m_fAfterCWF[j*nPoints*2+i*2+1]*m_fAfterCWF[(j+1)*nPoints*2+i*2+1];

			//I1*Q2-I2*Q1
			fsumQ+=m_fAfterCWF[j*nPoints*2+i*2]*m_fAfterCWF[(j+1)*nPoints*2+i*2+1]-m_fAfterCWF[(j+1)*nPoints*2+i*2]*m_fAfterCWF[(j)*nPoints*2+i*2+1];
		}
		myatan = atan2(fsumQ,fsumI);
		velocityPI2INT = GetVelocity(myatan);//获取相对速度，从-π到+π，映射到-127到127
		velocityARGB = colorMap256[velocityPI2INT];
		//Velocity(j,i)=C（声速）*fprf（采样频率）/(4*pi*cosTh(探头与血管夹角)*f0（探头频率）)*DeepAngleArray(j,i);
		//m_nVelocityAfterAC[i]=(SOUND_SPEED*fprf*myatan)/(4*PI*cosTh*transferFrequence);//血流的实际速度
		m_nVelocityAfterAC[i]=velocityARGB;
	}
	return 0;
}








