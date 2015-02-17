#include "subscriber.hpp"

#include "data_block.hpp"

Subscriber::Subscriber() {

}

Subscriber::~Subscriber() {
	if (data_block != NULL) {
		data_block->detachSubscriberQuietly(this);
	}
}

ModelData* Subscriber::getModelData() {
	if (data_block == NULL) {
		return NULL;
	}
	return data_block->getData();
}

void Subscriber::setDataBlock(DataBlock* new_data_block) {
	data_block = new_data_block;
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