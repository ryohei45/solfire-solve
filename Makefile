OUT_DIR := ./dist

include $(HOME)/.local/share/solana/install/active_release/bin/sdk/bpf/c/bpf.mk

BPF_C_FLAGS += -Wno-macro-redefined
BPF_C_FLAGS += -Wno-incompatible-pointer-types-discards-qualifiers
