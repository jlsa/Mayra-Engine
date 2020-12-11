//
//  OrthographicCamera.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 11/12/2020.
//

#ifndef OrthographicCamera_h
#define OrthographicCamera_h

#include <glm/glm.hpp>

namespace Mayra
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        const glm::vec3& GetPosition() const { return _position; }
        void SetPosition(const glm::vec3& position) { _position = position; RecalculateViewMatrix(); }

        float GetRotation() const { return _rotation; }
        void SetRotation(float rotation) { _rotation = rotation; RecalculateViewMatrix(); }

        const glm::mat4& GetProjectionMatrix() const { return _projectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return _viewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return _projectionMatrix; }
    private:
        void RecalculateViewMatrix();
    private:
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;
        glm::mat4 _viewProjectionMatrix;

        glm::vec3 _position;
        float _rotation = 0.0f;
    };
}

#endif /* OrthographicCamera_h */

