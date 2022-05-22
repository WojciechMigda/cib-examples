Minimal boilerplate code for ``CIB`` includes having definitions for a service, feature, and a component:

.. code-block:: c++

   struct my_service : public cib::callback_meta<some_arguments>{};

   struct my_component
   {
       static auto constexpr config =
           cib::config(cib::exports<my_service>);
   };

   struct my_feature
   {
       static auto constexpr config =
           cib::config(cib::extend<my_service>(some_callable));
   };

These must be defined in the global scope. It follows, that ``some_callable`` referenced in ``my_feature`` must be visible in the global scope, too, or possibly be a lambda.

As a consequence, any state that ``some_callable`` could be referencing must exist in the global scope, too.

While having global mutable scope is not uncommon in some embedded applications, it would be frowned upon in more general case.

With few tweaks we can circumvent the above, though.

The idea is that we can delegate state to some local object, which will be passed along with ``some_arguments``. We could use concrete type in place where we define ``my_service``, but for general case we will resort to generics, which will can also assist us when we turn to writing tests.

Templated versions of our service, feature, and component trio will look like this:

.. code-block:: c++

   template<typename Fn>
   struct my_service_T : public cib::callback_meta<Fn &, some_arguments>{};

   template<typename Fn>
   struct my_component_T
   {
        constexpr static auto config =
            cib::config(cib::exports<my_service_T<Fn>>);
   };

   template<typename Fn>
   struct my_feature_T
   {
       static auto constexpr config = cib::config(
           cib::extend<my_service_T<Fn>>(
               [](Fn & f, some_arguments)
               {
                   f(some_arguments);
               }
           )
       );
   };

In the above we introduce generic type ``Fn``, which will encapsulate state that we would like to associate with processing done by our ``my_feature``. Lambda we used imposes a requirement that ``Fn`` will have a call operator which accepts passed ``some_arguments``.

Approach described above is utilized in code example placed in this source subfolder.
This example illustrates simple use case, in which some module sends notification about executed order (through a provided callback), and this notification should reach another module, which will send an email to prospective recipients.

State associated with processing of the order execution notification (``Sender``) includes list of recipients, but in real world case could also include instances of ``boost`` io context, ``boost`` resolver, etc.

The callback captures ``Sender`` instance and passes it along with order indication to ``CIB`` nexus. This data ends up passed to the lambda we included in the definition of ``my_feature_T``.
