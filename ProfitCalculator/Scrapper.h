#pragma once

#include <string>
#include <optional>
#include <vector>
#include <algorithm>
#include "curl/curl.h"

#include "Struct.h"
#include "ItemData.h"
#include "Utils.h"

class Scrapper {
private:
	class TradeURL {
	public:
		TradeURL(std::string base_url_, std::string league_name_, bool online_, int want_, int have_, int stock_) :
			m_base_url(base_url_), m_league_name(league_name_), m_online(online_), m_want(want_), m_have(have_), m_stock(stock_) {}
		~TradeURL() {}

		void SetBaseURL(const std::string& base_url_) { m_base_url = base_url_; }
		void SetLeagueName(const std::string& league_name_) { m_league_name = league_name_; }
		void SetOnlineOnly(bool online_) { m_online = online_; }
		void SetWantedItem(int want_) { m_want = want_; }
		void SetPresentItem(int have_) { m_have = have_; }
		void SetMinStock(int stock_) { m_stock = stock_; }

		void Compose()
		{
			m_composed_url.append(m_base_url);
			m_composed_url.append("search?");
			m_composed_url.append("league=");   m_composed_url.append(m_league_name);
			m_composed_url.append("&online=");  m_composed_url.append(m_online ? "x" : "");
			m_composed_url.append("&stock=");   m_composed_url.append(std::to_string(m_stock));
			m_composed_url.append("&want=");    m_composed_url.append(std::to_string(m_want));
			m_composed_url.append("&have=");    m_composed_url.append(std::to_string(m_have));
		}

		std::string Get() { return m_composed_url; }

	private:
		std::string m_league_name;
		bool m_online;
		int m_want;
		int m_have;
		int m_stock;

		std::string m_base_url;
		std::string m_composed_url;
	};

public:
	Scrapper(const std::string& league_name_, const std::string& base_url_) :
		m_league_name(league_name_), m_base_url(base_url_) { Initialize(); }

	std::optional<std::vector<Item>> GetItemPrice(bool online_only_, int item_type_, int price_item_type_, int min_stock_);

private:
	bool m_initialized = false;
	void Initialize();

	static size_t WriteCallback(char* contents_, size_t size_, size_t nmemb_, void* userdata_);

	std::optional<std::string> GetPage(const std::string& page_address_);

	CURL* m_curl_ptr = nullptr;

	std::string m_base_url;
	std::string m_league_name;
};