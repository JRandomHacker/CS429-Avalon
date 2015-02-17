
#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include "model_data.hpp"

#include <iostream>
#include <functional>

class Subscriber {
public:
	Subscriber();
	~Subscriber();

	// Interface functions
	ModelData* getModelData();
	void setModelData(ModelData* new_model_data);

	// Model access functions
	virtual void dataUpdated() = 0;
	virtual void dataDestroyed() = 0;

private:
	ModelData* model_data = NULL;

	Subscriber(const Subscriber& that);
};

class ClosureSubscriber : public Subscriber {
public:
	ClosureSubscriber(std::function<void ((Subscriber*))> duc,
		std::function<void ((Subscriber*))> ddc);
	
	void dataUpdated();
	void dataDestroyed();

private:
	std::function<void ((Subscriber*))> dataUpdatedClosure;
	std::function<void ((Subscriber*))> dataDestroyedClosure;
};

class MockSubscriber : public Subscriber {
public:
	void dataUpdated();
	void dataDestroyed();

	int updatedCount = 0;
	bool wasDestroyed = false;
};

#endif // SUBSCRIBER_HPP
