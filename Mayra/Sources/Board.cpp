#include "Board.hpp"

#include "Sprite.hpp"

namespace Mayra
{
    Board::Board(int width, int height)
        : m_Width(width), m_Height(height)
    {
        SetName("Board");
        for (int y = 0; y < m_Height; ++y)
        {
            for (int x = 0; x < m_Width; ++x)
            {
                m_Board[y][x] = new Sprite(TEXTURES "block.png");
                m_Board[y][x]->SetPosition(glm::vec2(x * 0.075f, y * 0.075f));
                m_Board[y][x]->SetScale(glm::vec2(0.075f));
            }
        }
    }

    Board::~Board()
    {

    }

    void Board::Update(float /* deltaTime */)
    {
        if (!GetEnabled())
            return;
    }

    void Board::Render(Mayra::OrthographicCamera *camera)
    {
        if (!GetEnabled())
            return;

        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                m_Board[y][x]->Render(camera);
            }
        }
    }
}
