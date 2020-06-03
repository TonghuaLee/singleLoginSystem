//
// Created by dw on 19-2-28.
//

#include "network_utils.h"

#include <string>

using namespace grpc;

using grpc::Status;

// TokenAuthenticator 用来支持令牌认证
// https://grpc.io/docs/guides/auth.html
class TokenAuthenticator : public grpc::MetadataCredentialsPlugin
{
public:
    TokenAuthenticator(const std::string &token) : token_(token) {}

    grpc::Status GetMetadata(grpc::string_ref service_url, grpc::string_ref method_name, const grpc::AuthContext &channel_auth_context, std::multimap<grpc::string, grpc::string> *metadata) override
    {
        metadata->insert(std::make_pair("authorization-token", token_));
        return Status::OK;
    }

private:
    std::string token_;
};

std::shared_ptr<grpc::Channel> utils::NetworkUtils::channel = NULL;

std::shared_ptr<grpc::Channel> utils::NetworkUtils::getNetworkChannel()
{

    if (channel != NULL)
    {
        return channel;
    }

    const std::string MY_PEM = "-----BEGIN CERTIFICATE-----\n"
                               "MIIDmTCCAoECCQDCwTZmK/3aDDANBgkqhkiG9w0BAQUFADCBjzELMAkGA1UEBhMC\n"
                               "Q04xETAPBgNVBAgMCFNoZW5aaGVuMREwDwYDVQQHDAhTaGVuWmhlbjERMA8GA1UE\n"
                               "CgwIU2hlblpoZW4xDjAMBgNVBAsMBWhhcHB5MRYwFAYDVQQDDA13d3cuaGFwcHku\n"
                               "Y29tMR8wHQYJKoZIhvcNAQkBFhA5MjkyMzAxNTdAcXEuY29tMB4XDTIwMDYwMzE0\n"
                               "MTQyOFoXDTMwMDYwMTE0MTQyOFowgYwxCzAJBgNVBAYTAkNOMREwDwYDVQQIDAhT\n"
                               "aGVuWmhlbjERMA8GA1UEBwwIU2hlblpoZW4xDjAMBgNVBAoMBWhhcHB5MQ4wDAYD\n"
                               "VQQLDAVoYXBweTEWMBQGA1UEAwwNd3d3LmhhcHB5LmNvbTEfMB0GCSqGSIb3DQEJ\n"
                               "ARYQOTI5MjMwMTU3QHFxLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\n"
                               "ggEBAMu4nHpLbR55N3SrTCP7vQOl/6lIXMFMCCEHZRKT+3Qg2h6d/7H4l0yqA9oh\n"
                               "+ShQ53Uh4xdTyOPCu+YTeMKD3YPFXdXXTJ62fSc6/6EQMSiyzfX1/6aOkLPqbPEZ\n"
                               "tvusc1cPFWR3DE8zDts1h4FHGS1kFIfH+LjHifUo7fLiSlAC3zSQiFzsfaV5EMMv\n"
                               "+AxOpyUFIYQbgabSNY0mc7xRdm+JvYlHzEJdJMDkokkvy3Z3E9W10L5qm3mpyYvO\n"
                               "WbTwMr2kO8x9h0wPx1+k1MRnHvF5AZsp27DVF1XxkkWFD2rO6d9HGVw8SYR97qVt\n"
                               "GBmBnV9ejaTVLDVq3XxoWc96+AcCAwEAATANBgkqhkiG9w0BAQUFAAOCAQEAT+Fb\n"
                               "rUztdFXplnxr6dhHoe6gKOohg7jJDmq1ivP93xInR3M8BJS+CAOpMQ9w+Nufu5mt\n"
                               "+KUKgPObZZYvW/35r6YWsCRkPCgMFLX0Oys7kW907wve0manypuusnQnXel/Tl8Z\n"
                               "tUAys8r1JzIbSJOLbKiqCjVsyLUypapKV14rtQ/P5YUYsnGMX8xabw5W7pbkO6A1\n"
                               "y4/Tqb4ldnBSql+FfgqZ0NK29nHLwt0ZKT7F8kU1Ip0/w+8QNEmLo2J+dUx7Uuy8\n"
                               "SyWqHgwJnPy6q+e9IUqm2RdVjdL8ClnQmI6T0Yy/qA+X7GYMfep9732jXlKq0IWo\n"
                               "QD5cD8LnEwkyZXmj5g==\n"
                               "-----END CERTIFICATE-----";

    const std::string TOKEN = "www.happy.com";

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
