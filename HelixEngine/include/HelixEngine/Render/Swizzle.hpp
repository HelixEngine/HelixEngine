#pragma once
#include <HelixEngine/Math/Vector.hpp>

namespace helix::shader
{
	template<typename T> requires (std::is_arithmetic_v<T> || IsVector<T>::value)
	class Value;

	template<typename T, TemplateString NAME>
	class SwizzleValue
	{
		using AST = EmbeddedShader::Ast::AST;
		using ValueNode = EmbeddedShader::Ast::Value;
		template<typename Type> requires (std::is_arithmetic_v<Type> || IsVector<Type>::value)
		friend class Value;
		template<typename Type>
		friend class Swizzle2;
		template<typename Type>
		friend class Swizzle3;
		template<typename Type>
		friend class Swizzle4;

		std::shared_ptr<ValueNode> parent;
	public:
		explicit(false) operator Value<T>() const requires std::is_arithmetic_v<T>
		{
			return Value<T>(AST::access(parent, NAME.string, AST::createType<T>()));
		}

		explicit(false) operator Value<T>() const requires IsVector<T>::value
		{
			return Value<T>(AST::access(parent, NAME.string, AST::createType<typename T::KtmVec>()));
		}

		SwizzleValue& operator=(const Value<T>& val)
		{
			Value<T>(*this) = val;
			return *this;
		}

		// 负号
		Value<T> operator-() const
		{
			return -Value<T>(*this);
		}

		// 正号
		Value<T> operator+() const
		{
			return +Value<T>(*this);
		}

		Value<T> operator+(const Value<T>& val) const
		{
			return Value<T>(*this) + val;
		}

		Value<T> operator-(const Value<T>& val) const
		{
			return Value<T>(*this) - val;
		}

		SwizzleValue& operator+=(const Value<T>& val) { *this = Value<T>(*this) + val; return *this; }
		SwizzleValue& operator-=(const Value<T>& val) { *this = Value<T>(*this) - val; return *this; }
	private:
		explicit SwizzleValue(std::shared_ptr<ValueNode> p) : parent(std::move(p)) {}
	};
#define SWIZZLE_DEF(type,name) SwizzleValue<type,#name> name
	template<typename T>
	class Swizzle2
	{
		using AST = EmbeddedShader::Ast::AST;
		using ValueNode = EmbeddedShader::Ast::Value;
		template<typename Type> requires (std::is_arithmetic_v<Type> || IsVector<Type>::value)
		friend class Value;
	public:
		union
		{
			SWIZZLE_DEF(T,x);
			SWIZZLE_DEF(T,y);
			SWIZZLE_DEF(Vector2T<T>,xy);
			SWIZZLE_DEF(Vector2T<T>,xx);
			SWIZZLE_DEF(Vector2T<T>,yx);
			SWIZZLE_DEF(Vector2T<T>,yy);

			SWIZZLE_DEF(Vector3T<T>,xxx);
			SWIZZLE_DEF(Vector3T<T>,xxy);
			SWIZZLE_DEF(Vector3T<T>,xyx);
			SWIZZLE_DEF(Vector3T<T>,xyy);
			SWIZZLE_DEF(Vector3T<T>,yxx);
			SWIZZLE_DEF(Vector3T<T>,yxy);
			SWIZZLE_DEF(Vector3T<T>,yyx);
			SWIZZLE_DEF(Vector3T<T>,yyy);

			SWIZZLE_DEF(Vector4T<T>,xxxx);
			SWIZZLE_DEF(Vector4T<T>,xxxy);
			SWIZZLE_DEF(Vector4T<T>,xxyx);
			SWIZZLE_DEF(Vector4T<T>,xxyy);
			SWIZZLE_DEF(Vector4T<T>,xyxx);
			SWIZZLE_DEF(Vector4T<T>,xyxy);
			SWIZZLE_DEF(Vector4T<T>,xyyx);
			SWIZZLE_DEF(Vector4T<T>,xyyy);
			SWIZZLE_DEF(Vector4T<T>,yxxx);
			SWIZZLE_DEF(Vector4T<T>,yxxy);
			SWIZZLE_DEF(Vector4T<T>,yxyx);
			SWIZZLE_DEF(Vector4T<T>,yxyy);
			SWIZZLE_DEF(Vector4T<T>,yyxx);
			SWIZZLE_DEF(Vector4T<T>,yyxy);
			SWIZZLE_DEF(Vector4T<T>,yyyx);
			SWIZZLE_DEF(Vector4T<T>,yyyy);
		};
		~Swizzle2()
		{
			x.~SwizzleValue<T,"x">();
		}
	private:
		explicit Swizzle2 (std::shared_ptr<ValueNode> p) : x(std::move(p)) {}
	};

	template<typename T>
	class Swizzle3
	{
		using AST = EmbeddedShader::Ast::AST;
		using ValueNode = EmbeddedShader::Ast::Value;
		template<typename Type> requires (std::is_arithmetic_v<Type> || IsVector<Type>::value)
		friend class Value;
	public:
		union
		{
			SWIZZLE_DEF(T,x);
			SWIZZLE_DEF(T,y);
			SWIZZLE_DEF(T,z);

			SWIZZLE_DEF(Vector2T<T>,xx);
			SWIZZLE_DEF(Vector2T<T>,xy);
			SWIZZLE_DEF(Vector2T<T>,xz);
			SWIZZLE_DEF(Vector2T<T>,yx);
			SWIZZLE_DEF(Vector2T<T>,yy);
			SWIZZLE_DEF(Vector2T<T>,yz);
			SWIZZLE_DEF(Vector2T<T>,zx);
			SWIZZLE_DEF(Vector2T<T>,zy);
			SWIZZLE_DEF(Vector2T<T>,zz);

			SWIZZLE_DEF(Vector3T<T>,xxx);
			SWIZZLE_DEF(Vector3T<T>,xxy);
			SWIZZLE_DEF(Vector3T<T>,xxz);
			SWIZZLE_DEF(Vector3T<T>,xyx);
			SWIZZLE_DEF(Vector3T<T>,xyy);
			SWIZZLE_DEF(Vector3T<T>,xyz);
			SWIZZLE_DEF(Vector3T<T>,xzx);
			SWIZZLE_DEF(Vector3T<T>,xzy);
			SWIZZLE_DEF(Vector3T<T>,xzz);
			SWIZZLE_DEF(Vector3T<T>,yxx);
			SWIZZLE_DEF(Vector3T<T>,yxy);
			SWIZZLE_DEF(Vector3T<T>,yxz);
			SWIZZLE_DEF(Vector3T<T>,yyx);
			SWIZZLE_DEF(Vector3T<T>,yyy);
			SWIZZLE_DEF(Vector3T<T>,yyz);
			SWIZZLE_DEF(Vector3T<T>,yzx);
			SWIZZLE_DEF(Vector3T<T>,yzy);
			SWIZZLE_DEF(Vector3T<T>,yzz);
			SWIZZLE_DEF(Vector3T<T>,zxx);
			SWIZZLE_DEF(Vector3T<T>,zxy);
			SWIZZLE_DEF(Vector3T<T>,zxz);
			SWIZZLE_DEF(Vector3T<T>,zyx);
			SWIZZLE_DEF(Vector3T<T>,zyy);
			SWIZZLE_DEF(Vector3T<T>,zyz);
			SWIZZLE_DEF(Vector3T<T>,zzx);
			SWIZZLE_DEF(Vector3T<T>,zzy);
			SWIZZLE_DEF(Vector3T<T>,zzz);

			SWIZZLE_DEF(Vector4T<T>,xxxx);
			SWIZZLE_DEF(Vector4T<T>,xxxy);
			SWIZZLE_DEF(Vector4T<T>,xxxz);
			SWIZZLE_DEF(Vector4T<T>,xxyx);
			SWIZZLE_DEF(Vector4T<T>,xxyy);
			SWIZZLE_DEF(Vector4T<T>,xxyz);
			SWIZZLE_DEF(Vector4T<T>,xxzx);
			SWIZZLE_DEF(Vector4T<T>,xxzy);
			SWIZZLE_DEF(Vector4T<T>,xxzz);
			SWIZZLE_DEF(Vector4T<T>,xyxx);
			SWIZZLE_DEF(Vector4T<T>,xyxy);
			SWIZZLE_DEF(Vector4T<T>,xyxz);
			SWIZZLE_DEF(Vector4T<T>,xyyx);
			SWIZZLE_DEF(Vector4T<T>,xyyy);
			SWIZZLE_DEF(Vector4T<T>,xyyz);
			SWIZZLE_DEF(Vector4T<T>,xyzx);
			SWIZZLE_DEF(Vector4T<T>,xyzy);
			SWIZZLE_DEF(Vector4T<T>,xyzz);
			SWIZZLE_DEF(Vector4T<T>,xzxx);
			SWIZZLE_DEF(Vector4T<T>,xzxy);
			SWIZZLE_DEF(Vector4T<T>,xzxz);
			SWIZZLE_DEF(Vector4T<T>,xzyx);
			SWIZZLE_DEF(Vector4T<T>,xzyy);
			SWIZZLE_DEF(Vector4T<T>,xzyz);
			SWIZZLE_DEF(Vector4T<T>,xzzx);
			SWIZZLE_DEF(Vector4T<T>,xzzy);
			SWIZZLE_DEF(Vector4T<T>,xzzz);
			SWIZZLE_DEF(Vector4T<T>,yxxx);
			SWIZZLE_DEF(Vector4T<T>,yxxy);
			SWIZZLE_DEF(Vector4T<T>,yxxz);
			SWIZZLE_DEF(Vector4T<T>,yxyx);
			SWIZZLE_DEF(Vector4T<T>,yxyy);
			SWIZZLE_DEF(Vector4T<T>,yxyz);
			SWIZZLE_DEF(Vector4T<T>,yxzx);
			SWIZZLE_DEF(Vector4T<T>,yxzy);
			SWIZZLE_DEF(Vector4T<T>,yxzz);
			SWIZZLE_DEF(Vector4T<T>,yyxx);
			SWIZZLE_DEF(Vector4T<T>,yyxy);
			SWIZZLE_DEF(Vector4T<T>,yyxz);
			SWIZZLE_DEF(Vector4T<T>,yyyx);
			SWIZZLE_DEF(Vector4T<T>,yyyy);
			SWIZZLE_DEF(Vector4T<T>,yyyz);
			SWIZZLE_DEF(Vector4T<T>,yyzx);
			SWIZZLE_DEF(Vector4T<T>,yyzy);
			SWIZZLE_DEF(Vector4T<T>,yyzz);
			SWIZZLE_DEF(Vector4T<T>,yzxx);
			SWIZZLE_DEF(Vector4T<T>,yzxy);
			SWIZZLE_DEF(Vector4T<T>,yzxz);
			SWIZZLE_DEF(Vector4T<T>,yzyx);
			SWIZZLE_DEF(Vector4T<T>,yzyy);
			SWIZZLE_DEF(Vector4T<T>,yzyz);
			SWIZZLE_DEF(Vector4T<T>,yzzx);
			SWIZZLE_DEF(Vector4T<T>,yzzy);
			SWIZZLE_DEF(Vector4T<T>,yzzz);
			SWIZZLE_DEF(Vector4T<T>,zxxx);
			SWIZZLE_DEF(Vector4T<T>,zxxy);
			SWIZZLE_DEF(Vector4T<T>,zxxz);
			SWIZZLE_DEF(Vector4T<T>,zxyx);
			SWIZZLE_DEF(Vector4T<T>,zxyy);
			SWIZZLE_DEF(Vector4T<T>,zxyz);
			SWIZZLE_DEF(Vector4T<T>,zxzx);
			SWIZZLE_DEF(Vector4T<T>,zxzy);
			SWIZZLE_DEF(Vector4T<T>,zxzz);
			SWIZZLE_DEF(Vector4T<T>,zyxx);
			SWIZZLE_DEF(Vector4T<T>,zyxy);
			SWIZZLE_DEF(Vector4T<T>,zyxz);
			SWIZZLE_DEF(Vector4T<T>,zyyx);
			SWIZZLE_DEF(Vector4T<T>,zyyy);
			SWIZZLE_DEF(Vector4T<T>,zyyz);
			SWIZZLE_DEF(Vector4T<T>,zyzx);
			SWIZZLE_DEF(Vector4T<T>,zyzy);
			SWIZZLE_DEF(Vector4T<T>,zyzz);
			SWIZZLE_DEF(Vector4T<T>,zzxx);
			SWIZZLE_DEF(Vector4T<T>,zzxy);
			SWIZZLE_DEF(Vector4T<T>,zzxz);
			SWIZZLE_DEF(Vector4T<T>,zzyx);
			SWIZZLE_DEF(Vector4T<T>,zzyy);
			SWIZZLE_DEF(Vector4T<T>,zzyz);
			SWIZZLE_DEF(Vector4T<T>,zzzx);
			SWIZZLE_DEF(Vector4T<T>,zzzy);
			SWIZZLE_DEF(Vector4T<T>,zzzz);
		};
		~Swizzle3()
		{
			x.~SwizzleValue<T,"x">();
		}
	private:
		explicit Swizzle3 (std::shared_ptr<ValueNode> p) : x(std::move(p)) {}
	};

	template<typename T>
	class Swizzle4
	{
		using AST = EmbeddedShader::Ast::AST;
		using ValueNode = EmbeddedShader::Ast::Value;
		template<typename Type> requires (std::is_arithmetic_v<Type> || IsVector<Type>::value)
		friend class Value;
	public:
		union
		{
			SWIZZLE_DEF(T,x);
			SWIZZLE_DEF(T,y);
			SWIZZLE_DEF(T,z);
			SWIZZLE_DEF(T,w);

			SWIZZLE_DEF(Vector2T<T>,xx);
			SWIZZLE_DEF(Vector2T<T>,xy);
			SWIZZLE_DEF(Vector2T<T>,xz);
			SWIZZLE_DEF(Vector2T<T>,xw);
			SWIZZLE_DEF(Vector2T<T>,yx);
			SWIZZLE_DEF(Vector2T<T>,yy);
			SWIZZLE_DEF(Vector2T<T>,yz);
			SWIZZLE_DEF(Vector2T<T>,yw);
			SWIZZLE_DEF(Vector2T<T>,zx);
			SWIZZLE_DEF(Vector2T<T>,zy);
			SWIZZLE_DEF(Vector2T<T>,zz);
			SWIZZLE_DEF(Vector2T<T>,zw);
			SWIZZLE_DEF(Vector2T<T>,wx);
			SWIZZLE_DEF(Vector2T<T>,wy);
			SWIZZLE_DEF(Vector2T<T>,wz);
			SWIZZLE_DEF(Vector2T<T>,ww);

			SWIZZLE_DEF(Vector3T<T>,xxx);
			SWIZZLE_DEF(Vector3T<T>,xxy);
			SWIZZLE_DEF(Vector3T<T>,xxz);
			SWIZZLE_DEF(Vector3T<T>,xxw);
			SWIZZLE_DEF(Vector3T<T>,xyx);
			SWIZZLE_DEF(Vector3T<T>,xyy);
			SWIZZLE_DEF(Vector3T<T>,xyz);
			SWIZZLE_DEF(Vector3T<T>,xyw);
			SWIZZLE_DEF(Vector3T<T>,xzx);
			SWIZZLE_DEF(Vector3T<T>,xzy);
			SWIZZLE_DEF(Vector3T<T>,xzz);
			SWIZZLE_DEF(Vector3T<T>,xzw);
			SWIZZLE_DEF(Vector3T<T>,xwx);
			SWIZZLE_DEF(Vector3T<T>,xwy);
			SWIZZLE_DEF(Vector3T<T>,xwz);
			SWIZZLE_DEF(Vector3T<T>,xww);
			SWIZZLE_DEF(Vector3T<T>,yxx);
			SWIZZLE_DEF(Vector3T<T>,yxy);
			SWIZZLE_DEF(Vector3T<T>,yxz);
			SWIZZLE_DEF(Vector3T<T>,yxw);
			SWIZZLE_DEF(Vector3T<T>,yyx);
			SWIZZLE_DEF(Vector3T<T>,yyy);
			SWIZZLE_DEF(Vector3T<T>,yyz);
			SWIZZLE_DEF(Vector3T<T>,yyw);
			SWIZZLE_DEF(Vector3T<T>,yzx);
			SWIZZLE_DEF(Vector3T<T>,yzy);
			SWIZZLE_DEF(Vector3T<T>,yzz);
			SWIZZLE_DEF(Vector3T<T>,yzw);
			SWIZZLE_DEF(Vector3T<T>,ywx);
			SWIZZLE_DEF(Vector3T<T>,ywy);
			SWIZZLE_DEF(Vector3T<T>,ywz);
			SWIZZLE_DEF(Vector3T<T>,yww);
			SWIZZLE_DEF(Vector3T<T>,zxx);
			SWIZZLE_DEF(Vector3T<T>,zxy);
			SWIZZLE_DEF(Vector3T<T>,zxz);
			SWIZZLE_DEF(Vector3T<T>,zxw);
			SWIZZLE_DEF(Vector3T<T>,zyx);
			SWIZZLE_DEF(Vector3T<T>,zyy);
			SWIZZLE_DEF(Vector3T<T>,zyz);
			SWIZZLE_DEF(Vector3T<T>,zyw);
			SWIZZLE_DEF(Vector3T<T>,zzx);
			SWIZZLE_DEF(Vector3T<T>,zzy);
			SWIZZLE_DEF(Vector3T<T>,zzz);
			SWIZZLE_DEF(Vector3T<T>,zzw);
			SWIZZLE_DEF(Vector3T<T>,zwx);
			SWIZZLE_DEF(Vector3T<T>,zwy);
			SWIZZLE_DEF(Vector3T<T>,zwz);
			SWIZZLE_DEF(Vector3T<T>,zww);
			SWIZZLE_DEF(Vector3T<T>,wxx);
			SWIZZLE_DEF(Vector3T<T>,wxy);
			SWIZZLE_DEF(Vector3T<T>,wxz);
			SWIZZLE_DEF(Vector3T<T>,wxw);
			SWIZZLE_DEF(Vector3T<T>,wyx);
			SWIZZLE_DEF(Vector3T<T>,wyy);
			SWIZZLE_DEF(Vector3T<T>,wyz);
			SWIZZLE_DEF(Vector3T<T>,wyw);
			SWIZZLE_DEF(Vector3T<T>,wzx);
			SWIZZLE_DEF(Vector3T<T>,wzy);
			SWIZZLE_DEF(Vector3T<T>,wzz);
			SWIZZLE_DEF(Vector3T<T>,wzw);
			SWIZZLE_DEF(Vector3T<T>,wwx);
			SWIZZLE_DEF(Vector3T<T>,wwy);
			SWIZZLE_DEF(Vector3T<T>,wwz);
			SWIZZLE_DEF(Vector3T<T>,www);

			SWIZZLE_DEF(Vector4T<T>,xxxx);
			SWIZZLE_DEF(Vector4T<T>,xxxy);
			SWIZZLE_DEF(Vector4T<T>,xxxz);
			SWIZZLE_DEF(Vector4T<T>,xxxw);
			SWIZZLE_DEF(Vector4T<T>,xxyx);
			SWIZZLE_DEF(Vector4T<T>,xxyy);
			SWIZZLE_DEF(Vector4T<T>,xxyz);
			SWIZZLE_DEF(Vector4T<T>,xxyw);
			SWIZZLE_DEF(Vector4T<T>,xxzx);
			SWIZZLE_DEF(Vector4T<T>,xxzy);
			SWIZZLE_DEF(Vector4T<T>,xxzz);
			SWIZZLE_DEF(Vector4T<T>,xxzw);
			SWIZZLE_DEF(Vector4T<T>,xxwx);
			SWIZZLE_DEF(Vector4T<T>,xxwy);
			SWIZZLE_DEF(Vector4T<T>,xxwz);
			SWIZZLE_DEF(Vector4T<T>,xxww);
			SWIZZLE_DEF(Vector4T<T>,xyxx);
			SWIZZLE_DEF(Vector4T<T>,xyxy);
			SWIZZLE_DEF(Vector4T<T>,xyxz);
			SWIZZLE_DEF(Vector4T<T>,xyxw);
			SWIZZLE_DEF(Vector4T<T>,xyyx);
			SWIZZLE_DEF(Vector4T<T>,xyyy);
			SWIZZLE_DEF(Vector4T<T>,xyyz);
			SWIZZLE_DEF(Vector4T<T>,xyyw);
			SWIZZLE_DEF(Vector4T<T>,xyzx);
			SWIZZLE_DEF(Vector4T<T>,xyzy);
			SWIZZLE_DEF(Vector4T<T>,xyzz);
			SWIZZLE_DEF(Vector4T<T>,xyzw);
			SWIZZLE_DEF(Vector4T<T>,xywx);
			SWIZZLE_DEF(Vector4T<T>,xywy);
			SWIZZLE_DEF(Vector4T<T>,xywz);
			SWIZZLE_DEF(Vector4T<T>,xyww);
			SWIZZLE_DEF(Vector4T<T>,xzxx);
			SWIZZLE_DEF(Vector4T<T>,xzxy);
			SWIZZLE_DEF(Vector4T<T>,xzxz);
			SWIZZLE_DEF(Vector4T<T>,xzxw);
			SWIZZLE_DEF(Vector4T<T>,xzyx);
			SWIZZLE_DEF(Vector4T<T>,xzyy);
			SWIZZLE_DEF(Vector4T<T>,xzyz);
			SWIZZLE_DEF(Vector4T<T>,xzyw);
			SWIZZLE_DEF(Vector4T<T>,xzzx);
			SWIZZLE_DEF(Vector4T<T>,xzzy);
			SWIZZLE_DEF(Vector4T<T>,xzzz);
			SWIZZLE_DEF(Vector4T<T>,xzzw);
			SWIZZLE_DEF(Vector4T<T>,xzwx);
			SWIZZLE_DEF(Vector4T<T>,xzwy);
			SWIZZLE_DEF(Vector4T<T>,xzwz);
			SWIZZLE_DEF(Vector4T<T>,xzww);
			SWIZZLE_DEF(Vector4T<T>,xwxx);
			SWIZZLE_DEF(Vector4T<T>,xwxy);
			SWIZZLE_DEF(Vector4T<T>,xwxz);
			SWIZZLE_DEF(Vector4T<T>,xwxw);
			SWIZZLE_DEF(Vector4T<T>,xwyx);
			SWIZZLE_DEF(Vector4T<T>,xwyy);
			SWIZZLE_DEF(Vector4T<T>,xwyz);
			SWIZZLE_DEF(Vector4T<T>,xwyw);
			SWIZZLE_DEF(Vector4T<T>,xwzx);
			SWIZZLE_DEF(Vector4T<T>,xwzy);
			SWIZZLE_DEF(Vector4T<T>,xwzz);
			SWIZZLE_DEF(Vector4T<T>,xwzw);
			SWIZZLE_DEF(Vector4T<T>,xwwx);
			SWIZZLE_DEF(Vector4T<T>,xwwy);
			SWIZZLE_DEF(Vector4T<T>,xwwz);
			SWIZZLE_DEF(Vector4T<T>,xwww);
			SWIZZLE_DEF(Vector4T<T>,yxxx);
			SWIZZLE_DEF(Vector4T<T>,yxxy);
			SWIZZLE_DEF(Vector4T<T>,yxxz);
			SWIZZLE_DEF(Vector4T<T>,yxxw);
			SWIZZLE_DEF(Vector4T<T>,yxyx);
			SWIZZLE_DEF(Vector4T<T>,yxyy);
			SWIZZLE_DEF(Vector4T<T>,yxyz);
			SWIZZLE_DEF(Vector4T<T>,yxyw);
			SWIZZLE_DEF(Vector4T<T>,yxzx);
			SWIZZLE_DEF(Vector4T<T>,yxzy);
			SWIZZLE_DEF(Vector4T<T>,yxzz);
			SWIZZLE_DEF(Vector4T<T>,yxzw);
			SWIZZLE_DEF(Vector4T<T>,yxwx);
			SWIZZLE_DEF(Vector4T<T>,yxwy);
			SWIZZLE_DEF(Vector4T<T>,yxwz);
			SWIZZLE_DEF(Vector4T<T>,yxww);
			SWIZZLE_DEF(Vector4T<T>,yyxx);
			SWIZZLE_DEF(Vector4T<T>,yyxy);
			SWIZZLE_DEF(Vector4T<T>,yyxz);
			SWIZZLE_DEF(Vector4T<T>,yyxw);
			SWIZZLE_DEF(Vector4T<T>,yyyx);
			SWIZZLE_DEF(Vector4T<T>,yyyy);
			SWIZZLE_DEF(Vector4T<T>,yyyz);
			SWIZZLE_DEF(Vector4T<T>,yyyw);
			SWIZZLE_DEF(Vector4T<T>,yyzx);
			SWIZZLE_DEF(Vector4T<T>,yyzy);
			SWIZZLE_DEF(Vector4T<T>,yyzz);
			SWIZZLE_DEF(Vector4T<T>,yyzw);
			SWIZZLE_DEF(Vector4T<T>,yywx);
			SWIZZLE_DEF(Vector4T<T>,yywy);
			SWIZZLE_DEF(Vector4T<T>,yywz);
			SWIZZLE_DEF(Vector4T<T>,yyww);
			SWIZZLE_DEF(Vector4T<T>,yzxx);
			SWIZZLE_DEF(Vector4T<T>,yzxy);
			SWIZZLE_DEF(Vector4T<T>,yzxz);
			SWIZZLE_DEF(Vector4T<T>,yzxw);
			SWIZZLE_DEF(Vector4T<T>,yzyx);
			SWIZZLE_DEF(Vector4T<T>,yzyy);
			SWIZZLE_DEF(Vector4T<T>,yzyz);
			SWIZZLE_DEF(Vector4T<T>,yzyw);
			SWIZZLE_DEF(Vector4T<T>,yzzx);
			SWIZZLE_DEF(Vector4T<T>,yzzy);
			SWIZZLE_DEF(Vector4T<T>,yzzz);
			SWIZZLE_DEF(Vector4T<T>,yzzw);
			SWIZZLE_DEF(Vector4T<T>,yzwx);
			SWIZZLE_DEF(Vector4T<T>,yzwy);
			SWIZZLE_DEF(Vector4T<T>,yzwz);
			SWIZZLE_DEF(Vector4T<T>,yzww);
			SWIZZLE_DEF(Vector4T<T>,ywxx);
			SWIZZLE_DEF(Vector4T<T>,ywxy);
			SWIZZLE_DEF(Vector4T<T>,ywxz);
			SWIZZLE_DEF(Vector4T<T>,ywxw);
			SWIZZLE_DEF(Vector4T<T>,ywyx);
			SWIZZLE_DEF(Vector4T<T>,ywyy);
			SWIZZLE_DEF(Vector4T<T>,ywyz);
			SWIZZLE_DEF(Vector4T<T>,ywyw);
			SWIZZLE_DEF(Vector4T<T>,ywzx);
			SWIZZLE_DEF(Vector4T<T>,ywzy);
			SWIZZLE_DEF(Vector4T<T>,ywzz);
			SWIZZLE_DEF(Vector4T<T>,ywzw);
			SWIZZLE_DEF(Vector4T<T>,ywwx);
			SWIZZLE_DEF(Vector4T<T>,ywwy);
			SWIZZLE_DEF(Vector4T<T>,ywwz);
			SWIZZLE_DEF(Vector4T<T>,ywww);
			SWIZZLE_DEF(Vector4T<T>,zxxx);
			SWIZZLE_DEF(Vector4T<T>,zxxy);
			SWIZZLE_DEF(Vector4T<T>,zxxz);
			SWIZZLE_DEF(Vector4T<T>,zxxw);
			SWIZZLE_DEF(Vector4T<T>,zxyx);
			SWIZZLE_DEF(Vector4T<T>,zxyy);
			SWIZZLE_DEF(Vector4T<T>,zxyz);
			SWIZZLE_DEF(Vector4T<T>,zxyw);
			SWIZZLE_DEF(Vector4T<T>,zxzx);
			SWIZZLE_DEF(Vector4T<T>,zxzy);
			SWIZZLE_DEF(Vector4T<T>,zxzz);
			SWIZZLE_DEF(Vector4T<T>,zxzw);
			SWIZZLE_DEF(Vector4T<T>,zxwx);
			SWIZZLE_DEF(Vector4T<T>,zxwy);
			SWIZZLE_DEF(Vector4T<T>,zxwz);
			SWIZZLE_DEF(Vector4T<T>,zxww);
			SWIZZLE_DEF(Vector4T<T>,zyxx);
			SWIZZLE_DEF(Vector4T<T>,zyxy);
			SWIZZLE_DEF(Vector4T<T>,zyxz);
			SWIZZLE_DEF(Vector4T<T>,zyxw);
			SWIZZLE_DEF(Vector4T<T>,zyyx);
			SWIZZLE_DEF(Vector4T<T>,zyyy);
			SWIZZLE_DEF(Vector4T<T>,zyyz);
			SWIZZLE_DEF(Vector4T<T>,zyyw);
			SWIZZLE_DEF(Vector4T<T>,zyzx);
			SWIZZLE_DEF(Vector4T<T>,zyzy);
			SWIZZLE_DEF(Vector4T<T>,zyzz);
			SWIZZLE_DEF(Vector4T<T>,zyzw);
			SWIZZLE_DEF(Vector4T<T>,zywx);
			SWIZZLE_DEF(Vector4T<T>,zywy);
			SWIZZLE_DEF(Vector4T<T>,zywz);
			SWIZZLE_DEF(Vector4T<T>,zyww);
			SWIZZLE_DEF(Vector4T<T>,zzxx);
			SWIZZLE_DEF(Vector4T<T>,zzxy);
			SWIZZLE_DEF(Vector4T<T>,zzxz);
			SWIZZLE_DEF(Vector4T<T>,zzxw);
			SWIZZLE_DEF(Vector4T<T>,zzyx);
			SWIZZLE_DEF(Vector4T<T>,zzyy);
			SWIZZLE_DEF(Vector4T<T>,zzyz);
			SWIZZLE_DEF(Vector4T<T>,zzyw);
			SWIZZLE_DEF(Vector4T<T>,zzzx);
			SWIZZLE_DEF(Vector4T<T>,zzzy);
			SWIZZLE_DEF(Vector4T<T>,zzzz);
			SWIZZLE_DEF(Vector4T<T>,zzzw);
			SWIZZLE_DEF(Vector4T<T>,zzwx);
			SWIZZLE_DEF(Vector4T<T>,zzwy);
			SWIZZLE_DEF(Vector4T<T>,zzwz);
			SWIZZLE_DEF(Vector4T<T>,zzww);
			SWIZZLE_DEF(Vector4T<T>,zwxx);
			SWIZZLE_DEF(Vector4T<T>,zwxy);
			SWIZZLE_DEF(Vector4T<T>,zwxz);
			SWIZZLE_DEF(Vector4T<T>,zwxw);
			SWIZZLE_DEF(Vector4T<T>,zwyx);
			SWIZZLE_DEF(Vector4T<T>,zwyy);
			SWIZZLE_DEF(Vector4T<T>,zwyz);
			SWIZZLE_DEF(Vector4T<T>,zwyw);
			SWIZZLE_DEF(Vector4T<T>,zwzx);
			SWIZZLE_DEF(Vector4T<T>,zwzy);
			SWIZZLE_DEF(Vector4T<T>,zwzz);
			SWIZZLE_DEF(Vector4T<T>,zwzw);
			SWIZZLE_DEF(Vector4T<T>,zwwx);
			SWIZZLE_DEF(Vector4T<T>,zwwy);
			SWIZZLE_DEF(Vector4T<T>,zwwz);
			SWIZZLE_DEF(Vector4T<T>,zwww);
			SWIZZLE_DEF(Vector4T<T>,wxxx);
			SWIZZLE_DEF(Vector4T<T>,wxxy);
			SWIZZLE_DEF(Vector4T<T>,wxxz);
			SWIZZLE_DEF(Vector4T<T>,wxxw);
			SWIZZLE_DEF(Vector4T<T>,wxyx);
			SWIZZLE_DEF(Vector4T<T>,wxyy);
			SWIZZLE_DEF(Vector4T<T>,wxyz);
			SWIZZLE_DEF(Vector4T<T>,wxyw);
			SWIZZLE_DEF(Vector4T<T>,wxzx);
			SWIZZLE_DEF(Vector4T<T>,wxzy);
			SWIZZLE_DEF(Vector4T<T>,wxzz);
			SWIZZLE_DEF(Vector4T<T>,wxzw);
			SWIZZLE_DEF(Vector4T<T>,wxwx);
			SWIZZLE_DEF(Vector4T<T>,wxwy);
			SWIZZLE_DEF(Vector4T<T>,wxwz);
			SWIZZLE_DEF(Vector4T<T>,wxww);
			SWIZZLE_DEF(Vector4T<T>,wyxx);
			SWIZZLE_DEF(Vector4T<T>,wyxy);
			SWIZZLE_DEF(Vector4T<T>,wyxz);
			SWIZZLE_DEF(Vector4T<T>,wyxw);
			SWIZZLE_DEF(Vector4T<T>,wyyx);
			SWIZZLE_DEF(Vector4T<T>,wyyy);
			SWIZZLE_DEF(Vector4T<T>,wyyz);
			SWIZZLE_DEF(Vector4T<T>,wyyw);
			SWIZZLE_DEF(Vector4T<T>,wyzx);
			SWIZZLE_DEF(Vector4T<T>,wyzy);
			SWIZZLE_DEF(Vector4T<T>,wyzz);
			SWIZZLE_DEF(Vector4T<T>,wyzw);
			SWIZZLE_DEF(Vector4T<T>,wywx);
			SWIZZLE_DEF(Vector4T<T>,wywy);
			SWIZZLE_DEF(Vector4T<T>,wywz);
			SWIZZLE_DEF(Vector4T<T>,wyww);
			SWIZZLE_DEF(Vector4T<T>,wzxx);
			SWIZZLE_DEF(Vector4T<T>,wzxy);
			SWIZZLE_DEF(Vector4T<T>,wzxz);
			SWIZZLE_DEF(Vector4T<T>,wzxw);
			SWIZZLE_DEF(Vector4T<T>,wzyx);
			SWIZZLE_DEF(Vector4T<T>,wzyy);
			SWIZZLE_DEF(Vector4T<T>,wzyz);
			SWIZZLE_DEF(Vector4T<T>,wzyw);
			SWIZZLE_DEF(Vector4T<T>,wzzx);
			SWIZZLE_DEF(Vector4T<T>,wzzy);
			SWIZZLE_DEF(Vector4T<T>,wzzz);
			SWIZZLE_DEF(Vector4T<T>,wzzw);
			SWIZZLE_DEF(Vector4T<T>,wzwx);
			SWIZZLE_DEF(Vector4T<T>,wzwy);
			SWIZZLE_DEF(Vector4T<T>,wzwz);
			SWIZZLE_DEF(Vector4T<T>,wzww);
			SWIZZLE_DEF(Vector4T<T>,wwxx);
			SWIZZLE_DEF(Vector4T<T>,wwxy);
			SWIZZLE_DEF(Vector4T<T>,wwxz);
			SWIZZLE_DEF(Vector4T<T>,wwxw);
			SWIZZLE_DEF(Vector4T<T>,wwyx);
			SWIZZLE_DEF(Vector4T<T>,wwyy);
			SWIZZLE_DEF(Vector4T<T>,wwyz);
			SWIZZLE_DEF(Vector4T<T>,wwyw);
			SWIZZLE_DEF(Vector4T<T>,wwzx);
			SWIZZLE_DEF(Vector4T<T>,wwzy);
			SWIZZLE_DEF(Vector4T<T>,wwzz);
			SWIZZLE_DEF(Vector4T<T>,wwzw);
			SWIZZLE_DEF(Vector4T<T>,wwwx);
			SWIZZLE_DEF(Vector4T<T>,wwwy);
			SWIZZLE_DEF(Vector4T<T>,wwwz);
			SWIZZLE_DEF(Vector4T<T>,wwww);
		};
		~Swizzle4()
		{
			x.~SwizzleValue<T,"x">();
		}
	private:
		explicit Swizzle4 (std::shared_ptr<ValueNode> p) : x(std::move(p)) {}
	};
#undef SWIZZLE_DEF
}