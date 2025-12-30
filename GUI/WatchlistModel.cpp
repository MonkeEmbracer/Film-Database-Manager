#include "WatchlistModel.h"

WatchlistModel::WatchlistModel(Service& service, QObject *parent) : service(service), QAbstractTableModel(parent) {}

int WatchlistModel::rowCount(const QModelIndex& parent) const {
	return static_cast<int>(service.getWatchlist().size());
}

int WatchlistModel::columnCount(const QModelIndex& parent) const {
	return 5;
}

QVariant WatchlistModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid() or role != Qt::DisplayRole)
		return {};

	const auto& films = service.getWatchlist();
	const auto& film = films[index.row()];
	switch (index.column()) {
		case 0: return QString::fromStdString(film.getTitle());
		case 1: return QString::fromStdString(film.getGenre());
		case 2: return QString::number(film.getYear());
		case 3: return QString::number(film.getLikes());
		case 4: return QString::fromStdString(film.getTrailer());
		default: return {};
	}
}

QVariant WatchlistModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole)
		return {};
	if (orientation == Qt::Horizontal) {
		switch (section) {
			case 0: return "Title";
			case 1: return "Genre";
			case 2: return "Year";
			case 3: return "Likes";
			case 4: return "Trailer";
			default: return {};
		}
	}
	if (orientation == Qt::Vertical) {
		return QString::number(section + 1);
	}
	return {};
}