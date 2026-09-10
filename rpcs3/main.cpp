#include "stdafx.h"
#include "rpcs3.h"

LOG_CHANNEL(sys_log, "SYS");

// Keep the experimental modern UI bootstrap translation unit linked into the
// final executable. rpcs3_ui is a static library, so an otherwise unreferenced
// Qt startup hook may be discarded by the linker.
void force_link_modern_ui_bootstrap();

int main(int argc, char** argv)
{
	force_link_modern_ui_bootstrap();

	const int exit_code = run_rpcs3(argc, argv);
	sys_log.notice("RPCS3 terminated with exit code %d", exit_code);
	return exit_code;
}
