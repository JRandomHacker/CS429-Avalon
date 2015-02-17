#include "subscriber.hpp"

Subscriber::Subscriber() {

}

Subscriber::~Subscriber() {

}

ModelData* Subscriber::getModelData() {
	return model_data;
}

void Subscriber::setModelData(ModelData* new_model_data) {
	model_data = new_model_data;
}

ClosureSubscriber::ClosureSubscriber(std::function<void (Subscriber*)> duc,
		std::function<void (Subscriber*)> ddc) : dataUpdatedClosure(duc), dataDestroyedClosure(ddc) {

}

void ClosureSubscriber::dataUpdated() {
	if (dataUpdatedClosure) {
		dataUpdatedClosure((Subscriber*)this);
	}
}

void ClosureSubscriber::dataDestroyed() {
	if (dataDestroyedClosure) {
		dataDestroyedClosure((Subscriber*)this);
	}
}

void MockSubscriber::dataUpdated() {
	updatedCount++;
}

void MockSubscriber::dataDestroyed() {
	wasDestroyed = true;
}