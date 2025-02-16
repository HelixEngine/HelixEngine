#pragma once
#include <HelixEngine/Base/Object.hpp>
#include <HelixEngine/Math/Vector2.hpp>
#include <QtWidgets/qwidget.h>

namespace helix
{
	class Window final : public Object
	{
	public:
		class Flags
		{
		public:
			Flags();
			using ValueType = uint8_t;

			enum class Value : ValueType
			{
				MaximumButton,
				MinimumButton
			};

			using enum Value;
		private:
			ValueType flags{};
		};

		struct CreateInfo
		{
			std::u8string title = u8"HelixEngine";
			Vector2I32 initialSize = {600, 600};
			Window* parent = nullptr;
			bool isFixed = false;
			bool isShow = true;
			bool isEnabledMaximumButton = true;
			bool isEnabledMinimumButton = true;
		};

		explicit Window(std::u8string_view title = u8"HelixEngine", int32_t initialWidth = 600,
		                int32_t initialHeight = 600);
		explicit Window(std::u8string_view title = u8"HelixEngine", Vector2I32 initialSize = {600, 600});
		explicit Window(const CreateInfo& info);
		/**
		 * @brief 重新设置窗口工作区的尺寸
		 * @note 如果当前窗口尺寸已经固定，则该方法等效于 setFixedSize
		 * @note 如需取消固定，请调用 setFixedSize(false) 取消固定
		 * @param newSize 要设置的新窗口尺寸
		 */
		void setSize(Vector2I32 newSize) const;
		// /**
		//  * @brief 设置窗口工作区的尺寸
		//  * @note 如果窗口尺寸未固定，则会将窗口尺寸设置为固定状态
		//  * @note 固定窗口尺寸后，无法使用光标修改窗口坐标
		//  * @param fixedSize 要设置的新窗口固定尺寸
		//  */
		// void setFixedSize(Vector2I32 fixedSize) const;
		/**
		 * @brief 设置是否固定当前的窗口尺寸
		 * @param isFixed 是否固定
		 */
		void setFixedSize(bool isFixed = true) const;
		/**
		 * @brief 获取窗口当前尺寸
		 * @return 窗口当前的尺寸
		 */
		[[nodiscard]] Vector2I32 getSize() const;
		/**
		 * @brief 获取QWidget对象
		 * @return Window内部的QWidget对象
		 */
		[[nodiscard]] const QWidget* getQWidget() const;
	private:
		QWidget* qWidget = nullptr;
	};

	Window::Flags::ValueType operator |(const Window::Flags::Value& value1, const Window::Flags::Value& value2);
}
