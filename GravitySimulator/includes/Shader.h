#ifndef SHADER_H
#define SHADER_H
// modern C++ practice gets rid of the header guards and instead uses '#pragma once' at the top of a header file
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {

public:
	unsigned int ID;

	Shader(const char* vertPath, const char* fragPath);

	void use() const;

	// Utility functions to set shader uniforms
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setMat4(const std::string name, const glm::mat4& mat) const;

private:
	std::string readFile(const char* filePath);
	void checkCompileErrors(unsigned int shader, const std::string& type);
};

#endif
