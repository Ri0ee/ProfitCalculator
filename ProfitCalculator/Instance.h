#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Gui.h"
#include "Scrapper.h"

class Instance {
public:
	Instance(int argc_, char** argv_) { Initialize(argc_, argv_); }

	int Run();

private:
	bool m_initialized = false;
	void Initialize(int argc_, char** argv_);

	std::vector<std::string> m_argv;

	std::shared_ptr<Gui> m_gui_ptr;
	std::shared_ptr<Scrapper> m_scrapper_ptr;
};