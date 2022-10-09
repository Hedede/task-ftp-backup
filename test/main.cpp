#include "utility/test_cmd_parser.h"
#include "database/test_sqlite.h"
#include "database/test_query_builder.h"
#include "network/test_socket.h"
#include "ftp/test_ftp.h"

int main()
{
	if (!test_cmd_parser())
		return 1;
	// TODO: use google test
	if (!test_sqlite())
		return 1;
	if (!test_query_builder())
		return 1;
	test_socket();
	test_ftp();
}
