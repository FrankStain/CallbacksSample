#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	template< auto CANDIDATE >
	using FunctionType = typename SignatureSelector<decltype( CANDIDATE )>::Signature;

	template< typename TLambda >
	using LambdaFunctionType = typename SignatureSelector<decltype( &TLambda::operator() )>::Signature;
}
}
}
