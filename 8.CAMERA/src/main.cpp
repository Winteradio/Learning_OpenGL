//#include "glad/glad.h"
// GLFW를 부르기 전에 glad를 호출 해야 한다
//#include <GLFW/glfw3.h>
#include "common.h"
#include "shader.h"
#include "program.h"
#include "context.h"
#include <iostream>

using namespace std;

void OnFramebufferSizeChange(GLFWwindow* window, int WIDTH, int HEIGHT){
	auto context = (Context*)glfwGetWindowUserPointer(window);
	context->Reshape(WIDTH,HEIGHT);
	cout << "Window Size:: WIDTH : " << WIDTH << " / HEIGHT : " << HEIGHT << endl;
}

void processInput(GLFWwindow* window,int key, int scancode, int action, int mods){
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window,true);
	}
}

void OnCursorPos(GLFWwindow* window, double x, double y){
	auto context = (Context*)glfwGetWindowUserPointer(window);
	context->MouseMove(x,y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int modifier){
	auto context = (Context*)glfwGetWindowUserPointer(window);
	double x,y;
	glfwGetCursorPos(window, &x, &y);
	context->MouseButton(button,action,x,y);
}

void render(){
	glClearColor(0.2f,0.3f,0.3f,1.0f);
	// glClearColor(화면을 지울 색깔 설정);
	// 색깔 세팅 함수

	glClear(GL_COLOR_BUFFER_BIT);
	// glClear(GL_COLOR_BUFFER_BIT);
	// 실제로 화면 지우는 함수
}


int main(){

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// opengl profile을 core profile로 설정

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"OPENGL PIPELINE",NULL,NULL);

	if ( window == NULL ){
		cout<< " Failed to create GLFW window " << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); 
	// 지금 현재 진행 중인 window에 Context를 사용해라

	// OpenGL Context 생성 이후 실행
	// gladLoadGLLoader : OpenGL 함수를 로딩하는 함수
	// glfwGetProcAddress : glfw의 함수들의 주소
	if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress)){
		cout << " Failed to initalize GLAD " << endl;
		return -1;
	}

	// Context Initalize 
	auto context = Context::Create();
	if (!context){
		glfwTerminate();
		return -1;
	}
	glfwSetWindowUserPointer(window, context.get());
	
	// Events
	OnFramebufferSizeChange(window,WINDOW_WIDTH,WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
	glfwSetKeyCallback(window,processInput);
	glfwSetCursorPosCallback(window, OnCursorPos);
	glfwSetMouseButtonCallback(window, OnMouseButton);

	while (!glfwWindowShouldClose(window)){

		glfwPollEvents();
		context->ProcessInput(window);
		context->Render();
		glfwSwapBuffers(window);
	}
	
	context.reset();
	glfwTerminate();
	return 0;
}
