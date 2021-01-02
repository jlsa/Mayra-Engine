#include "Test.hpp"
#include "TestsManager.hpp"

namespace Test
{
    TestsManager::TestsManager()
    {
        std::cout << "TestsManager::Constructor" << std::endl;
        Attach();
    }

    TestsManager::~TestsManager()
    {
        std::cout << "TestsManager::Destructor" << std::endl;
        Detach();
    }

    void TestsManager::Attach()
    {
        // do some attaching thing?
        std::cout << "TestsManager::Attach" << std::endl;
    }

    void TestsManager::Detach()
    {
        while (!tests.empty())
        {
            tests.back()->OnDetach();
            tests.pop_back();
        }

        std::cout << "TestsManager::Detach" << std::endl;
    }

    void TestsManager::ChangeTest(Test* test)
    {
        if (!tests.empty())
        {
            tests.back()->OnDetach();
            tests.pop_back();
        }

        tests.push_back(test);
        tests.back()->OnAttach();
    }

    void TestsManager::PushTest(Test* test)
    {
        if (!tests.empty())
            tests.back()->Suspend();

        tests.push_back(test);
        tests.back()->OnAttach();
    }

    void TestsManager::PopTest()
    {
        if (!tests.empty())
        {
            tests.back()->OnDetach();
            tests.pop_back();
        }
    }

    void TestsManager::HandleEvents()
    {
//        testCases.back()->HandleEvents(this);
    }

    void TestsManager::Update(float deltaTime)
    {
        tests.back()->OnUpdate(this, deltaTime);
//        testCases.back()->Update(this);
    }

    void TestsManager::Render()
    {
        tests.back()->OnRender(this);
//        testCases.back()->Render(this);
    }

    void TestsManager::RenderGui()
    {
        tests.back()->OnImGuiRender(this);
    }
}
