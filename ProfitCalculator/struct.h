#pragma once

#include <string>

struct Item {
	int s_item_type;
	int s_price_item_type;

	double s_sell_price;
	double s_buy_price;

	double s_ratio;

	int s_stock;

	std::string s_seller_acc_name;
	std::string s_seller_char_name;
};

struct ItemData {
	ItemData(std::vector<Item>& buy_list_, std::vector<Item>& sell_list_) : 
		buy_list(buy_list_), sell_list(sell_list_) {}

	ItemData() {}

	std::vector<Item> buy_list;
	std::vector<Item> sell_list;
};