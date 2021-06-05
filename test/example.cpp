#include <iostream>
#include <memory>
#include <string>
#include <system_error>
#include <functional>
#include <experimental/net>
#include <html.hpp>

namespace net = std::experimental::net;
using tcp = net::ip::tcp;

class server
{
  using handler_t = std::function<void(tcp::iostream&&)>;

  public:
    server(int port) : acceptor_(ctx_)
    {
        tcp::endpoint endpoint(tcp::v4(), port);
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
        accept();
    }

    void run(handler_t handler)
    {
        handler_ = handler;
        ctx_.run();
    }

  private:
    void accept()
    {
        acceptor_.async_accept(
            ctx_,
            [this](std::error_code ec, tcp::socket socket)
            {
                tcp::iostream stream(std::move(socket));

                /* ignore request */
                bool status = true;
                std::string line;
                do {
                    std::getline(stream, line, '\n');
                    line.erase(line.size() - 1);
                    if (status)
                    {
                        std::cout << line << std::endl;
                        status = false;
                    }
                } while (!line.empty());

                /* send basic status / headers */
                stream << "HTTP/1.1 200 Ok\r\n"
                    << "Content-type: text/html\r\n"
                    << "\r\n";

                /* invoke handler */
                handler_(std::move(stream));
    
                accept();
            }
        );
    }

    net::io_context ctx_;
    tcp::acceptor acceptor_;
    handler_t handler_;
};

int
main(int argc, char **argv)
{
    int port = argc > 1 ? std::atoi(argv[1]) : 8080;
    server server(port);

    std::cout << "starting server at "
        "http://localhost:" << port << "/" << std::endl;

    server.run([](auto&& stream) {
        stream << html::indent;
        stream << html::html
            << html::head
            << html::script[html::language("javascript")]
            << "function sayHello() {" << std::endl
            << "  document.getElementById('message').innerHTML = 'Hello, world';" << std::endl
            << "}" << std::endl
            << html::head::end
            << html::body[html::attr("onload", "sayHello()")]
            << html::div[html::id("message")]
            << html::end;
    });

    return 0;
}
