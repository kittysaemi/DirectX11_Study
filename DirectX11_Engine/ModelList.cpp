#include "ModelList.h"


CModelList::CModelList(void)
{
	m_sModelList = 0;
}

CModelList::CModelList(const CModelList& otehr)
{

}

CModelList::~CModelList(void)
{
}

bool CModelList::Initialize(int numModels)
{
	// store the number of models.
	m_nModelCnt = numModels;

	// Create a list array of the model information.
	m_sModelList = new ModelInfoType[m_nModelCnt];
	if(!m_sModelList)
		return false;

	// see the random generator with the current time.
	srand((unsigned int)time(NULL));

	// Go through all the models and randomly generate the model color and position.
	for(int i=0; i<m_nModelCnt; i++)
	{
		// Generate a random color for the model.
		float red = (float)rand() / RAND_MAX;
		float green = (float)rand() / RAND_MAX;
		float blue = (float)rand() / RAND_MAX;

		m_sModelList[i].color = D3DXVECTOR4(red, green, blue, 1.0f);

		// Generate a random position in front of the viewer for the mode.
		m_sModelList[i].posX = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_sModelList[i].posY = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_sModelList[i].posZ = ((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 5.0f;
	}

	return true;
}

void CModelList::Shutdown()
{
	if(m_sModelList)
	{
		delete [] m_sModelList;
		m_sModelList = 0;
	}
}

int CModelList::GetModelCount()
{
	return m_nModelCnt;
}

void CModelList::GetData(int idx, float& _posX, float& _posY, float& _posZ, D3DXVECTOR4& _color)
{
	_posX = m_sModelList[idx].posX;
	_posY = m_sModelList[idx].posY;
	_posZ = m_sModelList[idx].posZ;

	_color = m_sModelList[idx].color;
}