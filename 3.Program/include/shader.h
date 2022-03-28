#ifndef __SHADER_H__
#define __SHADER_H__

#include "common.h"

CLASS_PTR(Shader);
class Shader{
	public :
		static ShaderUPtr CreateFromFile(const std::string& filename,GLenum shaderType);

		~Shader();
			
		uint32_t Get() const { return m_shader;}
		/*
		Get은 있지만, Set은 없는 이유
		외부에서 드러나지 않기 위해서
		*/

	private :

		Shader() {} //생성자
		/*
		외부에서 생성자 호출 불가능
		*/
			
		bool LoadFile(const std::string& filename,GLenum shaderType);

		uint32_t m_shader { 0 };
};

#endif // __SHADER_H__
