#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include "Circle.h"
#include "Shader.h"

Circle::Circle(glm::vec2 position, glm::vec2 velocity, glm::vec3 color, float radius, float mass): 
	radius_(radius), position_(position), velocity_(velocity), color_(color), mass_(mass) {
	setupCircleMesh();
}
Circle::~Circle() {
	if (VBO_) glDeleteBuffers(1, &VBO_);
	if (VAO_) glDeleteVertexArrays(1, &VAO_);
}

// utility
//-------------------------------------------------------------------
void Circle::draw(const Shader& shader) const {
	shader.use();
	shader.setVec3("circleColor", color_);
	shader.setFloat("brightness", 1.0f);

	// build model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position_, 0.0f));
	shader.setMat4("model", model);
	
	//std::cout << "Drawing circle with vertexCount_: " << vertexCount_ << "\n";

	glBindVertexArray(VAO_);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount_);
	glBindVertexArray(0);
}
void Circle::update(float deltaTime) {
	velocity_ += acceleration_ * deltaTime;
	position_ += velocity_ * deltaTime;
	acceleration_ = glm::vec2(0.0f);
}
void Circle::applyForce(const glm::vec2& force) {
	acceleration_ += force / mass_;
}
void Circle::constrainToBounds(float xMin, float xMax, float yMin, float yMax) {
	if (position_.x - radius_ <= xMin) {
		position_.x = xMin + radius_;
		velocity_.x *= -1;
	}
	if (position_.x + radius_ >= xMax) {
		position_.x = xMax - radius_;
		velocity_.x *= -1;
	}
	if (position_.y - radius_ <= yMin) {
		position_.y = yMin + radius_;
		velocity_.y *= -1;
	}
	if (position_.y + radius_ >= yMax) {
		position_.y = yMax - radius_;
		velocity_.y *= -1;
	}
}

void Circle::printPositionAndVelocity() const {
	std::cout << "Position (x,y): (" << position_.x << ", " << position_.y << ")" << std::endl;
	std::cout << "Velocity (x,y): (" << velocity_.x << ", " << velocity_.y << ")" << std::endl;
}

//private
//-------------------------------------------------------------------
void Circle::setupCircleMesh() {

	std::vector<glm::vec3> vertices;
	const int numSegments = 32;
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // adding the center vertex

	for (unsigned int i = 0; i <= numSegments; i++) {
		// finds the angle for each vertex along 360 degrees
		float angle = i * 2.0f * glm::pi<float>() / numSegments;
		// get the x and y coordinates the for vertex in relation to the center, radius of circle, and angle
		float x = radius_ * cos(angle);
		float y = radius_ * sin(angle);
		// emplace back does not need to create a temporary object before copying it into the vector
		vertices.emplace_back(x, y, 0.0f);
	}

	vertexCount_ = static_cast<int>(vertices.size());

	// create and bind VAO_ and VBO_
	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);

	// bind VAO, then VBO, then send over vertices data.
	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);

	// establish where the array pointer will start, assign the stride, and the amount of memory needed 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices.size()));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}