#include "data_block.hpp"

#include "subscriber.hpp"

DataBlock::DataBlock() {

}

DataBlock::~DataBlock() {
	detachAllSubscribers();
	releaseData();
}

DataBlock::DataBlock(const DataBlock& that) {
	data_payload = that.data_payload;
}


bool DataBlock::addSubscriber(Subscriber* new_subscriber) {
	subscribers.push_back(new_subscriber);
	new_subscriber->setDataBlock(this);
	return true;
}

void DataBlock::releaseSubscriber(Subscriber* old_subscriber) {
	old_subscriber->setDataBlock(NULL);
	old_subscriber->dataDestroyed();
}

bool DataBlock::detachSubscriber(Subscriber* old_subscriber) {
	for (auto subscriber_iter = subscribers.begin(); subscriber_iter != subscribers.end(); subscriber_iter++) {
		if (*subscriber_iter == old_subscriber) {
			releaseSubscriber(old_subscriber);
			subscribers.erase(subscriber_iter);
			return true;
		}
	}
	return false;
}

bool DataBlock::detachSubscriberQuietly(Subscriber* old_subscriber) {
	for (auto subscriber_iter = subscribers.begin(); subscriber_iter != subscribers.end(); subscriber_iter++) {
		if (*subscriber_iter == old_subscriber) {
			subscribers.erase(subscriber_iter);
			return true;
		}
	}
	return false;
}

void DataBlock::detachAllSubscribers() {
	for (auto subscriber_iter = subscribers.begin(); subscriber_iter != subscribers.end(); subscriber_iter++) {
		releaseSubscriber(*subscriber_iter);
	}
	subscribers.erase(subscribers.begin(), subscribers.end());
}

void DataBlock::releaseData() {
	if (data_payload != NULL) {
		delete data_payload;
	}
}

void DataBlock::alertAllSubscribersToUpdate() {
	for (auto subscriber_iter = subscribers.begin(); subscriber_iter != subscribers.end(); subscriber_iter++) {
		(*subscriber_iter)->dataUpdated();
	}
}