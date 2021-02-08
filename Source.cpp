#include "profile.h"

#include <iomanip>
#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <string>
#include <fstream>

using namespace std;

class ReadingManager {
public:
	ReadingManager()
		: page_to_id_(MAX_PAGE_ + 1)
		, user_page_counts_(MAX_USER_COUNT_ + 1, 0)
		, users_count(0)
	{};

	void Read(int user_id, int page_count) { // O(logQ)
		if (user_page_counts_[user_id] == 0) {
			user_page_counts_[user_id] = page_count;
			page_to_id_[page_count].insert(user_id);
			users_count++;
		} 
		else {
			int old_page_count = user_page_counts_[user_id];
			page_to_id_[old_page_count].erase(user_id);
			page_to_id_[page_count].insert(user_id);
			user_page_counts_[user_id] = page_count;
		}
	}

	double Cheer(int user_id) const {
		if (user_page_counts_[user_id] == 0) {
			return 0;
		}
		if (users_count == 1) {
			return 1;
		}
		const int users_count_less_page = GetUsersCount(user_page_counts_[user_id]);
		return users_count_less_page * 1.0 / (users_count - 1);
	}

private:
	static const int MAX_USER_COUNT_ = 100'000;
	static const int MAX_PAGE_ = 1'000;

	vector<set<int>> page_to_id_;
	vector<int> user_page_counts_;
	int users_count;
	
	int GetUsersCount(int page) const { // возвращает кол-во пользователей которое прочитало меньше страниц
		int result = 0;
		for (int i = 1; i < page; i++)
			result += page_to_id_[i].size();
		return result;
	}
};

int main() {
	LOG_DURATION("Tolal");
	// Для ускорения чтения данных отключается синхронизация
	// cin и cout с stdio,
	// а также выполняется отвязка cin от cout
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ReadingManager manager;
	ifstream in("input.txt");

	int query_count;
	in >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		in >> query_type;
		int user_id;
		in >> user_id;

		if (query_type == "READ") {
			int page_count;
			in >> page_count;
			manager.Read(user_id, page_count);
		}
		else if (query_type == "CHEER") {
			cout << setprecision(6) << manager.Cheer(user_id) << "\n";
		}
	}

	return 0;
}