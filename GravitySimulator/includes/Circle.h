#ifndef CIRCLE_H
#define CIRCLE_H

#include <glm/glm.hpp>
#include "Shader.h"
#include <iostream>

class Circle {

public:
	// deleting shallow copy constructor and following rule of 3 with overloading the = operator
	Circle(const Circle&) = delete;
	Circle& operator=(const Circle&) = delete;

	Circle(Circle&& other) noexcept :VAO_(other.VAO_), VBO_(other.VBO_), 
		radius_(other.radius_), position_(other.position_), 
		velocity_(other.velocity_), color_(other.color_), mass_(other.mass_), vertexCount_(other.vertexCount_) {
		other.VAO_ = 0;
		other.VBO_ = 0;
	}

	Circle& operator=(Circle&& other) noexcept {
		if (this != &other) {
			if (VAO_) glDeleteVertexArrays(1, &VAO_);
			if (VBO_) glDeleteBuffers(1, &VBO_);
			
			VAO_ = other.VAO_;
			VBO_ = other.VBO_;
			vertexCount_ = other.vertexCount_;
			radius_ = other.radius_;
			position_ = other.position_;
			velocity_ = other.velocity_;
			color_ = other.color_;
			mass_ = other.mass_;

			other.VAO_ = 0;
			other.VBO_ = 0;
		}

		return *this;
	}

	Circle(glm::vec2 position, glm::vec2 velocity, glm::vec3 color, float radius, float mass);
	~Circle();

	void draw(const Shader& shader) const;
	void update(float deltaTime);
	void applyForce(const glm::vec2& force);
	void constrainToBounds(float xMin, float xMax, float yMin, float yMax);
	inline float getMass() const { return mass_; }
	inline glm::vec2 getPosition() const { return position_;  }
	inline glm::vec3 getColor() const { return color_; }

	void printPositionAndVelocity() const;

private:

	void rebuildMesh();

	void setupCircleMesh();
	float radius_;
	glm::vec2 position_;
	glm::vec2 velocity_;
	glm::vec3 color_;
	float mass_;
	unsigned int VAO_, VBO_ = 0;
	int vertexCount_ = 0;
	glm::vec2 acceleration_ = glm::vec2(0.0f);
};

#endif // Circle.h!
