//
//  OrthographicCamera.cpp
//  Mayra
//
//  Created by Joel Hoekstra on 11/12/2020.
//

#include <OrthographicCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Mayra
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : _projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), _viewMatrix(1.0f)
    {
        _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) *
            glm::rotate(glm::mat4(1.0f), _rotation, glm::vec3(0.0f, 0.0f, 1.0f));

        _viewMatrix = glm::inverse(transform);
        _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
    } 
}
