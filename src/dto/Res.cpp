#include <string>

class Res {
private:
	int key;
	std::string val;

public:
	explicit Res(const int k, const std::string &v) : key{ k }, val{ v } {}
	int getKey() const { return key; }
	std::string getVal() const { return val; }
};