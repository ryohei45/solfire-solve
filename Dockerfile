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
    libudev-dev

RUN curl -sSfL https://release.solana.com/v1.8.14/install | sh

ENV PATH="/root/.local/share/solana/install/active_release/bin:${PATH}"

RUN cd /root/.local/share/solana/install/active_release/bin/sdk/bpf \
    && bash env.sh

WORKDIR /work
COPY . .
CMD ["make"]
