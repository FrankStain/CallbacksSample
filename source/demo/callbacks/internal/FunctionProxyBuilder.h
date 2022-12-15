#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	template< typename TResult, typename... TArguments >
	struct FunctionProxyBuilder final
	{
		template< TResult (*FUNCTION)( TArguments... ) >
		static inline TResult Proxy( void* const, TArguments... arguments )
		{
			return FUNCTION( std::forward<TArguments>( arguments )... );
		};
	};
}
}
}
