#include "SceneInventoryTest.hpp"
#include "Sprite.hpp"

#include <string>

#include <glad/glad.h>

#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }
#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

namespace Mayra
{
    void MultiplyMatrixByVector4by4OpenGL_FLOAT(float *resultvector, const float *matrix, const float *pvector);
    void MultiplyMatrices4by4OpenGL_FLOAT(float *result, float *matrix1, float *matrix2);
    int glhProjectf(float objx, float objy, float objz, float *modelview, float *projection, int *viewport, float *windowCoordinate);
    int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate);
    int glhInvertMatrixf2(float *m, float *out);


    int glhProjectf(float objx, float objy, float objz, float *modelview, float *projection, int *viewport, float *windowCoordinate)
    {
        // Transformation vectors
        float fTempo[8];
        // Modelview transform
        fTempo[0]=modelview[0]*objx+modelview[4]*objy+modelview[8]*objz+modelview[12]; // w is always 1
        fTempo[1]=modelview[1]*objx+modelview[5]*objy+modelview[9]*objz+modelview[13];
        fTempo[2]=modelview[2]*objx+modelview[6]*objy+modelview[10]*objz+modelview[14];
        fTempo[3]=modelview[3]*objx+modelview[7]*objy+modelview[11]*objz+modelview[15];
        // Projection transform, the final row of projection matrix is always [0 0 -1 0]
        // so we optimize for that.
        fTempo[4]=projection[0]*fTempo[0]+projection[4]*fTempo[1]+projection[8]*fTempo[2]+projection[12]*fTempo[3];
        fTempo[5]=projection[1]*fTempo[0]+projection[5]*fTempo[1]+projection[9]*fTempo[2]+projection[13]*fTempo[3];
        fTempo[6]=projection[2]*fTempo[0]+projection[6]*fTempo[1]+projection[10]*fTempo[2]+projection[14]*fTempo[3];
        fTempo[7]=-fTempo[2];
        // The result normalizes between -1 and 1
        if(fTempo[7]==0.0) // The w value
            return 0;
        fTempo[7]=1.0/fTempo[7];
        // Perspective division
        fTempo[4]*=fTempo[7];
        fTempo[5]*=fTempo[7];
        fTempo[6]*=fTempo[7];
        // Window coordinates
        // Map x, y to range 0-1
        windowCoordinate[0]=(fTempo[4]*0.5+0.5)*viewport[2]+viewport[0];
        windowCoordinate[1]=(fTempo[5]*0.5+0.5)*viewport[3]+viewport[1];
        // This is only correct when glDepthRange(0.0, 1.0)
        windowCoordinate[2]=(1.0+fTempo[6])*0.5;    // Between 0 and 1
        return 1;
    }

    int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate)
    {
        // Transformation matrices
        float m[16], A[16];
        float in[4], out[4];
        // Calculation for inverting a matrix, compute projection x modelview
        // and store in A[16]
        MultiplyMatrices4by4OpenGL_FLOAT(A, projection, modelview);
        // Now compute the inverse of matrix A
        if(glhInvertMatrixf2(A, m)==0)
            return 0;
        // Transformation of normalized coordinates between -1 and 1
        in[0]=(winx-(float)viewport[0])/(float)viewport[2]*2.0-1.0;
        in[1]=(winy-(float)viewport[1])/(float)viewport[3]*2.0-1.0;
        in[2]=2.0*winz-1.0;
        in[3]=1.0;
        // Objects coordinates
        MultiplyMatrixByVector4by4OpenGL_FLOAT(out, m, in);
        if(out[3]==0.0)
            return 0;
        out[3]=1.0/out[3];
        objectCoordinate[0]=out[0]*out[3];
        objectCoordinate[1]=out[1]*out[3];
        objectCoordinate[2]=out[2]*out[3];

        std::cout << "----" << std::endl;
        std::cout << objectCoordinate[0] << ", ";
        std::cout << objectCoordinate[1] << ", ";
        std::cout << objectCoordinate[2] << std::endl;
        std::cout << "----" << std::endl;

        return 1;
    }

    void MultiplyMatrices4by4OpenGL_FLOAT(float *result, float *matrix1, float *matrix2)
    {
        result[0]=matrix1[0]*matrix2[0]+
        matrix1[4]*matrix2[1]+
        matrix1[8]*matrix2[2]+
        matrix1[12]*matrix2[3];
        result[4]=matrix1[0]*matrix2[4]+
        matrix1[4]*matrix2[5]+
        matrix1[8]*matrix2[6]+
        matrix1[12]*matrix2[7];
        result[8]=matrix1[0]*matrix2[8]+
        matrix1[4]*matrix2[9]+
        matrix1[8]*matrix2[10]+
        matrix1[12]*matrix2[11];
        result[12]=matrix1[0]*matrix2[12]+
        matrix1[4]*matrix2[13]+
        matrix1[8]*matrix2[14]+
        matrix1[12]*matrix2[15];
        result[1]=matrix1[1]*matrix2[0]+
        matrix1[5]*matrix2[1]+
        matrix1[9]*matrix2[2]+
        matrix1[13]*matrix2[3];
        result[5]=matrix1[1]*matrix2[4]+
        matrix1[5]*matrix2[5]+
        matrix1[9]*matrix2[6]+
        matrix1[13]*matrix2[7];
        result[9]=matrix1[1]*matrix2[8]+
        matrix1[5]*matrix2[9]+
        matrix1[9]*matrix2[10]+
        matrix1[13]*matrix2[11];
        result[13]=matrix1[1]*matrix2[12]+
        matrix1[5]*matrix2[13]+
        matrix1[9]*matrix2[14]+
        matrix1[13]*matrix2[15];
        result[2]=matrix1[2]*matrix2[0]+
        matrix1[6]*matrix2[1]+
        matrix1[10]*matrix2[2]+
        matrix1[14]*matrix2[3];
        result[6]=matrix1[2]*matrix2[4]+
        matrix1[6]*matrix2[5]+
        matrix1[10]*matrix2[6]+
        matrix1[14]*matrix2[7];
        result[10]=matrix1[2]*matrix2[8]+
        matrix1[6]*matrix2[9]+
        matrix1[10]*matrix2[10]+
        matrix1[14]*matrix2[11];
        result[14]=matrix1[2]*matrix2[12]+
        matrix1[6]*matrix2[13]+
        matrix1[10]*matrix2[14]+
        matrix1[14]*matrix2[15];
        result[3]=matrix1[3]*matrix2[0]+
        matrix1[7]*matrix2[1]+
        matrix1[11]*matrix2[2]+
        matrix1[15]*matrix2[3];
        result[7]=matrix1[3]*matrix2[4]+
        matrix1[7]*matrix2[5]+
        matrix1[11]*matrix2[6]+
        matrix1[15]*matrix2[7];
        result[11]=matrix1[3]*matrix2[8]+
        matrix1[7]*matrix2[9]+
        matrix1[11]*matrix2[10]+
        matrix1[15]*matrix2[11];
        result[15]=matrix1[3]*matrix2[12]+
        matrix1[7]*matrix2[13]+
        matrix1[11]*matrix2[14]+
        matrix1[15]*matrix2[15];
    }

    void MultiplyMatrixByVector4by4OpenGL_FLOAT(float *resultvector, const float *matrix, const float *pvector)
    {
        resultvector[0]=matrix[0]*pvector[0]+matrix[4]*pvector[1]+matrix[8]*pvector[2]+matrix[12]*pvector[3];
        resultvector[1]=matrix[1]*pvector[0]+matrix[5]*pvector[1]+matrix[9]*pvector[2]+matrix[13]*pvector[3];
        resultvector[2]=matrix[2]*pvector[0]+matrix[6]*pvector[1]+matrix[10]*pvector[2]+matrix[14]*pvector[3];
        resultvector[3]=matrix[3]*pvector[0]+matrix[7]*pvector[1]+matrix[11]*pvector[2]+matrix[15]*pvector[3];
    }



    // This code comes directly from GLU except that it is for float
    int glhInvertMatrixf2(float *m, float *out)
    {
        float wtmp[4][8];
        float m0, m1, m2, m3, s;
        float *r0, *r1, *r2, *r3;
        r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
        r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
        r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
        r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
        r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
        r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
        r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
        r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
        r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
        r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
        r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
        r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
        r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
        /* choose pivot - or die */
        if (fabsf(r3[0]) > fabsf(r2[0]))
            SWAP_ROWS_FLOAT(r3, r2);
        if (fabsf(r2[0]) > fabsf(r1[0]))
            SWAP_ROWS_FLOAT(r2, r1);
        if (fabsf(r1[0]) > fabsf(r0[0]))
            SWAP_ROWS_FLOAT(r1, r0);
        if (0.0 == r0[0])
            return 0;
        /* eliminate first variable */
        m1 = r1[0] / r0[0];
        m2 = r2[0] / r0[0];
        m3 = r3[0] / r0[0];
        s = r0[1];
        r1[1] -= m1 * s;
        r2[1] -= m2 * s;
        r3[1] -= m3 * s;
        s = r0[2];
        r1[2] -= m1 * s;
        r2[2] -= m2 * s;
        r3[2] -= m3 * s;
        s = r0[3];
        r1[3] -= m1 * s;
        r2[3] -= m2 * s;
        r3[3] -= m3 * s;
        s = r0[4];
        if (s != 0.0) {
            r1[4] -= m1 * s;
            r2[4] -= m2 * s;
            r3[4] -= m3 * s;
        }
        s = r0[5];
        if (s != 0.0) {
            r1[5] -= m1 * s;
            r2[5] -= m2 * s;
            r3[5] -= m3 * s;
        }
        s = r0[6];
        if (s != 0.0) {
            r1[6] -= m1 * s;
            r2[6] -= m2 * s;
            r3[6] -= m3 * s;
        }
        s = r0[7];
        if (s != 0.0) {
            r1[7] -= m1 * s;
            r2[7] -= m2 * s;
            r3[7] -= m3 * s;
        }
        /* choose pivot - or die */
        if (fabsf(r3[1]) > fabsf(r2[1]))
            SWAP_ROWS_FLOAT(r3, r2);
        if (fabsf(r2[1]) > fabsf(r1[1]))
            SWAP_ROWS_FLOAT(r2, r1);
        if (0.0 == r1[1])
            return 0;
        /* eliminate second variable */
        m2 = r2[1] / r1[1];
        m3 = r3[1] / r1[1];
        r2[2] -= m2 * r1[2];
        r3[2] -= m3 * r1[2];
        r2[3] -= m2 * r1[3];
        r3[3] -= m3 * r1[3];
        s = r1[4];
        if (0.0 != s) {
            r2[4] -= m2 * s;
            r3[4] -= m3 * s;
        }
        s = r1[5];
        if (0.0 != s) {
            r2[5] -= m2 * s;
            r3[5] -= m3 * s;
        }
        s = r1[6];
        if (0.0 != s) {
            r2[6] -= m2 * s;
            r3[6] -= m3 * s;
        }
        s = r1[7];
        if (0.0 != s) {
            r2[7] -= m2 * s;
            r3[7] -= m3 * s;
        }
        /* choose pivot - or die */
        if (fabsf(r3[2]) > fabsf(r2[2]))
            SWAP_ROWS_FLOAT(r3, r2);
        if (0.0 == r2[2])
            return 0;
        /* eliminate third variable */
        m3 = r3[2] / r2[2];
        r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
        r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
        /* last check */
        if (0.0 == r3[3])
            return 0;
        s = 1.0 / r3[3];        /* now back substitute row 3 */
        r3[4] *= s;
        r3[5] *= s;
        r3[6] *= s;
        r3[7] *= s;
        m2 = r2[3];            /* now back substitute row 2 */
        s = 1.0 / r2[2];
        r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
        r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
        m1 = r1[3];
        r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
        r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
        m0 = r0[3];
        r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
        r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
        m1 = r1[2];            /* now back substitute row 1 */
        s = 1.0 / r1[1];
        r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
        r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
        m0 = r0[2];
        r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
        r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
        m0 = r0[1];            /* now back substitute row 0 */
        s = 1.0 / r0[0];
        r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
        r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
        MAT(out, 0, 0) = r0[4];
        MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
        MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
        MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
        MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
        MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
        MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
        MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
        MAT(out, 3, 3) = r3[7];
        return 1;
    }

    SceneInventoryTest::SceneInventoryTest()
        : m_ClearColor(glm::vec4(1.0f)), m_Projection(glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f))
    {
        Sprite* spriteA = new Sprite(TEXTURES "block.png");
        spriteA->SetScale(glm::vec2(0.5f));
        spriteA->SetPosition(glm::vec2(0.5f, 0.5f));
        spriteA->SetRotation(0.0f);
        m_GameObjects.push_back(spriteA);

        m_Inventory = new Mayra::Inventory();
    }

    void SceneInventoryTest::OnUpdate(float)
    {
        if (Input::Instance()->IsMouseDown(0))
        {
            {
                int x = Input::Instance()->GetMouse().x;
                int y = Input::Instance()->GetMouse().y;

                GLint viewport[4];
                GLdouble modelview[16];
                GLdouble projection[16];
                GLfloat winX, winY, winZ;
//                GLdouble object_x, object_y, object_z;
                float objectCoordinate[3];

                int mouse_x = x;
                int mouse_y = y;
                objectCoordinate[0] = 0.0f;
                objectCoordinate[1] = 0.0f;
                objectCoordinate[2] = 0.0f;

                glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
                glGetDoublev(GL_PROJECTION_MATRIX, projection);
                glGetIntegerv(GL_VIEWPORT, viewport);

                winX = (float)mouse_x;
                winY = (float)viewport[3] - (float)mouse_y - 1.0f;
                glReadBuffer(GL_BACK);
                glReadPixels(mouse_x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
                
//                glhUnProjectf(winX,
//                              winY,
//                              winZ,
//                              &modelview,
//                              &projection,
//                              &viewport,
//                              &objectCoordinate);
//                std::cout << "----" << std::endl;
//                std::cout << objectCoordinate[0] << ", ";
//                std::cout << objectCoordinate[1] << ", ";
//                std::cout << objectCoordinate[2] << std::endl;
//                std::cout << "----" << std::endl;
//                int glhUnProjectf(float winx,
//                                  float winy,
//                                  float winz,
//                                  float *modelview,
//                                  float *projection,
//                                  int *viewport,
//                                  float *objectCoordinate
//                                  );

//                glhUnProjectf(winX,
//                             winY,
//                             winZ,
//                             modelview,
//                             projection,
//                             viewport,
//                             &object_x,
//                             &object_y,
//                             &object_z);
            }
//            std::cout << "MOUSE BUTTON 0 DOWN" << std::endl;
//            glm::vec2 mousePosition = Input::Instance()->GetMouse();
//            std::cout << "x(" << mousePosition.x << ") y(" << mousePosition.y << ")" << std::endl;
////            glm::vec2 mousePosition = glm::vec2(0.5f, 0.5f);
//            Sprite* spriteA = new Sprite(TEXTURES "block.png");
//            spriteA->SetScale(glm::vec2(0.5f));
//            spriteA->SetPosition(mousePosition);
//            spriteA->SetRotation(0.0f);
//            m_GameObjects.push_back(spriteA);
        }
    }

    SceneInventoryTest::~SceneInventoryTest()
    {
        // delete all from vector
        delete m_Inventory;
    }

    void SceneInventoryTest::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        for (unsigned int i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects.at(i)->Render(nullptr);
        }
    }

    void SceneInventoryTest::OnImGuiRender()
    {
        {
            ImGui::Begin("Bags and Inventories.");
            std::string result = (!show) ? "Show" : "Hide";

            if (ImGui::Button((result + std::string(" Demo Window")).c_str()))
                show = !show;
            ImGui::End();
        }
        
        if (show)
            ImGui::ShowDemoWindow(&show);
    }
}
