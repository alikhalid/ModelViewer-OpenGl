//Ali Khalid
#pragma warning(disable:4996)
#include "Object.h"
#pragma comment(lib, "freeglut")
#pragma comment(lib, "glew32")

vec4 light_position(5.0, 5.0, 5.0, 0.0);//infinite light source ie vector
vec3 light_ambient(0.2, 0.2, 0.2);
vec3 light_diffuse(1.0, 1.0, 1.0);
vec3 light_specular(1.0, 1.0, 1.0);

vec3 mat_ambient;
vec3 mat_diffuse;
vec3 mat_specular;

const int s = 2; //number of objects
static int i = 0; //decides which object to creat first
int frame, fps, time, timebase = 0;
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Xaxis;
GLuint vao[s];
GLuint ProjectionLoc, ModelViewLoc, Lpos, Lamb, Ldiff, Lspec, Mamb, Mdiff, Mspec, shinLoc;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 }, aspect = 1, bmax, pfar, pnear, zeye = 0;
GLuint program;  // The location of the "theta" shader uniform variable
bool ortho = false;
vec4 cb, eye;
mat4 projection;
float border;

Object c[s] = { "cube","ali" };

void display()
{
	mat4 modelView;
	//clearign the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cb = c[i % 2].getCtr();
	mat4 rot = RotateX(Theta[Xaxis])*RotateY(Theta[Yaxis])*RotateZ(Theta[Zaxis]);
	if (ortho)
	{
		modelView = rot;
	}
	else
	{
		eye = vec4(0.0, 0.0, zeye, 1.0);
		modelView = LookAt(eye, cb, vec4(0.0, 1.0, 0.0, 1.0));
		modelView *= rot;
	}
	//sending modelview matrix over to the vshader
	glUniformMatrix4fv(ModelViewLoc, 1, GL_TRUE, modelView);

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	char display_string[100];
	if (time - timebase > 1000) {
		fps = frame*1000.0 / (time - timebase);
		sprintf(display_string, "Ali’s Object : FPS:%d ", fps);
		glutSetWindowTitle(display_string);
		timebase = time;
		frame = 0;
	}
	c[i % s].draw();

	glutSwapBuffers();
}

// OpenGL initialization
void init()
{
	// Create a vertex array object
	glGenVertexArrays(s, vao);

	// Load shaders and use the resulting shader program
	program = InitShader("vshader_a4.glsl", "fshader_a4.glsl");
	glUseProgram(program);
	for (int z = 0; z < s; z++)
	{
		glBindVertexArray(vao[z]);
		c[z].load(program);//loading all objects to a specfic vao
	}
	glBindVertexArray(vao[0]);//enabling the first object

	ModelViewLoc = glGetUniformLocation(program, "ModelView");
	ProjectionLoc = glGetUniformLocation(program, "Projection");

	//getting locations of uniform light variables from vshader
	Lpos = glGetUniformLocation(program, "LightPosition");
	Lamb = glGetUniformLocation(program, "LAmbient");
	Ldiff = glGetUniformLocation(program, "LDiffuse");
	Lspec = glGetUniformLocation(program, "LSpecular");
	shinLoc = glGetUniformLocation(program, "Shininess");

	//sending light vector over to the shader
	glUniform3fv(Lpos, 1, light_position);
	glUniform3fv(Lamb, 1, light_ambient);
	glUniform3fv(Ldiff, 1, light_diffuse);
	glUniform3fv(Lspec, 1, light_specular);

	//getting locations of uniform material variables from vshader
	Mamb = glGetUniformLocation(program, "MAmbient");
	Mdiff = glGetUniformLocation(program, "MDiffuse");
	Mspec = glGetUniformLocation(program, "MSpecular");

	//changing materials
	c[0].setMat(mat_ambient, mat_diffuse, mat_specular);
	//sending materials over to shader for the first one
	glUniform1f(shinLoc, c[0].getShin());
	glUniform3fv(Mamb, 1, mat_ambient);
	glUniform3fv(Mdiff, 1, mat_diffuse);
	glUniform3fv(Mspec, 1, mat_specular);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);

}

void reshape(int width, int height)
{
	//Theta[0] = Theta[1] = Theta[2] = 0;
	glViewport(0, 0, width, height);
	aspect = GLfloat(width) / height;
	cb = c[i % 2].getCtr();//getting the center of the box
	bmax = c[i % 2].getMax();//box max
	pnear = bmax;//near clipping field
	pfar = pnear + 10 * bmax;//far clipping field
	zeye = 2 * bmax + cb.z;
	eye = vec4(0, 0, zeye, 1.0);
	ortho = !ortho;
	if (ortho)
	{
		border = c[i % s].getMax();
		if (aspect > 1)
			projection = Ortho(-border*aspect, border*aspect, -border, border, -border, border);
		else
			projection = Ortho(-border, border, -border*aspect, border*aspect, -border, border);
	}
	else
		projection = Perspective(45, aspect, pnear, pfar);
	glUniformMatrix4fv(ProjectionLoc, 1, GL_TRUE, projection);

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 33: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 32:
		i++;
		glBindVertexArray(vao[i % s]);//enabling the specfic vao
		//changing materials
		c[i%s].setMat(mat_ambient, mat_diffuse, mat_specular);
		//sending materials over to shader
		glUniform1f(shinLoc, c[i%s].getShin());
		glUniform3fv(Mamb, 1, mat_ambient);
		glUniform3fv(Mdiff, 1, mat_diffuse);
		glUniform3fv(Mspec, 1, mat_specular);
		break;
	case 'o':
		//making smaller
		zeye++;
		break;
	case 'i':
		//making bigger
		zeye--;
		break;
	case 's':
		ortho = !ortho;
		if (ortho)
		{
			border = c[i % s].getMax();
			if (aspect > 1)
				projection = Ortho(-border*aspect, border*aspect, -border, border, -border, border);
			else
				projection = Ortho(-border, border, -border*aspect, border*aspect, -border, border);
		}
		else
			projection = Perspective(45, aspect, pnear, pfar);
		//sending the projection matrix over to the shader
		glUniformMatrix4fv(ProjectionLoc, 1, GL_TRUE, projection);
	}
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			Axis = Xaxis;
			break;
		case GLUT_MIDDLE_BUTTON:
			Axis = Yaxis;
			break;
		case GLUT_RIGHT_BUTTON:
			Axis = Zaxis;
			break;
		}
	}

	Theta[Axis] += 5;
	if (Theta[Axis] > 360.0) {
		Theta[Axis] -= 360.0;
	}
}

void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Object");
	glewExperimental = GL_TRUE;

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}