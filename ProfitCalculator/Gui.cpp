#include "Gui.h"

void Gui::Initialize() {
	//auto item_prices = m_scrapper_ptr->GetItemPrice(true, FUSING_ORB, CHAOS_ORB, 0, TEMP_SC).value();
	//fl_alert(item_prices[0].s_seller_acc_name.c_str());

	int x = 10, y = 10, w = 200, h = 20;

	int big_shift = x + 60;
	int small_shift = x + 50;

	Fl::get_system_colors();
	fl_font(FL_HELVETICA, 16);

	m_window = new Fl_Double_Window(0, 0, "Profitter");

	m_load_btn = new Fl_Button(small_shift + 200 + 80 + 10 - 220, y, 100, 20, "Load data");
	m_load_btn->callback(ButtonCallback, this);

	m_save_btn = new Fl_Button(small_shift + 200 + 80 + 10 - 110, y, 100, 20, "Save data");
	m_save_btn->callback(ButtonCallback, this);

	m_chaos_input = new Fl_Value_Input(small_shift, y, 50, h, "chaos:");
	m_chaos_input->callback(ChangeCallback, this);

	y += h * 2;

	m_average_count_input = new Fl_Value_Input(small_shift, y, 50, h, "avg. cnt:");
	m_average_count_input->value(1);

	m_refresh_btn = new Fl_Button(small_shift + 200 + 80 + 10 - 220, y, 100, 20, "Refresh");
	m_refresh_btn->callback(ButtonCallback, this);

	y += h * 2;

	m_list_shift_input = new Fl_Value_Input(small_shift, y, 50, h, "shift:");
	m_list_shift_input->value(5);

	y += h * 2;

	m_sell_box = new Fl_Box(small_shift, y, 80, 20, "Sell");
	m_buy_box = new Fl_Box(small_shift + 100, y, 80, 20, "Buy");
	m_profit_box = new Fl_Box(small_shift + 200, y, 80, 20, "Profit");

	y += h;

	int y1 = y;
	for (int i = 1; i <= 27; i++) {
		m_sell_items[GetCurrencyName(i, true)] = std::make_shared<CurrencyListItem>(GetCurrencyName(i, true), small_shift, y1, 80, h, this);
		y1 += h;
	}

	y1 = y;
	for (int i = 1; i <= 27; i++) {
		m_buy_items[GetCurrencyName(i, true)] = std::make_shared<CurrencyListItem>("", small_shift + 100, y1, 80, h, this);
		y1 += h;
	}

	y1 = y;
	for (int i = 1; i <= 27; i++) {
		m_profit[GetCurrencyName(i, true)] = std::make_shared<ProfitItem>(small_shift + 200, y1, 80, h);
		y1 += h;
	}

	y = y1;

	m_window->size(small_shift + 200 + 80 + 10, y + 10);
	m_window->end();
}

int Gui::Run() {
	m_window->show();

	while (Fl::wait() > 0) {

	}

	return 0;
}

void Gui::Check(Fl_Widget* w) {
	if (m_chaos_input->changed()) {
		for (auto& it : m_profit) {
			it.second->Calculate(m_chaos_input->value(), m_sell_items[it.first]->Value(), m_buy_items[it.first]->Value());
		}

		return;
	}

	for (auto& it : m_sell_items)
		if (it.second->Changed()) {
			m_profit[it.first]->Calculate(m_chaos_input->value(), it.second->Value(), m_buy_items[it.first]->Value());
			return;
		}

	for (auto& it : m_buy_items)
		if (it.second->Changed()) {
			m_profit[it.first]->Calculate(m_chaos_input->value(), m_sell_items[it.first]->Value(), it.second->Value());
			return;
		}
}

void Gui::ButtonClick(Fl_Widget* w) {
	std::string caption = w->label();
	if (caption == "Save data") {
		tinyxml2::XMLDocument xml_out;

		auto declaration_element = xml_out.NewDeclaration(nullptr);
		auto format_element = xml_out.NewComment("Currency data");

		xml_out.InsertFirstChild(declaration_element);
		xml_out.InsertEndChild(format_element);

		auto data_element = xml_out.NewElement("data");
		data_element->SetValue("data");
		data_element->SetAttribute("chaos", m_chaos_input->value());
		data_element->SetAttribute("avg.count", m_average_count_input->value());

		for (auto& it : m_sell_items)
		{
			auto currency_element = xml_out.NewElement("currency");
			currency_element->SetValue("currency");

			currency_element->SetAttribute("Name", it.first.c_str());
			currency_element->SetAttribute("Buy", m_buy_items[it.first]->Value());
			currency_element->SetAttribute("Sell", it.second->Value());
			currency_element->SetAttribute("Profit", m_profit[it.first]->Value());

			data_element->InsertEndChild(currency_element);
		}

		xml_out.InsertEndChild(data_element);

		std::filesystem::path root = m_argv[0];
		root = root.parent_path();

		xml_out.SaveFile((root.string() + "//data.xml").c_str());
		return;
	}

	if (caption == "Load data") {
		std::filesystem::path root = m_argv[0];
		root = root.parent_path();

		tinyxml2::XMLDocument xml_in;
		if (xml_in.LoadFile((root.string() + "//data.xml").c_str()) != tinyxml2::XML_SUCCESS)
			return;

		auto declaration_element = xml_in.FirstChild()->ToDeclaration();
		auto comment_element = declaration_element->NextSibling()->ToComment();

		auto data_element = comment_element->NextSibling()->ToElement();

		double val;
		data_element->QueryDoubleAttribute("chaos", &val);
		m_chaos_input->value(val);

		int int_val;
		data_element->QueryIntAttribute("avg.count", &int_val);
		m_average_count_input->value(int_val);

		const char* temp_str_buf;
		auto currency_element = data_element->FirstChild()->ToElement();
		for (unsigned i = 0; i < m_sell_items.size(); i++)
		{
			currency_element->QueryStringAttribute("Name", &temp_str_buf);
			std::string name(temp_str_buf);

			currency_element->QueryDoubleAttribute("Buy", &val);
			m_buy_items[name]->Value(val);

			currency_element->QueryDoubleAttribute("Sell", &val);
			m_sell_items[name]->Value(val);

			currency_element->QueryDoubleAttribute("Profit", &val);
			m_profit[name]->Value(val);

			if(i < m_sell_items.size() - 1)
				currency_element = currency_element->NextSibling()->ToElement();
		}
		return;
	}

	if (caption == "Refresh") {
		SettlePrices();
		return;
	}
}

void Gui::CurrencyListItem::Initialize() {
	m_input = new Fl_Value_Input(m_x, m_y, m_w, m_h, m_name.c_str());
	m_input->callback(m_gui_ptr->ChangeCallback, m_gui_ptr);
}

void Gui::ProfitItem::Initialize() {
	m_input = new Fl_Value_Input(m_x, m_y, m_w, m_h, "");
	m_input->set_output();
	m_input->precision(2);
}

void Gui::SettlePrices() {
	if (m_average_count_input->value() < 1)
		return;

	for (int i = 1; i <= 27; i++) {
		auto price_list_buy = m_scrapper_ptr->GetItemPrice(true, i, CHAOS_ORB, 0);
		auto price_list_sell = m_scrapper_ptr->GetItemPrice(true, CHAOS_ORB, i, 0);
		if (!price_list_buy.has_value() || !price_list_sell.has_value())
			continue;

		double average_sell = 0, average_buy = 0;
		for (int i = min((int)m_list_shift_input->value(), min(price_list_sell.value().size(), price_list_buy.value().size()));
				i < (int)m_average_count_input->value() && 
				i < price_list_buy.value().size() && 
				i < price_list_sell.value().size(); i++) {

			average_sell += price_list_buy.value()[i].s_sell_price / price_list_buy.value()[i].s_buy_price;
			average_buy += price_list_sell.value()[1].s_buy_price / price_list_sell.value()[1].s_sell_price;
		}
		average_sell /= min((int)m_average_count_input->value(), price_list_sell.value().size());
		average_buy /= min((int)m_average_count_input->value(), price_list_buy.value().size());

		m_sell_items[GetCurrencyName(i, true)]->Value(average_sell);
		m_buy_items[GetCurrencyName(i, true)]->Value(average_buy);
		m_profit[GetCurrencyName(i, true)]->Calculate(m_chaos_input->value(), average_buy, average_sell);
	}
}