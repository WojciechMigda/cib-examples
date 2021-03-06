#include "cib/cib.hpp"
#include "cib/callback.hpp"
#include "cib/config.hpp"
#include "cib/nexus.hpp"

#include "fmt/printf.h"

#include <string>
#include <cstdlib>


/*
 * This is how we define interface of an access point.
 * Here we say that an API for `publish_ticker` will accept two string
 * arguments. Later, we will be able to invoke a call to `publish_ticker`
 * and the arguments will be passed to registered listeners.
 */
struct publish_ticker : public cib::callback_meta<std::string, std::string>{};

/*
 * This is the definition of a service built on top of `publish_ticker`
 * interface. We will use it to bundle it together with other components
 * into single project.
 */
struct ticker_service
{
    constexpr static auto config =
        cib::config(cib::exports<publish_ticker>);
};

/*
 * This is how we define listener for `publish_ticker` interface. Argument
 * to `cib::extend` used here is simply a lambda, with arguments that match
 * those required by `publish_ticker`.
 */
struct ticker_processor
{
    constexpr static auto config =
        cib::config(
            cib::extend<publish_ticker>(
                [](std::string symbol, std::string rate)
                {
                    fmt::print("ticker_processor: {} {}\n", symbol, rate);
                }
            )
        );
};


/*
 * Here we bind endpoints that together form the project. In this simple case
 * we have a single access point and a single listener.
 *
 * `ticker_service[publish_ticker]` --> `ticker_processor`
 */
struct project
{
    static constexpr auto config =
        cib::components<ticker_service, ticker_processor>;
};

/*
 * With this construct we initialize instance of nexus, which will route data
 * between registered access points and listeners.
 */
cib::nexus<project> nexus{};


int main(int argc, char **argv)
{
    /*
     * If we initialize `nexus` at runtime like this it enables us to later
     * execute calls to access points with `cib:service<>` without having
     * to own reference to a `nexus` instance.
     */
    nexus.init();

    /*
     * Here we make a call to the `publish_ticker` access point we defined
     * earlier and which became part of the `nexus`.
     * The two string arguments will be received by all `publish_ticker`
     * listeners.
     */
    cib::service<publish_ticker>("EUR/USD", "1.10");

    return EXIT_SUCCESS;
}
