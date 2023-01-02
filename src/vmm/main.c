#include "lib/logging.h"

void vmm_main()
{
    init_logging(DEBUG_LEVEL);
    INFO("hello world");
}
