#include "database/test_sqlite.h"
#include "network/test_socket.h"
#include "ftp/test_ftp.h"

int main()
{
	// TODO: use google test
	if (!test_sqlite())
		return 1;
	test_socket();
	test_ftp();
}
