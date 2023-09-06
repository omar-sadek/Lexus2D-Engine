#ifndef GUIRENDERSYSTEM_H
#define GUIRENDERSYSTEM_H

#include "../ECS/ECS.h"
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>

class GUIRenderSystem : public System {
public:
	GUIRenderSystem() = default;

	void Draw(const std::shared_ptr<EntityManager>& i_entityManager) {
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		if (ImGui::Begin("Enemy Spawner")) {
			static int xPos = 0;
			static int yPos = 0;
			static int xScale = 1;
			static int yScale = 1;
			static float rotation = 0.0;
			static float xVelocity = 0;
			static float yVelocity = 0;
			static int frameCount = 4;
			static float frameTime = 0.1;
			static double projectileSpeed = 100;
			static float projectileXDirection = 0;
			static float projectileYDirection = 0;
			static float projectileFrequency = 0.5;
			static float projectileDuration = 5.0;
			static int projectileDamage = 10;
			const char* sprites[] = { "pink-slime", "purple-slime" };
			static int selectedSpriteIndex = 0;

			if (ImGui::CollapsingHeader("Sprite")) {
				ImGui::Combo("Sprite ID", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
			}

			if (ImGui::CollapsingHeader("Animation")) {
				ImGui::InputInt("Number of frames", &frameCount);
				ImGui::InputFloat("Frame Duration", &frameTime);
			}

			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Transform")) {
				ImGui::InputInt("X Position", &xPos);
				ImGui::InputInt("Y Position", &yPos);
				ImGui::SliderInt("X Scale", &xScale, 1,10);
				ImGui::SliderInt("Y Scale", &yScale, 1, 10);
				ImGui::SliderAngle("Rotation (deg)", &rotation, 0 , 360);
			}

			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Rigidbody")) {
				ImGui::InputFloat("X Velocity", &xVelocity);
				ImGui::InputFloat("Y Velocity", &yVelocity);
			}

			ImGui::Spacing();

			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Projectile Settings")) {
				ImGui::InputDouble("Projectile Speed", &projectileSpeed);
				ImGui::InputFloat("Projectile X Direction", &projectileXDirection);
				ImGui::InputFloat("Projectile Y Direction", &projectileYDirection);
				ImGui::SliderFloat("Projectile Frequency", &projectileFrequency, 0.1, 1000.0);
				ImGui::SliderFloat("Projectile Duration", &projectileDuration, 0.1, 1000.0);
				ImGui::InputInt("Projectile Damage", &projectileDamage);
			}

			ImGui::Spacing();

			if (ImGui::Button("Create Enemy")) {
				Entity enemy = i_entityManager->CreateEntity();
				enemy.SetGroup(Group::ENEMIES_G);
				enemy.AddComponent<TransformComponent>(glm::vec2(xPos, yPos), glm::vec2(xScale, yScale), glm::degrees(rotation));
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(xVelocity, yVelocity));
				enemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], 16, 16, RenderLayer::ENEMY_L);
				enemy.AddComponent<FrameSwapperComponent>(frameCount, frameTime, true, true);
				enemy.AddComponent<BoxColliderComponent>(16, 16, glm::vec2(0));
				enemy.AddComponent<ProjectileSpawnerComponent>(projectileSpeed,
					glm::vec2(projectileXDirection, projectileYDirection),
					projectileFrequency * 1000,
					projectileDuration * 1000,
					projectileDamage);
			}
		}
		ImGui::End();

		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
	}
};

#endif