#pragma once
#include "stdafx.h"

#include "interface/fonts/font_awesome.hpp"
#include "interface/fonts/lato.hpp"
#include "interface/fonts/IconsFontAwesome5.h"
#include "Utilities/Singleton.h"


class GUI : public Singleton<GUI>
{
	friend class Singleton<GUI>;

protected:
	// GUI state
    bool _showFileDialog;					//!< Shows file dialog
	bool _showAboutUs;						//!< About us window
	bool _showControls;						//!< Shows application controls
	bool _showDeveloperTools;				//!< Shows developer tools
	bool _showSettings;			            //!< Displays application settings

protected:
	GUI();

	void initializeDockSpace();

	void createElements();

	void showAboutUsWindow();

	void showControls();

    void showFileDialog();

	void showDeveloperTools();

	void showSettings();

	void loadFonts();

	void loadImGUIStyle();

	static void leaveSpace(const unsigned numSlots);

	static void renderHelpMarker(const char* message);


public:
	virtual ~GUI();

	void initialize(GLFWwindow* window, const int openglMinorVersion);

	void render();

	bool isMouseActive() { return ImGui::GetIO().WantCaptureMouse; }
};