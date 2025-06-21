#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Circle.h"
#include <filesystem>


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

glm::vec2 computeGravitationalForce(const Circle& a, const Circle& b) {
    const float G = 1e-1f;

    glm::vec2 direction = b.getPosition() - a.getPosition();
    float distance = glm::length(direction);
    distance = glm::max(distance, 0.05f);

    float forceMagnitude = (G * a.getMass() * b.getMass()) / (distance * distance);
    glm::vec2 force = glm::normalize(direction)  * forceMagnitude;
    std::cout << "Force (x,y): (" << force.x <<", " << force.y << ")" << std::endl;
    return force;
}

int main(void)
{
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Gravity Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // getting glad

    const glm::vec2 gravityOnEarth = glm::vec2(0.0f, -0.0000098);
    
// Custom Shader Object
//-----------------------------------------------------------------------------------------------------------------
    Shader shader("shaders\\basic.vert", "shaders\\basic.frag");

// Circles
//-----------------------------------------------------------------------------------------------------------------
    std::vector<Circle> circles;
    //circles.reserve(2);

    Circle circle(glm::vec2(0.0f, 0.0f),  // position
        glm::vec2(0.0f, 0.0f),  // velocity
        glm::vec3(1.0f, 0.0f, 0.0f),  // color (red)
        0.15f,  // radius
        1.0f); // mass

    Circle circle2(glm::vec2(-0.5f, 0.5f),  // position
        glm::vec2(0.0f, 0.0f),  // velocity
        glm::vec3(0.0f, 0.0f, 1.0f),  // color (blue)
        0.15f,  // radius
        1.0f); // mass

    circles.push_back(std::move(circle));
    circles.push_back(std::move(circle2));

// Window settings and Misc.
//-----------------------------------------------------------------------------------------------------------------
    // need this so that the window works with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // first two parameters tell the system that the window's lower left corner has coordinates (0,0)
    // last two are size of window
    glViewport(0, 0, 800, 800);

    float lastFrame = 0.0f;
    

    circles[0].applyForce(glm::vec2(-3.0f, 0.0f));
    circles[1].applyForce(glm::vec2(-0.0f, 0.0f));

   
//Render loop
//-----------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // close window if user presses escape key
        processInput(window);

        /* Render here */
        // background color
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render in the circles
        shader.use();
        
        for (int i = 0; i < circles.size(); i++) {
            for (int j = i + 1; j < circles.size(); j++) {
                glm::vec2 force = computeGravitationalForce(circles[i], circles[j]);
                circles[i].applyForce(force);
                circles[j].applyForce(-force);
            }
        }

        
        for (Circle& circle : circles) {
            std::cout << "Circle at: " << circle.getPosition().x << ", " << circle.getPosition().y << "\n";
            circle.update(deltaTime);
            circle.constrainToBounds(-1.0f, 1.0f, -1.0f, 1.0f);
            circle.draw(shader);
        }
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}