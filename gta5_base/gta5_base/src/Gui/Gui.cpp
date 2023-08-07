#include <Gui.h>

void Gui::render_menu() {
	if (m_open) {
		ImGui::Begin("Menu", nullptr); {
			ImGui::Text("Welcome :)"); 
		}
		ImGui::End(); 
	}
}