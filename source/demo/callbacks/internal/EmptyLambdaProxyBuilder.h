#pragma once


namespace Deom
{
namespace Callbacks
{
namespace Internal
{
	template< typename TResult, typename... TArguments >
	struct EmptyLambdaProxyBuilder final
	{
		template< typename TLambda >
		static inline TResult Proxy( void* const, TArguments... arguments )
		{
			// Pre C++20 fix: prepare the dummy object, since the objects of lambda types can't be created manually.
			alignas( TLambda ) std::byte buffer[ sizeof( TLambda ) ] {};
			auto& lambda = *reinterpret_cast<TLambda*>( buffer );

			return lambda( std::forward<TArguments>( arguments )... );
		};
	};
}
}
}
