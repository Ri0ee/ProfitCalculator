#include "Instance.h"

void Instance::Initialize(int argc_, char** argv_) {
	for (int i = 0; i < argc_; i++)
		m_argv.push_back(argv_[i]);

	m_scrapper_ptr = std::make_shared<Scrapper>("Legion", "http://currency.poe.trade/");
	m_gui_ptr = std::make_shared<Gui>(m_argv, m_scrapper_ptr);
}

int Instance::Run() {
	return m_gui_ptr->Run();
}