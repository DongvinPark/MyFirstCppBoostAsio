#include <string>

class ResSecond {
private:
	int key;
	std::string val;

public:
	explicit ResSecond(const int k, const std::string &v) : key{ k }, val{ v } {}
	int getKey() const { return key; }
	std::string getVal() const { return val; }
};