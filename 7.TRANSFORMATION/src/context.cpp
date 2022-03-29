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
  	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
   	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
   	0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
  	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

  	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
   	0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
   	0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
  	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

  	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
  	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
  	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

   	0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
   	0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
   	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
   	0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

  	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
   	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
   	0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
  	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

  	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
   	0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
   	0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
  	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
};

uint32_t indices[] = {
   	0,  2,  1,  2,  0,  3,
   	4,  5,  6,  6,  7,  4,
   	8,  9, 10, 10, 11,  8,
  	12, 14, 13, 14, 12, 15,
  	16, 17, 18, 18, 19, 16,
  	20, 22, 21, 22, 20, 23,
};

	// VAO - Array Object를 만들고 Buffer Object를 만든다
	m_vertexLayout = VertexLayout::Create();
	// VBO
	m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 120);
	// VAO - VBO 연결
	m_vertexLayout->SetAttrib(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*5,0);
	m_vertexLayout->SetAttrib(2,2,GL_FLOAT,GL_FALSE,sizeof(float)*5,sizeof(float)*3);
	// EBO
	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(float) *36);
	
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
	m_program->SetUniform("tex",0);
	m_program->SetUniform("tex2",1);
	return true;
}

void Context::Render() {
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

    auto projection = glm::perspective(glm::radians(45.0f),
        (float)640 / (float)480, 0.01f, 20.0f);
    auto view = glm::translate(glm::mat4(1.0f),
        glm::vec3(0.0f, 0.0f, -3.0f));

    for (size_t i = 0; i < cubePositions.size(); i++){
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model,
            glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),
            glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}
