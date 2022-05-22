#include "cib_plumbing.hpp"

#include "cib/cib.hpp"
#include "cib/callback.hpp"
#include "cib/config.hpp"
#include "cib/nexus.hpp"

#include "fmt/printf.h"

#include <string>
#include <cstdlib>
#include <vector>
#include <utility>


struct ORDER_IND
{
    std::string symbol;
    std::string rate;
    std::string size;
    std::string direction;
};


struct Sender
{
    std::vector<std::string> m_recipients;

    explicit Sender(std::vector<std::string> && recipients)
    : m_recipients(std::move(recipients))
    {}

    void operator()(ORDER_IND const & ind)
    {
        for (auto const & recipient : m_recipients)
        {
            fmt::print("Sending e-mail to {}. Order {} {} {} {}\n",
                recipient, ind.symbol, ind.rate, ind.size, ind.direction
            );
        }
    }
};


using order_ind_svc = order_ind_svc_T<Sender>;
using order_ind_component = order_ind_component_T<Sender>;
using order_ind_feat = order_ind_feat_T<Sender>;

struct project
{
    static constexpr auto config =
        cib::components<order_ind_feat, order_ind_component>;
};

cib::nexus<project> nexus{};


int main(int argc, char **argv)
{
    Sender sender({"john@acme.org"});

    auto callback = [&sender]()
    {
        nexus.service<order_ind_svc>(
            sender,
            ORDER_IND{"EUR/USD", "1.1", "100", "buy"}
        );
    };

    callback();

    return EXIT_SUCCESS;
}
