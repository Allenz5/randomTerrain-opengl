#pragma once
#include "faultingTerrain.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <iomanip>

/**
 * @brief create a terrain
 * @param size	number of triangles on the side
 * @param minX	minimun X value
 * @param maxX	maximum X value
 * @param minY	minimun Y value
 * @param maxY	maximum Y value
 */
FaultingTerrain::FaultingTerrain(int size, int minX, int maxX, int minY, int maxY)
{
	this->size = size;
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->position = new float[(size + 1) * (size + 1) * 3];
	this->element = new int[size * size * 3 * 2];
	this->normal = new float[(size + 1) * (size + 1) * 3];
	for (int i = 0; i < (size + 1) * (size + 1) * 3; i++) {
		normal[i] = 0;
	}
	this->positionNumber = (size + 1) * (size + 1);
	this->triangleNumber = size * size * 2;
	generateTriangles();
	shape();
	generateNormal();
	getZvalue();
}

/**
 * @brief fill in the position and element array
 */
void FaultingTerrain::generateTriangles()
{
	float length = (float)(maxX - minX) / size;
	float width = (float)(maxY - minY) / size;
	for (int i = 0; i <= size; i++) {
		for (int j = 0; j <= size; j++) {
			(position)[3 * (i * (size + 1) + j)] = (minX + (float)j * length);
			(position)[3 * (i * (size + 1) + j) + 1] = (minY + (float)i * width);
			(position)[3 * (i * (size + 1) + j) + 2] = (0);
		}
	}

	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			this->element[6 * (i * size + j)] = ((i * (this->size + 1) + j));
			this->element[6 * (i * size + j) + 1] = ((i * (this->size + 1) + j + 1));
			this->element[6 * (i * size + j) + 2] = ((i * (this->size + 1) + j + this->size + 1));
			this->element[6 * (i * size + j) + 3] = ((i * (this->size + 1) + j + 1));
			this->element[6 * (i * size + j) + 4] = ((i * (this->size + 1) + j + this->size + 2));
			this->element[6 * (i * size + j) + 5] = ((i * (this->size + 1) + j + this->size + 1));
		}
	}
}

/**
 * @brief print out position value
 */
void FaultingTerrain::ct() {
	for (int i = 0; i < positionNumber; i++) {
		std::cout << std::fixed << normal[3*i] << " ";
		std::cout << std::fixed << normal[3*i + 1] << " ";
		std::cout << std::fixed << normal[3*i + 2] <<std::endl;
	}
	
}

/**
 * @brief get the position value by a triangle index
 * @param index	the index of a triangle
 * @return a vector that contain the x,y,z value of a triangle
 */
std::vector<float> FaultingTerrain::getTriangle(int index)
{
	std::vector<float> toReturn;
	toReturn.push_back(position[3 * index]);
	toReturn.push_back(position[3 * index + 1]);
	toReturn.push_back(position[3 * index + 2]);
	return toReturn;
}

/**
 * @brief set the position value by a vector that contain the x,y,z value of a triangle and a triangle index
 * @param index	the index of a triangle
 * @param data	a vector that contains the x,y,z value of a triangle
 */
void FaultingTerrain::setTriangle(std::vector<float> data, int index) {
	position[3 * index] = data[0];
	position[3 * index + 1] = data[1];
	position[3 * index + 2] = data[2];
}

/**
 * @brief change the Z value of each vertex
 */
void FaultingTerrain::shape()
{
	float delta = 0.006;
	for (int i = 0; i < 300; i++) {
		float xAxis = minX + 1.0 * rand() / RAND_MAX * (float)(maxX - minX);
		float yAxis = minY + 1.0 * rand() / RAND_MAX * (float)(maxY - minY);
		int degree = rand() % 360;
		float radian = degree * 3.14 / 180;
		glm::vec3 direction = glm::vec3(cos(radian), sin(radian), 0.0f);
		for (int i = 0; i < positionNumber; i++) {
			std::vector<float> cur = getTriangle(i);
			glm::vec3 distance = glm::vec3(cur[0] - xAxis, cur[1] - yAxis, 0.0f);
			if (glm::dot(distance, direction) > 0) {
				cur[2] += delta;
			}
			else {
				cur[2] -= delta;
			}
			setTriangle(cur, i);
		}
		delta = delta / 1.001;
	}
}

/**
 * @brief calculate the normal vector of each vertex
 */
void FaultingTerrain::generateNormal()
{
	for (int i = 0; i < triangleNumber; i++) {
		int first = element[3 * i];
		int second = element[3 * i + 1];
		int third = element[3 * i + 2];

		std::vector<float> firstVertexData = getTriangle(first);
		std::vector<float> secondVertexData = getTriangle(second);
		std::vector<float> thirdVertexData = getTriangle(third);

		std::vector<float> secondMinusFirst;
		secondMinusFirst.push_back(secondVertexData[0] - firstVertexData[0]);
		secondMinusFirst.push_back(secondVertexData[1] - firstVertexData[1]);
		secondMinusFirst.push_back(secondVertexData[2] - firstVertexData[2]);
		std::vector<float> thirdMinusFirst;
		thirdMinusFirst.push_back(thirdVertexData[0] - firstVertexData[0]);
		thirdMinusFirst.push_back(thirdVertexData[1] - firstVertexData[1]);
		thirdMinusFirst.push_back(thirdVertexData[2] - firstVertexData[2]);
		std::vector<float> cross;
		cross.push_back(secondMinusFirst[1] * thirdMinusFirst[2] - thirdMinusFirst[1] * secondMinusFirst[2]);
		cross.push_back(secondMinusFirst[2] * thirdMinusFirst[0] - thirdMinusFirst[2] * secondMinusFirst[0]);
		cross.push_back(secondMinusFirst[0] * thirdMinusFirst[1] - thirdMinusFirst[0] * secondMinusFirst[1]);

		normal[first * 3] += cross[0];
		normal[first * 3 + 1] += cross[1];
		normal[first * 3 + 2] += cross[2];

		normal[second * 3] += cross[0];
		normal[second * 3 + 1] += cross[1];
		normal[second * 3 + 2] += cross[2];

		normal[third * 3] += cross[0];
		normal[third * 3 + 1] += cross[1];
		normal[third * 3 + 2] += cross[2];
	}

	for (int i = 0; i < positionNumber; i++) {
		float x = normal[3 * i];
		float y = normal[3 * i + 1];
		float z = normal[3 * i + 2];
		float temp = sqrt(x * x + y * y + z * z);
		normal[3 * i] = x / temp;
		normal[3 * i + 1] = y / temp;
		normal[3 * i + 2] = z / temp;
	}
}

/**
 * @brief get the maximum and minimum z value
 */
void FaultingTerrain::getZvalue() {
	minZ = position[2];
	maxZ = position[2];
	for (int i = 0; i < positionNumber; i++) {
		if (position[i * 3 + 2] < minZ) {
			minZ = position[i * 3 + 2];
		}
		if (position[i * 3 + 2] > maxZ) {
			maxZ = position[i * 3 + 2];
		}
	}
}
