FROM ubuntu

RUN apt-get update \
    && apt-get install -y ca-certificates curl make \
       --no-install-recommends \
    && rm -rf /var/lib/apt/lists/* \
    && (curl --proto '=https' --tlsv1.2 -sSf \
        https://sh.rustup.rs | sh -s -- -y) \
    && . $HOME/.cargo/env \
    && sh -c "$(curl -sSfL \
        https://release.solana.com/v1.10.8/install)" \
    && export PATH="/root/.local/share/solana/install/active_release/bin:$PATH" \
    && (cd ~/.local/share/solana/install/active_release/bin/sdk/bpf/ \
        && sh ./env.sh)

WORKDIR /work

ENV PATH="/root/.local/share/solana/install/active_release/bin:$PATH"

COPY . .

CMD ["make"]
