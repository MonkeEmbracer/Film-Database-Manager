#ifndef WATCHLIST_H
#define WATCHLIST_H

#include "../Domain/Film.h"
#include <vector>

template <typename T>
using DynamicVector = std::vector<T>;

class Watchlist {
protected:
	DynamicVector<Film> data;
	DynamicVector<Film> rejected;
	DynamicVector<Film> watched;

public:
	void add(const Film&);
	void reject(const Film&);
	void unreject(const Film&);
	void watch(const Film&);
	void clear();
	const DynamicVector<Film>& getWatchlist() const;
	const DynamicVector<Film>& getRejected() const;
	const DynamicVector<Film>& getWatched() const;

	virtual ~Watchlist() = default;
	virtual void saveFile(const std::string&);
	virtual void open() = 0;
};


class WatchlistCSV : public Watchlist {
private:
	std::string otherFile = "../watchlist.csv";

public:
	void saveFile(const std::string&) override;
	void open() override;
};


class WatchlistHTML : public Watchlist {
private:
	std::string otherFile = "../watchlist.html";

public:
	void saveFile(const std::string&) override;
	void open() override;
};

#endif
