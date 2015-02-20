
#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <unordered_map>

#include "model_data.hpp"
#include "data_block.hpp"
#include "subscriber.hpp"

class Model {
public:
	Model();
	~Model();

	/**
	 * If the given data_id is found, adds new subscriber.
	 * @param data_id id to search for
	 * @param new_subscriber Pointer to Subscriber object to add to subscribers
	 * @return true if successfully added, false if data_id not found
	**/
	bool subscribe(std::string data_id, Subscriber* new_subscriber);
	/**
	 * If the given data_id is found, releases subscriber.
	 * @param data_id id to search for
	 * @param old_subscriber Pointer to Subscriber object to remove from subscribers
	 * @return true if successfully removed, false if data_id not found
	**/
	bool unsubscribe(std::string data_id, Subscriber* old_subscriber);

	/**
	 * If the given data_id is found, updates current data with new_data.
	 * @param data_id id to search for
	 * @param new_data data to update with
	 * @return true if data successfully updated, false if data_id not found
	**/
	template <typename T>
	bool updateData(std::string data_id, const T& new_data);
	/**
	 * Adds the initial data.
	 * @param data_id id to search for
	 * @param initial_data data to initialize with
	 * @return true if data successfully added, false if data_id not found
	**/
	template <typename T>
	bool addData(std::string data_id, const T& initial_data);
	/**
	 * Adds data to new DataBlock.
	 * @param data_id id to search for
	 * @return true if data successfully added, false if data_id not found
	**/
	bool addData(std::string data_id);
	/**
	 * Removes data for given data_id.
	 * @param data_id id to search for
	 * @return true if data successfully removed, false if data_id not found
	**/
	bool removeData(std::string data_id);

private:
	/**
	 * Returns whether data exists for given data_id.
	 * @param data_id to search for
	 * @return true if data exists for data_id, false if data_id not found
	**/
	bool doesDataExist(std::string data_id);
	
	std::unordered_map<std::string, DataBlock> flat_data;

	Model(const Model& that);
};

template <typename T>
bool Model::updateData(std::string data_id, const T& new_data) {
	auto data_iter = flat_data.find(data_id);
	if (data_iter == flat_data.end()) {
		return false;
	}
	data_iter->second.updateData<T>(new_data);
	return true;
}

template <typename T>
bool Model::addData(std::string data_id, const T& initial_value) {
	bool data_created = addData(data_id);
	if (!data_created) {
		return false;
	}
	return updateData<T>(data_id, initial_value);
}

#endif // MODEL_HPP
