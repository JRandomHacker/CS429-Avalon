#include "model_data.hpp"

ModelData::ModelData() {

}

ModelData::~ModelData() {
	
}

void ModelData::setPayload(int new_payload) {
	payload = new_payload;
}

int ModelData::getPayload() {
	return payload;
}
