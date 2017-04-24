//Ali Khalid
#pragma once
#include <Angel.h>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include <algorithm>

using namespace std;

class Object
{
	string name;
	vec4 ctrBox;
	GLfloat boxMax, shin;
	int numV = 0, numN = 0, numI = 0;
	vector <vec4>  vec, nvec;
	GLuint buffer;// iBuffer;
	vec3 mAmb, mDiff, mSpec;
public:
	Object(string in);
	~Object();
	void draw();
	int load(GLuint program);
	vec4 getCtr() { return ctrBox; }
	GLfloat getMax() { return boxMax; }
	//setting the material components
	void setMat(vec3 & amb, vec3 & diff, vec3 & spec)
	{
		amb = mAmb; diff = mDiff; spec = mSpec;
	};
	GLfloat getShin() { return shin; }
};