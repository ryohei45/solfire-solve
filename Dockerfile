FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y \
        ca-certificates \
        curl \
        make \
        build-essential \
    && rm -rf /var/lib/apt/lists/*

RUN curl --proto '=https' \
    --tlsv1.2 \
    -sSfL \
    https://sh.rustup.rs | sh -s -- -y

ENV PATH="/root/.cargo/bin:${PATH}"

RUN sh -c "$(curl -sSfL https://release.solana.com/v1.10.8/install)"

ENV PATH="/root/.local/share/solana/install/active_release/bin:${PATH}"

RUN cd /root/.local/share/solana/install/active_release/bin/sdk/bpf/ \
    && sh ./env.sh

WORKDIR /work

COPY . .

CMD ["make"]
