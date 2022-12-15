#include <main.h>


namespace Demo
{
// Приватный стафф.
namespace Internal
{
	// Опознание функциональных объектов. Общая форма, ложное значение.
	template< typename, typename = void >
	struct IsCallable final : std::false_type {};

	// Опознание функциональных объектов. Проверка на функциональный объект.
	template< typename TCandidate >
	struct IsCallable<TCandidate, std::void_t<decltype( (void)&TCandidate::operator() )>> final : std::is_class<TCandidate> {};

	// Опознание глобальных функций. Обща форма, ложное значение.
	template< typename >
	struct IsGlobalFunction final : std::false_type {};

	// Опознание глобальных функций. Проверка свойств глобальной функции.
	template< typename TResult, typename... TArguments >
	struct IsGlobalFunction<TResult (*)( TArguments... )> final : std::true_type {};

	// Опознание методов. Общая форма, ложное значение.
	template< typename, typename >
	struct IsMemberFunction final : std::false_type {};

	// Опознание методов. Проверка свойств рядового метода.
	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ), THost> final : std::true_type {};

	// Опознание методов. Проверка свойств метода с константным контекстом.
	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ) const, THost> final : std::true_type {};

	// Опознание методов. Проверка свойств метода с волатильным контекстом.
	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ) volatile, THost> final : std::true_type {};

	// Опознание методов. Проверка свойств метода с константным волатильным контекстом.
	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ) const volatile, THost> final : std::true_type {};


	// Вывод сигнатуры коллбека для произвольного кандидата.
	template< auto CANDIDATE >
	using FunctionType = typename SignatureTypePolicy<decltype( CANDIDATE )>::Signature;

	// Вывод сигнатуры коллбека для лямбды.
	template< typename TLambda >
	using LambdaFunctionType = typename SignatureTypePolicy<decltype( &TLambda::operator() )>::Signature;


	// Является ли переданный кандидат глобальной функцией?
	template< auto CANDIDATE >
	inline constexpr bool IS_GLOBAL_FUNCTION = IsGlobalFunction<decltype( CANDIDATE )>::value;

	// Является ли переданный кандидат методом?
	template< auto CANDIDATE, typename THost >
	inline constexpr bool IS_MEMBER_FUNCTION = IsMemberFunction<decltype( CANDIDATE ), THost>::value;

	// Является ли переданный кандидат лямбдой?
	template< typename TCandidate >
	inline constexpr bool IS_FUNCTOR = IsCallable<TCandidate>::value;
}


	// Общая форма коллбека.
	template< typename TDeclaration >
	class Callback;

	// Единственно верная ветвь вывода коллбека.
	template< typename TResult, typename... TArguments >
	class Callback<TResult ( TArguments... )> final
	{
	// Дружественный интерфейс.
	public:
		// Проверка на пустоту.
		friend inline const bool empty( const Callback& callback )	{ return callback.IsEmpty(); };

		// Обмен состояниями.
		friend inline void swap( Callback& left, Callback& right )	{ left.Swap( right ); };

	// Фабричные функции.
	public:
		// Делает коллбек из указателя на глобальную функцию.
		template< auto FUNCTION >
		static inline Callback From();

		// Делает коллбек из указателя на метод и указателя на хост метода.
		template< auto FUNCTION, typename THost >
		static inline Callback From( THost* host );

		// Делает коллбек из предположительно лямбды.
		template< typename THost >
		static inline Callback From( THost&& host );

	// Управление временем жизни.
	public:
		inline Callback() noexcept						= default;
		inline Callback( const Callback& ) noexcept		= default;
		inline Callback( Callback&& other ) noexcept	: m_routine{ std::exchange( other.m_routine, nullptr ) }, m_context{ std::move( other.m_context ) } {};
		inline ~Callback() noexcept						= default;


		inline Callback& operator = ( const Callback& ) noexcept	= default;
		inline Callback& operator = ( Callback&& other ) noexcept	{ Callback{ std::move( other ) }.Swap( *this ); return *this; };

	// Публичный интерфейс.
	public:
		// Точка входа коллбека. Может использоваться для связи коллбеков в цепь.
		inline TResult Call( TArguments... arguments ) const;


		// Сбросить состояние, очистить коллбек.
		inline void Reset();

		// Обменять состояниями с другим коллбеком.
		inline void Swap( Callback& other );


		// Является ли этот коллбек пустым?
		inline const bool IsEmpty() const								{ return m_routine == nullptr; };

	// Операторы.
	public:
		inline TResult operator () ( TArguments... arguments ) const	{ return Call( std::forward<TArguments>( arguments )... ); };


		inline operator const bool () const								{ return !IsEmpty(); };
		inline const bool operator ! () const							{ return IsEmpty(); };


		inline const bool operator == ( const Callback& other ) const	{ return ( m_routine == other.m_routine ) && ( m_context == other.m_context ); };
		inline const bool operator != ( const Callback& other ) const	{ return ( m_routine != other.m_routine ) || ( m_context != other.m_context ); };

	// Приватные дочерние типы.
	private:
		// Сигнатура функции делегата.
		using Routine = TResult (*)( void* const, TArguments... );

	// Приватная часть состояния.
	private:
		Routine					m_routine = nullptr;	// Функция представления делегата.
		std::shared_ptr<void>	m_context;				// Контекст делегата.
	};

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
