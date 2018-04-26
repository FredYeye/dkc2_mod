#include "imgui/imgui.h"
#include "imgui/examples/opengl3_example/imgui_impl_glfw_gl3.h"
#include "gl_core/gl_core_3_3.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <vector>
#include <cstring>

#include "main.hpp"


int main(void)
{
	if(!glfwInit())
	{
		exit(1);
	}

	GLFWwindow* window = glfwCreateWindow(960, 680, "wah", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);

	if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		std::cout << "blopp";
		glfwTerminate();
		exit(1);
	}

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    int32_t frameTime = 0;
    std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();

	while(!glfwWindowShouldClose(window))
	{
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        frameTime += std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count();
        lastTime = currentTime;

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(2ms); //sleep some because we're running fast

        while(frameTime >= 25000)
        {
			frameTime -= 25000;

			if(ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_Escape])) 
			{
				glfwSetWindowShouldClose(window, GL_TRUE);
			}

			ImGui_ImplGlfwGL3_NewFrame();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3); ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);

			{
				ImGui::SetNextWindowPos(ImVec2(10, 10));
				ImGui::SetNextWindowSize(ImVec2(540,130));
				ImGui::Begin(" ", &showMain, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.75f);
				const bool enterLoad = ImGui::InputText("", dkc2.romName.data(), 80, ImGuiInputTextFlags_EnterReturnsTrue);
				ImGui::PopItemWidth();

				ImGui::SameLine();
				ImGui::Text(dkc2.romStatus.data());

				ImGui::PushItemWidth(80);
				if(ImGui::BeginCombo("Version", versionNames[version].data()))
				{
					for(int n = 0; n < versionNames.size(); ++n)
					{
						bool is_selected;
						if(ImGui::Selectable(versionNames[n].data()))
						{
							version = n;
							dkc2.SetVersion(Version(n));
						}
					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();

				if(ImGui::Button("Load rom") || enterLoad)
				{
					if(dkc2.OpenRom())
					{
						for(int x = 0; x < 3; ++x)
						{
							WordToCharArray(DKCoinThresholds[x], dkc2.DKCoinThresholds[x], dec);
						}
						WordToCharArray(posX, dkc2.world[currentWorld][currentLevel].posX[entry], hex);
						WordToCharArray(posY, dkc2.world[currentWorld][currentLevel].posY[entry], hex);
						ByteToCharArray(flags, dkc2.world[currentWorld][currentLevel].flags[entry], hex);
						WordToCharArray(waterTarget, dkc2.world[currentWorld][currentLevel].waterLevelTarget[entry], hex);
					}
				}

				if(dkc2.rom.size())
				{
					ImGui::SameLine();
					if(ImGui::Button("Save changes"))
					{
						dkc2.Save();
					}
					showValues = true;
					showCode = true;
				}

				// ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
				ImGui::End();
			}

			if(showValues)
			{
				WindowValues();
			}

			if(showCode)
			{
				ImGui::SetNextWindowPos(ImVec2(320, 150));
				ImGui::SetNextWindowSize(ImVec2(300,100));
				ImGui::Begin("Code options", &showCode, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

				ImGui::Checkbox("Always good eggs (Krow's Nest)", &dkc2.goodEggs);

				ImGui::End();
			}

			ImGui::PopStyleVar(); ImGui::PopStyleVar();

			if(show_demo_window)
			{
				ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
				// ImGui::ShowDemoWindow(&show_demo_window);
			}

			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
        	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
			glfwPollEvents();
        }
	}

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void WindowValues()
{
	ImGui::SetNextWindowPos(ImVec2(10, 150));
	ImGui::SetNextWindowSize(ImVec2(300,410));
	ImGui::Begin("Values", &showValues, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGui::Text("DK coin thresholds");

	ImGui::PushItemWidth(42);
	for(int x = 0; x < 3; ++x)
	{
		if(ImGui::InputText(dkc2.DKCoinNames[x].data(), DKCoinThresholds[x].data(), 5, ImGuiInputTextFlags_CharsDecimal))
		{
			dkc2.DKCoinThresholds[x] = CharHexWordToUint16(DKCoinThresholds[x]);
		}
	}
	ImGui::PopItemWidth();

	ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

	ImGui::PushItemWidth(190);
	if(ImGui::BeginCombo("Level", dkc2.world[currentWorld][currentLevel].name.data(), ImGuiComboFlags_HeightLarge))
	{
		for(int x = 0; x < dkc2.world.size(); ++x)
		{
			if(ImGui::BeginMenu(worldNames[x].data()))
			{
				for(int y = 0; y < dkc2.world[x].size(); ++y)
				{
					if(ImGui::MenuItem(dkc2.world[x][y].name.data()))
					{
						currentLevel = y;
						currentWorld = x;
						WordToCharArray(posX, dkc2.world[x][y].posX[entry], hex);
						WordToCharArray(posY, dkc2.world[x][y].posY[entry], hex);
						ByteToCharArray(flags, dkc2.world[x][y].flags[entry], hex);
						WordToCharArray(waterTarget, dkc2.world[x][y].waterLevelTarget[entry], hex);
					}
				}
				ImGui::EndMenu();
			}
		}

		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(100);
	if(ImGui::SliderInt("Entrypoint", &entry, 0, 5))
	{
		WordToCharArray(posX, dkc2.world[currentWorld][currentLevel].posX[entry], hex);
		WordToCharArray(posY, dkc2.world[currentWorld][currentLevel].posY[entry], hex);
		ByteToCharArray(flags, dkc2.world[currentWorld][currentLevel].flags[entry], hex);
		WordToCharArray(waterTarget, dkc2.world[currentWorld][currentLevel].waterLevelTarget[entry], hex);
	}
	ImGui::PopItemWidth();
	ShowHelpMarker("0: stage start\n1: midway barrel\n2+: return points from rooms and bonuses\n\nCurrently, this program doesn't know how many entry points a stage has, the slider is simply hardcoded to 0-5.\nBe careful or you might edit unintended values in ROM!");

	ImGui::NewLine();

	ImGui::Text("Level ID: %02X", dkc2.world[currentWorld][currentLevel].ID);

	ImGui::Text("Connected levels:");
	const std::vector<uint8_t> asd = dkc2.GetConnectedLevels(dkc2.world[currentWorld][currentLevel].ID);
	for(const auto &v : asd)
	{
		ImGui::SameLine();
		ImGui::Text("%02X", v);
	}

	ImGui::NewLine();

	ImGui::PushItemWidth(42);
	if(ImGui::InputText("X Starting position (hex)", posX.data(), 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
	{
		dkc2.world[currentWorld][currentLevel].posX[entry] = CharHexWordToUint16(posX);
	}

	if(ImGui::InputText("Y Starting position (hex)", posY.data(), 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
	{
		dkc2.world[currentWorld][currentLevel].posY[entry] = CharHexWordToUint16(posY);
	}

	ImGui::NewLine();

	if(ImGui::InputText("flags (hex)", flags.data(), flags.size(), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
	{
		dkc2.world[currentWorld][currentLevel].flags[entry] = CharHexByteToUint8(flags);
	}

	ImGui::NewLine();

	if(ImGui::InputText("Water level target (hex)", waterTarget.data(), 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
	{
		dkc2.world[currentWorld][currentLevel].waterLevelTarget[entry] = CharHexWordToUint16(waterTarget);
	}
	ImGui::PopItemWidth();
	ShowHelpMarker("0x8000 means no water in this stage and changing it does nothing currently");

	ImGui::End();
}


uint16_t CharHexWordToUint16(std::array<char, 5> charArray)
{
	char* p = charArray.data() + charArray.size();
	return strtoul(charArray.data(), &p, 16);
}


uint8_t CharHexByteToUint8(std::array<char, 3> charArray)
{
	char* p = charArray.data() + charArray.size();
	return strtoul(charArray.data(), &p, 16);
}


void WordToCharArray(std::array<char, 5> &charArray, uint16_t value, Base base)
{
	std::stringstream stream;

	if(base == dec)
	{
		stream << value;
	}
	else
	{
		stream << std::hex << std::setfill('0') << std::setw(4) << std::uppercase << value;
	}

	const std::string result(stream.str());
	std::strcpy(charArray.data(), result.data());
}


void ByteToCharArray(std::array<char, 3> &charArray, uint8_t value, Base base)
{
	std::stringstream stream;

	if(base == dec)
	{
		stream << value;
	}
	else
	{
		stream << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << +value;
	}

	const std::string result(stream.str());
	std::strcpy(charArray.data(), result.data());
}


void ShowHelpMarker(const char* desc)
{
	ImGui::SameLine();

	ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
