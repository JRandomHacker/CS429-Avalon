#include "model.hpp"

Model::Model() {

}

Model::~Model() {

}

bool Model::subscribe(std::string data_id, Subscriber* new_subscriber) {
	auto data_iter = flat_data.find(data_id);
	if (data_iter == flat_data.end()) {
		return false;
	}
	data_iter->second.addSubscriber(new_subscriber);
	return true;
}

bool Model::unsubscribe(std::string data_id, Subscriber* old_subscriber) {
	auto data_iter = flat_data.find(data_id);
	if (data_iter == flat_data.end()) {
		return false;
	}
	data_iter->second.detachSubscriber(old_subscriber);
	return true;
}

bool Model::updateData(std::string data_id, int new_data) {
	auto data_iter = flat_data.find(data_id);
	if (data_iter == flat_data.end()) {
		return false;
	}
	data_iter->second.updateData(new_data);
	return true;
}

bool Model::addData(std::string data_id, int initial_value) {
	auto data_iter = flat_data.find(data_id);
	if (data_iter != flat_data.end()) {
		return false;
	}
	DataBlock new_block;
	flat_data.insert({data_id, new_block});
	return updateData(data_id, initial_value);
}

bool Model::removeData(std::string data_id) {
	auto data_iter = flat_data.find(data_id);
	if (data_iter == flat_data.end()) {
		return false;
	}
	flat_data.erase(data_iter);
	return true;
}

bool Model::doesDataExist(std::string data_id) {
	return flat_data.find(data_id) != flat_data.end();
}

DataBlock::DataBlock() {

}

DataBlock::~DataBlock() {
	detachAllSubscribers();
}

DataBlock::DataBlock(const DataBlock& that) {
	data_payload = that.data_payload;
}


void DataBlock::addSubscriber(Subscriber* new_subscriber) {
	subscribers.push_back(new_subscriber);
	new_subscriber->setModelData(&data_payload);
}

void DataBlock::releaseSubscriber(Subscriber* old_subscriber) {
	old_subscriber->setModelData(NULL);
	old_subscriber->dataDestroyed();
}

void DataBlock::detachSubscriber(Subscriber* old_subscriber) {
	for (auto subscriber_iter = subscribers.begin(); subscriber_iter != subscribers.end(); subscriber_iter++) {
		if (*subscriber_iter == old_subscriber) {
			releaseSubscriber(old_subscriber);
			subscribers.erase(subscriber_iter);
			break;
		}
	}
}

void DataBlock::detachAllSubscribers() {
	for (auto subscriber_iter = subscribers.begin(); subscriber_iter != subscribers.end(); subscriber_iter++) {
		releaseSubscriber(*subscriber_iter);
	}
	subscribers.erase(subscribers.begin(), subscribers.end());
}

void DataBlock::updateData(int new_data) {
	data_payload.setPayload(new_data);
	for (auto subscriber_iter = subscribers.begin(); subscriber_iter != subscribers.end(); subscriber_iter++) {
		(*subscriber_iter)->dataUpdated();
	}
}
