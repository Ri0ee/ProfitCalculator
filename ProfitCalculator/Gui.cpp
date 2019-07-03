#include "Gui.h"

void Gui::Initialize() {
	int x = 10, y = 10, w = 200, h = 20;

	int shift = x + 70;

	Fl::get_system_colors();
	fl_font(FL_HELVETICA, 16);

	m_window = new Fl_Double_Window(0, 0, "Profitter");

	m_load_btn = new Fl_Button(shift + 200 + 80 + 10 - 220, y, 100, 20, "Load data");
	m_load_btn->callback(ButtonCallback, this);

	m_save_btn = new Fl_Button(shift + 200 + 80 + 10 - 110, y, 100, 20, "Save data");
	m_save_btn->callback(ButtonCallback, this);

	m_chaos_input = new Fl_Value_Input(shift, y, 50, h, "chaos:");
	m_chaos_input->callback(ChangeCallback, this);

	y += h * 2;

	m_average_count_input = new Fl_Value_Input(shift, y, 50, h, "avg. cnt:");
	m_average_count_input->value(1);

	m_refresh_btn = new Fl_Button(shift + 200 + 80 + 10 - 220, y, 100, 20, "Refresh");
	m_refresh_btn->callback(ButtonCallback, this);

	m_refresher_progress = new Fl_Progress(shift + 200 + 80 + 10 - 110, y, 100, 20, "Progress");

	y += h * 2;

	m_list_shift_input = new Fl_Value_Input(shift, y, 50, h, "shift:");
	m_list_shift_input->value(5);

	y += h * 2;

	m_sell_box = new Fl_Box(shift, y, 80, 20, "Sell");
	m_buy_box = new Fl_Box(shift + 100, y, 80, 20, "Buy");
	m_profit_box = new Fl_Box(shift + 200, y, 80, 20, "Profit");

	y += h;

	int y1 = y;
	for (int i = 1; i <= 27; i++) {
		m_sell_items[GetCurrencyName(i, true)] = std::make_shared<CurrencyListItem>(GetCurrencyName(i, true), shift, y1, 80, h, this);
		y1 += h;
	}

	y1 = y;
	for (int i = 1; i <= 27; i++) {
		m_buy_items[GetCurrencyName(i, true)] = std::make_shared<CurrencyListItem>("", shift + 100, y1, 80, h, this);
		y1 += h;
	}

	y1 = y;
	for (int i = 1; i <= 27; i++) {
		m_profit[GetCurrencyName(i, true)] = std::make_shared<ProfitItem>(shift + 200, y1, 80, h);
		y1 += h;
	}

	y1 = y;
	for (int i = 1; i <= 27; i++) {
		m_info[GetCurrencyName(i, true)] = std::make_shared<AdditionalItemInfo>(shift + 290, y1, 20, h, this, GetCurrencyName(i, true));
		y1 += h;
	}

	// Reserve data
	for (int i = 1; i <= 27; i++)
		m_item_data[GetCurrencyName(i, true)] = std::make_shared<ItemData>();
	
	y = y1;

	m_window->size(shift + 200 + 80 + 10 + 30, y + 10);
	m_window->end();
}

int Gui::Run() {
	m_window->show();

	while (Fl::wait() > 0) {
		if (m_refresher_thread_state == THREAD_STATE_FINISHED) {
			m_refresher_thread->join();
			m_refresher_progress->value(0);

			for (unsigned i = 1; i <= CURRENCY_TYPE_COUNT; i++) {
				m_sell_items[GetCurrencyName(i, true)]->Apply();
				m_buy_items[GetCurrencyName(i, true)]->Apply();
				m_profit[GetCurrencyName(i, true)]->Apply();
			}

			delete m_refresher_thread;
			m_refresher_thread = nullptr;
			m_refresher_thread_state = THREAD_STATE_CALM;
		}

		if (m_refresher_thread_state == THREAD_STATE_WORKING)
			m_refresher_progress->value((float)m_refresher_thread_progress);
	}

	return 0;
}

void Gui::Check(Fl_Widget* w) {
	if (m_chaos_input->changed()) {
		for (auto& it : m_profit)
			it.second->Calculate(m_chaos_input->value(), m_sell_items[it.first]->Value(), m_buy_items[it.first]->Value(), true);

		return;
	}

	for (auto& it : m_sell_items)
		if (it.second->Changed()) {
			m_profit[it.first]->Calculate(m_chaos_input->value(), it.second->Value(), m_buy_items[it.first]->Value(), true);
			return;
		}

	for (auto& it : m_buy_items)
		if (it.second->Changed()) {
			m_profit[it.first]->Calculate(m_chaos_input->value(), m_sell_items[it.first]->Value(), it.second->Value(), true);
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
		data_element->SetAttribute("chaos", m_chaos_input->value());
		data_element->SetAttribute("avg.count", m_average_count_input->value());

		for (auto& it : m_sell_items)
		{
			auto currency_element = xml_out.NewElement("currency");
			currency_element->SetAttribute("Name", it.first.c_str());
			currency_element->SetAttribute("Buy", m_buy_items[it.first]->Value());
			currency_element->SetAttribute("Sell", it.second->Value());
			currency_element->SetAttribute("Profit", m_profit[it.first]->Value());

			if (m_item_data.find(it.first) == m_item_data.end())
				continue;

			auto buyer_info_element = xml_out.NewElement("Buy");
			buyer_info_element->SetAttribute("Count", m_item_data[it.first]->buy_list.size());

			for (unsigned i = 0; i < m_item_data[it.first]->buy_list.size(); i++) {
				auto trade_element = xml_out.NewElement("trader");

				trade_element->SetAttribute("ID", i);
				trade_element->SetAttribute("AccName", m_item_data[it.first]->buy_list[i].s_seller_acc_name.c_str());
				trade_element->SetAttribute("CharName", m_item_data[it.first]->buy_list[i].s_seller_char_name.c_str());
				trade_element->SetAttribute("Stock", std::to_string(m_item_data[it.first]->buy_list[i].s_stock).c_str());
				trade_element->SetAttribute("BuyPrice", std::to_string(m_item_data[it.first]->buy_list[i].s_buy_price).c_str());
				trade_element->SetAttribute("SellPrice", std::to_string(m_item_data[it.first]->buy_list[i].s_sell_price).c_str());
				trade_element->SetAttribute("ItemType", GetCurrencyName(m_item_data[it.first]->buy_list[i].s_item_type, true).c_str());

				buyer_info_element->InsertEndChild(trade_element);
			}

			auto seller_info_element = xml_out.NewElement("Sell");
			seller_info_element->SetAttribute("Count", m_item_data[it.first]->sell_list.size());

			for (unsigned i = 0; i < m_item_data[it.first]->sell_list.size(); i++) {
				auto trade_element = xml_out.NewElement("trader");

				trade_element->SetAttribute("ID", i);
				trade_element->SetAttribute("AccName", m_item_data[it.first]->sell_list[i].s_seller_acc_name.c_str());
				trade_element->SetAttribute("CharName", m_item_data[it.first]->sell_list[i].s_seller_char_name.c_str());
				trade_element->SetAttribute("Stock", std::to_string(m_item_data[it.first]->sell_list[i].s_stock).c_str());
				trade_element->SetAttribute("BuyPrice", std::to_string(m_item_data[it.first]->sell_list[i].s_buy_price).c_str());
				trade_element->SetAttribute("SellPrice", std::to_string(m_item_data[it.first]->sell_list[i].s_sell_price).c_str());

				seller_info_element->InsertEndChild(trade_element);
			}

			currency_element->InsertEndChild(buyer_info_element);
			currency_element->InsertEndChild(seller_info_element);

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
		auto currency_element = data_element->FirstChildElement();
		for (unsigned i = 0; i < m_sell_items.size(); i++)
		{
			if (currency_element->QueryStringAttribute("Name", &temp_str_buf) == tinyxml2::XML_NO_ATTRIBUTE)
				continue;

			std::string name(temp_str_buf);

			currency_element->QueryDoubleAttribute("Buy", &val);
			m_buy_items[name]->Value(val, true);

			currency_element->QueryDoubleAttribute("Sell", &val);
			m_sell_items[name]->Value(val, true);

			currency_element->QueryDoubleAttribute("Profit", &val);
			m_profit[name]->Value(val);

			auto buyer_info_element = currency_element->FirstChildElement("Buy");
			auto seller_info_element = currency_element->FirstChildElement("Sell");

			int buy_trade_element_count = 0, sell_trade_element_count = 0;
			buyer_info_element->QueryIntAttribute("Count", &buy_trade_element_count);
			seller_info_element->QueryIntAttribute("Count", &sell_trade_element_count);

			m_item_data[name]->buy_list.resize(buy_trade_element_count);
			m_item_data[name]->sell_list.resize(sell_trade_element_count);

			auto trade_element = buyer_info_element->FirstChildElement();
			for (int j = 0; j < buy_trade_element_count; j++) {
				int id;
				trade_element->QueryIntAttribute("ID", &id);

				trade_element->QueryStringAttribute("AccName", &temp_str_buf);
				m_item_data[name]->buy_list[j].s_seller_acc_name = std::string(temp_str_buf);

				trade_element->QueryStringAttribute("CharName", &temp_str_buf);
				m_item_data[name]->buy_list[j].s_seller_char_name = std::string(temp_str_buf);	

				trade_element->QueryDoubleAttribute("Stock", &val);
				m_item_data[name]->buy_list[j].s_stock = (int)val;

				trade_element->QueryDoubleAttribute("BuyPrice", &val);
				m_item_data[name]->buy_list[j].s_buy_price = val;

				trade_element->QueryDoubleAttribute("SellPrice", &val);
				m_item_data[name]->buy_list[j].s_sell_price = val;

				if (j < buy_trade_element_count - 1)
					trade_element = trade_element->NextSiblingElement();
			}

			trade_element = seller_info_element->FirstChildElement();
			for (int j = 0; j < sell_trade_element_count; j++) {
				int id;
				trade_element->QueryIntAttribute("ID", &id);

				trade_element->QueryStringAttribute("AccName", &temp_str_buf);
				m_item_data[name]->sell_list[j].s_seller_acc_name = std::string(temp_str_buf);

				trade_element->QueryStringAttribute("CharName", &temp_str_buf);
				m_item_data[name]->sell_list[j].s_seller_char_name = std::string(temp_str_buf);

				trade_element->QueryDoubleAttribute("Stock", &val);
				m_item_data[name]->sell_list[j].s_stock = (int)val;

				trade_element->QueryDoubleAttribute("BuyPrice", &val);
				m_item_data[name]->sell_list[j].s_buy_price = val;

				trade_element->QueryDoubleAttribute("SellPrice", &val);
				m_item_data[name]->sell_list[j].s_sell_price = val;

				if (j < sell_trade_element_count - 1)
					trade_element = trade_element->NextSiblingElement();
			}

			if(i < m_sell_items.size() - 1)
				currency_element = currency_element->NextSiblingElement();
		}
		return;
	}

	if (caption == "Refresh") {
		SettlePrices();
		return;
	}
}

void Gui::AdditionalItemInfo::ButtonClick(Fl_Widget* w) {
	m_gui->ShowItemInfo(m_item_type);
}

void Gui::ShowItemInfo(const std::string& item_type_) {
	m_info_windows.push_back(std::make_shared<AdditionalItemInfoWindow>(item_type_, this));
}

void Gui::CurrencyListItem::Initialize() {
	m_input = new Fl_Value_Input(m_x, m_y, m_w, m_h, m_name.c_str());
	m_input->callback(m_gui->ChangeCallback, m_gui);
}

void Gui::ProfitItem::Initialize() {
	m_input = new Fl_Value_Input(m_x, m_y, m_w, m_h, "");
	m_input->set_output();
	m_input->precision(2);
}

void Gui::AdditionalItemInfo::Initialize() {
	m_button = new Fl_Button(m_x, m_y, m_w, m_h, "...");
	m_button->callback(AdditionalItemInfo::ButtonCallback, this);
}

void Gui::AdditionalItemInfoWindow::InfoTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
	switch (context)
	{
	case CONTEXT_STARTPAGE:
		fl_font(FL_HELVETICA, 16);
		return;

	case CONTEXT_COL_HEADER:
		fl_push_clip(X, Y, W, H);
		{
			fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, color());
			fl_color(FL_BLACK);

			switch (C)
			{
			case 0:
				fl_draw("Account", X, Y, W, H, FL_ALIGN_CENTER);
				break;

			case 1:
				fl_draw("Character", X, Y, W, H, FL_ALIGN_CENTER);
				break;

			case 2:
				fl_draw("Stock", X, Y, W, H, FL_ALIGN_CENTER);
				break;

			case 3:
				fl_draw("Buy Price", X, Y, W, H, FL_ALIGN_CENTER);
				break;

			case 4:
				fl_draw("Sell Price", X, Y, W, H, FL_ALIGN_CENTER);
				break;

			default:
				break;
			}
			
		}
		fl_pop_clip();
		return;

	case CONTEXT_CELL:
	{
		fl_push_clip(X, Y, W, H);
		{
			// BG COLOR
			fl_color(row_selected(R) ? selection_color() : FL_WHITE);
			fl_rectf(X, Y, W, H);

			// TEXT
			fl_color(FL_BLACK);

			switch (C)
			{
			case 0:
				fl_draw(m_item_data.buy_list[R].s_seller_acc_name.c_str(), X, Y, W, H, FL_ALIGN_CENTER);
				break;

			case 1:
				fl_draw(m_item_data.buy_list[R].s_seller_char_name.c_str(), X, Y, W, H, FL_ALIGN_CENTER);
				break;

			case 2:
				fl_draw(std::to_string(m_item_data.buy_list[R].s_stock).c_str(), X, Y, W, H, FL_ALIGN_CENTER);
				break;

			case 3:
				fl_draw(to_string_fixed(m_item_data.buy_list[R].s_buy_price, 2).c_str(), X, Y, W, H, FL_ALIGN_CENTER);
				break;

			case 4:
				fl_draw(to_string_fixed(m_item_data.buy_list[R].s_sell_price, 2).c_str(), X, Y, W, H, FL_ALIGN_CENTER);
				break;

			default:
				break;
			}

			// BORDER
			fl_color(FL_LIGHT2);
			fl_rect(X, Y, W, H);
		}
		fl_pop_clip();
		return;
	}

	default:
		return;
	}
}

void Gui::AdditionalItemInfoWindow::Initialize() {
	int x = 10, y = 10, w = 800, h = 20;
	int shift = x + 70;

	auto item_data = m_gui->GetItemData(m_item_type);
	if (!item_data.has_value())
		return;

	m_window = new Fl_Double_Window(0, 0, m_item_type.c_str());

	m_info_table = new InfoTable(item_data.value(), 0, 0, w, 500, "Info");
	m_info_table->rows(item_data.value().buy_list.size());
	m_info_table->cols(5);
	m_info_table->col_header(1);
	m_info_table->col_resize(4);
	m_info_table->row_resize(4);
	m_info_table->end();

	m_window->end();
	m_window->size(w, 500);
	m_window->show();
}

void Gui::SettlePrices() {
	if (m_average_count_input->value() < 1)
		return;

	if (m_refresher_thread != nullptr)
		return;

	m_refresher_progress->maximum(CURRENCY_TYPE_COUNT);
	m_refresher_progress->value(0);

	m_refresher_thread = new std::thread(&Gui::Settler, this);
}

void Gui::Settler() {
	m_refresher_thread_state = THREAD_STATE_WORKING;
	m_refresher_thread_progress = 0;

	for (int i = 1; i <= CURRENCY_TYPE_COUNT; i++) {
		m_refresher_thread_progress++;

		auto price_list_buy = m_scrapper_ptr->GetItemPrice(true, i, CHAOS_ORB, 0);
		auto price_list_sell = m_scrapper_ptr->GetItemPrice(true, CHAOS_ORB, i, 0);
		if (!price_list_buy.has_value() || !price_list_sell.has_value())
			continue;

		m_item_data[GetCurrencyName(i, true)]->buy_list = price_list_buy.value();
		m_item_data[GetCurrencyName(i, true)]->sell_list = price_list_sell.value();

		double average_sell = 0, average_buy = 0;
		unsigned int real_shift = (int)min(m_list_shift_input->value(), min(price_list_sell.value().size(), price_list_buy.value().size()));

		for (unsigned i = real_shift;
			i < (unsigned int)m_average_count_input->value() + real_shift &&
			i < price_list_buy.value().size() &&
			i < price_list_sell.value().size(); i++) {

			average_sell += price_list_buy.value()[i].s_sell_price / price_list_buy.value()[i].s_buy_price;
			average_buy += price_list_sell.value()[1].s_buy_price / price_list_sell.value()[1].s_sell_price;
		}
		average_sell /= (int)min(m_average_count_input->value(), price_list_sell.value().size());
		average_buy /= (int)min(m_average_count_input->value(), price_list_buy.value().size());

		m_sell_items[GetCurrencyName(i, true)]->Value(average_sell, false);
		m_buy_items[GetCurrencyName(i, true)]->Value(average_buy, false);
		m_profit[GetCurrencyName(i, true)]->Calculate(m_chaos_input->value(), average_sell, average_buy, false);
	}

	m_refresher_thread_state = THREAD_STATE_FINISHED;
}