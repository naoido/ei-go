FROM gcc:latest

WORKDIR /app

COPY . .

RUN apt-get update && apt-get install -y \
    cmake \
    make \
    libjsoncpp-dev \
    git \
    uuid-dev \
    zlib1g-dev \
    openssl \
    libssl-dev

RUN git clone --recurse-submodules https://github.com/drogonframework/drogon.git && \
    cd drogon && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && \
    make -j$(nproc) && \
    make install

RUN apt-get update && apt-get install -y cmake && \
    mkdir build && cd build && \
    cmake .. && make

EXPOSE 5173

WORKDIR /app/build

CMD ["./backend"]
