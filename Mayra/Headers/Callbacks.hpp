//
//  callbacks.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

#ifndef callbacks_h
#define callbacks_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     camera.ProcessKeyboard(FORWARD, (float)deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     camera.ProcessKeyboard(LEFT, (float)deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     camera.ProcessKeyboard(RIGHT, (float)deltaTime);
}

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    //     std::cout << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << std::endl;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // if (firstMouse)
    // {
    //     lastX = (float)xpos;
    //     lastY = (float)ypos;
    //     firstMouse = false;
    // }

    // double xoffset = xpos - lastX;
    // double yoffset = lastY - ypos; // reversed since y-coords go from bottom to top

    // lastX = (float)xpos;
    // lastY = (float)ypos;

    // camera.ProcessMouseMovement((float)xoffset, (float)yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // camera.ProcessMouseScroll((float)yoffset);
}

#endif /* callbacks_h */
