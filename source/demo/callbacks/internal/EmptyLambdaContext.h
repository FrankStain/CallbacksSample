#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	/**
		@brief	Callback execution context for empty lambdas.

		Context is used by callback instances to produce the proxy function for certain empty lambda.
		`AdaptedCall` inner template adapts the given type of lambda to common interface.
		Since the only empty lambdas used here, there is no need to operate with instances of lambda.

		@tparam	TResult		Result of function.
		@tparam	TArguments	List of function parameters.
	*/
	template< typename TResult, typename... TArguments >
	struct EmptyLambdaContext final
	{
		/**
			@brief	Adapter-function for given type of lambda.

			Template adapts the given lambda to required interface of callback routine.
			It remembers only type of lambda on instantiation. Empty lambdas are stateless, so the first argument here remains unused.

			@tparam	TLambda	Type of lambda to adapt.
		*/
		template< typename TLambda >
		static inline TResult AdaptedCall( void* const, TArguments... arguments )
		{
			// Pre C++20 fix: prepare the dummy object, since the objects of lambda types can't be created manually.
			alignas( TLambda ) std::byte buffer[ sizeof( TLambda ) ] {};
			auto& lambda = *reinterpret_cast<TLambda*>( buffer );

			// FIXME: C++20. `TLambda{}` may be used here instead of `lambda`.
			return lambda( std::forward<TArguments>( arguments )... );
		};
	};
}
}
}
