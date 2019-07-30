//////////////////////////////////////////////////////////////////////////
// Tutorial 16
// 매 실행마다 임의로 생성된 25개의 구체들의 위치와 색상 정보.
// 장면의 모든 모델들에 대한 정보를 유지/관리하는 클래스
// 단지 한 가지 모델만 있으므로 지금은 구체 모델의 크기와 색상만을 유지 한다.
// 한 장면 내의 다른 타입들의 모델들을 모두 관리할 수 있게 확장할 수 있다

#pragma once
#ifndef _MODELLIST_H_
#define _MODELLIST_H_

#include <D3DX10math.h>
#include <stdlib.h>
#include <time.h>

class CModelList
{
private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		float posX;
		float posY;
		float posZ;
	};
public:
	CModelList(void);
	CModelList(const CModelList& otehr);
	~CModelList(void);

	bool Initialize(int numModels);
	void Shutdown();

	int GetModelCount();
	void GetData(int idx, float& _posX, float& _posY, float& _posZ, D3DXVECTOR4& _color);

private:
	int m_nModelCnt;
	ModelInfoType * m_sModelList;
};

#endif
