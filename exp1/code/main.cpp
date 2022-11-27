#include "Angel.h"
#include <string>

const glm::vec3 WHITE(1.0, 1.0, 1.0);
const glm::vec3 BLACK(0.0, 0.0, 0.0);
const glm::vec3 RED(1.0, 0.0, 0.0);
const glm::vec3 GREEN(0.0, 1.0, 0.0);
const glm::vec3 BLUE(0.0, 0.0, 1.0);
const int CIRCLE_NUM_POINTS = 100;
const int ELLIPSE_NUM_POINTS = 100;
const int TRIANGLE_NUM_POINTS = 3;
const int SQUARE_NUM = 6;
const int SQUARE_NUM_POINTS = 4 * SQUARE_NUM;
const int LINE_NUM_POINTS = 2;

// 每当窗口改变大小，GLFW会调用这个函数并填充相应的参数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// 根据角度生成颜色
float generateAngleColor(double angle)
{
	return 1.0 / (2 * M_PI) * angle;
}

// 获得三角形的每个角度
double getTriangleAngle(int point)
{
	return 2 * M_PI / 3 * point;
}

// 获得正方形的每个角度
double getSquareAngle(int point)
{
	return M_PI / 4 + (M_PI / 2 * point);
}

// 计算椭圆/圆上的点
glm::vec2 getEllipseVertex(glm::vec2 center, double scale, double verticalScale, double angle)
{
	glm::vec2 vertex(sin(angle), cos(angle));
	vertex *= scale;
	vertex.y *= verticalScale;
	vertex += center;
	return vertex;
}

// 获得椭圆/圆的每个顶点
void generateEllipsePoints(glm::vec2 vertices[], glm::vec3 colors[], int startVertexIndex, int numPoints,
	glm::vec2 center, double scale, double verticalScale)
{
	double angleIncrement = (2 * M_PI) / numPoints;
	double currentAngle = M_PI / 2;

	for (int i = startVertexIndex; i < startVertexIndex + numPoints; ++i) {
		vertices[i] = getEllipseVertex(center, scale, verticalScale, currentAngle);
		if (verticalScale == 1.0) {
			colors[i] = glm::vec3(1-generateAngleColor(currentAngle), 0.0, 0.0);
		}
		else {
			colors[i] = RED;
		}
		currentAngle += angleIncrement;
	}

}

void generateSemiEllipsePoints(glm::vec2 vertices[], glm::vec3 colors[], int startVertexIndex, int numPoints,
	glm::vec2 center, double scale, double verticalScale)
{
	double angleIncrement = (1 * M_PI) / numPoints;
	double currentAngle = M_PI / 2;

	for (int i = startVertexIndex; i < startVertexIndex + numPoints; ++i) {
		vertices[i] = getEllipseVertex(center, scale, verticalScale, currentAngle);
		if (verticalScale == 1.0) {
			colors[i] = glm::vec3(generateAngleColor(currentAngle), 0.0, 0.0);
		}
		else {
			colors[i] = RED;
		}
		currentAngle += angleIncrement;
	}
}

// 获得三角形的每个顶点
void generateTrianglePoints(glm::vec2 vertices[], glm::vec3 colors[], int startVertexIndex)
{
	glm::vec2 scale(0.15, 0.15);
	glm::vec2 center(0.0, 0.15);

	for (int i = 0; i < 3; ++i) {
		double currentAngle = getTriangleAngle(i);
		vertices[startVertexIndex + i] = glm::vec2(sin(currentAngle), cos(currentAngle)) * scale + center;
	}

	colors[startVertexIndex] = RED;
	colors[startVertexIndex + 1] = BLUE;
	colors[startVertexIndex + 2] = BLUE;
}

// 获得正方形的每个顶点
void generateSquarePoints(glm::vec2 vertices[], glm::vec3 colors[], int squareNumber, int startVertexIndex)
{
	glm::vec2 scale(1.0, 1.0);
	glm::vec2 center(0.0, 0.1);
	int vertexIndex = startVertexIndex;

		glm::vec3 currentColor = GREEN;
		for (int j = 0; j < 4; ++j) {
			double currentAngle = getSquareAngle(j);
			vertices[vertexIndex] = glm::vec2(sin(currentAngle), cos(currentAngle)) * scale + center;
			colors[vertexIndex] = currentColor;
			vertexIndex++;
		}
}


GLuint vao[5], program;
void init()
{

	// 定义三角形的点
	glm::vec2 triangle_vertices[TRIANGLE_NUM_POINTS];
	glm::vec3 triangle_colors[TRIANGLE_NUM_POINTS];
	// 定义矩形的点
	glm::vec2 square_vertices[SQUARE_NUM_POINTS];
	glm::vec3 square_colors[SQUARE_NUM_POINTS];


	//定义圆的点
	glm::vec2 left_circle_vertices[CIRCLE_NUM_POINTS];
	glm::vec3 left_circle_colors[CIRCLE_NUM_POINTS];
	glm::vec2 right_circle_vertices[CIRCLE_NUM_POINTS];
	glm::vec3 right_circle_colors[CIRCLE_NUM_POINTS];
	//定义椭圆的点
	glm::vec2 ellipse_vertices[ELLIPSE_NUM_POINTS];
	glm::vec3 ellipse_colors[ELLIPSE_NUM_POINTS];

	glm::vec2 ellipse_center(0.0, -0.1);
	generateSemiEllipsePoints(ellipse_vertices, ellipse_colors, 0, ELLIPSE_NUM_POINTS, ellipse_center, 0.4, 0.8);

	glm::vec2 left_circle_center(-0.5, 0.40);
	generateEllipsePoints(left_circle_vertices, left_circle_colors, 0, CIRCLE_NUM_POINTS, left_circle_center, 0.1, 1.0);

	glm::vec2 right_circle_center(0.5, 0.40);
	generateEllipsePoints(right_circle_vertices, right_circle_colors, 0, CIRCLE_NUM_POINTS, right_circle_center, 0.1, 1.0);

	// 调用生成形状顶点位置的函数
	generateTrianglePoints(triangle_vertices, triangle_colors, 0);
	generateSquarePoints(square_vertices, square_colors, SQUARE_NUM, 0);


	// 读取着色器并使用
	std::string vshader, fshader;
	vshader = "shaders/vshader.glsl";
	fshader = "shaders/fshader.glsl";
	program = InitShader(vshader.c_str(), fshader.c_str());
	glUseProgram(program);

	// 创建顶点缓存对象，vbo[2]是因为我们将要使用两个缓存对象
	GLuint vbo[2];


	/*
	* 初始化正方形的数据
	*/

	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);
	GLuint location = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(
		location,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		BUFFER_OFFSET(0));

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_colors), square_colors, GL_STATIC_DRAW);
	GLuint cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(
		cLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		BUFFER_OFFSET(0));

	/*
	* 初始化三角形的数据
	*/

	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	location = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(
		location,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		BUFFER_OFFSET(0));

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors, GL_STATIC_DRAW);
	cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(
		cLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		BUFFER_OFFSET(0));


	/*
	* 初始化椭圆的数据
	*/
	glGenVertexArrays(1, &vao[2]);
	glBindVertexArray(vao[2]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_vertices), ellipse_vertices, GL_STATIC_DRAW);
	location = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(
		location,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		BUFFER_OFFSET(0));

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_colors), ellipse_colors, GL_STATIC_DRAW);
	cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(
		cLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		BUFFER_OFFSET(0));


	/*
	* 初始化左圆的数据
	*/
	glGenVertexArrays(1, &vao[3]);
	glBindVertexArray(vao[3]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(left_circle_vertices), left_circle_vertices, GL_STATIC_DRAW);
	location = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(
		location,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		BUFFER_OFFSET(0));

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(left_circle_colors), left_circle_colors, GL_STATIC_DRAW);
	cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(
		cLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		BUFFER_OFFSET(0));

	/*
	* 初始化右圆的数据
	*/

	glGenVertexArrays(1, &vao[4]);
	glBindVertexArray(vao[4]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(right_circle_vertices), right_circle_vertices, GL_STATIC_DRAW);
	location = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(
		location,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		BUFFER_OFFSET(0));

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(right_circle_colors), right_circle_colors, GL_STATIC_DRAW);
	cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(
		cLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

	//绘制矩形
	glBindVertexArray(vao[0]);
	for (int i = 0; i < SQUARE_NUM; ++i) {
		glDrawArrays(GL_TRIANGLE_FAN, (i * 4), 4);
	}

	//绘制三角形
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_NUM_POINTS);


	// 绘制半椭圆（笑脸）
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, ELLIPSE_NUM_POINTS);

	// 绘制左圆（左眼）
	glBindVertexArray(vao[3]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_NUM_POINTS);

	//绘制右圆（右眼）
	glBindVertexArray(vao[4]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, ELLIPSE_NUM_POINTS);

	glFlush();
}

int main(int argc, char** argv)
{
	// 初始化GLFW库
	glfwInit();

	// 配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 配置窗口属性
	GLFWwindow* window = glfwCreateWindow(512, 512, "2020284022_吴洪涛_实验一", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 调用任何OpenGL的函数之前初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	init();

	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	while (!glfwWindowShouldClose(window))
	{
		display();
		// 交换颜色缓冲 以及 检查有没有触发什么事件（比如键盘输入、鼠标移动等）
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}