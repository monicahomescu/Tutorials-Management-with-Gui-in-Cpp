#include "Gui.h"
#include "Exceptions.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

Gui::Gui(Repository& repo, Service& serv, InputValidator& val) : repo{ repo }, serv { serv }, val{ val }
{
	this->textLabel = new QLabel(this);
	this->adminPushButton = new QPushButton(this);
	this->userPushButton = new QPushButton(this);
	this->initGui();
	this->connect();
}

Gui::~Gui() = default;

void Gui::initGui()
{
    auto* layout = new QVBoxLayout(this);
    this->textLabel->setText("Select a mode:");
    layout->addWidget(this->textLabel);
    this->adminPushButton->setText("Admin");
    layout->addWidget(this->adminPushButton);
    this->userPushButton->setText("User");
    layout->addWidget(this->userPushButton);
    this->setLayout(layout);
}

void Gui::connect() 
{
    QObject::connect(this->adminPushButton, &QPushButton::clicked, this, &Gui::adminMode);
    QObject::connect(this->userPushButton, &QPushButton::clicked, this, &Gui::userMode);
}

void Gui::adminMode() 
{
    auto* admin = new AdminGui(this, this->repo, this->serv, this->val);
    admin->show();
}

AdminGui::AdminGui(QWidget* parent, Repository& repo, Service& serv, InputValidator& val) : repo{ repo }, serv{ serv }, val{ val }
{
    this->textLabel = new QLabel(this);
    this->tutorialsListWidget = new QListWidget{};
    this->titleLineEdit = new QLineEdit{};
    this->presenterLineEdit = new QLineEdit{};
    this->durationLineEdit = new QLineEdit{};
    this->likesLineEdit = new QLineEdit{};
    this->linkLineEdit = new QLineEdit{};
    this->addPushButton = new QPushButton("Add");
    this->deletePushButton = new QPushButton("Delete");
    this->updatePushButton = new QPushButton("Update");
    this->filterLineEdit = new QLineEdit{};
    setParent(parent);
    setWindowFlag(Qt::Window);
    this->initAdminGui();
    this->connect();
    this->listTutorials();
}

AdminGui::~AdminGui() = default;

void AdminGui::initAdminGui()
{
    auto* layout = new QVBoxLayout(this);
    this->textLabel->setText("Admin mode");
    layout->addWidget(this->textLabel);
    layout->addWidget(this->tutorialsListWidget);
    auto* tutorialInputLayout = new QFormLayout{};
    tutorialInputLayout->addRow("Title", this->titleLineEdit);
    tutorialInputLayout->addRow("Presenter", this->presenterLineEdit);
    tutorialInputLayout->addRow("Duration", this->durationLineEdit);
    tutorialInputLayout->addRow("Likes", this->likesLineEdit);
    tutorialInputLayout->addRow("Link", this->linkLineEdit);
    tutorialInputLayout->addRow("Filter", this->filterLineEdit);
    layout->addLayout(tutorialInputLayout);
    auto* buttonsLayout = new QGridLayout{};
    buttonsLayout->addWidget(this->addPushButton, 0, 0);
    buttonsLayout->addWidget(this->deletePushButton, 1, 0);
    buttonsLayout->addWidget(this->updatePushButton, 2, 0);
    layout->addLayout(buttonsLayout);
}

void AdminGui::listTutorials()
{
    this->tutorialsListWidget->clear();
    std::vector<Tutorial> allTutorials = this->serv.getRepo().getAll();
    for (Tutorial& t : allTutorials)
        this->tutorialsListWidget->addItem(QString::fromStdString("Title: " + t.getTitle() + " | Presenter: " + t.getPresenter() + " | Duration: " + t.getDuration() + " | Likes: " + std::to_string(t.getLikes())));
}

void AdminGui::connect() 
{
    QObject::connect(this->tutorialsListWidget, &QListWidget::itemSelectionChanged, [this]() 
        {
        int selectedIndex = this->index();
        if (selectedIndex < 0)
            return;
        Tutorial t = this->serv.getRepo().getAll()[selectedIndex];
        this->titleLineEdit->setText(QString::fromStdString(t.getTitle()));
        this->presenterLineEdit->setText(QString::fromStdString(t.getPresenter()));
        this->durationLineEdit->setText(QString::fromStdString(t.getDuration()));
        this->likesLineEdit->setText(QString::fromStdString(std::to_string(t.getLikes())));
        this->linkLineEdit->setText(QString::fromStdString(t.getLink()));
        });
    QObject::connect(this->filterLineEdit, &QLineEdit::textChanged, this, &AdminGui::filterTutorials);
    QObject::connect(this->addPushButton, &QPushButton::clicked, this, &AdminGui::addTutorial);
    QObject::connect(this->deletePushButton, &QPushButton::clicked, this, &AdminGui::deleteTutorial);
    QObject::connect(this->updatePushButton, &QPushButton::clicked, this, &AdminGui::updateTutorial);
}

void AdminGui::addTutorial()
{
    std::string title = this->titleLineEdit->text().toStdString();
    std::string presenter = this->presenterLineEdit->text().toStdString();
    std::string duration = this->durationLineEdit->text().toStdString();
    std::string likesString = this->likesLineEdit->text().toStdString();
    std::string link = this->linkLineEdit->text().toStdString();
    int likes;
    try
    {
        likes = stoi(likesString);
        Tutorial t{ title, presenter, duration, likes, link };
        this->val.errors(t);
        this->serv.addServ(t);
        this->listTutorials();
        this->titleLineEdit->clear();
        this->presenterLineEdit->clear();
        this->durationLineEdit->clear();
        this->likesLineEdit->clear();
        this->linkLineEdit->clear();
        this->serv.getRepo().write();
    }
    catch (ValidationException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
    catch (RepositoryException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
}

void AdminGui::deleteTutorial()
{
    int i = this->index();
    std::string title = this->titleLineEdit->text().toStdString();
    std::string presenter = this->presenterLineEdit->text().toStdString();
    try
    {
        if (i < 0)
        {
            auto* er = new QMessageBox();
            er->setIcon(QMessageBox::Critical);
            er->setText("No tutorial selected!");
            er->exec();
        }
        else
        {
            this->serv.deleteServ(title, presenter);
            this->listTutorials();
            this->serv.getRepo().write();
        }
    }
    catch (ValidationException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
    catch (RepositoryException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
}

void AdminGui::updateTutorial()
{
    int i = this->index();
    try
    {
        if (i < 0) 
        {
            auto* er = new QMessageBox();
            er->setIcon(QMessageBox::Critical);
            er->setText("No tutorial selected!");
            er->exec();
        }
        else
        {
            std::string findTitle = this->serv.getRepo().getAll()[i].getTitle();
            std::string findPresenter = this->serv.getRepo().getAll()[i].getPresenter();
            std::string title = this->titleLineEdit->text().toStdString();
            std::string presenter = this->presenterLineEdit->text().toStdString();
            std::string duration = this->durationLineEdit->text().toStdString();
            std::string likesString = this->likesLineEdit->text().toStdString();
            std::string link = this->linkLineEdit->text().toStdString();
            int likes;
            likes = stoi(likesString);
            Tutorial t{ title, presenter, duration, likes, link };
            this->val.errors(t);
            this->serv.updateServ(findTitle, findPresenter, t);
            this->listTutorials();
            this->serv.getRepo().write();
        }
    }
    catch (ValidationException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
    catch (RepositoryException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
}

int AdminGui::index() const
{
    QModelIndexList selectedIndexes = this->tutorialsListWidget->selectionModel()->selectedIndexes();
    if (selectedIndexes.empty()) 
    {
        this->titleLineEdit->clear();
        this->presenterLineEdit->clear();
        this->durationLineEdit->clear();
        this->likesLineEdit->clear();
        this->linkLineEdit->clear();
        return -1;
    }
    int selectedIndex = selectedIndexes.at(0).row();
    return selectedIndex;
}

void AdminGui::filterTutorials()
{
    this->tutorialsListWidget->clear();
    std::string f = this->filterLineEdit->text().toStdString();
    std::vector<Tutorial> allTutorials = this->serv.getRepo().getAll();
    std::vector<Tutorial> filtered;
    std::string s;
    for (Tutorial& t : allTutorials)
    {
        s = "Title: " + t.getTitle() + " | Presenter: " + t.getPresenter() + " | Duration: " + t.getDuration() + " | Likes: " + std::to_string(t.getLikes());
        if (s.find(f) != std::string::npos)
            filtered.push_back(t);
    }
    for (Tutorial& t : filtered)
        this->tutorialsListWidget->addItem(QString::fromStdString("Title: " + t.getTitle() + " | Presenter: " + t.getPresenter() + " | Duration: " + t.getDuration() + " | Likes: " + std::to_string(t.getLikes())));
}

void Gui::userMode() 
{
    auto* user = new UserGui(this, this->serv, this->val);
    user->show();
}

UserGui::UserGui(QWidget* parent, Service& serv, InputValidator& val1) : serv{ serv }, val{ val } 
{
    this->textLabel = new QLabel(this);
    this->titleLineEdit = new QLineEdit{};
    this->presenterLineEdit = new QLineEdit{};
    this->durationLineEdit = new QLineEdit{};
    this->likesLineEdit = new QLineEdit{};
    this->linkLineEdit = new QLineEdit{};
    this->searchLineEdit = new QLineEdit{};
    this->searchPushButton = new QPushButton("Search");
    this->addPushButton = new QPushButton("Add");
    this->next1PushButton = new QPushButton("Next");
    this->playPushButton = new QPushButton("Play");
    this->deletePushButton = new QPushButton("Delete");
    this->likePushButton = new QPushButton("Like");
    this->next2PushButton = new QPushButton("Next");
    setParent(parent);
    setWindowFlag(Qt::Window);
    this->initUserGui();
    this->connect();
}

UserGui::~UserGui() = default;

void UserGui::initUserGui() 
{
    auto* layout = new QVBoxLayout(this);
    this->textLabel->setText("User mode");
    layout->addWidget(this->textLabel);

    auto* tutorialInfoLayout = new QFormLayout{};
    tutorialInfoLayout->addRow("Title", this->titleLineEdit);
    tutorialInfoLayout->addRow("Presenter", this->presenterLineEdit);
    tutorialInfoLayout->addRow("Duration", this->durationLineEdit);
    tutorialInfoLayout->addRow("Likes", this->likesLineEdit);
    tutorialInfoLayout->addRow("Link", this->linkLineEdit);
    layout->addLayout(tutorialInfoLayout);

    auto* buttonsLayout = new QGridLayout{};
    buttonsLayout->addWidget(this->searchLineEdit, 0, 0);
    buttonsLayout->addWidget(this->searchPushButton, 1, 0);
    buttonsLayout->addWidget(this->addPushButton, 2, 0);
    buttonsLayout->addWidget(this->next1PushButton, 3, 0);
    buttonsLayout->addWidget(this->playPushButton, 0, 1);
    buttonsLayout->addWidget(this->deletePushButton, 1, 1);
    buttonsLayout->addWidget(this->likePushButton, 2, 1);
    buttonsLayout->addWidget(this->next2PushButton, 3, 1);
    layout->addLayout(buttonsLayout);
}

void UserGui::connect() 
{
    QObject::connect(this->searchPushButton, &QPushButton::clicked, this, &UserGui::searchPresenter);
    QObject::connect(this->addPushButton, &QPushButton::clicked, this, &UserGui::addTutorial);
    QObject::connect(this->next1PushButton, &QPushButton::clicked, this, &UserGui::nextPresenter);
    QObject::connect(this->playPushButton, &QPushButton::clicked, this, &UserGui::playWlist);
    QObject::connect(this->deletePushButton, &QPushButton::clicked, this, &UserGui::deleteTutorial);
    QObject::connect(this->likePushButton, &QPushButton::clicked, this, &UserGui::likeTutorial);
    QObject::connect(this->next2PushButton, &QPushButton::clicked, this, &UserGui::nextWlist);
}

void UserGui::searchPresenter()
{
    std::string presenter = this->searchLineEdit->text().toStdString();
    p = presenter;
    try
    {
        std::vector<Tutorial> tutorials = this->serv.seePresenterTutorials(presenter);
        this->current = 0;
        Tutorial t = tutorials[current];
        this->titleLineEdit->setText(QString::fromStdString(t.getTitle()));
        this->presenterLineEdit->setText(QString::fromStdString(t.getPresenter()));
        this->durationLineEdit->setText(QString::fromStdString(t.getDuration()));
        this->likesLineEdit->setText(QString::fromStdString(std::to_string(t.getLikes())));
        this->linkLineEdit->setText(QString::fromStdString(t.getLink()));
        tutorials[current].play();
    }
    catch (RepositoryException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
}

void UserGui::addTutorial()
{
    std::string title = this->titleLineEdit->text().toStdString();
    std::string presenter = this->presenterLineEdit->text().toStdString();
    std::string duration = this->durationLineEdit->text().toStdString();
    std::string likesString = this->likesLineEdit->text().toStdString();
    std::string link = this->linkLineEdit->text().toStdString();
    int likes;
    try
    {
        likes = stoi(likesString);
        Tutorial t{ title, presenter, duration, likes, link };
        this->val.errors(t);
        this->serv.addWlist(t);
        this->serv.getWlist().write();
    }
    catch (RepositoryException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
}

void UserGui::nextPresenter()
{
    std::string presenter = p;
    std::vector<Tutorial> tutorials = this->serv.seePresenterTutorials(presenter);
    if (current + 1 == tutorials.size())
        current = 0;
    else
        current++;
    Tutorial t = tutorials[current];
    this->titleLineEdit->setText(QString::fromStdString(t.getTitle()));
    this->presenterLineEdit->setText(QString::fromStdString(t.getPresenter()));
    this->durationLineEdit->setText(QString::fromStdString(t.getDuration()));
    this->likesLineEdit->setText(QString::fromStdString(std::to_string(t.getLikes())));
    this->linkLineEdit->setText(QString::fromStdString(t.getLink()));
    tutorials[current].play();
}

void UserGui::playWlist()
{
    try
    {
        std::vector<Tutorial> tutorials = this->serv.getWlist().getAll();
        current = 0;
        tutorials[current].play();
    }
    catch (RepositoryException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
}

void UserGui::deleteTutorial()
{
    std::string title = this->titleLineEdit->text().toStdString();
    std::string presenter = this->presenterLineEdit->text().toStdString();
    std::string duration = this->durationLineEdit->text().toStdString();
    std::string likesString = this->likesLineEdit->text().toStdString();
    std::string link = this->linkLineEdit->text().toStdString();
    int likes;
    try
    {
        likes = stoi(likesString);
        Tutorial t{ title, presenter, duration, likes, link };
        this->serv.deleteWlist(t);
        this->serv.getWlist().write();
    }
    catch (RepositoryException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
}

void UserGui::likeTutorial()
{
    std::string title = this->titleLineEdit->text().toStdString();
    std::string presenter = this->presenterLineEdit->text().toStdString();
    std::string duration = this->durationLineEdit->text().toStdString();
    std::string likesString = this->likesLineEdit->text().toStdString();
    std::string link = this->linkLineEdit->text().toStdString();
    int likes;
    try
    {
        likes = stoi(likesString);
        Tutorial t{ title, presenter, duration, likes, link };
        this->serv.likeTutorial(t);
        this->serv.getWlist().write();
    }
    catch (RepositoryException& ex)
    {
        auto* er = new QMessageBox();
        er->setIcon(QMessageBox::Critical);
        er->setText(QString::fromStdString(ex.getMessage()));
        er->exec();
    }
}

void UserGui::nextWlist()
{
    std::vector<Tutorial> tutorials = this->serv.getWlist().getAll();
    if (current + 1 == tutorials.size())
        current = 0;
    else
        current++;
    Tutorial t = tutorials[current];
    this->titleLineEdit->setText(QString::fromStdString(t.getTitle()));
    this->presenterLineEdit->setText(QString::fromStdString(t.getPresenter()));
    this->durationLineEdit->setText(QString::fromStdString(t.getDuration()));
    this->likesLineEdit->setText(QString::fromStdString(std::to_string(t.getLikes())));
    this->linkLineEdit->setText(QString::fromStdString(t.getLink()));
    tutorials[current].play();
}