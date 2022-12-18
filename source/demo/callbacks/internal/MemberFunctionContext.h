#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	/**
		@brief	Callback execution context for member functions.

		Context is used by callback instances to produce the proxy function for certain member function.
		`AdaptedCall` inner templates are used to adapt the given member function to common interface.
		First argument of `AdaptedCall` accept the instance of type `THost` - the host object for member function to be called.

		@tparam	THost		Type of host class, which objects accept the function call.
		@tparam	TResult		Result of function.
		@tparam	TArguments	List of function parameters.
	*/
	template< typename THost, typename TResult, typename... TArguments >
	struct MemberFunctionContext final
	{
		/**
			@brief	Adapter-function for given pointer to member function.

			Template adapts the given function to required interface of callback routine.
			Actual function is remembered due to instantiation. First argument here represents the host object, which will be used to call the function.

			@tparam	METHOD	Pointer to actual member function being called on adapter call.
		*/
		template< TResult (THost::*METHOD)( TArguments... ) >
		static inline TResult AdaptedCall( void* const context, TArguments... arguments )
		{
			THost* const host = reinterpret_cast<THost*>( context );
			return (host->*METHOD)( std::forward<TArguments>( arguments )... );
		};

		/**
			@brief	Adapter-function for given pointer to member function.

			Template adapts the given function to required interface of callback routine.
			Actual function is remembered due to instantiation. First argument here represents the host object, which will be used to call the function.

			@tparam	METHOD	Pointer to actual member function being called on adapter call.
		*/
		template< TResult (THost::*METHOD)( TArguments... ) const >
		static inline TResult AdaptedCall( void* const context, TArguments... arguments )
		{
			THost* const host = reinterpret_cast<THost*>( context );
			return (host->*METHOD)( std::forward<TArguments>( arguments )... );
		};

		/**
			@brief	Adapter-function for given pointer to member function.

			Template adapts the given function to required interface of callback routine.
			Actual function is remembered due to instantiation. First argument here represents the host object, which will be used to call the function.

			@tparam	METHOD	Pointer to actual member function being called on adapter call.
		*/
		template< TResult (THost::*METHOD)( TArguments... ) volatile >
		static inline TResult AdaptedCall( void* const context, TArguments... arguments )
		{
			THost* const host = reinterpret_cast<THost*>( context );
			return (host->*METHOD)( std::forward<TArguments>( arguments )... );
		};

		/**
			@brief	Adapter-function for given pointer to member function.

			Template adapts the given function to required interface of callback routine.
			Actual function is remembered due to instantiation. First argument here represents the host object, which will be used to call the function.

			@tparam	METHOD	Pointer to actual member function being called on adapter call.
		*/
		template< TResult (THost::*METHOD)( TArguments... ) const volatile >
		static inline TResult AdaptedCall( void* const context, TArguments... arguments )
		{
			THost* const host = reinterpret_cast<THost*>( context );
			return (host->*METHOD)( std::forward<TArguments>( arguments )... );
		};
	};
}
}
}
