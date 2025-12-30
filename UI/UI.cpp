#include "UI.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <algorithm>

#include "../Exceptions/Exceptions.h"
#include "../Validator/Validator.h"

void UI::start() {
	std::cout << "\nWelcome to Bogdan's Film Database Management!🎥\n";

	std::cout << "\n• Do you want to start the app in CSV or HTML mode?\n";
	std::cout << "1. CSV mode.\n";
	std::cout << "2. HTML mode.\n";

	std::string input;
	int option = 0;
	while (option == 0) {
		std::cout << "Hmmm? >";
		std::getline(std::cin, input);

		if (input == "testing")
			return;

		try {
			option = Validator::initialOption(input);
		} catch (ValidationException& e) {
			std::cout << e.what();
			option = 0;
		}
	}
	if (option == 2)
		this->service.switchMode();

	std::cout << "\n• Who's logging in?🔑\n";
	std::cout << "1. Admin.\n";
	std::cout << "2. User.\n";
	this->loadAdminFile("../admin.txt");

	option = 0;
	while (option == 0) {
		std::cout << "Say it! >";
		std::getline(std::cin, input);

		try {
			option = Validator::initialOption(input);
		} catch (ValidationException& e) {
			std::cout << e.what();
			option = 0;
		}
	}

	while (option) {
		if (option == 1)
			option = this->admin();
		else
			option = this->user();
	}
}

int UI::admin() {
	int returnCode = 0;
	std::cout << "\n• Welcome Mr. Admin! Manage the database as you please!🫡\n";

	bool exited = false;
	while (!exited) {
		printMenuAdmin();
		int command = -1;
		std::cin >> command;
		if (std::cin.fail())
			command = -1;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << '\n';

		switch (command) {
			case 1: // Display
				this->seeDatabase();
			break;

			case 2: // Add
				try {
					Film film = readFilm();
					this->service.add(film);
					std::cout << "\n• Successfully added new film. Hell yeah!💯\n";
				} catch (MyException& e) {
					std::cout << '\n' << e.what();
				}
			break;

			case 3: // Remove
				std::cout << "Enter the index of the movie to be removed: ";
				try {
					std::string buffer;
					std::getline(std::cin, buffer);
					const int index = std::stoi(buffer) - 1;
					this->service.removeByIndex(index);
					std::cout << "\n• Film #" << index + 1 << " has been removed.🥀\n";
				} catch (MyException& e) {
					std::cout << '\n' << e.what();
				}
			break;

			case 4: // Update
				std::cout << "Enter the index of the movie to be updated: ";
				try {
					std::string buffer;
					std::getline(std::cin, buffer);
					const int index = std::stoi(buffer) - 1;
					std::cout << "\nNow enter the new movie's details.\n";
					Film film = readFilm();
					this->service.update(index, film);
					std::cout << "\n• Film #" << index + 1 << " has been updated.✅\n";
				} catch (MyException& e) {
					std::cout << '\n' << e.what();
				}
			break;

			case 5: // Switch
				std::cout << "Switching to user mode...\n";
				returnCode = 2;
				exited = true;
			break;

			case 0: // Exit
				returnCode = 0;
				exited = true;
			break;

			default:
				std::cout << "• Invalid option. Please try again.❌\n";
		}
		this->saveAdminFile("../admin.txt");
	}
	return returnCode;
}

int UI::user() {
	int returnCode = 0;
	std::cout << "\n• Hello! Have fun using our app!✨\n";
	this->loadUserFile("../user.txt");

	bool exited = false;
	while (!exited) {
		printMenuUser();
		int command = -1, index = -1;
		std::cin >> command;
		if (std::cin.fail())
			command = -1;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << '\n';
		std::string buffer;

		switch (command) {
			case 1: // Browse
				this->browse();
			break;

			case 2: // See watchlist
				this->seeWatchlist();
			break;

			case 3: // Log movie
				std::cout << "• Just watched a movie? Hope it was stellar!🌟\n";
				std::cout << "What's the index of the movie you've watched? >";
				std::getline(std::cin, buffer);
				try {
					index = std::stoi(buffer) - 1;
					auto films = this->service.getWatchlist();
					auto film = films[index];
					this->service.addToWatched(film);
					std::cout << "\n• Successfully logged movie " << film.getTitle() << " (" << film.getYear() << ")!✅\n";

					std::cout << "\nDo you want to give it a thumbs up?👍 (Y/N) >";
					std::getline(std::cin, buffer);
					if (buffer == "Y" or buffer == "y") {
						this->service.likeFilm(film);
						std::cout << "\n• Hell yeah!💯🔥\n";
					}
					else
						std::cout << "\n• Oh... Okay.😔\n";

				} catch (...) {
					std::cout << "\n• Sorry, but there is no film #" << index + 1 << " in your watchlist!🥀\n";
				}
			break;

			case 4: // Switch
				std::cout << "Switching to admin mode...\n";
				returnCode = 1;
				exited = true;
			break;

			case 0: // Exit
				returnCode = 0;
				exited = true;
			break;

			default:
				std::cout << "• Invalid option. Please try again.❌\n";
		}
		this->saveAdminFile("../admin.txt");
		this->saveUserFile("../user.txt");
	}
	return returnCode;
}

void UI::printMenuAdmin() {
	std::cout << "\n1. Display all movies in the database.\n";
	std::cout << "2. Add a new movie to the database.\n";
	std::cout << "3. Remove a movie from the database.\n";
	std::cout << "4. Update a movie from the database.\n";
	std::cout << "5. Switch to user mode.\n";
	std::cout << "0. Exit.\n";
	std::cout << "\nChoose an option from above: >";
}

void UI::printMenuUser() {
	std::cout << "\n1. Browse for new movies.\n";
	std::cout << "2. See your watchlist.\n";
	std::cout << "3. Log a movie you watched.\n";
	std::cout << "4. Switch to admin mode.\n";
	std::cout << "0. Exit.\n";
	std::cout << "\nChoose an option from above: >";
}

void UI::openLink(const std::string& url) {
	std::string command = "/mnt/c/Program\\ Files/BraveSoftware/Brave-Browser/Application/brave.exe " + url;
	system(command.c_str());
}

Film UI::readFilm() {
	std::string title, genre, trailer, buffer;
	int year = 0, likes = 0;

	std::cout << "Title: >";
	std::getline(std::cin, title);
	std::cout << "Genre: >";
	std::getline(std::cin, genre);

	std::cout << "Year: >";
	std::getline(std::cin, buffer);
	year = Validator::year(buffer);

	std::cout << "Likes: >";
	std::getline(std::cin, buffer);
	likes = Validator::likes(buffer);

	std::cout << "Trailer: >";
	std::getline(std::cin, trailer);

	Film film = Service::createFilm(title, genre, year, likes, trailer);
	return film;
}

void UI::saveAdminFile(const std::string& filename) const {
	std::ofstream file(filename);
	auto data = this->service.getAll();
	file << data.size() << '\n';
	for (const auto& film : data) {
		file << film.getTitle() << '\n';
		file << film.getGenre() << '\n';
		file << film.getYear() << '\n';
		file << film.getLikes() << '\n';
		file << film.getTrailer() << '\n';
	}
	file.close();
}

void UI::loadAdminFile(const std::string& filename) {
	std::ifstream file(filename);
	int count = 0;
	file >> count;
	file.get();
	for (int i = 0; i < count; ++i) {
		Film film;
		file >> film;
		this->service.add(film);
	}
	file.close();
}

void UI::loadUserFile(const std::string& filename) {
	this->service.clearWatchlist();

	std::ifstream file(filename);
	auto data = this->service.getAll();
	int count = 0;

	file >> count;
	file.get();
	for (int i = 0; i < count; ++i) {
		Film film;
		file >> film;
		if (std::find(data.begin(), data.end(), film) != data.end())
			this->service.addToWatchlist(film);
	}

	file >> count;
	file.get();
	for (int i = 0; i < count; ++i) {
		Film film;
		file >> film;
		if (std::find(data.begin(), data.end(), film) != data.end())
			this->service.addToRejected(film);
	}

	file >> count;
	file.get();
	for (int i = 0; i < count; ++i) {
		Film film;
		file >> film;
		if (std::find(data.begin(), data.end(), film) != data.end())
			this->service.addToWatched(film);
	}
}

void UI::saveUserFile(const std::string& filename) {
	this->service.saveUserFile(filename);
}

void UI::browse() {
	std::cout << "• What genre would you like to browse?🍿\n";
	std::cout << "Here's a list of the available genres. Just press ENTER if you don't want to browse a specific genre.\n\n";

	auto genres = this->service.genres();
	int index = 0;
	for (const auto& genre : genres) {
		std::cout << ++index << ". " << genre <<  '\n';
	}

	std::string genre;
	std::cout << "\nWhich genre do you prefer? >";
	std::getline(std::cin, genre);

	DynamicVector<Film> films;
	bool generalSearch = false;
	if (!genre.empty()) {
		int choice = 0;
		try {
			choice = std::stoi(genre);
			if (choice < 1 || choice > genres.size())
				throw std::exception();
		} catch (const std::exception&) {
			std::cout << "\n• Oops! Invalid option, please try again.😔\n";
			return;
		}
		genre = genres[choice - 1];
		films = this->service.getFilmsByGenre(genre);
	}
	else {
		films = this->service.getUnbrowsed();
		generalSearch = true;
	}


	for (auto it = films.begin(); it != films.end();) {
		const Film& film = *it;
		std::string option;
		std::cout << "\n• How does this film sound to you?🧐\n";
		std::cout << "|- Title: " << film.getTitle() << '\n';
		if (generalSearch)
			std::cout << "|- Genre: " << film.getGenre() << '\n';
		std::cout << "|- Year: " << film.getYear() << '\n';
		std::cout << "|- Likes: " << film.getLikes() << '\n';
		std::cout << "\nWanna see a trailer?🎦 (Y/N) >";
		std::getline(std::cin, option);

		if (option == "Y" or option == "y") {
			openLink(film.getTrailer());
			std::cout << "\n• Hope it was interesting! Would you like to add this movie to your watchlist?🕰️ (Y/N) >";
		}
		else {
			std::cout << "\n• As you wish! Would you still want to add this movie to your watchlist?🕰️ (Y/N) >";
		}

		std::getline(std::cin, option);
		std::cout << '\n';
		if (option == "Y" or option == "y") {
			this->service.addToWatchlist(film);
			std::cout << film.getTitle() << " (" << film.getYear() <<  ") was added to your watchlist.✅\n\n";
		}
		else
			this->service.addToRejected(film);


		++it;
		if (it != films.end()) {
			std::cout << "• Would you like to continue browsing?🌐 (Y/N) >";
			std::getline(std::cin, option);
			if (option != "Y" and option != "y")
				return;
		}
	}

	if (generalSearch)
		std::cout << "• Wow... You've browsed every movie... That's crazy!\nYou can still review the movies you didn't add to your watchlist.🫣\n";
	else
		std::cout << "• You've browsed every \"" << genre << "\" movie! Nice!💯\n";
}

void UI::seeDatabase() const {
	/*
	 * Prints all films in the repository to the console.
	 * If the repository is empty, a message is displayed: "Database is empty."
	 * return: none
	 */
	auto data = this->service.getAll();
	if (data.empty()) {
		std::cout << "Database is empty.\n";
		return;
	}

	int index = 0;
	for (const auto& film : data) {
		std::cout << "|===================== Film #" << ++index << " =====================|\n";
		std::cout << "|- Title: " << film.getTitle() << '\n';
		std::cout << "|- Genre: " << film.getGenre() << '\n';
		std::cout << "|- Year: " << film.getYear() << '\n';
		std::cout << "|- Likes: " << film.getLikes() << '\n';
		std::cout << "|- Trailer: " << film.getTrailer() << "\n\n";
	}
}


void UI::seeWatchlist() const {
	/*
	 * Prints all films in the watchlist to the console.
	 * If the watchlist is empty, a message is displayed: "Watchlist is empty."
	 * return: none
	 */
	auto watchlist = this->service.getWatchlist();
	//int index = 0;

	if (watchlist.empty()) {
		std::cout << "• Watchlist is empty.😔🥀\n";
		return;
	}

	/*for (const auto& film : watchlist) {
		std::cout << "|===================== Film #" << ++index << " =====================|\n";
		std::cout << "|- Title: " << film.getTitle() << '\n';
		std::cout << "|- Genre: " << film.getGenre() << '\n';
		std::cout << "|- Year: " << film.getYear() << '\n';
		std::cout << "|- Likes: " << film.getLikes() << '\n';
		std::cout << "|- Trailer: " << film.getTrailer() << "\n\n";
	}*/

	this->service.seeWatchlist();
}
