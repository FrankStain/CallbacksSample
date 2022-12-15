#pragma once


namespace Demo
{
inline namespace Callbacks
{
	template< auto FUNCTION >
	inline auto GetCallback() -> std::enable_if_t<Internal::IS_GLOBAL_FUNCTION<FUNCTION>, Callback<Internal::FunctionType<FUNCTION>>>
	{
		return Callback<Internal::FunctionType<FUNCTION>>::template From<FUNCTION>();
	}

	template< auto FUNCTION, typename THost >
	inline auto GetCallback( THost* host ) -> std::enable_if_t<Internal::IS_MEMBER_FUNCTION<FUNCTION, THost>, Callback<Internal::FunctionType<FUNCTION>>>
	{
		return Callback<Internal::FunctionType<FUNCTION>>::template From<FUNCTION>( host );
	}

	template< typename TFunctor >
	inline auto GetCallback( TFunctor&& functor ) -> std::enable_if_t<Internal::IS_FUNCTOR<TFunctor>, Callback<Internal::LambdaFunctionType<TFunctor>>>
	{
		return Callback<Internal::LambdaFunctionType<TFunctor>>::template From( std::forward<TFunctor>( functor ) );
	}
}
}
