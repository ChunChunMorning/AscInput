# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	/// <summary>
	/// XInputの軸の種類
	/// </summary>
	enum class XInputAxisType : uint8
	{
		LeftThumbX,

		LeftThumbY,

		RightThumbX,

		RightThumbY,
	};

	/// <summary>
	/// 軸
	/// </summary>
	class Axis
	{
	private:

		enum class InputDevice : uint8
		{
			None,

			Key,

			Gamepad,

			XInput,
		};

		struct KeyAxis
		{
			Key negative;
			Key positive;
		};

		struct GamepadAxis
		{
			size_t userIndex;
			size_t axisIndex;
		};

		struct XInputAxis
		{
			size_t userIndex;
			XInputAxisType axisType;
		};

		[[nodiscard]] double rawValue() const
		{
			assert(isValid());

			switch (m_device)
			{
			case InputDevice::None:
				return 0.0;

			case InputDevice::Key:
				return (m_keyAxis.negative.pressed() ? -1.0 : 0.0) + (m_keyAxis.positive.pressed() ? 1.0 : 0.0);

			case InputDevice::Gamepad:
			{
				const auto& gamepad = Gamepad(m_gamepadAxis.userIndex);

				return gamepad.axes[m_gamepadAxis.axisIndex];
			}

			case InputDevice::XInput:
			{
				const auto& xinput = XInput(m_xinputAxis.userIndex);

				switch (m_xinputAxis.axisType)
				{
				case XInputAxisType::LeftThumbX:
					return xinput.leftThumbX;

				case XInputAxisType::LeftThumbY:
					return xinput.leftThumbY;

				case XInputAxisType::RightThumbX:
					return xinput.rightThumbX;

				case XInputAxisType::RightThumbY:
					return xinput.rightThumbY;

				default:
					assert(false);
					return {};
				}
			}

			default:
				assert(false);
				return {};
			}
		}

		union
		{
			KeyAxis m_keyAxis;
			GamepadAxis m_gamepadAxis;
			XInputAxis m_xinputAxis;
		};

		InputDevice m_device = InputDevice::None;
		bool m_doReverse = false;

	public:

		/// <summary>
		/// キーで表される軸を作成します。
		/// </summary>
		/// <param name="negative">
		/// 負の方向を表すキー
		/// </param>
		/// <param name="positive">
		/// 正の方向を表すキー
		/// </param>
		/// <param name="doReverse">
		/// true 軸の方向を反転する / false 軸の方向を反転しない
		/// </param>
		constexpr Axis(Key negative, Key positive, bool doReverse = false) noexcept
			: m_keyAxis({ negative, positive })
			, m_device(InputDevice::Key)
			, m_doReverse(doReverse)
		{
		}

		/// <summary>
		/// ゲームパッドから取得される軸を作成します。
		/// </summary>
		/// <param name="userIndex">
		/// ゲームパッドのユーザインデックス
		/// </param>
		/// <param name="axisIndex">
		/// 取得する軸のインデックス
		/// </param>
		/// <param name="doReverse">
		/// true 軸の方向を反転する / false 軸の方向を反転しない
		/// </param>
		constexpr Axis(size_t userIndex, size_t axisIndex, bool doReverse = false) noexcept
			: m_gamepadAxis({ userIndex, axisIndex })
			, m_device(InputDevice::Gamepad)
			, m_doReverse(doReverse)
		{
		}

		/// <summary>
		/// XInputから取得される軸を作成します。
		/// </summary>
		/// <param name="userIndex">
		/// ゲームパッドのユーザインデックス
		/// </param>
		/// <param name="axisType">
		/// 取得する軸の種類
		/// </param>
		/// <param name="doReverse">
		/// true 軸の方向を反転する / false 軸の方向を反転しない
		/// </param>
		constexpr Axis(size_t userIndex, XInputAxisType axisType, bool doReverse = false) noexcept
			: m_xinputAxis({ userIndex, axisType })
			, m_device(InputDevice::XInput)
			, m_doReverse(doReverse)
		{
		}

		/// <summary>
		/// 軸が有効かを返します。
		/// </summary>
		/// <returns>
		/// true 軸が有効 / false 軸が無効
		/// </retunrs>
		[[nodiscard]] bool isValid() const
		{
			switch (m_device)
			{
			case InputDevice::None:
			case InputDevice::Key:
				return true;

			case InputDevice::Gamepad:
			{
				const auto& gamepad = Gamepad(m_gamepadAxis.userIndex);

				return gamepad.isConnected() && m_gamepadAxis.axisIndex < gamepad.axes.size();
			}

			case InputDevice::XInput:
			{
				const auto& xinput = XInput(m_xinputAxis.userIndex);

				return xinput.isConnected();
			}

			default:
				assert(false);
				return {};
			}
		}

		/// <summary>
		/// 軸の状態を取得します。
		/// </summary>
		/// <returns>
		/// 軸の状態。軸が無効、または閾値以下の場合は 0 を返します。
		/// </returns>
		[[nodiscard]] operator double() const
		{
			return get();
		}

		/// <summary>
		/// 軸の状態を取得します。
		/// </summary>
		/// <param name="threshold">
		/// 軸の閾値
		/// </param>
		/// <returns>
		/// 軸の状態。軸が無効、または閾値以下の場合は 0 を返します。
		/// </returns>
		[[nodiscard]] double get(double threshold = 0.12) const
		{
			return getOr(0.0, threshold);
		}

		/// <summary>
		/// 軸の状態を取得します。
		/// </summary>
		/// <param name="threshold">
		/// 軸の閾値
		/// </param>
		/// <param name="defaultValue">
		/// 軸のデフォルト値
		/// </param>
		/// <returns>
		/// 軸の状態。軸が無効、または閾値以下の場合はデフォルト値を返します。
		/// </returns>
		[[nodiscard]] double getOr(double&& defaultValue, double threshold = 0.12) const
		{
			return getOpt(threshold).value_or(defaultValue);
		}

		/// <summary>
		/// 軸の状態を取得します。
		/// </summary>
		/// <param name="threshold">
		/// 軸の閾値
		/// </param>
		/// <returns>
		/// 軸の状態。軸が無効、または閾値以下の場合はnoneを返します。
		/// </returns>
		[[nodiscard]] Optional<double> getOpt(double threshold = 0.12) const
		{
			if (!isValid())
			{
				return none;
			}

			const auto value = rawValue();

			if (Abs(value) < threshold)
			{
				return none;
			}

			return m_doReverse ? -value : value;
		}
	};
}
