#include "Undo.h"

void ActionAdd::executeUndo() {
	repo.remove(film);
}

void ActionAdd::executeRedo() {
	repo.add(film);
}

void ActionRemove::executeUndo() {
	repo.add(film);
}

void ActionRemove::executeRedo() {
	repo.remove(film);
}

void ActionUpdate::executeUndo() {
	repo.remove(newFilm);
	repo.add(oldFilm);
}

void ActionUpdate::executeRedo() {
	repo.remove(oldFilm);
	repo.add(newFilm);
}
