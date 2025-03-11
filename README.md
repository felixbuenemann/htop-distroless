# Distroless htop Docker Image

A minimal, secure Docker image for running htop in containers with extremely small footprint.

## Features

- **Distroless**: Contains only htop and its minimal dependencies - no shell, package manager, or unnecessary binaries
- **Tiny**: Final image is a fraction of the size of typical htop containers
- **Secure**: Minimal attack surface with no unnecessary components
- **Kubernetes-Ready**: Custom entrypoint ensures proper functioning with kubectl

## Usage

```bash
# Run htop in the current terminal
docker run --rm -it --pid=host felixbuenemann/htop

# Monitor a specific container
docker run --rm -it --pid=container:target_container_name felixbuenemann/htop

# Use with kubectl to monitor a specific node
kubectl run htop --rm -it --image=felixbuenemann/htop --privileged --overrides='{"spec":{"hostPID":true,"nodeSelector":{"kubernetes.io/hostname":"your-node-name"}}}'
```

## How It Works

This image:
1. Builds from Alpine Linux in the build stage
2. Compiles a custom C entrypoint (/usr/local/bin/htop)
3. Extracts only htop and its required libraries
4. Creates a final image based on scratch (empty) with only essential files

The custom entrypoint waits for kubectl to fully attach and establish a proper TTY before starting htop, ensuring correct keyboard input and display.

## Build

To build the image yourself:

```bash
docker build -t felixbuenemann/htop .
```

## License

This project is open source and available under the MIT License.