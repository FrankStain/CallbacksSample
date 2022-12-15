#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	template< auto CANDIDATE >
	using FunctionType = typename SignatureTypePolicy<decltype( CANDIDATE )>::Signature;

	template< typename TLambda >
	using LambdaFunctionType = typename SignatureTypePolicy<decltype( &TLambda::operator() )>::Signature;
}
}
}
