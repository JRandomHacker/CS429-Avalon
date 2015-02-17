
#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include "model_data.hpp"

#include <iostream>

class Subscriber {
public:
	Subscriber();
	~Subscriber();

	ModelData* getModelData();
	void setModelData(ModelData* new_model_data);

	virtual void dataUpdated() = 0;
	virtual void dataDestroyed() = 0;

private:
	ModelData* model_data = NULL;

	Subscriber(const Subscriber& that);
};

class MockSubscriber : public Subscriber {
public:
	void dataUpdated();
	void dataDestroyed();

	int updatedCount = 0;
	bool wasDestroyed = false;
};

#endif // SUBSCRIBER_HPP
