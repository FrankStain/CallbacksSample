#pragma once


namespace Demo
{
inline namespace Callbacks
{
	template< auto FUNCTION >
	inline auto GetCallback() -> std::enable_if_t<Internal::IS_GLOBAL_FUNCTION<FUNCTION>, Callback<Internal::FunctionSignature<FUNCTION>>>
	{
		return Callback<Internal::FunctionSignature<FUNCTION>>::template From<FUNCTION>();
	}

	template< auto FUNCTION, typename THost >
	inline auto GetCallback( THost* host ) -> std::enable_if_t<Internal::IS_MEMBER_FUNCTION<FUNCTION, THost>, Callback<Internal::FunctionSignature<FUNCTION>>>
	{
		return Callback<Internal::FunctionSignature<FUNCTION>>::template From<FUNCTION>( host );
	}

	template< typename TFunctor >
	inline auto GetCallback( TFunctor&& functor ) -> std::enable_if_t<Internal::IS_FUNCTOR<TFunctor>, Callback<Internal::FunctorSignature<TFunctor>>>
	{
		return Callback<Internal::FunctorSignature<TFunctor>>::template From( std::forward<TFunctor>( functor ) );
	}
}
}
