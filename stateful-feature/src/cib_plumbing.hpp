#pragma once

#ifndef STATEFUL_FEATURE_SRC_CIB_PLUMBING_HPP_
#define STATEFUL_FEATURE_SRC_CIB_PLUMBING_HPP_


#include "cib/cib.hpp"
#include "cib/callback.hpp"
#include "cib/config.hpp"
#include "cib/nexus.hpp"


struct ORDER_IND;


template<typename Fn>
struct order_ind_svc_T : public cib::callback_meta<Fn &, ORDER_IND const &>{};

template<typename Fn>
struct order_ind_component_T
{
    constexpr static auto config =
        cib::config(cib::exports<order_ind_svc_T<Fn>>);
};

template<typename Fn>
struct order_ind_feat_T
{
    static auto constexpr config = cib::config(
        cib::extend<order_ind_svc_T<Fn>>(
            [](Fn & f, ORDER_IND const & m)
            {
                f(m);
            }
        )
    );
};


#endif /* STATEFUL_FEATURE_SRC_CIB_PLUMBING_HPP_ */
