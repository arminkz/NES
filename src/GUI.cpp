#include "GUI.h"

#include <nfd.h>

#include "Renderer.h"
#include "NES.h"
#include "cpu6502.h"

/// [Protected methods]

GUI::GUI() :
	_showFileDialog(false),
	_showSettings(false), 
	_showAboutUs(false),
	_showControls(false), 
	_showDeveloperTools(true),
	_showLoadAssembly(false),

	_dev_disassembled(true),
	_dev_patternTables(true),
	_dev_oam(true)
{
}

void GUI::createElements()
{
	ImGuiIO& io = ImGui::GetIO();

	showGameWindow();
	if (_showFileDialog)		showFileDialog();
	if (_showSettings)          showSettings();
	if (_showAboutUs)		    showAboutUsWindow();
	if (_showControls)		    showControls();
	if (_showDeveloperTools)    showDeveloperTools();
	if (_showLoadAssembly) 	    showLoadAssembly();

	if(_dev_disassembled)		showDisassembledCode();
	if(_dev_patternTables)		showPatternTables();
	if(_dev_oam)				showOAMViewer();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(ICON_FA_FILE "File"))
		{
			ImGui::MenuItem(ICON_FA_GAMEPAD "Open...", NULL, &_showFileDialog);
			ImGui::MenuItem(ICON_FA_COG "Settings", NULL, &_showSettings);
			ImGui::EndMenu();
		}

        if (ImGui::BeginMenu(ICON_FA_BINOCULARS "View"))
		{
			ImGui::MenuItem(ICON_FA_MICROCHIP "CPU/RAM Inspector", NULL, &_showDeveloperTools);
			//ImGui::MenuItem(ICON_FA_CODE "Load Assembly", NULL, &_showLoadAssembly);
			ImGui::MenuItem(ICON_FA_LAPTOP_CODE "Disassembled Code", NULL, &_dev_disassembled);
			ImGui::MenuItem(ICON_FA_TABLE "Pattern Tables", NULL, &_dev_patternTables);
			ImGui::MenuItem(ICON_FA_GHOST "Object Attribute Memory (OAM)", NULL, &_dev_oam);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(ICON_FA_QUESTION_CIRCLE "Help"))
		{
			ImGui::MenuItem(ICON_FA_INFO "About the project", NULL, &_showAboutUs);
			ImGui::MenuItem(ICON_FA_GAMEPAD "Controls", NULL, &_showControls);
			ImGui::EndMenu();
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX(io.DisplaySize.x - 125);
		this->renderHelpMarker("Help text here");

		ImGui::SameLine(0, 20);
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::EndMainMenuBar();
	}
}

void GUI::leaveSpace(const unsigned numSlots)
{
	for (int i = 0; i < numSlots; ++i)
	{
		ImGui::Spacing();
	}
}

void GUI::renderHelpMarker(const char* message)
{
	ImGui::TextDisabled(ICON_FA_QUESTION);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(message);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void GUI::showGameWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	if (ImGui::Begin("Game", NULL, window_flags))
	{
		const float window_width = ImGui::GetContentRegionAvail().x;
		const float window_height = ImGui::GetContentRegionAvail().y;

		Renderer::getInstance()->render_game(window_width, window_height);
	} 
	ImGui::End();
	ImGui::PopStyleVar(2);
}

void GUI::showPatternTables()
{
	if(ImGui::Begin("Pattern Tables", &_dev_patternTables))
	{
		//Show active palette
		ImGui::Text("Active Palette: %02X", NES::getInstance()->dev_active_palette);
		
		const float window_width = ImGui::GetContentRegionAvail().x;
		const float window_height = ImGui::GetContentRegionAvail().y;

		Renderer::getInstance()->render_pattern_tables(window_width, window_height);
	}
	ImGui::End();
}

void GUI::showOAMViewer()
{
	if(ImGui::Begin("OAM Viewer", &_dev_oam))
	{
		//Print OAM memory from PPU
		for (int i = 0; i < 64; i++)
		{
			//Print OAM entries
			ImGui::Text("[%02d] : (%02d, %02d)    ID: %02X    AT: %02X", i, 
			NES::getInstance()->ppu.pOAM[i * 4 + 3], 
			NES::getInstance()->ppu.pOAM[i * 4 + 0],
			NES::getInstance()->ppu.pOAM[i * 4 + 1],
			NES::getInstance()->ppu.pOAM[i * 4 + 2]);
		}
	}
	ImGui::End();
}

void GUI::showAboutUsWindow()
{
	if (ImGui::Begin("About the project", &_showAboutUs))
	{
		ImGui::Text("Created by: Armin Kazemi    github.com/arminkz");
	}
	ImGui::End();
}

void GUI::showControls()
{
	if (ImGui::Begin("Scene controls", &_showControls))
	{
		ImGui::Columns(2, "ControlColumns"); // 4-ways, with border
		ImGui::Separator();
		ImGui::Text("Movement"); ImGui::NextColumn();
		ImGui::Text("Control"); ImGui::NextColumn();
		ImGui::Separator();

		const int NUM_MOVEMENTS = 11;
		const char* movement[] = { "Orbit (XZ)", "Undo Orbit (XZ)", "Orbit (Y)", "Undo Orbit (Y)", "Z Movement", "XY Movement", "Reset Camera", "Take Screenshot", "Zoom +/-",     "Pan",                                  "Tilt" };
		const char* controls[] = { "X",          "Ctrl + X",        "Y",         "Ctrl + Y",       "W, S",       "Arrow Keys",  "R",            "K",               "Scroll wheel", "Drag mouse horizontally", "Drag mouse vertically" };

		for (int i = 0; i < NUM_MOVEMENTS; i++)
		{
			ImGui::Text(movement[i]); ImGui::NextColumn();
			ImGui::Text(controls[i]); ImGui::NextColumn();
		}

		ImGui::Columns(1);
		ImGui::Separator();

	}
	ImGui::End();
}

void GUI::showFileDialog()
{
	spdlog::info("Showing file dialog...");

	NFD_Init();

    nfdu8char_t *outPath;
    nfdu8filteritem_t filters[1] = { { "NES files", "nes" } };
    nfdopendialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;
    nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
    if (result == NFD_OKAY)
    {
		spdlog::info("Loading NES file : {}", outPath);
		NES::getInstance()->insertCartridge(std::make_shared<Cartridge>(outPath));
		NES::getInstance()->mapAsm = NES::getInstance()->cpu.disassemble(0x0000, 0xFFFF);
		NES::getInstance()->reset();
		NES::getInstance()->emulationRun = true;

        NFD_FreePathU8(outPath);
    }
    else if (result == NFD_CANCEL)
    {
		spdlog::info("User pressed cancel.");
    }
    else 
    {
		spdlog::error("Error: {}", NFD_GetError());
    }

    NFD_Quit();
	_showFileDialog = false;
}

void GUI::showDeveloperTools()
{
	if (ImGui::Begin("Developer Tools", &_showDeveloperTools))
	{
		ImGui::Text("CPU: ");
		ImGui::Text("A: %02X   X: %02X   Y: %02X   PC: %04X   SP: %02X  CYC: %3i", 
		NES::getInstance()->cpu.a, 
		NES::getInstance()->cpu.x, 
		NES::getInstance()->cpu.y, 
		NES::getInstance()->cpu.pc, 
		NES::getInstance()->cpu.stkp,
		NES::getInstance()->cpu.cycles);

		ImGui::Text("Status: ");

		auto print_status_bit = [](CPU6502::FLAGS6502 flag, const char* label) 
    	{ 
			uint32_t green = IM_COL32(0,255,0,255);
			uint32_t red = IM_COL32(255,0,0,255);
			uint32_t color = NES::getInstance()->cpu.status & flag ? green : red;
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::SameLine();
			ImGui::Text("%s", label);
			ImGui::PopStyleColor();
    	};

		print_status_bit(CPU6502::FLAGS6502::C, "C");
		print_status_bit(CPU6502::FLAGS6502::Z, "Z");
		print_status_bit(CPU6502::FLAGS6502::I, "I");
		print_status_bit(CPU6502::FLAGS6502::D, "D");
		print_status_bit(CPU6502::FLAGS6502::B, "B");
		print_status_bit(CPU6502::FLAGS6502::U, "U");
		print_status_bit(CPU6502::FLAGS6502::V, "V");
		print_status_bit(CPU6502::FLAGS6502::N, "N");

		ImGui::NewLine();
		ImGui::Text("PPU: ");
		// cycle, scanline, frame
		ImGui::Text("Cycle: %3i, Scanline: %3i, Frame Complete: %i", NES::getInstance()->ppu.cycle, NES::getInstance()->ppu.scanline, NES::getInstance()->ppu.frameComplete);

		ImGui::NewLine();
		ImGui::Text("RAM:");

		for (int i = 0; i < 2048; i++) {
			if (i % 16 == 0) {
				//ImGui::NewLine();
				ImGui::Text("%04X: ", i);
			}
			ImGui::SameLine();
			ImGui::Text("%02X ", NES::getInstance()->cpuRam[i]);
		}		
	}
	ImGui::End();
}

void GUI::showDisassembledCode()
{
	// Show Disassembled Code near CPU's PC
	const int no_of_lines = 20;

	if (ImGui::Begin("Disassembled Code", &_dev_disassembled))
	{
		ImGui::Text("Disassembly");
		ImGui::Separator();

		if(NES::getInstance()->mapAsm.empty())
		{
			ImGui::Text("No disassembled code available.");
		}

		// Get iterator to current line of code
		auto it = NES::getInstance()->mapAsm.find(NES::getInstance()->cpu.pc);

		// Decrease iterator to show previous lines of code
		if (it != NES::getInstance()->mapAsm.end())
		{
			for (int i = 0; i < no_of_lines; i++)
			{
				if (it == NES::getInstance()->mapAsm.begin())
				{
					break;
				}
				it--;
			}
		}

		// Show disassembled code
		
		for (int i = 0; i < no_of_lines * 2; i++)
		{
			if (it == NES::getInstance()->mapAsm.end())
			{
				break;
			}

			// Highlight current line of code with a different color and different background
			if (it->first == NES::getInstance()->cpu.pc)
			{
				//ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
				//ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, IM_COL32(0, 0, 255, 255));
			}
			ImGui::Text("$%04X: %s", it->first, it->second.c_str());
			if (it->first == NES::getInstance()->cpu.pc)
			{
				//ImGui::PopStyleColor();
				//ImGui::PopStyleColor();
			}
			it++;
		}
	}
	ImGui::End();
}

void GUI::showLoadAssembly()
{
	if (ImGui::Begin("Load Assembly", &_showLoadAssembly))
	{
		ImGui::Text("Load Assembly");
		ImGui::Separator();

		ImGui::Text("Enter Assembled Code: ");
		ImGui::InputTextMultiline("##AssemblyFile", _assemblyString, IM_ARRAYSIZE(_assemblyString),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8),
                                  ImGuiInputTextFlags_None);

		if (ImGui::Button("Load"))
		{
			// Load assembly file
			spdlog::info("Loading assembly...");
			spdlog::info("Assembly Hex: {}", _assemblyString);

			// std::stringstream ss;
			// ss << _assemblyString;

			// uint16_t nOffset = 0x8000;
			// while (!ss.eof())
			// {
			// 	std::string b;
			// 	ss >> b;
			// 	NES::getInstance()->cpuRam[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);;
			// }

			// // Set Reset Vector
			// NES::getInstance()->cpuRam[0xFFFC] = 0x00;
			// NES::getInstance()->cpram[0xFFFD] = 0x80;

			// NES::getInstance()->cpu.reset();
			// spdlog::info("Assembly Hex loaded.");
		}
	}
	ImGui::End();
}

void GUI::showSettings()
{
	// RenderingParameters* _renderingParams = Renderer::getInstance()->getRenderingParameters();
	// if (ImGui::Begin("Rendering Settings", &_showRenderingSettings))
	// {
	// 	//ImGui::ColorEdit3("Background color", &_renderingParams->_backgroundColor[0]);

	// 	this->leaveSpace(3);
	// 	ImGui::Separator();
	// 	ImGui::Text(ICON_FA_TREE "Point Cloud");

	// 	this->leaveSpace(2);
	// 	const char* visualizationTitles[] = { "Depth", "Colors", "Normals", "Full" };
	// 	ImGui::NewLine();
	// 	ImGui::SameLine(30, 0);
	// 	ImGui::PushItemWidth(200.0f);
	// 	ImGui::Combo("Visualization", &_renderingParams->_displayMode, visualizationTitles, IM_ARRAYSIZE(visualizationTitles));

	// 	this->leaveSpace(1);
	// 	ImGui::NewLine();
	// 	ImGui::SameLine(30, 0);
	// 	ImGui::PushItemWidth(200.0f);
	// 	ImGui::SliderFloat("Point Size", &_renderingParams->_pointSize, 0.01f, 0.5f);


	// 	this->leaveSpace(3);
	// 	ImGui::Separator();
	// 	ImGui::Text(ICON_FA_LIGHTBULB "Lighting");

	// 	this->leaveSpace(2);
	// 	ImGui::NewLine();
	// 	ImGui::SameLine(30, 0);
	// 	ImGui::PushItemWidth(200.0f);
	// 	ImGui::ColorEdit3("Light Color", &_renderingParams->_lightColor[0]);

	// 	this->leaveSpace(1);
	// 	ImGui::NewLine();
	// 	ImGui::SameLine(30, 0);
	// 	ImGui::PushItemWidth(200.0f);
	// 	ImGui::SliderFloat("Ambient Strength", &_renderingParams->_ambient_strength, 0.01f, 1.0f);

	// 	this->leaveSpace(1);
	// 	ImGui::NewLine();
	// 	ImGui::SameLine(30, 0);
	// 	ImGui::PushItemWidth(200.0f);
	// 	ImGui::SliderFloat("Specular Strength", &_renderingParams->_specular_strength, 0.01f, 1.0f);


	// 	this->leaveSpace(3);
	// 	ImGui::Separator();
	// 	ImGui::Text(ICON_FA_CUBE "Level of Detail");

	// 	this->leaveSpace(2);
	// 	ImGui::NewLine();
	// 	ImGui::SameLine(30, 0);
	// 	ImGui::PushItemWidth(200.0f);
	// 	ImGui::Checkbox("Enable LoD and CPU Frustum Culling", &_renderingParams->_cpuCulling);
	// 	this->leaveSpace(1);
	// 	ImGui::NewLine();
	// 	ImGui::SameLine(30, 0);
	// 	ImGui::PushItemWidth(200.0f);
	// 	ImGui::Text("Press B to recalculate.");
	// 	//ImGui::Checkbox("Recalculate Every Frame", &_renderingParams->_cpuCullingEveryFrame);
	// }
	// ImGui::End();
}

GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::initializeDockSpace() {

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", nullptr, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	// MenuBarSignal.Emit();

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("VulkanAppDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
}

/// [Public methods]

void GUI::initialize(GLFWwindow* window, const int openglMinorVersion)
{
	const std::string openGLVersion = "#version 4" + std::to_string(openglMinorVersion) + "0 core";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	this->loadImGUIStyle();
	this->loadFonts();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(openGLVersion.c_str());
}

void GUI::render()
{
	bool show_demo_window = true;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	this->initializeDockSpace();
	this->createElements();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::loadImGUIStyle()
{
	ImGui::StyleColorsDark();
}

void GUI::loadFonts()
{
	ImFontConfig cfg;
	ImGuiIO& io = ImGui::GetIO();

	std::copy_n("Lato", 5, cfg.Name);
	io.Fonts->AddFontFromMemoryCompressedBase85TTF(lato_compressed_data_base85, 15.0f, &cfg);

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	cfg.MergeMode = true;
	cfg.PixelSnapH = true;
	cfg.GlyphMinAdvanceX = 20.0f;
	cfg.GlyphMaxAdvanceX = 20.0f;
	std::copy_n("FontAwesome", 12, cfg.Name);

	io.Fonts->AddFontFromFileTTF("fonts/fa-regular-400.ttf", 13.0f, &cfg, icons_ranges);
	io.Fonts->AddFontFromFileTTF("fonts/fa-solid-900.ttf", 13.0f, &cfg, icons_ranges);
}