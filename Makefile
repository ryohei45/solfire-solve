OUT_DIR := ./dist

include $(HOME)/.local/share/solana/install/active_release/bin/sdk/bpf/c/bpf.mk

CFLAGS += -Wno-macro-redefined
CFLAGS += -Wno-incompatible-pointer-types-discards-qualifiers
