#pragma once

#include <string>

struct Item
{
	int s_item_type;
	int s_price_item_type;

	double s_sell_price;
	double s_buy_price;

	int s_stock;

	std::string s_seller_acc_name;
	std::string s_seller_char_name;
};