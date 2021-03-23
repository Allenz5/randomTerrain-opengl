#pragma once
#include <iostream>
#include <vector>


class FaultingTerrain
{
public:
	FaultingTerrain(int size, int minX, int maxX, int minY, int maxY);
	void generateTriangles();
	void ct();
	std::vector<float> getTriangle(int index);
	void setTriangle(std::vector<float> data, int index);
	void shape();
	void generateNormal();
	void getZvalue();
	~FaultingTerrain()
	{
		delete[] position;
		delete[] element;
		delete[] normal;
	}
	int positionNumber = 0;
	int triangleNumber = 0;
	float* position;
	int* element;
	float* normal;
	float minZ;
	float maxZ;


private:
	int size;
	int minX;
	int minY;
	int maxX;
	int maxY;
};