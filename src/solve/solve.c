#include <solana_sdk.h>
#include <stdint.h>

static SolAccountInfo accounts[10];
static SolParameters params = { .ka = accounts };

extern uint64_t entrypoint(const uint8_t *input)
{
    sol_log("TEST PROGRAM");

    if (!sol_deserialize(input, &params, 10)) {
        return ERROR_INVALID_ARGUMENT;
    }

    return SUCCESS;
}
