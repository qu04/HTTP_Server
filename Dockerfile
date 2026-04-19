FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN sed -i 's|http://archive.ubuntu.com/ubuntu|http://mirrors.tuna.tsinghua.edu.cn/ubuntu|g' /etc/apt/sources.list \
 && sed -i 's|http://security.ubuntu.com/ubuntu|http://mirrors.tuna.tsinghua.edu.cn/ubuntu|g' /etc/apt/sources.list \
 && apt-get update -o Acquire::Retries=3 -o Acquire::ForceIPv4=true \
 && apt-get install -y --no-install-recommends ca-certificates cmake build-essential \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN cmake -S . -B build \
 && cmake --build build -j

FROM ubuntu:22.04

WORKDIR /app

COPY --from=builder /app/build/personal_server /app/personal_server
COPY --from=builder /app/data /app/data

EXPOSE 8080

CMD ["./personal_server"]