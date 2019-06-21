#pragma once

#define WIN32
#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Button.H"
#include "FL/fl_draw.H"
#include "FL/fl_ask.H"
#include "FL/Fl_Value_Input.H"
#include "FL/Fl_Group.H"
#include "FL/Fl_Box.H"

#include "tinyxml2.h"

#include <map>
#include <string>
#include <filesystem>
#include <vector>

#include "item_data.h"
#include "Scrapper.h"

class Gui {
private:
	class CurrencyListItem {
	public:
		CurrencyListItem(const std::string& name_, int x_, int y_, int w_, int h_, Gui* gui_ptr_) :
			m_name(name_), m_x(x_), m_y(y_), m_w(w_), m_h(h_), m_gui_ptr(gui_ptr_) { Initialize(); }

		double Value() {
			return m_input->value();
		}

		void Value(double val_) {
			m_input->value(val_);
		}

		bool Changed() {
			return m_input->changed();
		}

	private:
		bool m_initialized = false;
		void Initialize();

		std::string m_name;
		int m_x, m_y, m_w, m_h;
		Gui* m_gui_ptr;

		Fl_Value_Input* m_input = nullptr;
	};

	class ProfitItem {
	public:
		ProfitItem(int x_, int y_, int w_, int h_) :
			m_x(x_), m_y(y_), m_w(w_), m_h(h_) { Initialize(); }

		void Calculate(double chaos_amount_, double sell_price_, double buy_price_) {
			m_input->value((chaos_amount_ * sell_price_ / buy_price_) - chaos_amount_);
		}

		double Value() {
			return m_input->value();
		}

		void Value(double val_) {
			m_input->value(val_);
		}

	private:
		bool m_initialized = false;
		void Initialize();

		int m_x, m_y, m_w, m_h;

		Fl_Value_Input* m_input = nullptr;
	};

public:
	Gui(const std::vector<std::string>& argv_, std::shared_ptr<Scrapper> scrapper_ptr_) :
		m_argv(argv_), m_scrapper_ptr(scrapper_ptr_) { Initialize(); }

	int Run();

private:
	bool m_initialized = false;
	void Initialize();

	std::vector<std::string> m_argv;

	std::shared_ptr<Scrapper> m_scrapper_ptr;

	static void ChangeCallback(Fl_Widget* w, void* f) { ((Gui*)f)->Check(w); }
	void Check(Fl_Widget* w);

	static void ButtonCallback(Fl_Widget* w, void* f) { ((Gui*)f)->ButtonClick(w); }
	void ButtonClick(Fl_Widget* w);

	void SettlePrices();

	Fl_Double_Window* m_window = nullptr;
	Fl_Value_Input* m_chaos_input = nullptr;
	Fl_Value_Input* m_average_count_input = nullptr;
	Fl_Value_Input* m_list_shift_input = nullptr;

	Fl_Button* m_save_btn = nullptr;
	Fl_Button* m_load_btn = nullptr;
	Fl_Button* m_refresh_btn = nullptr;

	Fl_Box* m_sell_box = nullptr;
	Fl_Box* m_buy_box = nullptr;
	Fl_Box* m_profit_box = nullptr;

	std::map<std::string, std::shared_ptr<CurrencyListItem>> m_sell_items;
	std::map<std::string, std::shared_ptr<CurrencyListItem>> m_buy_items;
	std::map<std::string, std::shared_ptr<ProfitItem>> m_profit;
};