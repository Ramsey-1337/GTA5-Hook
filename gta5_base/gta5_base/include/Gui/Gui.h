#pragma once
#include <Includes.h>

class Gui {
public:
	void render_menu(); 
	bool m_open = false; 
};

inline Gui* m_gui = new Gui; 