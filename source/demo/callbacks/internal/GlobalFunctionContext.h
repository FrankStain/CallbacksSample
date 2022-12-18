#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	/**
		@brief	Callback execution context for global functions.

		Context is used by callback instances to produce the proxy function for certain global function.
		`AdaptedCall` inner template adapts the given global function to common interface.

		@tparam	TResult		Result of function.
		@tparam	TArguments	List of function parameters.
	*/
	template< typename TResult, typename... TArguments >
	struct GlobalFunctionContext final
	{
		/**
			@brief	Adapter-function for given pointer to global function.

			Template adapts the given function to required interface of callback routine.
			Actual function is remembered due to instantiation. First argument does not used here due to specific of global functions.

			@tparam	FUNCTION	Pointer to actual global function being called on adapter call.
		*/
		template< TResult (*FUNCTION)( TArguments... ) >
		static inline TResult AdaptedCall( void* const, TArguments... arguments )
		{
			return FUNCTION( std::forward<TArguments>( arguments )... );
		};
	};
}
}
}
