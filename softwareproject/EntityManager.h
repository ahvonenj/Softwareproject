#pragma once
#include <vector>
#include "Entity.h"

//template-luokka entityiden elosta huolehtimiseen,
template <typename T>
class EntityManager
{
private:
	vector<T>* entities;
public:
	void addEntity(T entity);
	void removeEntityAt(int index);
	vector<T>* getEntityList();
	void cleanEntities();
	void specialCleanEntities();

	EntityManager(void);
	~EntityManager(void);
};


// NÄITÄ EI SAA TUNKEA EntityManager.cpp
// http://stackoverflow.com/questions/12848876/unresolved-external-symbol-template-class

template<typename T>
EntityManager<T>::EntityManager()
{
	entities = new vector<T>();
}

template<typename T>
void EntityManager<T>::addEntity(T entity)
{
	entities->push_back(entity);
}

template<typename T>
void EntityManager<T>::removeEntityAt(int index)
{
	entities->erase(entities->begin() + (index - 1));
}

template<typename T>
vector<T>* EntityManager<T>::getEntityList()
{
	return this->entities;
}

template<typename T>
void EntityManager<T>::cleanEntities()
{
	for(int i = 0; i < this->entities->size(); i++)
	{
		if(!this->entities->at(i)->getAlive())
		{
			this->entities->erase(entities->begin() + i);			
		}
	}
}

template<typename T>
void EntityManager<T>::specialCleanEntities()
{
	for(int i = 0; i < this->entities->size(); i++)
	{
		if(!this->entities->at(i)->getAlive() && this->entities->at(i)->getHasSpawned())
		{
			this->entities->erase(entities->begin() + i);			
		}
	}
}

template<typename T>
EntityManager<T>::~EntityManager(void)
{

	delete entities;
	entities = nullptr;
}

