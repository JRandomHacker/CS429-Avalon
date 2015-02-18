
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

	bool subscribe(std::string data_id, Subscriber* new_subscriber);
	bool unsubscribe(std::string data_id, Subscriber* old_subscriber);
	
	bool updateData(std::string data_id, int new_data);
	bool addData(std::string data_id, int inital_value);
	bool removeData(std::string data_id);

private:
	bool doesDataExist(std::string data_id);

	std::unordered_map<std::string, DataBlock> flat_data;

	Model(const Model& that);
};

#endif // MODEL_HPP
