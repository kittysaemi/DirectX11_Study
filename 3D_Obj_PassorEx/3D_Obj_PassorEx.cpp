// 3D_Obj_PassorEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

//////////////////////////////////////////////////////////////////////////
// Tutorial 8
// 마야 3D의  Obj 파일 파서 프로그램.

#include "stdafx.h"

typedef struct  
{
	float x;
	float y;
	float z;
}VertexType;

typedef struct  
{
	int vIndex[3];
	int tIndex[3];
	int nIndex[3];
}FaceType;

void GetModelFilename(char* _fileName);
bool ReadFileCounts(char * _fileName, int&, int&, int&, int&);
bool LoadDataStructures(char * _fileName, char * _saveFileName, int, int, int, int);


int main(int argc, char* argv[])
{
	// Read in the name of the model file.
	char filenmae[256];
	GetModelFilename(filenmae);

	// Read in the number of vertices, tex coords, normals, and faces so that the data structures can be initialized with the exact sizes needed.
	int _nVCount = 0, _nTCount = 0, _nNCount = 0, _nFCount = 0;
	if(!ReadFileCounts(filenmae, _nVCount, _nTCount, _nNCount, _nFCount))
	{
		cout << "Read FIle Counts Faild" << endl;

		return -1;
	}

	// Display the counts to the screen for information purposes.
	cout << endl;
	cout << "Vertices: " << _nVCount << endl;
	cout << "UVs: " << _nTCount << endl;
	cout << "Normals : " << _nNCount << endl;
	cout << "Faces :" << _nFCount << endl;

	char saveFileName[256];
	cout << "Save Model FileName : ";
	cin >> saveFileName;

	// Now read the data from the file into the data structures and then output it in our model format.
	if(!LoadDataStructures(filenmae, saveFileName, _nVCount, _nTCount, _nNCount, _nFCount))
	{
		cout << "LoadDataStructures Faild" << endl;
		return -1;
	}

	// Notify the user the model has been converted.
	char garbage;
	cout << "\n File has been converted." << endl;
	cout << "\n Do you wish to exit (y/n)? ";
	cin >> garbage;

	return 0;
}

void GetModelFilename(char* _fileName)
{
	bool done = false;

	while(!done)
	{
		cout << "Enter model filename : " ;

		cin >> _fileName;

		ifstream _fileIn;
		_fileIn.open(_fileName);

		if(_fileIn.good())
		{
			done = true;
		}
		else
		{
			_fileIn.clear();
			cout << endl;
			cout << "File " << _fileName << " could not be opened." << endl << endl;
		}
	}

	return;
}

bool ReadFileCounts(char * _fileName, int& _verC, int& _texC, int& _norC, int& _facC)
{
	ifstream fileIn;
	fileIn.open(_fileName);

	if(fileIn.fail())
	{
		cout << "Not Fount File " << _fileName << endl;
		return false;
	}

	char input;
	fileIn.get(input);
	while (!fileIn.eof())
	{
		if(input == 'v')
		{
			fileIn.get(input);
			if(input == ' ') {_verC++;}
			if(input == 't') {_texC++;}
			if(input == 'n') {_norC++;}
		}

		if(input == 'f')
		{
			fileIn.get(input);
			if(input == ' ') {_facC++;}
		}

		while(input != '\n')
		{
			fileIn.get(input);
		}

		fileIn.get(input);
	}

	fileIn.close();

	return true;


}

bool LoadDataStructures(char * _fileName, char * _saveFileName, int vC, int tC, int nC, int fC)
{
	VertexType * vertices = new VertexType[vC];
	if(!vertices)
		return false;

	VertexType * texcoords = new VertexType[tC];
	if(!texcoords)
		return false;

	VertexType * normals = new VertexType[nC];
	if(!normals)
		return false;

	FaceType * faces = new FaceType[fC];
	if(!faces)
		return false;

	ifstream fileIn;
	fileIn.open(_fileName);
	if(fileIn.fail())
	{
		cout << "Not Fount file " << _fileName << endl;
		return false;
	}

	int AllIdx = 0;
	int vIdex = 0, tIndex = 0, nIndex = 0, fIndex =0;

	char input;
	fileIn.get(input);
	while (!fileIn.eof())
	{
		if(input == 'v')
		{
			fileIn.get(input);
			if(input == ' ')
			{
				fileIn >> vertices[vIdex].x >> vertices[vIdex].y >> vertices[vIdex].z;
				vertices[vIdex].z = vertices[vIdex].z * -1.0f;
				vIdex++;

				cout << "vertices " << vIdex << endl; AllIdx++;
			}
		}
		if(input == 't')
		{
			fileIn >> texcoords[tIndex].x >> texcoords[tIndex].y;

			texcoords[tIndex].y = 1.0f - texcoords[tIndex].y;
			tIndex++;

			cout << "texcoords " << tIndex << endl; AllIdx++;
		}

		if(input == 'n')
		{
			fileIn >> normals[nIndex].x >> normals[nIndex].y >> normals[nIndex].z;

			normals[nIndex].z = normals[nIndex].z * -1.0f;
			nIndex++;

			cout << "normals " << nIndex << endl; AllIdx++;
		}
		if(input == 'f')
		{
			char Temp;

			fileIn.get(input);
			if(input == ' ')
			{
				fileIn >> faces[fIndex].vIndex[2] >> Temp >> faces[fIndex].tIndex[2] >> Temp >> faces[fIndex].nIndex[2]
				>> faces[fIndex].vIndex[1] >> Temp >> faces[fIndex].tIndex[1] >> Temp >> faces[fIndex].nIndex[1]
				>> faces[fIndex].vIndex[0] >> Temp >> faces[fIndex].tIndex[0] >> Temp >> faces[fIndex].nIndex[0];
				fIndex++;

				cout << "faces " << fIndex << endl; AllIdx++;
			}
		}

		bool fileEof = false;

		while(input != 0x0a)
		{
			fileIn.get(input);	
		}

		if(fileEof)
			break;

		fileIn.get(input);

		cout << "next Line " << AllIdx++ << endl;
	}

	cout << "Read End" << endl;

	fileIn.close();
	
	ofstream fileOut;
	fileOut.open(_saveFileName);

	fileOut << "Vertex Count: " << (fC * 3) << endl;
	fileOut << endl;
	fileOut << "Data:" << endl;
	fileOut << endl;

	int idxV = 0, idxT = 0, idxN = 0, idxF = 0;

	for(int i=0; i<fIndex; i++)
	{
		for(int faceIdx = 0; faceIdx<3; faceIdx++)
		{
			idxV = idxT = idxN = 0;

			idxV = faces[i].vIndex[faceIdx] - 1;
			idxT = faces[i].tIndex[faceIdx] - 1;
			idxN = faces[i].nIndex[faceIdx] - 1;

			fileOut << vertices[idxV].x << ' ' << vertices[idxV].y << ' ' << vertices[idxV].z << ' '
				<< texcoords[idxT].x << ' ' << texcoords[idxT].y << ' '
				<< normals[idxN].x << ' ' << normals[idxN].y << ' ' << normals[idxN].z << endl;
		}	
	}

	fileOut.close();

	if(vertices)
	{
		delete [] vertices;
		vertices = 0;
	}
	if(texcoords)
	{
		delete [] texcoords;
		texcoords = 0;
	}
	if(normals)
	{
		delete [] normals;
		normals = 0;
	}
	if(faces)
	{
		delete [] faces;
		faces = 0;
	}

	return true;

}