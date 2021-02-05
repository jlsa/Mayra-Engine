#include "SceneInventoryTest.hpp"
#include "Sprite.hpp"

#include <string>

namespace Mayra
{
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

    void SceneInventoryTest::OnUpdate(float) {}

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
        ImGui::Begin("Bags and Inventories.");
        std::string result = (!show) ? "Show" : "Hide";

        if (ImGui::Button((result + std::string(" Demo Window")).c_str()))
            show = !show;
        ImGui::End();

        {
            ImGui::Begin("Reordering");
            ImGui::BulletText("Drag and drop to re-order");
            ImGui::Indent();
            static const char* names[] = {
                "1. Khiro",
                "2. Hedwig",
                "3. Apple",
                "4. Iere",
                "5. Phoebe",
                "6. Mayra"
            };
            int move_from = -1, move_to = -1;

            for (int n = 0; n < IM_ARRAYSIZE(names); n++)
            {
                std::string result;

                result = std::string(names[n]) + "##" + std::to_string(n);//"Name: " + std::to_string(n) + "##" + std::to_string(n);
                //            result = "Name: " + names[n] + "" + std::to_string(n) + "##" + std::to_string(n);
//                ImGui::Button(result.c_str(), ImVec2(100, 50));
                ImGui::Selectable(names[n]);

                ImGuiDragDropFlags src_flags = 0;
                src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
                src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
                //            src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip;
                if (ImGui::BeginDragDropSource(src_flags))
                {
                    if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
                        ImGui::Text("Moving \"%s\"", names[n]);
                    ImGui::SetDragDropPayload("DND_DEMO_NAME", &n, sizeof(int));
                    ImGui::EndDragDropSource();
                }

                if (ImGui::BeginDragDropTarget())
                {
                    ImGuiDragDropFlags target_flags = 0;
                    //                target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
                    target_flags |= ImGuiDragDropFlags_AcceptPeekOnly;
                    target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME", target_flags))
                    {
                        move_from = *(const int*)payload->Data;
                        move_to = n;
                    }
                    ImGui::EndDragDropTarget();
                }
            }

            if (move_from != -1 && move_to != -1)
            {
                // reorder items
                int copy_dst = (move_from < move_to) ? move_from : move_to + 1;
                int copy_src = (move_from < move_to) ? move_from + 1 : move_to;
                int copy_count = (move_from < move_to) ? move_to - move_from : move_from - move_to;
                const char* tmp = names[move_from];
                printf("[%05d] move %d->%d (copy %d..%d to %d..%d)\n", ImGui::GetFrameCount(), move_from, move_to, copy_src, copy_src + copy_count - 1, copy_dst, copy_dst + copy_count - 1);
                memmove(&names[copy_dst], &names[copy_src], (size_t)copy_count * sizeof(const char*));
                names[move_to] = tmp;
                ImGui::SetDragDropPayload("DND_DEMO_NAME", &move_to, sizeof(int));
            }
            ImGui::Unindent();
            ImGui::End();
        }

        {
            ImGui::Begin("Reordering #2");
            ImGui::BulletText("Drag and drop to re-order");
            ImGui::Indent();
            static const char* names[] = {
                "1",
                "2",
                "3",
                "4",
                "5",
                "6"
            };
            int move_from = -1, move_to = -1;

            static char selectedNames[] = {
                0, 0, 0, 0, 0, 0
            };

            for (int n = 0; n < IM_ARRAYSIZE(names); n++)
            {
                std::string result;

                result = std::string(names[n]) + "##" + std::to_string(n);
                ImGui::Selectable(names[n]);
//                ImGui::PushID(n);
//                if ()
////                if (ImGui::Selectable(names[n], selectedNames[n] != 0, 0, ImVec2(50, 50)))
//                {
//                    selectedNames[n] ^= 1;
//                }
//                ImGui::PopID();

                result = "DND_DEMO_NAME##" + std::string(names[n]);
                ImGuiDragDropFlags src_flags = 0;
                src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
                src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
//                src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip;
                if (ImGui::BeginDragDropSource(src_flags))
                {
                    if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
                        ImGui::Text("Moving \"%s\"", names[n]);
                    ImGui::SetDragDropPayload(result.c_str(), &n, sizeof(int));
                    ImGui::EndDragDropSource();
                }

                if (ImGui::BeginDragDropTarget())
                {
                    ImGuiDragDropFlags target_flags = 0;
//                    target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
                    target_flags |= ImGuiDragDropFlags_AcceptPeekOnly;
                    target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(result.c_str(), target_flags))
                    {
                        move_from = *(const int*)payload->Data;
                        move_to = n;
                    }
                    ImGui::EndDragDropTarget();
                }
            }

            if (move_from != -1 && move_to != -1)
            {
                // reorder items
                int copy_dst = (move_from < move_to) ? move_from : move_to + 1;
                int copy_src = (move_from < move_to) ? move_from + 1 : move_to;
                int copy_count = (move_from < move_to) ? move_to - move_from : move_from - move_to;
                const char* tmp = names[move_from];
                printf("[%05d] move %d->%d (copy %d..%d to %d..%d)\n", ImGui::GetFrameCount(), move_from, move_to, copy_src, copy_src + copy_count - 1, copy_dst, copy_dst + copy_count - 1);
                memmove(&names[copy_dst], &names[copy_src], (size_t)copy_count * sizeof(const char*));
                names[move_to] = tmp;
                ImGui::SetDragDropPayload("DND_DEMO_NAME", &move_to, sizeof(int));
            }
            ImGui::Unindent();
            ImGui::End();
        }

        {
            ImGui::Begin("Grid");
            static char selected[4][4] = {
                { 1, 0, 0, 0 },
                { 0, 1, 0, 0 },
                { 0, 0, 1, 0 },
                { 0, 0, 0, 1 }
            };

            // Add in a bit of silly fun...
            const float time = (float)ImGui::GetTime();
            const bool winning_state = memchr(selected, 0, sizeof(selected)) == NULL; // If all cells are selected...
            if (winning_state)
                ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f + 0.5f * cosf(time * 2.0f), 0.5f + 0.5f * sinf(time * 3.0f)));

            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (x > 0)
                        ImGui::SameLine();
                    ImGui::PushID(y * 4 + x);
                    if (ImGui::Selectable("", selected[y][x] != 0, 0, ImVec2(50, 50)))
                    {
                        selected[y][x] ^= 1;
                        if (x > 0) { selected[y][x - 1] ^= 1; }
                        if (x < 3) { selected[y][x + 1] ^= 1; }
                        if (y > 0) { selected[y - 1][x] ^= 1; }
                        if (y < 3) { selected[y + 1][x] ^= 1; }
                    }
                    ImGui::PopID();
                }
            }
            if (winning_state)
                ImGui::PopStyleVar();

            ImGui::End();
        }
        
        if (show)
            ImGui::ShowDemoWindow(&show);
    }
}
