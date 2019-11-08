#include <boost/beast.hpp>
#include <boost/log/trivial.hpp>

#include "Downloader.h"
#include "RootCertificates.h"

Downloader::Downloader(std::string host, std::string port)
    : _host(std::move(host)),
      _port(std::move(port))
{
    load_root_certificates(sslContext);

    if (!SSL_set_tlsext_host_name(stream.native_handle(), _host.c_str())) {
        boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
        throw boost::system::system_error{ec};
    }

    resolverResults = resolver.resolve(_host, _port);

    BOOST_LOG_TRIVIAL(debug) << "Downloader connected to " << _host << " " << _port;
}

std::string Downloader::download(const std::string &url, int httpVersion)
{
    namespace asio = boost::asio;
    namespace http = boost::beast::http;

    asio::connect(stream.next_layer(), resolverResults.begin(), resolverResults.end());
    stream.handshake(ssl::stream_base::client);

    http::request<http::string_body> request{http::verb::get, url, httpVersion};
    request.set(http::field::host, _host);
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    boost::beast::flat_buffer buffer{};
    http::response<http::dynamic_body> response{};

    http::write(stream, request);
    http::read(stream, buffer, response);

    BOOST_LOG_TRIVIAL(debug) << "Download " << _host << " " << _port << " " << url;

    // Convert to std::string
    return boost::beast::buffers_to_string(response.body().data());
}
