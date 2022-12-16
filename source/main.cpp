#include <main.h>
#include <demo/callbacks/callbacks.h>


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
	Demo::Callback<void ()> on_method{ Demo::GetCallback<&UseMe::PrintMe>( value ) };
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
