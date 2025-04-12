# Webserv

Webserv is easy version nginx.

## implementation detail
* used epoll as I/O Multipulexing
* can handle HTML content, file, redirect and CGI request
* can parse nginx like configuration files (format is [here](/.github/default.conf))
* acceptable requests are listed [here](/.github/acceptableRequest.txt)
* can handle cookies in header

## build
```bash
make
```
or
```bash
# if you need debug messages
make debug
```
![alt text](/.github/debug_message.png)

## development

## reference
* https://nginx.org/
* B. Stroustrup: The C++ Programming Language (Fourth Edition)
