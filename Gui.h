#pragma once
#include "Service.h"
#include "Exceptions.h"
#include <QtWidgets/QMainWindow>
#include "ui_Gui.h"
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QRadioButton>

class Gui : public QWidget
{
private:
	Repository& repo;
	Service& serv;
	InputValidator val;

	QLabel* textLabel;
	QPushButton* adminPushButton;
	QPushButton* userPushButton;

	void initGui();
	void connect();
	void adminMode();
	void userMode();

public:
	explicit Gui(Repository& repo, Service& serv, InputValidator& val);
	~Gui() override;
};

class AdminGui : public QWidget
{
private:
	Repository& repo;
	Service& serv;
	InputValidator val;

	QLabel* textLabel;
	QListWidget* tutorialsListWidget;
	QLineEdit* titleLineEdit, * presenterLineEdit, * durationLineEdit, * likesLineEdit, * linkLineEdit;
	QPushButton* addPushButton, * deletePushButton, * updatePushButton;
	QLineEdit* filterLineEdit;

	void initAdminGui();
	void connect();
	int index() const;
	void addTutorial();
	void deleteTutorial();
	void updateTutorial();
	void listTutorials();
	void filterTutorials();

public:
	explicit AdminGui(QWidget* parent, Repository& repo, Service& serv, InputValidator& val);
	~AdminGui() override;
};

class UserGui : public QWidget 
{
private:
	Service& serv;
	InputValidator val;

	QLabel* textLabel;
	QLineEdit* titleLineEdit, * presenterLineEdit, * durationLineEdit, * likesLineEdit, * linkLineEdit;
	QLineEdit* searchLineEdit;
	QPushButton* searchPushButton, * addPushButton, * next1PushButton;
	QPushButton* playPushButton, * deletePushButton, * likePushButton, * next2PushButton;

	int current;
	std::string p;

	void initUserGui();
	void connect();
	void searchPresenter();
	void addTutorial();
	void nextPresenter();
	void playWlist();
	void deleteTutorial();
	void likeTutorial();
	void nextWlist();

public:
	explicit UserGui(QWidget* parent, Service& serv, InputValidator& val);
	~UserGui() override;
};
