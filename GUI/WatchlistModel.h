#ifndef WATCHLISTMODEL_H
#define WATCHLISTMODEL_H

#include <QAbstractTableModel>
#include "Service.h"

class WatchlistModel : public QAbstractTableModel {
private:
	Service& service;

public:
	explicit WatchlistModel(Service& service, QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

#endif
