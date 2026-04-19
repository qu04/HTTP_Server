#!/bin/bash
set -e

PROJECT_DIR="$HOME/HTTP_Server"
IMAGE_NAME="personal-cpp-server"
CONTAINER_NAME="personal-cpp-server"

cd "$PROJECT_DIR"

echo "[1/4] Pull latest code..."
git pull

echo "[2/4] Build Docker image..."
sudo docker build -t "$IMAGE_NAME" .

echo "[3/4] Restart container..."
sudo docker stop "$CONTAINER_NAME" >/dev/null 2>&1 || true
sudo docker rm "$CONTAINER_NAME" >/dev/null 2>&1 || true

echo "[4/4] Start new container..."
sudo docker run -d -p 8080:8080 --name "$CONTAINER_NAME" "$IMAGE_NAME"

echo "Redeploy finished."
