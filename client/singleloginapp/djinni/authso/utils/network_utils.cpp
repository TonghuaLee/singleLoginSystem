//
// Created by dw on 19-2-28.
//

#include "network_utils.h"

#include <string>

using namespace grpc;

using grpc::Status;



// TokenAuthenticator 用来支持令牌认证
// https://grpc.io/docs/guides/auth.html
class TokenAuthenticator : public grpc::MetadataCredentialsPlugin {
public:
    TokenAuthenticator(const std::string& token) : token_(token) {}

    grpc::Status GetMetadata(grpc::string_ref service_url, grpc::string_ref method_name, const grpc::AuthContext& channel_auth_context, std::multimap<grpc::string, grpc::string>* metadata) override {
        metadata->insert(std::make_pair("authorization-token", token_));
        return Status::OK;
    }

private:
    std::string token_;
};

std::shared_ptr<grpc::Channel> utils::NetworkUtils::channel = NULL;

std::shared_ptr<grpc::Channel> utils::NetworkUtils::getNetworkChannel(){

    if (channel != NULL){
        return channel;
    }

    const std::string MY_PEM = "-----BEGIN CERTIFICATE-----\n"
                               "MIIDoDCCAogCCQDCwTZmK/3aCzANBgkqhkiG9w0BAQUFADCBkTELMAkGA1UEBhMC\n"
                               "Q04xETAPBgNVBAgMCFNoZW5aaGVuMREwDwYDVQQHDAhTaGVuWmhlbjEOMAwGA1UE\n"
                               "CgwFaGFwcHkxDjAMBgNVBAsMBWhhcHB5MRswGQYDVQQDDBJ3d3cuaGFwcHlicm9h\n"
                               "ZC5jb20xHzAdBgkqhkiG9w0BCQEWEDkyOTIzMDE1N0BxcS5jb20wHhcNMjAwNjAz\n"
                               "MTAwNzQ5WhcNMzAwNjAxMTAwNzQ5WjCBkTELMAkGA1UEBhMCQ04xETAPBgNVBAgM\n"
                               "CFNoZW5aaGVuMREwDwYDVQQHDAhTaGVuWmhlbjEOMAwGA1UECgwFaGFwcHkxDjAM\n"
                               "BgNVBAsMBWhhcHB5MRswGQYDVQQDDBJ3d3cuaGFwcHlicm9hZC5jb20xHzAdBgkq\n"
                               "hkiG9w0BCQEWEDkyOTIzMDE1N0BxcS5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IB\n"
                               "DwAwggEKAoIBAQDLuJx6S20eeTd0q0wj+70Dpf+pSFzBTAghB2USk/t0INoenf+x\n"
                               "+JdMqgPaIfkoUOd1IeMXU8jjwrvmE3jCg92DxV3V10yetn0nOv+hEDEoss319f+m\n"
                               "jpCz6mzxGbb7rHNXDxVkdwxPMw7bNYeBRxktZBSHx/i4x4n1KO3y4kpQAt80kIhc\n"
                               "7H2leRDDL/gMTqclBSGEG4Gm0jWNJnO8UXZvib2JR8xCXSTA5KJJL8t2dxPVtdC+\n"
                               "apt5qcmLzlm08DK9pDvMfYdMD8dfpNTEZx7xeQGbKduw1RdV8ZJFhQ9qzunfRxlc\n"
                               "PEmEfe6lbRgZgZ1fXo2k1Sw1at18aFnPevgHAgMBAAEwDQYJKoZIhvcNAQEFBQAD\n"
                               "ggEBAGJL6OJUJdE5VoqZknlo3uWCjJV05FzHxzyTci6nVAO/w74Lg1OVCZe2WhK5\n"
                               "NY2Pi9lQyBrT86F/QSVIUsQ4ApOYmLPLVPEtFxZaHIPJ4cbVQ9I55zb9MjwkJ3fk\n"
                               "9UVLQt/t+LC/MacGuVhtu7Yh2kx2ll1uI5UlKxYoI9ncDqRT35VRR73HbzVYKkit\n"
                               "WplHWGUI5+ZzbL0cJLk38Cd3hhdJhvWNNP4WWVqFR6LX0stdY4/HX569j8esTt0J\n"
                               "K2hUdI63/3T3mL76bZ9siI81iSi2IJGAlWozg+zYl/XGvBB4lDYL4bj7SXXkgp5i\n"
                               "vU8fijkjeO9d8l1NyHBs/37wTIY="
                               "-----END CERTIFICATE-----";

    const std::string TOKEN = "happy-broad";

    const std::string host("118.126.89.19");
    const int port = 50051;

    const int host_port_buf_size = 1024;
    char host_port[host_port_buf_size];
    snprintf(host_port, host_port_buf_size, "%s:%d", host.c_str(), port);


    grpc::SslCredentialsOptions ssl_options;
    ssl_options.pem_root_certs = MY_PEM;

    // Create a default SSL ChannelCredentials object.
    auto channel_creds = grpc::SslCredentials(ssl_options);

    grpc::ChannelArguments cargs;
    cargs.SetSslTargetNameOverride(TOKEN); // common name

    auto call_creds = grpc::MetadataCredentialsFromPlugin(std::unique_ptr<grpc::MetadataCredentialsPlugin>(new TokenAuthenticator(TOKEN)));
    auto compsited_creds = grpc::CompositeChannelCredentials(channel_creds, call_creds);

    // Create a channel using the credentials created in the previous step.
    channel = grpc::CreateCustomChannel(host_port, compsited_creds, cargs);

    return channel;
}
