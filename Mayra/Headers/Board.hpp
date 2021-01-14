#ifndef Board_h
#define Board_h

#include <Mayra.hpp>
#include "GameObject.hpp"
#include "OrthographicCamera.hpp"

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10

namespace Mayra
{
    class Board : public GameObject
    {
    public:
        Board(int width, int height);
        ~Board();

        void Update(float deltaTime);
        void Render(Mayra::OrthographicCamera* camera);

    private:
        int m_Width;
        int m_Height;
        Mayra::GameObject* m_Board[BOARD_HEIGHT][BOARD_WIDTH];
    };
}

#endif /* Board_h */
