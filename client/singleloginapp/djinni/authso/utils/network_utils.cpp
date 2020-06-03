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
                               "MIIDljCCAn4CCQDCwTZmK/3aCjANBgkqhkiG9w0BAQUFADCBjDELMAkGA1UEBhMC\n "
                               "Q04xETAPBgNVBAgMCFNoZW5aaGVuMREwDwYDVQQHDAhTaGVuWmhlbjEOMAwGA1UE\n"
                               "CgwFaGFwcHkxDjAMBgNVBAsMBWhhcHB5MRYwFAYDVQQDDA13d3cuaGFwcHkuY29t\n"
                               "MR8wHQYJKoZIhvcNAQkBFhA5MjkyMzAxNTdAcXEuY29tMB4XDTIwMDYwMzA4NTcy\n"
                               "MFoXDTMwMDYwMTA4NTcyMFowgYwxCzAJBgNVBAYTAkNOMREwDwYDVQQIDAhTaGVu\n"
                               "WmhlbjERMA8GA1UEBwwIU2hlblpoZW4xDjAMBgNVBAoMBWhhcHB5MQ4wDAYDVQQL\n"
                               "DAVoYXBweTEWMBQGA1UEAwwNd3d3LmhhcHB5LmNvbTEfMB0GCSqGSIb3DQEJARYQ\n"
                               "OTI5MjMwMTU3QHFxLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n"
                               "AMu4nHpLbR55N3SrTCP7vQOl/6lIXMFMCCEHZRKT+3Qg2h6d/7H4l0yqA9oh+ShQ\n"
                               "53Uh4xdTyOPCu+YTeMKD3YPFXdXXTJ62fSc6/6EQMSiyzfX1/6aOkLPqbPEZtvus\n"
                               "c1cPFWR3DE8zDts1h4FHGS1kFIfH+LjHifUo7fLiSlAC3zSQiFzsfaV5EMMv+AxO\n"
                               "pyUFIYQbgabSNY0mc7xRdm+JvYlHzEJdJMDkokkvy3Z3E9W10L5qm3mpyYvOWbTw\n"
                               "Mr2kO8x9h0wPx1+k1MRnHvF5AZsp27DVF1XxkkWFD2rO6d9HGVw8SYR97qVtGBmB\n"
                               "nV9ejaTVLDVq3XxoWc96+AcCAwEAATANBgkqhkiG9w0BAQUFAAOCAQEAbmU0H9PV\n"
                               "67/KlnoE9LnuY6m9j+QJlN5iU17QRJ8gr+OVQC9/dhSYhYggMG6I/X7jTCxw2I1E\n"
                               "klXApoSaAP8Zv3qWfSwBOzUtlXPuugsB8ZwIt8lwVPq79u0dNRAlcMymVunjzgBq\n"
                               "B9IBPLrhuMpzxExj0v3B8Tpta2IOa72yr1h/niyZGPssGRU5MLEnFNCc6Nw6OXG6\n"
                               "u6aGi/jPePMR3Ji4poFeQgeVEeCQ/gkFgeUWO7/3lqG3iTwadXIb6rdQ3+CcnJv7\n"
                               "S31ruNVZeNjuHBp3s95Xl4VP2sxLQKKlN0mPh0f1OfYYlxjflKEg/TeeYbEmGSJx\n"
                               "JA39aArgKsNQMA=="
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
