#pragma once
#include <Codegen/TypeAlias.h>
#include <Codegen/BuiltinVariate.h>
#include <Codegen/CustomLibrary.h>
#include <HelixEngine/Util/TemplateString.hpp>
#include <HelixEngine/Render/Swizzle.hpp>
#include <HelixEngine/Math/Vector.hpp>

namespace helix::shader
{

	template<typename T> requires (std::is_arithmetic_v<T> || IsVector<T>::value)
	class Value
	{
		using AST = EmbeddedShader::Ast::AST;
		using ValueNode = EmbeddedShader::Ast::Value;
		using PH = EmbeddedShader::ParseHelper;
		using T_ = T::ValueType; // 仅限Vector

		template<typename Type> requires
			(std::is_arithmetic_v<Type> || IsVector<Type>::value)
		friend class Value;

		template<typename Type>
		friend class Swizzle2;
		template<typename Type>
		friend class Swizzle3;
		template<typename Type>
		friend class Swizzle4;
		template<typename Type,TemplateString NAME>
		friend class SwizzleValue;
	public:
		Value()
		{
			if (PH::isInShaderCodeLambda())
			{
				initLocal(nullptr);
				return;
			}

			if (PH::isInInputParameter())
			{
				node = AST::defineInputVariate<T>(PH::getCurrentInputIndex());
				return;
			}

			node = AST::defineUniformVariate<T>();
		}

		Value() requires IsVector<T>::value
		{
			if (PH::isInShaderCodeLambda())
			{
				initLocal(nullptr);
				return;
			}

			if (PH::isInInputParameter())
			{
				node = AST::defineInputVariate<typename T::KtmVec>(PH::getCurrentInputIndex());
				return;
			}

			node = AST::defineUniformVariate<typename T::KtmVec>();
		}

		explicit(false) Value(T value)
		{
			if (PH::isInShaderCodeLambda())
				initLocal(createValue(value));

			if (PH::isInInputParameter())
				node = AST::defineInputVariate<T>(PH::getCurrentInputIndex());

			node = AST::defineUniformVariate<T>();
		}

		explicit(false) Value(T value) requires IsVector<T>::value
		{
			if (PH::isInShaderCodeLambda())
				initLocal(createValue(value));

			if (PH::isInInputParameter())
				node = AST::defineInputVariate<typename T::KtmVec>(PH::getCurrentInputIndex());

			node = AST::defineUniformVariate<typename T::KtmVec>();
		}

		Value(Value&& other) noexcept = default;

		Value(const Value& other)
		{
			if (PH::isInShaderCodeLambda())
			{
				initLocal(other.node);
			}
		}

		Value(T_ x, T_ y) requires IsVector2<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(x, y)})
		{
		}

		Value(T_ x, T_ y, T_ z) requires IsVector3<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(x, y, z)})
		{
		}

		Value(Value<Vector2T<T_>> xy, T_ z) requires IsVector3<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(xy.node, z)})
		{
		}

		Value(T_ x, Value<Vector2T<T_>> yz) requires IsVector3<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(x, yz.node)})
		{
		}

		Value(T_ x, T_ y, T_ z, T_ w) requires IsVector4<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(x, y, z, w)})
		{
		}

		Value(Value<Vector2T<T_>> xy, T_ z, T_ w) requires IsVector4<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(xy.node, z, w)})
		{
		}

		Value(T_ x, Value<Vector2T<T_>> yz, T_ w) requires IsVector4<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(x, yz.node, w)})
		{
		}

		Value(T_ x, T_ y, Value<Vector2T<T_>> zw) requires IsVector4<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(x, y, zw.node)})
		{
		}

		Value(Value<Vector2T<T_>> xy, Value<Vector2T<T_>> zw) requires IsVector4<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(xy.node, zw.node)})
		{
		}

		Value(Value<Vector3T<T_>> xyz, T_ w) requires IsVector4<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(xyz.node, w)})
		{
		}

		Value(T_ x, Value<Vector3T<T_>> yzw) requires IsVector4<T>::value :
			Value(Value{AST::createVecValue<typename T::KtmVec>(x, yzw.node)})
		{
		}

		~Value()
		{
			if (node.use_count() == 1)
				AST::addLocalUniversalStatement(node);
		}

		// Operators

		Value& operator=(Value&& other) noexcept = default;
		Value& operator=(const Value& other)
		{
			if (this == &other) return *this;
			AST::assign(node, other.node);
			return *this;
		}
		Value& operator=(T value)
		{
			AST::assign(node, createValue(value));
			return *this;
		 }

		Value& operator++() requires std::is_arithmetic_v<T>
		{
			return Value{AST::unaryOperator(node,"++")};
		}

		Value operator++(int) requires std::is_arithmetic_v<T>
		{
			return Value{AST::unaryOperator(node,"++"),false};
		}

		// 前缀 --
		Value& operator--() requires std::is_arithmetic_v<T>
		{
			return Value{AST::unaryOperator(node,"--")};
		}

		// 后缀 --
		Value operator--(int) requires std::is_arithmetic_v<T>
		{
			return Value{AST::unaryOperator(node,"--"),false};
		}

		// 取反
		bool operator!() const requires std::is_arithmetic_v<T>
		{
			return Value{AST::unaryOperator(node,"!")};
		}

		// 按位取反
		Value operator~() const requires std::is_integral_v<T>
		{
			return Value{AST::unaryOperator(node,"~")};
		}

		// 负号
		Value operator-() const
		{
			return Value{AST::unaryOperator(node,"-")};
		}

		// 正号
		Value operator+() const
		{
			return Value{AST::unaryOperator(node,"+")};
		}

		auto operator->() const requires IsVector2<T>::value
		{
			return std::unique_ptr<Swizzle2<T_>>(new Swizzle2<T_>(node));
		}

		auto operator->() const requires IsVector3<T>::value
		{
			return std::unique_ptr<Swizzle3<T_>>(new Swizzle3<T_>(node));
		}

		auto operator->() const requires IsVector4<T_>::value
		{
			return std::unique_ptr<Swizzle4<T_>>(new Swizzle4<T_>(node));
		}

#define EMPTY
#define ARITHMETIC requires std::is_arithmetic_v<T>
#define VECTOR requires IsVector<T>::value
#define INTEGRAL requires std::is_integral_v<T>
#define BIN_OP_DEF(op, ret,rqe)\
ret operator op(const Value& value) const rqe\
{\
return ret(AST::binaryOperator(node,value.node,#op));\
}\
\
ret operator op(T value) const rqe\
{\
return ret(AST::binaryOperator(node,createValue(value),#op));\
}\
friend ret operator op(T value1,const Value<T>& value2) rqe\
{\
return ret(AST::binaryOperator(createValue(value1),value2.node,#op));\
}
#define RET_BOOL_BIN_OP_DEF(op,rqe) BIN_OP_DEF(op,Value<bool>, rqe)
		RET_BOOL_BIN_OP_DEF(==,EMPTY)
		RET_BOOL_BIN_OP_DEF(!=,EMPTY)
		RET_BOOL_BIN_OP_DEF(<,ARITHMETIC)
		RET_BOOL_BIN_OP_DEF(>,ARITHMETIC)
		RET_BOOL_BIN_OP_DEF(<=,ARITHMETIC)
		RET_BOOL_BIN_OP_DEF(>=,ARITHMETIC)
		RET_BOOL_BIN_OP_DEF(||,ARITHMETIC)
		RET_BOOL_BIN_OP_DEF(&&,ARITHMETIC)
#undef RET_BOOL_BIN_OP_DEF

#define DEFAULT_BIN_OP_DEF(op,rqe) BIN_OP_DEF(op,Value,rqe)
		DEFAULT_BIN_OP_DEF(+,EMPTY)
		DEFAULT_BIN_OP_DEF(-,EMPTY)
		DEFAULT_BIN_OP_DEF(*,ARITHMETIC)
		DEFAULT_BIN_OP_DEF(/,ARITHMETIC)
		DEFAULT_BIN_OP_DEF(%,INTEGRAL) //shader-lang 仅允许整数用%取整。浮点用mod
		DEFAULT_BIN_OP_DEF(&,INTEGRAL)
		DEFAULT_BIN_OP_DEF(|,INTEGRAL)
		DEFAULT_BIN_OP_DEF(^,INTEGRAL)
		DEFAULT_BIN_OP_DEF(<<,INTEGRAL)
		DEFAULT_BIN_OP_DEF(>>,INTEGRAL)
#undef DEFAULT_BIN_OP_DEF
#undef BIN_OP_DEF

#define COMBINED_ASSIGN_OP(op,rqe) \
	Value& operator op##= (const Value& other) rqe \
	{\
		return (*this = (((*this) op (other))));\
	}\
	Value& operator op##= (T value) rqe \
	{\
	return (*this = (((*this) op (value))));\
	}
		COMBINED_ASSIGN_OP(+,EMPTY)
		COMBINED_ASSIGN_OP(-,EMPTY)
		COMBINED_ASSIGN_OP(*,ARITHMETIC)
		COMBINED_ASSIGN_OP(/,ARITHMETIC)
		COMBINED_ASSIGN_OP(%,INTEGRAL)
		COMBINED_ASSIGN_OP(&,INTEGRAL)
		COMBINED_ASSIGN_OP(|,INTEGRAL)
		COMBINED_ASSIGN_OP(^,INTEGRAL)
		COMBINED_ASSIGN_OP(>>,INTEGRAL)
		COMBINED_ASSIGN_OP(<<,INTEGRAL)
#undef COMBINED_ASSIGN_OP
#undef ARITHMETIC
#undef VECTOR
#undef INTEGRAL
#undef EMPTY
	private:
		explicit Value(std::shared_ptr<ValueNode> node) : node(std::move(node)) {}
		void initLocal(std::shared_ptr<ValueNode> initValue)
		{
			node = AST::defineLocalVariate(AST::createType<T>(),std::move(initValue));
		}

		void initLocal(std::shared_ptr<ValueNode> initValue) requires IsVector<T>::value
		{
			node = AST::defineLocalVariate(AST::createType<typename T::KtmVec>(),std::move(initValue));
		}
		std::shared_ptr<EmbeddedShader::Ast::BasicType> createType() requires std::is_arithmetic_v<T>
		{
			return AST::createType<T>();
		}
		std::shared_ptr<EmbeddedShader::Ast::VecType> createType() requires IsVector<T>::value
		{
			return AST::createVecType<typename T::KtmVec>();
		}

		static std::shared_ptr<EmbeddedShader::Ast::BasicValue> createValue(T value) requires std::is_arithmetic_v<T>
		{
			return AST::createValue<T>(value);
		}

		static std::shared_ptr<EmbeddedShader::Ast::VecValue> createValue(const T& value) requires IsVector<T>::value
		{
			return AST::createValue<typename T::KtmVec>(typename T::KtmVec(value));
		}

		std::shared_ptr<ValueNode> node;
	};

	using namespace EmbeddedShader::TypeAlias;
	using namespace EmbeddedShader::BuiltinVariate;
	using namespace EmbeddedShader::CustomLibrary;
}
