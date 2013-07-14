#ifndef BASE_H
#define BASE_H
#include <vector>
#include <algorithm>
struct Point3d
{
public:
	float X;
	float Y;
	float Z;
	Point3d() 
	{
		X = 0;
		Y = 0;
		Z = 0;
	}
	~Point3d()
	{
	}
	Point3d(float x, float y, float z) 
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
	}
};

struct Triangle
{
public :
	int P0Index;
	int P1Index;
	int P2Index;
	Triangle(int p0index, int p1index, int p2index)
	{
		this->P0Index=p0index;
		this->P1Index=p1index;
		this->P2Index=p2index;
	}
	Triangle()
	{
		P0Index=-1;
		P1Index=-1;
		P2Index=-1;
	}
};

struct Vector
{
public:
	float X;
	float Y;
	float Z;
	Vector() 
	{
		X = 0;
		Y = 0;
		Z = 0;
	}
	~Vector()
	{
	}
	Vector(float x, float y, float z) 
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
	}
};


Vector CaculateTriangleNormal(Point3d& p0, Point3d& p1, Point3d& p2)
{
	Vector Normal;
	float v1x = p1.X - p0.X;
	float v1y = p1.Y - p0.Y;
	float v1z = p1.Z - p0.Z;
	float v2x = p2.X - p1.X;
	float v2y = p2.Y - p1.Y;
	float v2z = p2.Z - p1.Z;
	Normal.X= v1y * v2z - v1z * v2y;
	Normal.Y = v1z * v2x - v1x * v2z;
	Normal.Z = v1x * v2y - v1y * v2x;
	float len = (float)sqrt(Normal.X * Normal.X + Normal.Y * Normal.Y + Normal.Z * Normal.Z);
	if (len == 0)
	{
		//throw Exception();
	}
	else
	{
		Normal.X /= len;
		Normal.Y /= len;
		Normal.Z /= len;
	}
	return Normal;
}

class Mesh
{
public:
	std::vector<Point3d> Vertices;//��ŵ�ļ���
	std::vector<Triangle> Faces;//��������εļ���
	std::vector<Vector> FaceNormals;//����淨��ļ��ϣ��������μ���һһ��Ӧ
	std::vector<Vector> VertexNormals;//��ŵ㷨��ļ��ϣ���㼯һһ��Ӧ
	std::vector<std::vector<int>*> AdjacentFacesPerVertex;//��ŵ���ڽ���ļ��ϣ���㼯һһ��Ӧ
	std::vector<std::vector<int>*> AdjacentVerticesPerVertex;//��ŵ���ڽӵ�ļ��ϣ���㼯һһ��Ӧ
	Mesh()
	{
	}
	~Mesh()
	{

	}
	int AddVertex(Point3d& toAdd)
	{
		int index = Vertices.size();
		Vertices.push_back(toAdd);
		return index;
	}
	int AddFace(Triangle& tri)
	{
		int index = Faces.size();
		Faces.push_back(tri);
		return index;
	}
	void CaculateFaceNormals()
	{
		FaceNormals.reserve(Faces.size());
		for(size_t i=0;i<Faces.size();i++)
		{
			Point3d& p0=Vertices[Faces[i].P0Index];
			Point3d& p1=Vertices[Faces[i].P1Index];
			Point3d& p2=Vertices[Faces[i].P2Index];
			Vector v=CaculateTriangleNormal(p0,p1,p2);
			FaceNormals.push_back(v);
		}//��ÿһ�������μ����淨�򲢴���FaceNormals
	}//�����淢��
	void CaculateVertexNormals()
	{
		if(FaceNormals.size()==0)
			CaculateFaceNormals();//����㷨��ǰ��������淨��
		if(AdjacentFacesPerVertex.size()==0)
			CaculateAdjacentFacesPerVertex();//����㷨��ǰҲ����������ڽ���
		VertexNormals.reserve(Vertices.size());
		for (size_t i = 0; i < Vertices.size(); i++)
		{
			float sumx = 0;
			float sumy = 0;
			float sumz = 0;
			std::vector<int>& tlist = *(AdjacentFacesPerVertex[i]);
			if(tlist.size()!=0)
			{
				for (size_t j = 0; i < tlist.size(); j++)
				{
					sumx += FaceNormals[tlist[j]].X;
					sumy += FaceNormals[tlist[j]].Y;
					sumz += FaceNormals[tlist[j]].Z;
				}
				VertexNormals.push_back(Vector(sumx / tlist.size(), sumy / tlist.size(), sumz /tlist.size()));//���ڽ��淢���ֵ
			}
			else
			{
				VertexNormals.push_back(Vector(0,0,0));//��Ϊ��������ʹ��Ĭ��ֵ
			}
		}
	}//����㷨��
	void CaculateAdjacentFacesPerVertex()
	{
		AdjacentFacesPerVertex.reserve(Vertices.size());
		for (size_t i = 0; i < Vertices.size(); i++)
		{
			std::vector<int>* list=new std::vector<int>();
			list->reserve(4);//Ԥ�ȼ���ÿ��������4���ڽ���
			AdjacentFacesPerVertex.push_back(list);
		}//���ȷ���ô洢�ռ�
		for (size_t i = 0; i < Faces.size(); i++)
		{
			Triangle& t = Faces[i];
			std::vector<int> *t0list= AdjacentFacesPerVertex[t.P0Index];
			std::vector<int> *t1list= AdjacentFacesPerVertex[t.P1Index];
			std::vector<int> *t2list= AdjacentFacesPerVertex[t.P2Index];
			t0list->push_back(i);
			t1list->push_back(i);
			t2list->push_back(i);
		}//���������μ��ϣ�ʹ����������Ϣ�����ڽ����
	}//�����ڽ���
	void CaculateAdjacentVerticesPerVertex()
	{
		AdjacentVerticesPerVertex.reserve(Vertices.size());
		for (size_t i = 0; i < Vertices.size(); i++)
		{
			std::vector<int>* list=new std::vector<int>();
			list->reserve(4);//Ԥ�ȼ���ÿ��������4���ڽӵ�
			AdjacentVerticesPerVertex.push_back(list);
		}//���ȷ���ô洢�ռ�
		for (size_t i = 0; i < Faces.size(); i++)
		{
			Triangle &t = Faces[i];
			std::vector<int> *p0list= AdjacentVerticesPerVertex[t.P0Index];
			std::vector<int> *p1list= AdjacentVerticesPerVertex[t.P1Index];
			std::vector<int> *p2list= AdjacentVerticesPerVertex[t.P2Index];

			if (std::find(p0list->begin(), p0list->end(), t.P1Index)==p0list->end())
				p0list->push_back(t.P1Index);
			if (std::find(p0list->begin(), p0list->end(), t.P2Index)==p0list->end())
				p0list->push_back(t.P2Index);

			if (std::find(p1list->begin(), p1list->end(), t.P0Index)==p1list->end())
				p1list->push_back(t.P0Index);
			if (std::find(p1list->begin(), p1list->end(), t.P2Index)==p1list->end())
				p1list->push_back(t.P2Index);

			if (std::find(p2list->begin(), p2list->end(), t.P0Index)==p2list->end())
				p2list->push_back(t.P0Index);
			if (std::find(p2list->begin(), p2list->end(), t.P1Index)==p2list->end())
				p2list->push_back(t.P1Index);
		}//���������μ��ϣ�ʹ����������Ϣ�����ڽ����
	}//�����ڽӵ�
	void LaplacianSmooth(int time)
	{
		if(AdjacentVerticesPerVertex.size()==0)
			CaculateAdjacentVerticesPerVertex();//ƽ��ǰ��Ҫ�����ڽӵ�
		Point3d* tempPos=new Point3d[Vertices.size()];//�����ռ�洢ÿ��ƽ����ĵ㽫ҪŲ����λ��
		for(int k=0;k<time;k++)
		{
			for (size_t i = 0; i < Vertices.size(); i++)
			{
				float xav = 0;
				float yav = 0;
				float zav = 0;
				std::vector<int>& adjlist =*(AdjacentVerticesPerVertex[i]);
				size_t adjcount=adjlist.size();
				if(adjcount==0)
					continue;
				for (size_t j = 0; j < adjcount; j++)
				{
					xav += Vertices[adjlist[j]].X;
					yav += Vertices[adjlist[j]].Y;
					zav += Vertices[adjlist[j]].Z;
				}
				xav /= adjcount;
				yav /= adjcount;
				zav /= adjcount;
				tempPos[i].X = xav;
				tempPos[i].Y = yav;
				tempPos[i].Z = zav;
			}//�����е�����ڽӵ�ƽ��λ�ò��浽�����ռ�
			for (size_t i = 0; i < Vertices.size(); i++)
			{
				Vertices[i].X = tempPos[i].X;
				Vertices[i].Y = tempPos[i].Y;
				Vertices[i].Z = tempPos[i].Z;
			}//����������µ�λ�ø���ԭ�����λ��
		}//ÿ��ѭ����ζ��һ��ƽ��
		delete[] tempPos;
	}
};



#endif