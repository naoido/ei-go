FROM gcc:latest

WORKDIR /app

COPY ../../backend .

RUN apt-get update && apt-get install -y cmake && \
    mkdir build && cd build && \
    cmake .. && make

EXPOSE 5173

CMD ["./build/dragon_server"]
