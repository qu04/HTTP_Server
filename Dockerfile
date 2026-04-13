FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y cmake g++ && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN cmake -S . -B build && cmake --build build

FROM ubuntu:22.04

WORKDIR /app

COPY --from=builder /app/build/personal_server /app/personal_server
COPY --from=builder /app/data /app/data

EXPOSE 8080

CMD ["./personal_server"]
