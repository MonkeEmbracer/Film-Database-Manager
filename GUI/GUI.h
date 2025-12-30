#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>
#include <QAbstractItemView>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>

#include "../Service/Service.h"
#include "AdminGUI.h"
#include "UserGUI.h"

class GUI final : public QWidget {

private:
	Service service;
	AdminGUI* adminGUI;
	UserGUI* userGUI;

	QTabWidget* tabWidget;

public:
	explicit GUI(QWidget* parent = nullptr);

	void loadAdminFile(const std::string&);
	void saveAdminFile(const std::string&) const;
	void loadUserFile(const std::string&);
	void saveUserFile(const std::string&);
	void populateAll();

	static void openLink(const std::string&);
};

#endif
