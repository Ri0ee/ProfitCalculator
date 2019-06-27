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
#include "FL/Fl_Progress.H"

#include "tinyxml2.h"

#include <map>
#include <string>
#include <filesystem>
#include <vector>
#include <thread>

#include "ItemData.h"
#include "Scrapper.h"

constexpr int THREAD_STATE_CALM = 1;
constexpr int THREAD_STATE_WORKING = 2;
constexpr int THREAD_STATE_FINISHED = 3;

class Gui {
private:
	class CurrencyListItem {
	public:
		CurrencyListItem(const std::string& name_, int x_, int y_, int w_, int h_, Gui* gui_) :
			m_name(name_), m_x(x_), m_y(y_), m_w(w_), m_h(h_), m_gui(gui_) { Initialize(); }

		double Value() {
			return m_input->value();
		}

		void Value(double val_, bool apply_) {
			if (apply_)
				m_input->value(val_);
			else
				m_value = val_;
		}

		void Apply() {
			m_input->value(m_value);
		}

		bool Changed() {
			return m_input->changed();
		}

	private:
		bool m_initialized = false;
		void Initialize();

		std::string m_name;
		int m_x, m_y, m_w, m_h;
		double m_value = 0;

		Gui* m_gui;
		Fl_Value_Input* m_input = nullptr;
	};

	class ProfitItem {
	public:
		ProfitItem(int x_, int y_, int w_, int h_) :
			m_x(x_), m_y(y_), m_w(w_), m_h(h_) { Initialize(); }

		void Calculate(double chaos_amount_, double sell_price_, double buy_price_, bool apply_) {
			if (apply_)
				m_input->value((chaos_amount_ * sell_price_ / buy_price_) - chaos_amount_);
			else
				m_value = (chaos_amount_ * sell_price_ / buy_price_) - chaos_amount_;
		}

		double Value() {
			return m_input->value();
		}

		void Value(double val_) {
			m_input->value(val_);
		}

		void Apply() {
			m_input->value(m_value);
		}

	private:
		bool m_initialized = false;
		void Initialize();

		int m_x, m_y, m_w, m_h;
		double m_value = 0;

		Fl_Value_Input* m_input = nullptr;
	};

	class AdditionalItemInfo {
	public:
		AdditionalItemInfo(int x_, int y_, int w_, int h_, Gui& gui_, const std::string& currency_type_) :
			m_x(x_), m_y(y_), m_w(w_), m_h(h_), m_gui(gui_), m_item_type(m_item_type) { Initialize(); }

	private:
		bool m_initialized = false;
		void Initialize();

		static void ButtonCallback(Fl_Widget* w, void* f) { ((Gui::AdditionalItemInfo*)f)->ButtonClick(w); }
		void ButtonClick(Fl_Widget* w);

		int m_x, m_y, m_w, m_h;
		std::string m_item_type;

		Gui& m_gui;
		Fl_Button* m_button = nullptr;
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
	void Settler();

	void ShowItemInfo(const std::string& item_type_);

	Fl_Double_Window* m_window = nullptr;
	Fl_Value_Input* m_chaos_input = nullptr;
	Fl_Value_Input* m_average_count_input = nullptr;
	Fl_Value_Input* m_list_shift_input = nullptr;
	Fl_Progress* m_refresher_progress = nullptr;

	Fl_Button* m_save_btn = nullptr;
	Fl_Button* m_load_btn = nullptr;
	Fl_Button* m_refresh_btn = nullptr;

	Fl_Box* m_sell_box = nullptr;
	Fl_Box* m_buy_box = nullptr;
	Fl_Box* m_profit_box = nullptr;

	std::map<std::string, std::shared_ptr<CurrencyListItem>> m_sell_items;
	std::map<std::string, std::shared_ptr<CurrencyListItem>> m_buy_items;
	std::map<std::string, std::shared_ptr<ProfitItem>> m_profit;

	std::thread* m_refresher_thread = nullptr;
	int m_refresher_thread_state = THREAD_STATE_CALM;
	int m_refresher_thread_progress = 0;
};