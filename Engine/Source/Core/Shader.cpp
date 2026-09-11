#include "Shader.h"

using namespace AtomToolKit;

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) :
	vertexShader(0), fragmentShader(0){

	std::ifstream vertexFile, fragmentFile;


	vertexFile.open(vertexShaderPath);
	fragmentFile.open(fragmentShaderPath);

	if(vertexFile.fail() || fragmentFile.fail()){
		std::cerr << "Error at opening shader files!" << (vertexFile ? std::string(vertexShaderPath) : std::string(fragmentShaderPath)) << "\n";
	}

	std::stringstream vertexStream, fragmentStream;
	
	vertexStream << vertexFile.rdbuf();
	fragmentStream << fragmentFile.rdbuf();
	
	vertexFile.close();
	fragmentFile.close();
	
	vertexCode = vertexStream.str();
	fragmentCode = fragmentStream.str();

	programId = glCreateProgram();

	CompileShaders();
	AttachShaders();
}

void Shader::CompileShaders(){
	const char* vVertexCode = vertexCode.c_str();
	const char* vFragmentCode = fragmentCode.c_str();
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vVertexCode, nullptr);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &vFragmentCode, nullptr);
	glCompileShader(fragmentShader);

	try {
		CheckForErrors(vertexShader);
		CheckForErrors(fragmentShader);
	}
	catch (std::exception& e) { std::cerr << e.what() << "\n"; }
}

void Shader::AttachShaders() const {
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);

	try { CheckProgramErrors(programId); } 
	catch (std::exception& e) { std::cerr << e.what() << "\n"; }
}

void Shader::CheckForErrors(const GLenum& shaderType) const {
	int success;
	char InfoLog[512];

	glGetShaderiv(shaderType, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderType, 512, nullptr, InfoLog);
		std::string msg = "Error at compiling Shader!" + std::string(InfoLog);
		throw std::runtime_error(msg);
	}
}

void Shader::CheckProgramErrors(const GLuint& shaderType) const {
	int success;
	char InfoLog[512];

	glGetProgramiv(shaderType, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderType, 512, nullptr, InfoLog);
		std::string msg = "Error at compiling Program!" + std::to_string(shaderType) + std::string(InfoLog);
		
		throw std::runtime_error(msg);
	}
}

Shader::~Shader() {
	glDeleteProgram(programId);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
}