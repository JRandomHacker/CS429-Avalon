
#ifndef MODEL_DATA_HPP
#define MODEL_DATA_HPP

#include <stdlib.h>

class ModelData {
public:
	ModelData();
	~ModelData();

	// Interface functions
	int* getPayload();

	// Model functions
	void setPayload(int new_payload);
	void dumpPayload();

private:
	void allocatePayload(size_t new_payload_size);
	void deallocatePayload();

	size_t payload_size = 0;
	void* payload = NULL;

	ModelData(const ModelData& that);
};

#endif // MODEL_DATA_HPP
