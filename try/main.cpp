
#include <fffuncparser.h>

int main(int argc, char* argv[])
{
	FuncParser fp(argc, argv);

	fp.parse("/home/blackcat/cplusplus/bracket-match/heap-match.cpp");
	
	return 0;
}
