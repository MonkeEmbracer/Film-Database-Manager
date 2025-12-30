#ifndef REPO_H
#define REPO_H

#include "../Domain/Film.h"
#include <vector>

template <typename T>
using DynamicVector = std::vector<T>;

class Repo {
private:
	DynamicVector<Film> data;

public:
	void add(const Film&);
	void remove(const Film&);
	void removeByIndex(int);
	void update(int, const Film&);
	bool find(const Film&) const;
	const DynamicVector<Film>& getAll() const;
	int getSize() const;
};

#endif
