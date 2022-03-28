#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType){
	auto shader = ShaderUPtr(new Shader());

	if (!shader->LoadFile(filename,shaderType)){
		return nullptr;
	}
	return std::move(shader);
}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType){
	auto result = LoadTextFile(filename);
	if (!result.has_value()){
		return false;
	}
	auto& code = result.value();
	const char* codePtr = code.c_str();
	int32_t codeLength = (int32_t)code.length();
	
	/*
	uint8_t = unsigned char 
	int8_t = char

	uint16_t = unsigned short
	int16_t = short

	uint32_t = unsigned int
	int32_t = int
	
	uint64_t = unsigned long long
	int64_t = long long
	*/


	// Create Shader
	m_shader = glCreateShader(shaderType);
	glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength);
	glCompileShader(m_shader);


	// Check Compile Error
	int success = 0;
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
	if(!success){
		char infoLog[1024];
		glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
		return false;
	}

	return true;
}

Shader::~Shader(){
	if (m_shader){
		glDeleteShader(m_shader);
	}
}
