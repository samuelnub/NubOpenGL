#include "shader.h"

Shader::Shader(const GLchar *vertPath, const GLchar *fragPath)
{
	std::string vertCode;
	std::string fragCode;
	std::ifstream vertFile;
	std::ifstream fragFile;

	vertFile.exceptions(std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::badbit);

	try
	{
		vertFile.open(vertPath);
		fragFile.open(fragPath);
		std::stringstream vertStream, fragStream;

		vertStream << vertFile.rdbuf();
		fragStream << fragFile.rdbuf();

		vertFile.close();
		fragFile.close();

		vertCode = vertStream.str();
		fragCode = fragStream.str();
	}
	catch (std::ifstream::failure err)
	{
		std::cout << "couldnt read shader file given :((((\n";
	}

	const GLchar* vShaderCode = vertCode.c_str();
	const GLchar* fShaderCode = fragCode.c_str();



	GLuint vert, frag;
	GLint succ;
	GLchar loggo[512];

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vShaderCode, NULL);
	glCompileShader(vert);

	glGetShaderiv(vert, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		glGetShaderInfoLog(vert, 512, NULL, loggo);
		std::cout << "couldnt compile vert shader :((((\n" << loggo << "\n\n";
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fShaderCode, NULL);
	glCompileShader(frag);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		glGetShaderInfoLog(vert, 512, NULL, loggo);
		std::cout << "couldnt compile frag shader :((((\n" << loggo << "\n\n";
	}

	this->_program = glCreateProgram();
	glAttachShader(this->_program, vert);
	glAttachShader(this->_program, frag);
	glLinkProgram(this->_program);
	glGetProgramiv(this->_program, GL_LINK_STATUS, &succ);
	if (!succ)
	{
		glGetProgramInfoLog(this->_program, 512, NULL, loggo);
		std::cout << "couldnt link shaders :((((\n" << loggo << "\n\n";
	}
	else
	{
		std::cout << "successfully compiled and linked the shaders:\n" <<
			vertPath << "\n" << fragPath << "\n\n";
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

void Shader::use()
{
	glUseProgram(this->_program);
}
