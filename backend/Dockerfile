FROM naoido/drogon:latest

WORKDIR /app

COPY . .

RUN apt-get update && apt-get install -y cmake && \
    mkdir build && cd build && \
    cmake .. && make

EXPOSE 5173

WORKDIR /app/build

CMD ["./backend"]
