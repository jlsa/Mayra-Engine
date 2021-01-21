#include "Tetromino.hpp"

namespace Mayra
{
    Tetromino::Tetromino()
    {
        SetName("Tetromino");
        glm::vec2 positions[4] {
            glm::vec2(0.075f, 0.0f),
            glm::vec2(0.075f * 2.0f, 0.0f),
            glm::vec2(0.075f * 3.0f, 0.0f),
            glm::vec2(0.075f, 0.075f),
        };

        m_Shape = Mayra::Shape::Oo;
        std::cout << "Shape: " << m_Shape << std::endl;

//        0
//        1
//        2
//        3
//
//        4
//        5
//        6
//        7
//
//        8
//        9
//        10
//        11
//
//        12
//        13
//        14
//        15
//
//        16
//        17
//        18
//        19
//
//        20
//        21
//        22
//        23
//
//        24
//        25
//        26
//        27

        for (int i = 0; i < Mayra::Shape::NUM_OF_SHAPES * 4; i+=4)
        {
            std::cout << "i: " << i << " - " << shapes[i][0] << " " << shapes[i][1] << " " << shapes[i][2] << " " << shapes[i][3] << std::endl;
            std::cout << "i: " << i << " - " << shapes[i + 1][0] << " " << shapes[i + 1][1] << " " << shapes[i + 1][2] << " " << shapes[i + 1][3] << std::endl;
            std::cout << "i: " << i << " - " << shapes[i + 2][0] << " " << shapes[i + 2][1] << " " << shapes[i + 2][2] << " " << shapes[i + 2][3] << std::endl;
            std::cout << "i: " << i << " - " << shapes[i + 3][0] << " " << shapes[i + 3][1] << " " << shapes[i + 3][2] << " " << shapes[i + 3][3] << std::endl;
            std::cout << std::endl;
        }

        m_Color = glm::vec4(0.294f, 0.000f, 0.510f, 1.0f);

        m_Tetromino0 = new Sprite(TEXTURES "block_solid.png");
        m_Tetromino0->SetScale(glm::vec2(1.0f, 1.0f));
        m_Tetromino0->SetPosition(positions[0]);
        m_Tetromino0->SetRotation(0.0f);
        m_Tetromino0->SetColor(m_Color);

        m_Tetromino1 = new Sprite(TEXTURES "block_solid.png");
        m_Tetromino1->SetScale(glm::vec2(1.0f, 1.0f));
        m_Tetromino1->SetPosition(positions[1]);
        m_Tetromino1->SetRotation(0.0f);
        m_Tetromino1->SetColor(m_Color);

        m_Tetromino2 = new Sprite(TEXTURES "block_solid.png");
        m_Tetromino2->SetScale(glm::vec2(1.0f, 1.0f));
        m_Tetromino2->SetPosition(positions[2]);
        m_Tetromino2->SetRotation(0.0f);
        m_Tetromino2->SetColor(m_Color);

        m_Tetromino3 = new Sprite(TEXTURES "block_solid.png");
        m_Tetromino3->SetScale(glm::vec2(1.0f, 1.0f));
        m_Tetromino3->SetPosition(positions[3]);
        m_Tetromino3->SetRotation(0.0f);
        m_Tetromino3->SetColor(m_Color);

        AddChild(m_Tetromino0);
        AddChild(m_Tetromino1);
        AddChild(m_Tetromino2);
        AddChild(m_Tetromino3);
    }

    Tetromino::~Tetromino()
    {
        delete m_Tetromino0;
        delete m_Tetromino1;
        delete m_Tetromino2;
        delete m_Tetromino3;
    }

    void Tetromino::Render(OrthographicCamera* camera)
    {
        if (!GetEnabled())
            return;

        for (unsigned int i = 0; i < m_Children.size(); i++)
        {
            m_Children.at(i)->Render(camera);
        }
    }

    void Tetromino::Update(float deltaTime)
    {
        if (!GetEnabled())
            return;

        m_Tetromino0->SetScale(this->GetScale());
        m_Tetromino0->SetPosition(this->GetPosition());
        m_Tetromino0->SetRotation(this->GetRotation());

        m_Tetromino1->SetScale(this->GetScale());
        m_Tetromino1->SetPosition(this->GetPosition() + glm::vec2(0.075f, 0.0f));
        m_Tetromino1->SetRotation(this->GetRotation());

        m_Tetromino2->SetScale(this->GetScale());
        m_Tetromino2->SetPosition(this->GetPosition() + glm::vec2(0.15f, 0.0f));
        m_Tetromino2->SetRotation(this->GetRotation());

        m_Tetromino3->SetScale(this->GetScale());
        m_Tetromino3->SetPosition(this->GetPosition() + glm::vec2(0.075f, 0.075f));
        m_Tetromino3->SetRotation(this->GetRotation());

        for (unsigned int i = 0; i < m_Children.size(); i++)
        {
            m_Children.at(i)->Update(deltaTime);
        }
    }
}
