#include "dev_ctx.h"

static struct dev_enigma_ctx g_dev_ctx = { 0 };

struct dev_enigma_ctx *dev_ctx(void) {
    return &g_dev_ctx;
}
