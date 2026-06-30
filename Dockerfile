# Use Ubuntu 26.04 LTS for updated package repositories (SFML 3.x support)
FROM ubuntu:26.04

# Disable interactive configuration prompts during apt installs
ENV DEBIAN_FRONTEND=noninteractive

# Install toolchain and dependencies (CMake, GCC, SFML, GTest)
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    libsfml-dev \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# Define working directory inside the container
WORKDIR /app

# Copy CMake configuration and source code
COPY CMakeLists.txt ./
COPY src/ ./src/
COPY tests/ ./tests/

# Configure the build system in Release mode
RUN cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# Compile the project utilizing all available CPU cores
RUN cmake --build build -j$(nproc)

# Execute all registered tests via CTest and output details on failure
CMD ["ctest", "--test-dir", "build", "--output-on-failure"]