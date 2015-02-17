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

void MockSubscriber::dataUpdated() {
	updatedCount++;
}

void MockSubscriber::dataDestroyed() {
	wasDestroyed = true;
}