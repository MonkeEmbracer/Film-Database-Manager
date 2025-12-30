#include "Tests.h"
#include "../Service/Service.h"
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <algorithm>

static void testFilm() {
	Film film("Dead Poets Society", "Drama", 1989, 24124, "idk");
	assert(film.getTitle() == "Dead Poets Society");
	assert(film.getGenre() == "Drama");
	assert(film.getYear() == 1989);
	assert(film.getLikes() == 24124);
	assert(film.getTrailer() == "idk");

	Film film2;
	film2 = film;
	assert(film == film2);
	assert(!(film != film2));

	assert(film2.getTitle() == "Dead Poets Society");
	assert(film2.getGenre() == "Drama");
	assert(film2.getYear() == 1989);
	assert(film2.getLikes() == 24124);
	assert(film2.getTrailer() == "idk");

	film2.setTitle("Alive Poets Society");
	film2.setGenre("Comedy");
	film2.setYear(1955);
	film2.setLikes(1);
	film2.setTrailer("ik");
	assert(film != film2);
	assert(!(film == film2));
	assert(film2.getTitle() == "Alive Poets Society");
	assert(film2.getGenre() == "Comedy");
	assert(film2.getYear() == 1955);
	assert(film2.getLikes() == 1);
	assert(film2.getTrailer() == "ik");
}

static void testRepo() {
	Repo repo;
	Film film1("Inception", "Sci-Fi", 2010, 1000000, "https://trailer.link");
	Film film2("Interstellar", "Sci-Fi", 2014, 1500000, "https://trailer.link");

	repo.add(film1);
	assert(repo.getSize() == 1);
	assert(repo.find(film1) == true);

	repo.add(film2);
	assert(repo.getSize() == 2);
	repo.remove(film1);
	assert(repo.getSize() == 1);
	assert(repo.find(film1) == false);

	repo.add(film1);
	assert(repo.getSize() == 2);
	repo.removeByIndex(0);
	assert(repo.getSize() == 1);
	assert(repo.find(film2) == false);

	repo.update(0, film1);
	assert(repo.getSize() == 1);
	assert(repo.find(film1) == true);
	assert(repo.find(film2) == false);

	try {
		repo.update(3, film2);
		assert(false);
	} catch (...) {
		assert(true);
	}

	repo.add(film2);

	assert(repo.find(film1) == true);
	assert(repo.find(film2) == true);

	const auto& data = repo.getAll();
	assert(data.size() == 2);
}

static void testService() {
	Service service;

	Film film1 = Service::createFilm("Inception", "Sci-Fi", 2010, 1000000, "https://trailer.link");
	Film film2 = Service::createFilm("Interstellar", "Sci-Fi", 2014, 1500000, "https://trailer.link");

	service.add(film1);
	assert(service.find(film1) == true);

	try {
		service.add(film1);
		assert(false);
	} catch (...) {
		assert(true);
	}

	service.add(film2);
	assert(service.find(film2) == true);
	service.remove(film2);
	assert(service.find(film2) == false);

	try {
		service.remove(film2);
		assert(false);
	} catch (...) {
		assert(true);
	}

	assert(service.find(film1) == true);
	service.removeByIndex(0);
	assert(service.find(film1) == false);

	try {
		service.removeByIndex(5);
		assert(false);
	} catch (...) {
		assert(true);
	}

	service.add(film1);
	assert(service.find(film1) == true);
	service.update(0, film2);
	assert(service.find(film1) == false);
	assert(service.find(film2) == true);

	assert(service.getAll().size() == 1);

	Film created = Service::createFilm("Tenet", "Action", 2020, 500000, "https://trailer.link");
	assert(created.getTitle() == "Tenet");
	assert(created.getGenre() == "Action");

	service.clearWatchlist();
	service.addToWatchlist(film2);
	assert(std::find(service.getWatchlist().begin(), service.getWatchlist().end(), film2) != service.getWatchlist().end());

	service.addToRejected(film2);
	assert(std::find(service.getRejected().begin(), service.getRejected().end(), film2) != service.getRejected().end());

	service.addToWatched(film2);
	assert(std::find(service.getWatched().begin(), service.getWatched().end(), film2) != service.getWatched().end());

	service.clearWatchlist();
	Film film3 = Service::createFilm("Whiplash", "Drama", 2014, 10000, "https://trailer.link");
	service.add(film3);
	DynamicVector<std::string> genres = service.genres();
	assert(std::find(genres.begin(), genres.end(), "Drama") != genres.end());

	DynamicVector<Film> scifi = service.getFilmsByGenre("Sci-Fi");
	assert(scifi.size() >= 0);

	DynamicVector<Film> none = service.getFilmsByGenre("Nonexistent");
	assert(none.size() == 0);

	DynamicVector<Film> unbrowsed = service.getUnbrowsed();
	for (int i = 0; i < unbrowsed.size(); ++i)
		assert(std::find(service.getWatchlist().begin(), service.getWatchlist().end(), unbrowsed[i])
			== service.getWatchlist().end());

	int likesBefore = film3.getLikes();
	service.likeFilm(film3);
	assert(film3.getLikes() == likesBefore + 1);

	service.clearWatchlist();
	assert(service.getWatchlist().size() == 0);
	assert(service.getRejected().size() == 0);
	assert(service.getWatched().size() == 0);

	service.add(film1);
	Film film4 = Service::createFilm("Something", "Drama", 1, 1, "https://trailer.link");
	service.add(film4);
	Film film5 = Service::createFilm("Something2", "Drama", 1, 1, "https://trailer.link");
	service.add(film5);

	service.addToWatchlist(film3);
	service.addToRejected(film4);
	service.addToRejected(film5);
	genres = service.genres();
	assert(std::find(genres.begin(), genres.end(), "Drama") != genres.end());
	assert(std::find(genres.begin(), genres.end(), "Sci-Fi") != genres.end());
}

static void testWatchlist() {
	WatchlistCSV watchlist;

	Film film1("Inception", "Sci-Fi", 2010, 1000000, "https://trailer.link");
	Film film2("Whiplash", "Drama", 2014, 500000, "https://trailer.link");
	Film film3("Tenet", "Action", 2020, 750000, "https://trailer.link");

	watchlist.add(film1);
	assert(watchlist.getWatchlist().size() == 1);
	assert(std::find(watchlist.getWatchlist().begin(), watchlist.getWatchlist().end(), film1) != watchlist.getWatchlist().end());

	watchlist.reject(film2);
	assert(watchlist.getRejected().size() == 1);
	assert(std::find(watchlist.getRejected().begin(), watchlist.getRejected().end(), film2) != watchlist.getRejected().end());

	watchlist.unreject(film2);
	assert(std::find(watchlist.getRejected().begin(), watchlist.getRejected().end(), film2) == watchlist.getRejected().end());
	assert(watchlist.getRejected().size() == 0);

	watchlist.add(film3);
	assert(std::find(watchlist.getWatchlist().begin(), watchlist.getWatchlist().end(), film3) != watchlist.getWatchlist().end());

	watchlist.watch(film3);
	assert(std::find(watchlist.getWatchlist().begin(), watchlist.getWatchlist().end(), film3) == watchlist.getWatchlist().end());
	assert(std::find(watchlist.getWatched().begin(), watchlist.getWatched().end(), film3) != watchlist.getWatched().end());

	watchlist.watch(film2);
	assert(std::find(watchlist.getWatched().begin(), watchlist.getWatched().end(), film2) != watchlist.getWatched().end());

	watchlist.clear();
	assert(watchlist.getWatchlist().size() == 0);
	assert(watchlist.getRejected().size() == 0);
	assert(watchlist.getWatched().size() == 0);
}


void tests() {
	testFilm();
	testRepo();
	testService();
	testWatchlist();
}