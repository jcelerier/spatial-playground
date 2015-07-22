#include <iostream>
#include <functional>
#include <vector>
using Fun = std::function<bool(int,int,int)>;

class Space
{
	public:
		std::vector<Fun> functions;

		int xdim, ydim;


};

int main()
{
	return 0;
}
