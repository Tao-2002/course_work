#include "Angel.h"
#include "TriMesh.h"

#include <vector>
#include <string>

const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;

const int TRANSFORM_SCALE = 0;
const int TRANSFORM_ROTATE = 1;
const int TRANSFORM_TRANSLATE = 2;

const double DELTA_DELTA = 0.3;		// Delta的变化率
const double DEFAULT_DELTA = 0.5;	// 默认的Delta值

double scaleDelta = DEFAULT_DELTA;
double rotateDelta = DEFAULT_DELTA;
double translateDelta = DEFAULT_DELTA;

glm::vec3 scaleTheta(1.0, 1.0, 1.0);		// 缩放控制变量
glm::vec3 rotateTheta(0.0, 0.0, 0.0);    // 旋转控制变量
glm::vec3 translateTheta(0.0, 0.0, 0.0);	// 平移控制变量

int currentTransform = TRANSFORM_ROTATE;	// 设置当前变换
int mainWindow;
bool rotateFlag = false;				//鼠标自动旋转标志
bool XAxisFlag = false, YAxisFlag = false, ZAxisFlag = false;//自动旋转轴控制

struct openGLObject
{
	// 顶点数组对象
	GLuint vao;
	// 顶点缓存对象
	GLuint vbo;

	// 着色器程序
	GLuint program;
	// 着色器文件
	std::string vshader;
	std::string fshader;
	// 着色器变量
	GLuint pLocation;
	GLuint cLocation;
	GLuint matrixLocation;
	GLuint darkLocation;
};

openGLObject cube_object;

TriMesh* cube = new TriMesh();

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void bindObjectAndData(TriMesh* mesh, openGLObject& object, const std::string& vshader, const std::string& fshader) {

	// 创建顶点数组对象
    glGenVertexArrays(1, &object.vao);  	// 分配1个顶点数组对象
	glBindVertexArray(object.vao);  	// 绑定顶点数组对象

	// 创建并初始化顶点缓存对象
	glGenBuffers(1, &object.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBufferData(GL_ARRAY_BUFFER,
		mesh->getPoints().size() * sizeof(glm::vec3) + mesh->getColors().size() * sizeof(glm::vec3),
		NULL,
		GL_STATIC_DRAW);

	 glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->getPoints().size() * sizeof(glm::vec3), &mesh->getPoints()[0]);
	 glBufferSubData(GL_ARRAY_BUFFER, mesh->getPoints().size() * sizeof(glm::vec3), mesh->getColors().size() * sizeof(glm::vec3), &mesh->getColors()[0]);

	object.vshader = vshader;
	object.fshader = fshader;
	object.program = InitShader(object.vshader.c_str(), object.fshader.c_str());

	// 从顶点着色器中初始化顶点的位置
	object.pLocation = glGetAttribLocation(object.program, "vPosition");
	glEnableVertexAttribArray(object.pLocation);
	glVertexAttribPointer(object.pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// 从顶点着色器中初始化顶点的颜色
	object.cLocation = glGetAttribLocation(object.program, "vColor");
	glEnableVertexAttribArray(object.cLocation);
	glVertexAttribPointer(object.cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mesh->getPoints().size() * sizeof(glm::vec3)));

	// 获得矩阵存储位置
	object.matrixLocation = glGetUniformLocation(object.program, "matrix");

}

void init()
{
	std::string vshader, fshader;
	// 读取着色器并使用
    vshader = "shaders/vshader.glsl";
	fshader = "shaders/fshader.glsl";

	//cube->generateCube();
	//cube->readOff("./Models/cube.off");
	cube->readOff("./Models/cow.off");
	bindObjectAndData(cube, cube_object, vshader, fshader);

	// 黑色背景
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display()
{
	// 清理窗口
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(cube_object.program);

    glBindVertexArray(cube_object.vao);

	// 初始化变换矩阵
	glm::mat4 m(1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);

	// 调用函数传入三种变化的变化量，累加得到变化矩阵
	// 注意三种变化累加的顺序:TRS
	m = glm::translate(m, translateTheta);
	m = glm::rotate(m, glm::radians(rotateTheta[X_AXIS]), glm::vec3(1.0, 0.0, 0.0));
	m = glm::rotate(m, glm::radians(rotateTheta[Y_AXIS]), glm::vec3(0.0, 1.0, 0.0));
	m = glm::rotate(m, glm::radians(rotateTheta[Z_AXIS]), glm::vec3(0.0, 0.0, 1.0));
	m = glm::scale(m, scaleTheta);

	// 从指定位置matrixLocation中传入变换矩阵m
	glUniformMatrix4fv(cube_object.matrixLocation, 1, GL_FALSE, glm::value_ptr(m));

	// 绘制立方体中的各个三角形
	glDrawArrays(GL_TRIANGLES, 0, cube->getPoints().size());
}

// 通过Delta值更新Theta
void updateTheta(int axis, int sign) {
	switch (currentTransform) {
		// 根据变换类型，增加或减少某种变换的变化量
	case TRANSFORM_SCALE:
		scaleTheta[axis] += sign * scaleDelta;
		break;
	case TRANSFORM_ROTATE:
		rotateTheta[axis] += sign * rotateDelta;
		break;
	case TRANSFORM_TRANSLATE:
		translateTheta[axis] += sign * translateDelta;
		break;
	}
}

// 复原Theta和Delta
void resetTheta()
{
	scaleTheta = glm::vec3(1.0, 1.0, 1.0);
	rotateTheta = glm::vec3(0.0, 0.0, 0.0);
	translateTheta = glm::vec3(0.0, 0.0, 0.0);
	scaleDelta = DEFAULT_DELTA;
	rotateDelta = DEFAULT_DELTA;
	translateDelta = DEFAULT_DELTA;
	XAxisFlag = YAxisFlag = ZAxisFlag = false;
}

// 更新变化Delta值
void updateDelta(int sign)
{
	switch (currentTransform) {
		// 根据变化类型增加或减少每一次变化的单位变化量
	case TRANSFORM_SCALE:
		scaleDelta += sign * DELTA_DELTA;
		break;
	case TRANSFORM_ROTATE:
		rotateDelta += sign * DELTA_DELTA;
		break;
	case TRANSFORM_TRANSLATE:
		translateDelta += sign * DELTA_DELTA;
		break;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{	
		// 退出。
		case GLFW_KEY_ESCAPE:
			if(action==GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		// 1：缩放。
		case GLFW_KEY_1:
			if(action==GLFW_PRESS) currentTransform = TRANSFORM_SCALE;
			break;
		// 2: 旋转。
		case GLFW_KEY_2:
			if(action==GLFW_PRESS) currentTransform = TRANSFORM_ROTATE;
			break;
		// 3: 移动。
		case GLFW_KEY_3:
			if(action==GLFW_PRESS) currentTransform = TRANSFORM_TRANSLATE;
			break;
		// 4: 绘制线。
		case GLFW_KEY_4:
			if(action==GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		// 5: 绘制面。
		case GLFW_KEY_5:
			if(action==GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);;
			break;
		// Q: 增加 x。
		case GLFW_KEY_Q:
			if(action==GLFW_PRESS || action==GLFW_REPEAT) updateTheta(X_AXIS, 1);
			break;
		// A: 减少 x。
		case GLFW_KEY_A:
			if(action==GLFW_PRESS || action==GLFW_REPEAT) updateTheta(X_AXIS, -1);
			break;
		// W: 增加 y。
		case GLFW_KEY_W:
			if(action==GLFW_PRESS || action==GLFW_REPEAT) updateTheta(Y_AXIS, 1);
			break;
		// S: 减少 x。
		case GLFW_KEY_S:
			if(action==GLFW_PRESS || action==GLFW_REPEAT) updateTheta(Y_AXIS, -1);
			break;
		// E: 增加 z。
		case GLFW_KEY_E:
			if(action==GLFW_PRESS || action==GLFW_REPEAT) updateTheta(Z_AXIS, 1);
			break;
		// D: 减少 z。
		case GLFW_KEY_D:
			if(action==GLFW_PRESS || action==GLFW_REPEAT) updateTheta(Z_AXIS, -1);
			break;
		// R: 增加变化量。
		case GLFW_KEY_R:
			if(action==GLFW_PRESS) updateDelta(1);
			break;
		// F: 减少变化量。
		case GLFW_KEY_F:
			if(action==GLFW_PRESS) updateDelta(-1);
			break;

		case GLFW_KEY_X:
			if (action == GLFW_PRESS) {
				XAxisFlag = true; YAxisFlag = false; ZAxisFlag = false;
				std::cout << "select X_AXIS rotation" << std::endl;
				break;
			}
		case GLFW_KEY_Y:
			if (action == GLFW_PRESS) {
				XAxisFlag = false; YAxisFlag = true; ZAxisFlag = false;
				std::cout << "select Y_AXIS rotation" << std::endl;
				break;
			}
		case GLFW_KEY_Z:
			if (action == GLFW_PRESS) {
				XAxisFlag = false; YAxisFlag = false; ZAxisFlag = true;
				std::cout << "select Z_AXIS rotation" << std::endl;
				break;
			}



		// T: 所有值重置。
		case GLFW_KEY_T:
			if(action==GLFW_PRESS) resetTheta();
			break;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
	// 按下鼠标左键。
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		rotateFlag = true;
		std::cout << "turn on auto rotate" << std::endl;
	}
	// 按下鼠标右键。
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		rotateFlag = false;
		std::cout << "turn off auto rotate" << std::endl;
	}
}

void printHelp() {
	printf("%s\n\n", "3D Transfomations");
	printf("Keyboard options:\n");
	printf("1: Transform Scale\n");
	printf("2: Transform Rotate\n");
	printf("3: Transform Translate\n");
	printf("4: Deep Testing On\n");
	printf("5: Deep Testing Off\n");
	printf("q: Increase x\n");
	printf("a: Decrease x\n");
	printf("w: Increase y\n");
	printf("s: Decrease y\n");
	printf("e: Increase z\n");
	printf("d: Decrease z\n");
	printf("r: Increase delta of currently selected transform\n");
	printf("f: Decrease delta of currently selected transform\n");
	printf("------------------------------------------------\n");
	printf("mouse_button_left: turn on auto rotate\n");
	printf("mouse_button_right: turn off auto rotate\n");
	printf("x: auto increase delta of X_AXIS\n");
	printf("y: auto increase delta of Y_AXIS\n");
	printf("z: auto increase delta of Z_AXIS\n");
	printf("t: Reset all transformations and deltas\n");
	printf("------------------------------------------------\n");
}

void cleanData() {
	cube->cleanData();

	// 释放内存
	delete cube;
	cube = NULL;

	// 删除绑定的对象
    glDeleteVertexArrays(1, &cube_object.vao);

	glDeleteBuffers(1, &cube_object.vbo);
	glDeleteProgram(cube_object.program);
}

int main(int argc, char** argv)
{
	// 初始化GLFW库，必须是应用程序调用的第一个GLFW函数
	glfwInit();

	// 配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// 配置窗口属性
	GLFWwindow* window = glfwCreateWindow(600, 600, "2020284022_吴洪涛_实验2", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 调用任何OpenGL的函数之前初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	init();
	// 输出帮助信息
	printHelp();
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	
	double previoustime = glfwGetTime();
	double nowtime;
	while (!glfwWindowShouldClose(window))
	{
		display();

		//定时器功能，每隔0.01s，如果rotateFlag为真，则进行旋转。
		nowtime = glfwGetTime();
		if (rotateFlag && nowtime - previoustime >= 0.01) {
			if (XAxisFlag)
				updateTheta(X_AXIS, 1);
			else if (YAxisFlag)
				updateTheta(Y_AXIS, 1);
			else if (ZAxisFlag)
				updateTheta(Z_AXIS, 1);
			previoustime = nowtime;
		}
		// 交换颜色缓冲 以及 检查有没有触发什么事件（比如键盘输入、鼠标移动等）
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	cleanData();

	return 0;
}