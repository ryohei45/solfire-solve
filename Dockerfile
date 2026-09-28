FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV HOME=/root

RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    llvm \
    curl \
    ca-certificates \
    git \
    pkg-config \
    libudev-dev \
    bzip2

RUN mkdir -p /root/.local/share/solana/install/releases/1.8.14

RUN curl -fL --retry 5 \
    https://github.com/solana-labs/solana/releases/download/v1.8.14/solana-release-x86_64-unknown-linux-gnu.tar.bz2 \
    -o /tmp/solana.tar.bz2

RUN tar -xjf /tmp/solana.tar.bz2 \
    -C /root/.local/share/solana/install/releases/1.8.14 \
    --strip-components=1

RUN ln -s /root/.local/share/solana/install/releases/1.8.14 \
    /root/.local/share/solana/install/active_release

ENV PATH="/root/.local/share/solana/install/active_release/bin:${PATH}"

WORKDIR /work

COPY . .

CMD ["make"]
