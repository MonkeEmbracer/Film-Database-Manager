#ifndef UI_H
#define UI_H

#include "../Service/Service.h"

class UI {
private:
	Service service;

public:
	void start();
	int user();
	int admin();
	void browse();
	void seeDatabase() const;
	void seeWatchlist() const;

	void loadAdminFile(const std::string&);
	void saveAdminFile(const std::string&) const;
	void loadUserFile(const std::string&);
	void saveUserFile(const std::string&);

	static void printMenuAdmin();
	static void printMenuUser();
	static Film readFilm();
	static void openLink(const std::string&);
};

#endif
