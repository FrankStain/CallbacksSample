#pragma once


namespace Demo
{
inline namespace Callbacks
{
	/**
		@brief	Produce the callback for given pointer to global function.

		This template will be selected only for valid pointers to global function.

		@tparam	FUNCTION	Pointer to valid global function.
		@return				The value returned is an callback with same signature as given function.
	*/
	template< auto FUNCTION >
	inline auto GetCallback() -> std::enable_if_t<Internal::IS_GLOBAL_FUNCTION<FUNCTION>, Callback<Internal::FunctionSignature<FUNCTION>>>
	{
		return Callback<Internal::FunctionSignature<FUNCTION>>::template From<FUNCTION>();
	}

	/**
		@brief	Produce the callback for given pointer to member function.

		This template will be selected only for valid pointers to member function.
		`host` will be not owned by callback.

		@param	host		Reference to valid host object.
		@tparam	FUNCTION	Pointer to valid member function.
		@tparam	THost		Type of host object for function call.
		@return				The value returned is an callback with same signature as given function.
	*/
	template< auto FUNCTION, typename THost >
	inline auto GetCallback( THost& host ) -> std::enable_if_t<Internal::IS_MEMBER_FUNCTION<FUNCTION, THost>, Callback<Internal::FunctionSignature<FUNCTION>>>
	{
		return Callback<Internal::FunctionSignature<FUNCTION>>::template From<FUNCTION>( host );
	}

	/**
		@brief	Produce the callback for given functor object.

		This template will be selected only for valid functors.
		`functor` will be moved, if possible, or copied and owned by callback.

		@tparam	TFunctor	Type of functor.
		@return				The value returned is an callback with same signature as given functor.
	*/
	template< typename TFunctor >
	inline auto GetCallback( TFunctor&& functor ) -> std::enable_if_t<Internal::IS_FUNCTOR<TFunctor>, Callback<Internal::FunctorSignature<TFunctor>>>
	{
		return Callback<Internal::FunctorSignature<TFunctor>>::template From( std::forward<TFunctor>( functor ) );
	}
}
}
