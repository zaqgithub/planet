#include"camera.h"

#include"ShaderClass.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//capture errors 
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//read shader file 
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(std::ifstream::failure e){
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ " <<e.what()<< std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[1024];
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "ERROR::VERTEXSHADER::COMPILE::FAILED\n" <<infoLog <<std::endl;
	}

	glShaderSource(fragment, 1, &fShaderCode,NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "ERROR::FRAGMENTSHADER::COMPILE::FAILED\n" <<infoLog<< std::endl;
	}
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK::FAILED\n" << infoLog<<std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

}
Shader::Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath) {
	std::string vertexCode, geometryCode, fragmentCode;
	std::ifstream vertexBuffer, geometryBuffer, fragmentBuffer;
	vertexBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometryBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vertexBuffer.open(vertexPath);
		geometryBuffer.open(geometryPath);
		fragmentBuffer.open(fragmentPath);

		std::stringstream vertexShaderStream, geometryShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexBuffer.rdbuf();
		geometryShaderStream << geometryBuffer.rdbuf();
		fragmentShaderStream << fragmentBuffer.rdbuf();

		vertexBuffer.close();
		geometryBuffer.close();
		fragmentBuffer.close();

		vertexCode = vertexShaderStream.str();
		geometryCode = geometryShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ " << e.what() << std::endl;
	}
	const char* vertexShaderCode = vertexCode.c_str();
	const char* geometryShaderCode = geometryCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	unsigned int vertex, geometry, fragment;
	int success =0;
	char infoLog[1024];
	vertex = glCreateShader(GL_VERTEX_SHADER);
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "ERROR::VERTEXSHADER::COMPILE::FAILED\n" << infoLog << std::endl;
	}
	glShaderSource(geometry, 1, &geometryShaderCode, NULL);
	glCompileShader(geometry);
	glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geometry, 1024, NULL, infoLog);
		std::cout << "ERROR::VERTEXSHADER::COMPILE::FAILED\n" << infoLog << std::endl;
	}
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "ERROR::VERTEXSHADER::COMPILE::FAILED\n" << infoLog << std::endl;
	}
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, geometry);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK::FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);

}
void Shader::use() {
	glUseProgram(ID);
}
void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMFloat(const std::string& object, const std::string& name, const int counter,const std::vector<float>& array)const {
	for (int i=0;i<counter;i++)
		glUniform1f(glGetUniformLocation(ID, (object + "[" + std::to_string(i) + "]." + name).c_str()), array[i]);
}
void Shader::setFloat(const std::string& name, float value) const{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2(const std::string& name,const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()),1, &value[0]);
}
void Shader::setVec3(const std::string& name,const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()),1, &value[0]);
}
void Shader::setVec3(const std::string& name, const float x, const float y, const float z)const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &glm::vec3(x,y,z)[0]);
};

void Shader::setMVec3(const std::string& object, const std::string& name, const int counter,const glm::vec3 matrix[])const {

	for (int i = 0;i < counter;i++) {

		glUniform3fv(glGetUniformLocation(ID, (object + "[" + std::to_string(i) + "]." + name).c_str()), 1, &(matrix[i][0]));
	}
}
void Shader::setVec4(const std::string& name,const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1,&value[0]);
}
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
