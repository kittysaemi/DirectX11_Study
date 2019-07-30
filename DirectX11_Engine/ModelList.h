//////////////////////////////////////////////////////////////////////////
// Tutorial 16
// �� ���ึ�� ���Ƿ� ������ 25���� ��ü���� ��ġ�� ���� ����.
// ����� ��� �𵨵鿡 ���� ������ ����/�����ϴ� Ŭ����
// ���� �� ���� �𵨸� �����Ƿ� ������ ��ü ���� ũ��� ������ ���� �Ѵ�.
// �� ��� ���� �ٸ� Ÿ�Ե��� �𵨵��� ��� ������ �� �ְ� Ȯ���� �� �ִ�

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
