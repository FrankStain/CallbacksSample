#include <main.h>
#include <demo/callbacks/callbacks.h>


namespace Demo
{
	template< typename TResult, typename... TArguments >
	template< auto FUNCTION >
	inline Callback<TResult ( TArguments... )> Callback<TResult ( TArguments... )>::From()
	{
		Callback<TResult ( TArguments... )> result{};

		result.m_routine = &Internal::FunctionProxyBuilder<TResult, TArguments...>::template Proxy<FUNCTION>;

		return result;
	}

	template< typename TResult, typename... TArguments >
	template< auto FUNCTION, typename THost >
	inline Callback<TResult ( TArguments... )> Callback<TResult ( TArguments... )>::From( THost* host )
	{
		Callback<TResult ( TArguments... )> result{};

		result.m_routine = &Internal::MethodProxyBuilder<THost, TResult, TArguments...>::template Proxy<FUNCTION>;
		result.m_context = std::static_pointer_cast<void>( std::shared_ptr<THost>{ host, []( THost* ) {} } );

		return result;
	}

	template< typename TResult, typename... TArguments >
	template< typename THost >
	inline Callback<TResult ( TArguments... )> Callback<TResult ( TArguments... )>::From( [[maybe_unused]] THost&& host )
	{
		static_assert( Internal::IsCallable<THost>::value, "Only callable types allowed." );
		Callback<TResult ( TArguments... )> result{};

		if constexpr( std::is_empty_v<THost> )
		{
			result.m_routine = &Internal::EmptyLambdaProxyBuilder<TResult, TArguments...>::template Proxy<THost>;
		}
		else
		{
			result.m_routine = &Internal::MethodProxyBuilder<THost, TResult, TArguments...>::template Proxy<&THost::operator()>;
			result.m_context = std::static_pointer_cast<void>( std::make_shared<THost>( std::forward<THost>( host ) ) );
		}

		return result;
	}

	template< typename TResult, typename... TArguments >
	inline TResult Callback<TResult ( TArguments... )>::Call( TArguments... arguments ) const
	{
		if( IsEmpty() )
		{
			return TResult();
		}

		return m_routine( m_context.get(), std::forward<TArguments>( arguments )... );
	}

	template< typename TResult, typename... TArguments >
	inline void Callback<TResult ( TArguments... )>::Reset()
	{
		Callback temp{};
		swap( *this, temp );
	}

	template< typename TResult, typename... TArguments >
	inline void Callback<TResult ( TArguments... )>::Swap( Callback& other )
	{
		using std::swap;
		swap( m_routine, other.m_routine );
		swap( m_context, other.m_context );
	}


	// Строит коллбек из переданного указателя на функцию.
	template< auto FUNCTION >
	inline auto GetCallback() -> std::enable_if_t<Internal::IS_GLOBAL_FUNCTION<FUNCTION>, Callback<Internal::FunctionType<FUNCTION>>>
	{
		return Callback<Internal::FunctionType<FUNCTION>>::template From<FUNCTION>();
	}

	// Строит коллбек из переданного указателя на метод и хоста этого метода.
	template< auto FUNCTION, typename THost >
	inline auto GetCallback( THost* host ) -> std::enable_if_t<Internal::IS_MEMBER_FUNCTION<FUNCTION, THost>, Callback<Internal::FunctionType<FUNCTION>>>
	{
		return Callback<Internal::FunctionType<FUNCTION>>::template From<FUNCTION>( host );
	}

	// Строит коллбек из переданной лямбды.
	template< typename TFunctor >
	inline auto GetCallback( TFunctor&& functor ) -> std::enable_if_t<Internal::IS_FUNCTOR<TFunctor>, Callback<Internal::LambdaFunctionType<TFunctor>>>
	{
		return Callback<Internal::LambdaFunctionType<TFunctor>>::template From( std::forward<TFunctor>( functor ) );
	}
}


void PrintMe()
{
	std::cout << "PrintMe()" << std::endl;
}


class UseMe final
{
public:
	void PrintMe()
	{
		std::cout << "UseMe::PrintMe( " << m_payload << " )" << std::endl;
	}

public:
	int m_payload = 0;
};


int main()
{
	UseMe value{ 42 };

	Demo::Callback<void ()> cb;
	Demo::Callback<void ()> on_function{ Demo::GetCallback<&PrintMe>() };
	Demo::Callback<void ()> on_method{ Demo::GetCallback<&UseMe::PrintMe>( &value ) };
	Demo::Callback<void ()> on_trivial_lambda{ Demo::GetCallback( []() { std::cout << "From lambda!" << std::endl; } ) };
	Demo::Callback<void ()> on_closure_lambda{ Demo::GetCallback( [&value]() { std::cout << "From lambda! "; value.PrintMe(); } ) };

	cb();
	cb.Reset();

	on_function();
	on_method();
	on_trivial_lambda();
	on_closure_lambda();

	return 0;
}
