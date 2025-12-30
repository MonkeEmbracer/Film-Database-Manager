#ifndef SERVICE_H
#define SERVICE_H

#include "../Repo/Repo.h"
#include "../Watchlist/Watchlist.h"
#include "Undo.h"

#include <stack>
#include <memory>

class Service {
private:
	Repo repo;
	Watchlist* watchlist;
	WatchlistCSV watchlistCSV;
	WatchlistHTML watchlistHTML;
	std::string otherFile;
	int mode;
	std::stack<std::unique_ptr<Action>> undoStack;
	std::stack<std::unique_ptr<Action>> redoStack;

public:
	Service() : watchlist(&watchlistCSV), mode(0), otherFile("../watchlist.csv") {}
	WatchlistCSV& getWatchlistCSV() { return watchlistCSV; }
	WatchlistHTML& getWatchlistHTML() { return watchlistHTML; }
	void add(const Film&);
	void remove(const Film&);
	void removeByIndex(int);
	void update(int, const Film&);
	bool find(const Film&) const;
	const DynamicVector<Film>& getAll() const;
	DynamicVector<std::string> genres();
	DynamicVector<Film> getFilmsByGenre(const std::string&) const;
	void addToWatchlist(const Film&);
	void addToRejected(const Film&);
	void addToWatched(const Film&);
	const DynamicVector<Film>& getWatchlist() const;
	const DynamicVector<Film>& getRejected() const;
	const DynamicVector<Film>& getWatched() const;
	DynamicVector<Film> getUnbrowsed() const;
	void likeFilm(Film&);
	void clearWatchlist();
	void switchMode();
	void saveUserFile(const std::string&);
	void loadUserFile(const std::string&);
	void seeWatchlist() const;
	void undo();
	void redo();
	void clearUndoRedo();

	static Film createFilm(const std::string&, const std::string&, int, int, const std::string&);
};

#endif
