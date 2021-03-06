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
 *
 * Note that compared to our basic ticker example, where both string arguments
 * were passed by value, here we changed it to have the second argument passed
 * by const reference. The same change had to be made to signatures of all
 * `publish_ticker` listeners.
 */
struct publish_ticker : public cib::callback_meta<std::string, std::string const &>{};

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
                [](std::string symbol, std::string const & rate)
                {
                    fmt::print("ticker_processor: {} {}\n", symbol, rate);
                }
            )
        );
};

/*
 * We can have more than one listener for a single access point.
 * Here we define a logger service for `publish_ticker`.
 */
struct ticker_logger
{
    constexpr static auto config =
        cib::config(
            cib::extend<publish_ticker>(
                [](std::string symbol, std::string const & rate)
                {
                    fmt::print("ticker_logger: {} {}\n", symbol, rate);
                }
            )
        );
};


/*
 * In addition to `publish_ticker` access point we will add another one:
 * `publish_spread`. It will require passing user-defined structure to convey
 * a spread payload.
 */
struct spread_ind
{
    std::string symbol;
    std::string bid;
    std::string ask;
};
struct publish_spread : public cib::callback_meta<spread_ind const &>{};


/*
 * Just like with `publish_ticker`, for `publish_spread` we also need to define
 * associated service.
 */
struct spread_service
{
    constexpr static auto config =
        cib::config(cib::exports<publish_spread>);
};

/*
 * In a similar fashion we will create a listener for `publish_spread` service.
 */
struct spread_processor
{
    constexpr static auto config =
        cib::config(
            cib::extend<publish_spread>(
                [](spread_ind const & ind)
                {
                    fmt::print("spread_processor: {} {} {}\n",
                        ind.symbol, ind.bid, ind.ask);
                }
            )
        );
};


/*
 * Here we bind endpoints that together form the project. In this extended case
 * we have a `publish_ticker` access point with two listeners, and
 * `publish_spread` access point with a single listener.
 *
 * `ticker_service[publish_ticker]` --> `ticker_processor`
 *                                  +-> `ticker_logger`
 * `spread_service[publish_spread]` --> `spread_processor`
 */
struct project
{
    static constexpr auto config =
        cib::components<
            ticker_service
            , ticker_processor
            , ticker_logger
            , spread_service
            , spread_processor
        >;
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
     * Note, that the order in which listeners' code will be invoked
     * is implementation-specific.
     */
    cib::service<publish_ticker>("EUR/USD", "1.10");

    /*
     * Another call, this time to `publish_spread`, will propagate spread
     * data to `publish_spread` listeners.
     */
    spread_ind ind{"USD/JPY", "111.8", "111.9"};
    cib::service<publish_spread>(ind);

    return EXIT_SUCCESS;
}
