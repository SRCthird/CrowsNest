# CrowsNest

CrowsNest is a C++ application that uses the [Crow](https://crowcpp.org/) framework to mimic the capabilities of [Nest.js](https://nestjs.com/). This application is designed to provide a simple and efficient way to build web services with C++.

## Features

- Lightweight and fast web server using Crow.
- Structured and modular design similar to Nest.js.
- Example user management API.

## Requirements

- C++ compiler with C++11 support (e.g., GCC, Clang).
- Crow library (included as a submodule).
- SOCI library for database access.
- Any database system that SOCI allows.
- OpenSSL for SSL support.

## Installation

1. Clone the repository:

    ```bash
    git clone --recursive https://github.com/SRCthird/CrowsNest.git
    ```

2. Install the required libraries:

    For Ubuntu/Debian:
    ```bash
    sudo apt-get install libssl-dev libmysqlclient-dev libsqlite3-dev
    ```
    For Arch:
    ```bash
    sudo pacman -S boost mariadb-libs mysql
    yay -S soci crow
    ```

    For other distributions, please refer to their respective package managers and install the equivalent packages.

3. Build the application:

    ```bash
    make
    ```

## Usage

- To run the server:

    ```bash
    # Sudo is required to bind to port 443 or 80
    sudo ./CrowsNest
    ```

- To clean the build:

    ```bash
    make clean
    ```

- To run tests:

    ```bash
    # Requires curl and bash
    make test
    ```

## Structure

The application is structured as follows:

- `src/driver.cpp`: The main entry point of the application.
- `src/app`: Contains the application controllers and services for the frontend.
- `src/api`: Contains the API controllers and services.
- `src/core`: Contains core functionality such as environment configuration.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues to improve the application or suggest new features. I've only been programming for a couple years so this is a major 'work in progress'.

## License

This project is licensed under the [ISC License](LICENSE).

