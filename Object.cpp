//Ali Khalid
#include "Object.h"

Object::Object(string in)
{
	vector <GLuint> vIList, nIList;
	vector <vec4> vList, nList;
	name = in;
	string temp = in;
	temp += ".mtl";
	name += ".obj";
	ifstream cin;
	cin.open(name);
	int a, b;
	GLfloat maX = -1, maY = -1, maZ = -1,
		miX = 1, miY = 1, miZ = 1;
	GLfloat x, y, z, ind, nor;
	string type, t;

	auto&ins0 = [&x, &y, &z]() {vec4 temp(x, y, z, 0);
	return temp; };

	auto&ins1 = [&x, &y, &z]() {vec4 temp(x, y, z, 1);
	return temp; };

	auto &read = [&cin](int n) {for (int i = 0; i < n; i++) {
		string waste; getline(cin, waste);
	}};

	read(4);
	cin >> type;

	while (type == "v")
	{
		cin >> x >> y >> z;
		maX = max(maX, x); maY = max(maY, y); maZ = max(maZ, z);
		miX = min(miX, x); miY = min(miY, y); miZ = min(miZ, z);
		vList.push_back(ins1());
		cin >> type;
	}
	boxMax = max((maX - miX), (maY - miY));
	boxMax = max(boxMax, (maZ - miZ));
	ctrBox.x = (maX + miX) / 2;
	ctrBox.y = (maY + miY) / 2;
	ctrBox.z = (maZ + miZ) / 2;
	ctrBox.w = 1;
	while (type == "vn")
	{
		cin >> x >> y >> z;
		nList.push_back(ins0());
		cin >> type;
	}
	read(2);
	while (cin >> type)
	{
		for (int i = 0; i < 3; i++)
		{
			cin >> ind;
			vIList.push_back(ind - 1);
			cin.ignore(2);
			cin >> nor;
			nIList.push_back(nor - 1);
		}
	}
	numV = vList.size();
	numN = nList.size();
	numI = vIList.size();

	cout << "Object " << name << endl;
	cout << "Number of vertices = " << numV << endl;
	cout << "Number of normals = " << numN << endl << endl;

	for (int x = 0; x < numI; x++)
		vec.push_back(vList[vIList[x]]);
	for (int x = 0; x < numI; x++)
		nvec.push_back(nList[nIList[x]]);

	cin.close();
	cin.open(temp);
	while (true)
	{
		cin >> type;
		if (type == "Ns")
			break;
	}
	//reading materials
	cin >> shin;
	cin >> type >> mAmb.x >> mAmb.y >> mAmb.z;
	cin >> type >> mDiff.x >> mDiff.y >> mDiff.z;
	cin >> type >> mSpec.x >> mSpec.y >> mSpec.z;
	cin.close();

	vList.clear();
	vIList.clear();
}

Object::~Object()
{
	vec.clear();
	nvec.clear();
}

void Object::draw()
{
	glDrawArrays(GL_TRIANGLES, 0, vec.size()); //draws triangles starting from position 0, 
											   //using vertices from vec
}

int Object::load(GLuint program)
{
	int size = vec.size() * 16; //size in bytes
	glGenBuffers(1, &buffer); //creates 1 buffer object, stores the name in buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //binds the buffer object to GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, size + size, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, size, &vec[0]); //stores vec data in the buffer from 0 to size
	glBufferSubData(GL_ARRAY_BUFFER, size, size, &nvec[0]);

	//all the following calls are necessary to draw using glDrayArrays
	//finds the location and stores it in Loc variable 
	GLuint vPositionLoc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPositionLoc);
	glVertexAttribPointer(vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNorLoc = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNorLoc);
	glVertexAttribPointer(vNorLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(size));

	return 0;
}