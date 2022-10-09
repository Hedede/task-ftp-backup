#include "database/test_sqlite.h"

int main()
{
	// TODO: use google test
	if (!test_sqlite())
		return 1;
}
