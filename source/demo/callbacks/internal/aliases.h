#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	template< auto CANDIDATE >
	using FunctionSignature = typename SignatureSelector<decltype( CANDIDATE )>::Signature;

	template< typename TLambda >
	using FunctorSignature = typename SignatureSelector<decltype( &TLambda::operator() )>::Signature;
}
}
}
