#include "subscriber.hpp"

Subscriber::Subscriber() {

}

Subscriber::~Subscriber() {
	if (data_block != NULL) {
		data_block->detachSubscriberQuietly(this);
	}
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