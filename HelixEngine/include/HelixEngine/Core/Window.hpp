#pragma once
#include <HelixEngine/Util/Object.hpp>
#include <HelixEngine/Math/Vector2.hpp>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Node/Scene.hpp>
#include <QWidget>

#include "HelixEngine/Util/BitOption.hpp"

namespace helix::qt
{
	class Widget;
}

namespace helix
{
	class Scene;
}

namespace helix
{
	class Window final : public Object
	{
		friend class qt::Widget;
	public:
		enum class Flag
		{
			MaximumButton = 0b01,
			MinimumButton = 0b10,
			MinMaxButton = MaximumButton | MinimumButton,
		};

		struct Property
		{
			std::u8string title = u8"HelixEngine";
			Vector2I32 size = {600, 600};
			Window* parent = nullptr;
			bool isFixed = false;
			bool isDisplay = true;
			BitOption<Flag> flag = Flag::MinMaxButton;
		};

		explicit Window(std::u8string_view title = u8"HelixEngine", int32_t width = 600,
		                int32_t height = 600);
		explicit Window(std::u8string_view title = u8"HelixEngine", Vector2I32 size = {600, 600});
		explicit Window(const Property& property);
		/**
		 * @brief 重新设置窗口工作区的尺寸
		 * @note 如果当前窗口尺寸已经固定，则该方法等效于 setFixedSize
		 * @note 如需取消固定，请调用 setFixedSize(false) 取消固定
		 * @param newSize 要设置的新窗口尺寸
		 */
		void setSize(Vector2I32 newSize) const;
		/**
		 * @brief 设置是否固定当前的窗口尺寸
		 * @param isFixed 是否固定
		 */
		void setFixedSize(bool isFixed) const;
		/**
		 * @brief 获取窗口当前尺寸
		 * @return 窗口当前的尺寸
		 */
		[[nodiscard]] Vector2I32 getSize() const;
		/**
		 * @brief 获取QWidget对象
		 * @return Window内部的QWidget对象
		 */
		[[nodiscard]] QWidget* getQWidget() const;

		void setParent(Window* parent) const;
		[[nodiscard]] Window* getParent() const;

		void setFlag(BitOption<Flag> flag) const;
		[[nodiscard]] BitOption<Flag> getFlag() const;

		void setProperty(const Property& property) const;
		[[nodiscard]] Property getProperty() const;

		void setDisplay(bool isDisplay = true) const;
		[[nodiscard]] bool isDisplay() const;

		void setTitle(std::u8string_view newTitle) const;
		[[nodiscard]] std::u8string getTitle() const;
	private:
		std::unique_ptr<QWidget> qWidget;
		static constexpr auto qtParentPropertyName = "HelixEngine.Window:Parent";
	public:
		//Scene
		void enter(Ref<Scene> scene);
	private:
		Ref<Scene> scene;
	};
}
