#ifndef UNDO_H
#define UNDO_H

#include "../Repo/Repo.h"

class Action {
protected:
	Repo& repo;

public:
	explicit Action(Repo& repo) : repo(repo) {}
	virtual void executeUndo() = 0;
	virtual void executeRedo() = 0;
	virtual ~Action() = default;
};

class ActionAdd : public Action {
private:
	Film film;

public:
	ActionAdd(Repo& repo, Film film) : Action(repo), film(film) {}
	void executeUndo() override;
	void executeRedo() override;
};

class ActionRemove : public Action {
private:
	Film film;

public:
	ActionRemove(Repo& repo, Film film) : Action(repo), film(film) {}
	void executeUndo() override;
	void executeRedo() override;
};

class ActionUpdate : public Action {
private:
	Film oldFilm, newFilm;

public:
	ActionUpdate(Repo& repo, Film oldFilm, Film newFilm) : Action(repo), oldFilm(oldFilm), newFilm(newFilm) {}
	void executeUndo() override;
	void executeRedo() override;
};

#endif
