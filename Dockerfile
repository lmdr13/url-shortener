FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    cmake \
    ninja-build \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    perl \
    python3 \
    bison \
    flex \
    autoconf \
    automake \
    libtool \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY vcpkg ./vcpkg
RUN ./vcpkg/bootstrap-vcpkg.sh -disableMetrics

COPY vcpkg.json ./
RUN ./vcpkg/vcpkg install

COPY CMakeLists.txt CMakePresets.json ./
COPY src ./src
COPY include ./include
COPY db ./db
COPY network ./network

RUN cmake --preset "Configure preset using toolchain file" -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --parallel

FROM ubuntu:24.04 AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/main /app/main

EXPOSE 8080

ENTRYPOINT ["/app/main"]