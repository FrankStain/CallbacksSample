#include <main.h>
#include <demo/callbacks/callbacks.h>


namespace Demo
{
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
