#include "context.h"
#include "image.h"

ContextUPtr Context::Create(){
	auto context = ContextUPtr(new Context());

	if (!context->Init()){
		return nullptr;
	}

	return std::move(context);
}

bool Context::Init(){

	float vertices[] = {
		0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,
		0.5f,0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,0.0f,
		-0.5f,0.5f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
		-0.5f,-0.5f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f
	};

	uint32_t indices[] = {
		0,1,3,
		1,2,3
	};

	// VAO - Array Object를 만들고 Buffer Object를 만든다
	m_vertexLayout = VertexLayout::Create();
	// VBO
	m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);
	// VAO - VBO 연결
	m_vertexLayout->SetAttrib(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*8,0);
	m_vertexLayout->SetAttrib(1,3,GL_FLOAT,GL_FALSE,sizeof(float)*8,sizeof(float)*3);
	m_vertexLayout->SetAttrib(2,2,GL_FLOAT,GL_FALSE,sizeof(float)*8,sizeof(float)*6);
	// EBO
	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(float) *6);
	
	// Shader
	ShaderPtr vertShader = Shader::CreateFromFile("shader/texture.vs",GL_VERTEX_SHADER);
	ShaderPtr fragShader = Shader::CreateFromFile("shader/texture.fs",GL_FRAGMENT_SHADER);

	if(!vertShader|| !fragShader){ // or >> || :: and >> &&
		return false;
	}


	// Program
	m_program = Program::Create({fragShader,vertShader});
	if (!m_program){
		return false;
	}

	glClearColor(0.1f,0.2f,0.3f,0.0f);
	auto image = Image::Load("images/container.jpg");
	if (!image){
		return false;
	}
	m_texture = Texture::CreateFromImage(image.get());

	auto image2 = Image::Load("images/awesomeface.png");
	m_texture2 = Texture::CreateFromImage(image2.get());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->Get());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

	m_program->Use();
	glUniform1i(glGetUniformLocation(m_program->Get(),"tex"),0);
	glUniform1i(glGetUniformLocation(m_program->Get(),"tex2"),1);

	return true;
}

void Context::Render(){
	glClear(GL_COLOR_BUFFER_BIT);

	m_program->Use();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

}
