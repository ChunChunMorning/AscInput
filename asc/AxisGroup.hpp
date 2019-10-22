# pragma once
# include <Siv3D.hpp>
# include "Axis.hpp"

namespace asc
{
	using namespace s3d;

	/// <summary>
	/// 軸グループ
	/// </summary>
	class AxisGroup
	{
	private:

		Array<Axis> m_axes;

		template <class Type, class ...Args>
		void append(const Type& axis, const Args&... args)
		{
			append(axis);
			append(args...);
		}

		void append()
		{
		}

		void append(const Axis& axis)
		{
			m_axes << axis;
		}

		void append(const AxisGroup& group)
		{
			m_axes.append(group.m_axes);
		}

	public:

		template <class ...Args>
		explicit AxisGroup(const Args&... args)
		{
			append(args...);
		}

		[[nodiscard]] operator double() const
		{
			return get();
		}

		AxisGroup& operator |=(const Axis& axis)
		{
			append(axis);

			return *this;
		}

		AxisGroup& operator |=(const AxisGroup& group)
		{
			m_axes.append(group.m_axes);

			return *this;
		}

		/// <summary>
		/// 軸の状態を取得します。
		/// </summary>
		/// <returns>
		/// 軸の状態。軸が無効、または閾値以下の場合は 0 を返します。
		/// </returns>
		[[nodiscard]] double get(double threshold = 0.12) const
		{
			return getValueOr(0.0, threshold);
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
		[[nodiscard]] double getValueOr(double&& defaultValue, double threshold = 0.12) const
		{
			return getValueOpt(threshold).value_or(defaultValue);
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
		[[nodiscard]] Optional<double> getValueOpt(double threshold = 0.12) const
		{
			for (const auto& axis : m_axes)
			{
				const auto axisValueOpt = axis.getOpt(threshold);

				if (axisValueOpt)
				{
					return axisValueOpt;
				}
			}

			return none;
		}
	};

	[[nodiscard]] inline AxisGroup operator |(const Axis& axis1, const Axis& axis2)
	{
		return AxisGroup(axis1, axis2);
	}

	[[nodiscard]] inline AxisGroup operator |(const Axis& axis, const AxisGroup& group)
	{
		return AxisGroup(axis, group);
	}

	[[nodiscard]] inline AxisGroup operator |(const AxisGroup& group, const Axis& axis)
	{
		return AxisGroup(group, axis);
	}
}
