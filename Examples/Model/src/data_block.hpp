

#ifndef DATA_BLOCK_HPP
#define DATA_BLOCK_HPP

#include <vector>

#include "model_data.hpp"

class Subscriber;

class DataBlock{
public:
	DataBlock();
	~DataBlock();
	DataBlock(const DataBlock& that);

	bool addSubscriber(Subscriber* new_subscriber);
	bool detachSubscriber(Subscriber* old_subscriber);
	bool detachSubscriberQuietly(Subscriber* old_subscriber);
	void detachAllSubscribers();

	template <typename T>
	void updateData(const T& new_data);

	template <typename T>
	T* getData();

private:
	ModelData* data_payload = NULL;
	std::vector<Subscriber*> subscribers;
	
	void releaseData();
	void releaseSubscriber(Subscriber* old_subscriber);

	void alertAllSubscribersToUpdate();
};

template <typename T>
void DataBlock::updateData(const T& new_data) {
	releaseData();
	data_payload = new TypedModelData<T>(new_data);
	alertAllSubscribersToUpdate();
}

template <typename T>
T* DataBlock::getData() {
	if (data_payload != NULL) {
		TypedModelData<T>* data_payload_typed = dynamic_cast<TypedModelData<T>*>(data_payload);
		if (data_payload_typed != NULL) {
			return data_payload_typed->getPayloadReference();
		}
	}
	return NULL;
}

#endif // DATA_BLOCK_HPP