#include <memory>
#include "Instance.h"

int main(int argc, char** argv) {
	std::unique_ptr<Instance> instance = std::make_unique<Instance>(argc, argv);
	return instance->Run();
}