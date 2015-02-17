

#ifndef DATA_BLOCK_HPP
#define DATA_BLOCK_HPP

#include <vector>

#include "model_data.hpp"
#include "subscriber.hpp"

class DataBlock{
public:
	DataBlock();
	~DataBlock();
	DataBlock(const DataBlock& that);

	bool addSubscriber(Subscriber* new_subscriber);
	bool detachSubscriber(Subscriber* old_subscriber);
	bool detachSubscriberQuietly(Subscriber* old_subscriber);
	void detachAllSubscribers();

	void updateData(int new_data);

	ModelData* getData();

private:
	ModelData data_payload;
	std::vector<Subscriber*> subscribers;
	
	void releaseSubscriber(Subscriber* old_subscriber);
};

#endif // DATA_BLOCK_HPP